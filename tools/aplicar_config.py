#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
aplicar_config.py - Actualizacion de configuracion [Mu]Misiones (97x / MuEMU S6)
--------------------------------------------------------------------------------
Perfil aplicado : HARD / CLASICO   |   Red: LAN (192.168.1.215)

El script es IDEMPOTENTE: podes correrlo las veces que quieras, siempre deja
los archivos en el mismo estado final.

Reglas de seguridad que respeta:
  * NO cambia el formato de fin de linea (se conserva LF, que es como venia).
  * NO reordena ni borra claves: solo reemplaza el VALOR de las que toca.
  * Escribe en ASCII puro (sin acentos) para evitar mojibake en el cliente 97x.
  * Si una clave no existe en el archivo, avisa y NO la inventa.

Uso:  python3 tools/aplicar_config.py
"""

import os
import re
import sys

RAIZ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# IP de la LAN elegida por el usuario
IP_LAN = "192.168.1.215"
IP_INTERNA = "127.0.0.1"   # los servidores hablan entre si en la misma maquina

VERSION_CLIENTE = "1.04.05"
SERIAL_CLIENTE = "RDDLVMuMiSiones9"


# =============================================================================
#  Motor de edicion
# =============================================================================

class Editor:
    """Edita archivos tipo INI/DAT clave = valor conservando todo lo demas."""

    def __init__(self, ruta):
        self.ruta = ruta
        self.abs = os.path.join(RAIZ, ruta)
        with open(self.abs, "rb") as fh:
            crudo = fh.read()
        # Los .dat traen restos de mojibake (EF BF BD). Se decodifica tolerante.
        self.texto = crudo.decode("utf-8", errors="replace")
        self.crlf = "\r\n" in self.texto
        self.cambios = []
        self.faltantes = []

    def set(self, clave, valor, nota="", normalizar_sep=False):
        """Reemplaza 'clave = valor' respetando la indentacion original.

        normalizar_sep=True fuerza el separador a ' = ' (un solo espacio),
        util cuando el archivo traia espacios de mas alrededor del igual.
        """
        patron = re.compile(
            r"^([ \t]*)(%s)([ \t]*=[ \t]*)(.*?)([ \t]*)$" % re.escape(clave),
            re.MULTILINE,
        )
        encontrado = {"n": 0, "antes": None}

        def _sub(m):
            encontrado["n"] += 1
            if encontrado["n"] > 1:          # solo la primera aparicion
                return m.group(0)
            encontrado["antes"] = m.group(4)
            sep = " = " if normalizar_sep else m.group(3)
            return "%s%s%s%s" % (m.group(1), m.group(2), sep, valor)

        nuevo = patron.sub(_sub, self.texto)
        if encontrado["n"] == 0:
            self.faltantes.append(clave)
            return False

        antes = encontrado["antes"]
        if antes.strip() != str(valor).strip():
            self.cambios.append((clave, antes.strip(), str(valor), nota))
        self.texto = nuevo
        return True

    def set_muchos(self, plantilla, valores, nota=""):
        """set() para familias _AL0.._AL3.  valores = lista de 4."""
        for i, v in enumerate(valores):
            self.set(plantilla % i, v, nota if i == 0 else "")

    def reemplazar_texto(self, viejo, nuevo):
        if viejo in self.texto:
            self.texto = self.texto.replace(viejo, nuevo)
            return True
        return False

    def limpiar_mojibake(self):
        """Quita los caracteres corruptos U+FFFD que arrastraban los .dat."""
        n = self.texto.count("\ufffd")
        if n:
            # Reemplazos de palabras portuguesas/espanolas rotas mas comunes
            arreglos = {
                "CONEXI\ufffdN": "CONEXION",
                "CL\ufffdSICO": "CLASICO",
                "CR\ufffdTICO": "CRITICO",
                "Ba\ufffdl": "Baul",
                "imped\ufffda": "impedia",
                "volve\ufffdas": "volvias",
                "seg\ufffdn": "segun",
                "N\ufffdmero": "Numero",
                "m\ufffdximo": "maximo",
                "m\ufffdnimo": "minimo",
                "n\ufffdvel": "nivel",
                "Transforma\ufffd\ufffdo": "Transformacion",
                "Voc\ufffd": "Voce",
                "come\ufffdar": "comecar",
                "Come\ufffdou": "Comecou",
                "n\ufffdo": "nao",
                "est\ufffd": "esta",
                "\ufffd": "",   # cualquier resto
            }
            for a, b in arreglos.items():
                self.texto = self.texto.replace(a, b)
            self.cambios.append(
                ("(encoding)", "%d bytes corruptos" % n, "limpiado a ASCII", "")
            )
        return n

    def guardar(self):
        salida = self.texto
        # se conserva el estilo de fin de linea original
        if self.crlf:
            salida = salida.replace("\r\n", "\n").replace("\n", "\r\n")
        with open(self.abs, "w", encoding="ascii", errors="replace",
                  newline="") as fh:
            fh.write(salida)

    def reporte(self):
        if not self.cambios and not self.faltantes:
            return ""
        out = ["\n### %s" % self.ruta]
        for clave, antes, despues, nota in self.cambios:
            linea = "  %-42s %s  ->  %s" % (clave, antes, despues)
            if nota:
                linea += "    ; %s" % nota
            out.append(linea)
        for clave in self.faltantes:
            out.append("  [!] clave no encontrada, omitida: %s" % clave)
        return "\n".join(out)


REPORTES = []


def procesar(ed):
    ed.guardar()
    r = ed.reporte()
    if r:
        REPORTES.append(r)


# =============================================================================
#  1. GameServer - Common.dat   (nucleo: red, exp, drop, economia)
# =============================================================================

def common():
    ed = Editor("GameServer/Data/GameServerInfo - Common.dat")
    ed.limpiar_mojibake()

    # --- Identidad y version -------------------------------------------------
    ed.set("ServerVersion", VERSION_CLIENTE, "debe coincidir con Maininfo y MHP")
    ed.set("ServerSerial", SERIAL_CLIENTE, "debe coincidir con Maininfo y MHP")

    # --- Nivel maximo --------------------------------------------------------
    # Estaba en 350 pero el Master Reset exige nivel 400 -> era imposible.
    # El source 97x define MAX_CHARACTER_LEVEL 350 y gLevelExperience[] tiene
    # 351 entradas. Un MaxUserLevel mayor hace leer fuera del array.
    ed.set("MaxUserLevel", "350", "tope real del source (MAX_CHARACTER_LEVEL)")
    ed.set("ServerMaxUserNumber", "100")

    # --- Conexiones (todo en la misma maquina) -------------------------------
    ed.set("DataServerAddress", IP_INTERNA)
    ed.set("JoinServerAddress", IP_INTERNA)
    ed.set("ConnectServerAddress", IP_INTERNA)
    ed.set("DataServerPort", "55960")
    ed.set("JoinServerPort", "55970")
    ed.set("ConnectServerPort", "55557")
    ed.set("ServerPort", "55901")

    # --- Antiflood -----------------------------------------------------------
    ed.set("MaxIpConnection", "4", "alineado con ConnectServer.ini")

    # --- EXPERIENCIA: perfil HARD / CLASICO ----------------------------------
    # Antes: 100 / 800 / 900 / 700  (el VIP mas caro rendia MENOS que el medio)
    # Ahora: progresion baja y estrictamente creciente por rango VIP.
    ed.set_muchos("AddExperienceRate_AL%d", [20, 30, 40, 50],
                  "HARD: era 100/800/900/700 (AL3 < AL2, incoherente)")
    ed.set_muchos("AddMasterExperienceRate_AL%d", [20, 30, 40, 50],
                  "estaban en 0: el master level no subia nunca")
    ed.set_muchos("AddEventExperienceRate_AL%d", [40, 50, 60, 70], "era 500 fijo")
    ed.set_muchos("AddQuestExperienceRate_AL%d", [40, 50, 60, 70], "era 500 fijo")
    # El modo offline siempre debe rendir menos que jugar online.
    ed.set_muchos("OffAttackExperienceRate_AL%d", [10, 15, 20, 25],
                  "offline ahora rinde la mitad que online")
    ed.set_muchos("AutoResetExperienceRate_AL%d", [20, 30, 40, 50])

    # --- DROP: perfil HARD ---------------------------------------------------
    ed.set_muchos("ItemDropRate_AL%d", [10, 12, 14, 16],
                  "HARD: era 50/60/60/70")
    ed.set_muchos("ExcellentItemDropRate_AL%d", [50, 60, 70, 80],
                  "HARD: era 2000 (=20%, excesivo)")
    ed.set_muchos("ExcItemDropRate_AL%d", [50, 60, 70, 80], "idem anterior")

    # --- ZEN -----------------------------------------------------------------
    ed.set_muchos("MoneyAmountDropRate_AL%d", [10, 12, 14, 16], "HARD: era 20/20/20/30")
    ed.set_muchos("MoneyValue_AL%d", [300, 350, 400, 450],
                  "HARD: era 1000 fijo; el zen debe costar")

    # --- Level up ------------------------------------------------------------
    ed.set("MaxLevelUp", "1", "HARD: un nivel por vez (era 5)")
    ed.set("MaxLevelUpEvent", "1")
    ed.set("MaxLevelUpQuest", "1")

    # --- Economia / comercio -------------------------------------------------
    ed.set("PersonalShopSwitch", "1",
           "estaba apagado: sin tiendas no hay economia entre jugadores")
    ed.set("TradeSwitch", "1")

    # --- PK clasico ----------------------------------------------------------
    ed.set("PKLimitCount", "6", "CLASICO: era 9999 (PK sin ninguna penalidad)")

    # --- Guild ---------------------------------------------------------------
    ed.set("GuildCreateMinLevel", "100")

    # --- Party ---------------------------------------------------------------
    ed.set("DifferenceMaxLevel", "350", "acompana el tope real del source")

    # --- Master Skill Tree: apagado a proposito en 97x -----------------------
    ed.set("MasterSkillTree", "0", "97x no tiene arbol de maestria")

    procesar(ed)


# =============================================================================
#  2. GameServer - Command.dat  (resets: el corazon del servidor)
# =============================================================================

def command():
    ed = Editor("GameServer/Data/GameServerInfo - Command.dat")
    ed.limpiar_mojibake()

    # --- Reset normal --------------------------------------------------------
    ed.set_muchos("CommandResetLevel_AL%d", [350, 350, 350, 350],
                  "HARD: reset recien en el nivel maximo real")
    ed.set_muchos("CommandResetStartLevel_AL%d", [1, 1, 1, 1])
    ed.set_muchos("CommandResetMoney_AL%d",
                  [50000000, 45000000, 40000000, 35000000],
                  "escalonado suave; antes AL3 pagaba 5 veces menos")
    ed.set_muchos("CommandResetCount_AL%d", [1, 1, 1, 1])
    ed.set_muchos("CommandResetLimit_AL%d", [400, 400, 400, 400])
    # Un servidor hard NO puede permitir resets infinitos por dia.
    ed.set_muchos("CommandResetLimitDay_AL%d", [3, 4, 5, 6],
                  "HARD: era 999 por dia (equivalente a sin limite)")
    ed.set_muchos("CommandResetLimitWek_AL%d", [21, 28, 35, 42], "coherente con el tope diario")
    ed.set_muchos("CommandResetLimitMon_AL%d", [90, 120, 150, 180], "coherente con el tope diario")
    # CommandResetPoint lo fija tools/aplicar_balance.py (150 por reset).
    # Antes se ponia en 0 = "puntos acumulativos", lo que hacia que el reset
    # fuera la unica via de progreso; el balance busca lo contrario.
    # No se toca aca para que los dos scripts no se pisen.

    # --- Master Reset --------------------------------------------------------
    ed.set_muchos("CommandMasterResetLevel_AL%d", [350, 350, 350, 350])
    ed.set_muchos("CommandMasterResetStartLevel_AL%d", [1, 1, 1, 1])
    ed.set_muchos("CommandMasterResetReset_AL%d", [100, 95, 90, 85],
                  "resets exigidos; antes AL3 necesitaba 30 menos")
    ed.set_muchos("CommandMasterResetRemoveReset_AL%d", [100, 95, 90, 85],
                  "debe coincidir con los resets exigidos")
    ed.set_muchos("CommandMasterResetMoney_AL%d",
                  [500000000, 450000000, 400000000, 350000000],
                  "el Master Reset era GRATIS (0 zen)")
    ed.set_muchos("CommandMasterResetPoint_AL%d", [500, 600, 700, 800],
                  "HARD: era 5000-8000 puntos, rompia el balance")
    ed.set_muchos("CommandMasterResetLimitDay_AL%d", [1, 1, 1, 1],
                  "HARD: era 10 por dia")
    ed.set_muchos("CommandMasterResetLimitWek_AL%d", [7, 7, 7, 7])
    ed.set_muchos("CommandMasterResetLimitMon_AL%d", [30, 30, 30, 30])

    # --- Clases 97x: DL / SU / RF no existen ---------------------------------
    ed.set("CommandChangeClassToDL", "0")
    ed.set("CommandChangeClassToSU", "0")
    ed.set("CommandResetPointRateDL", "0")
    ed.set("CommandResetPointRateSU", "0")
    ed.set("CommandResetPointRateRF", "0")

    # --- Baul extra por VIP --------------------------------------------------
    ed.set_muchos("CommandWareNumber_AL%d", [1, 2, 3, 5])

    # --- Casamiento ----------------------------------------------------------
    ed.set("CommandMarryLevel", "200")
    ed.set("CommandMarryCost", "20000000")

    # --- Textos de recompensa en espanol ------------------------------------
    ed.set("CommandRewardType1Text", "[%s] %s recibio %d Cash!")
    ed.set("CommandRewardType2Text", "[%s] %s recibio %d Gold!")
    ed.set("CommandRewardType3Text", "[%s] %s recibio %d PcPoint!")
    ed.set("CommandRewardAllType1Text", "[%s] %d Cash para todos!")
    ed.set("CommandRewardAllType2Text", "[%s] %d Gold para todos!")
    ed.set("CommandRewardAllType3Text", "[%s] %d PcPoint para todos!")

    procesar(ed)


# =============================================================================
#  3. GameServer - Custom.dat  (sistemas propios + textos al espanol)
# =============================================================================

def custom():
    ed = Editor("GameServer/Data/GameServerInfo - Custom.dat")
    ed.limpiar_mojibake()

    ed.set("DisabledSD", "1", "97x no usa Shield Defense")

    # Anuncio de +15 en el chaos mix: estaba apagado
    ed.set("AnnounceChaosMix", "1", "estaba apagado")
    ed.set("AnnounceChaosMixText", "%s logro crear el item %s +15!")

    # --- Arena: textos al espanol -------------------------------------------
    textos_arena = {
        "CustomArenaText1": "[%s] cierra en %d minuto(s)",
        "CustomArenaText2": "[%s] cerrada",
        "CustomArenaText3": "[%s] comienza en %d minuto(s)",
        "CustomArenaText4": "[%s] comenzo!",
        "CustomArenaText5": "[%s] finalizada",
        "CustomArenaText6": "[%s] %s es el ganador!",
        "CustomArenaText7": "[%s] Entraste con %d puntos",
        "CustomArenaText8": "No podes entrar a la arena",
        "CustomArenaText9": "Esta arena no esta abierta ahora",
        "CustomArenaText10": "Se supero el maximo de jugadores",
        "CustomArenaText11": "Necesitas ser nivel %d como minimo",
        "CustomArenaText12": "Tenes que ser nivel %d como maximo",
        "CustomArenaText13": "Necesitas Master Level %d como minimo",
        "CustomArenaText14": "Tenes que ser Master Level %d como maximo",
        "CustomArenaText15": "Necesitas %d resets como minimo",
        "CustomArenaText16": "Tenes que tener %d resets como maximo",
        "CustomArenaText17": "Necesitas %d Master Resets como minimo",
        "CustomArenaText18": "Tenes que tener %d Master Resets como maximo",
        "CustomArenaText19": "Tu clase no esta permitida en la arena",
    }
    for k, v in textos_arena.items():
        ed.set(k, v)

    ed.set("CustomEventDropText1", "[%s] comienza en %d minuto(s)")
    ed.set("CustomEventDropText2", "[%s] comenzo!")
    ed.set("CustomEventDropText3", "[%s] finalizado")

    ed.set("CustomOnlineLotteryText1", "%s en %d minuto(s)")
    ed.set("CustomOnlineLotteryText2", "%s gano el sorteo. Felicitaciones!")
    ed.set("CustomOnlineLotteryText3", "%s quedo sin ganador")

    # --- Off-store / off-attack: coherencia con PersonalShopSwitch = 1 -------
    ed.set_muchos("CustomStoreTime_AL%d", [0, 2, 4, 6],
                  "horas de tienda offline por rango VIP (estaba todo en 0)")
    ed.set_muchos("MuOffHelperTime_AL%d", [4, 6, 8, 10], "era 10 para todos")

    procesar(ed)


# =============================================================================
#  4. GameServer - Event.dat
# =============================================================================

def event():
    ed = Editor("GameServer/Data/GameServerInfo - Event.dat")
    ed.limpiar_mojibake()

    # Eventos base del 97x encendidos
    ed.set("BloodCastleEvent", "1")
    ed.set("ChaosCastleEvent", "1")
    ed.set("DevilSquareEvent", "1")
    ed.set("BloodCastleMaxUser", "10")
    ed.set("DevilSquareMaxUser", "15")
    ed.set("ChaosCastleMinUser", "2", "era 1: se podia ganar solo")

    # Eventos de temporadas posteriores: apagados (no existen en 97x)
    for clave in ("CrywolfEvent", "KanturuEvent", "RaklionEvent",
                  "IllusionTempleEvent", "ImperialGuardianEvent",
                  "CastleDeepEvent"):
        ed.set(clave, "0", "no pertenece al 97x")

    procesar(ed)


# =============================================================================
#  5. Cliente - Maininfo.ini   (el archivo mas critico para poder entrar)
# =============================================================================

def maininfo():
    ed = Editor("Ferramentas/MAIN_INFO/Maininfo.ini")
    ed.limpiar_mojibake()

    # >>> BUG CRITICO: version y serial traian espacios al final.
    #     El servidor compara byte a byte -> "version incorrecta" al conectar.
    ed.set("ClientVersion", VERSION_CLIENTE, "tenia un espacio al final (rechazo de login)", normalizar_sep=True)
    ed.set("ClientSerial", SERIAL_CLIENTE, "tenia un espacio al final (rechazo de login)", normalizar_sep=True)

    # IP de la LAN, sin el doble espacio que traia
    ed.set("IpAddress", IP_LAN, "tenia doble espacio delante", normalizar_sep=True)
    ed.set("IpAddressPort", "44405", "puerto TCP del ConnectServer")

    ed.set("CustomerName", "[Mu]Misiones")
    ed.set("WindowName", "[MU] Misiones", "tenia espacios sobrantes al final")
    ed.set("ClientName", "Main.exe")

    # Rendimiento
    ed.set("LimitFPS", "60", "estaba en 34, se veia con tirones")
    ed.set("MonitorFPS", "1")
    ed.set("MonitorMS", "1", "mostrar el ping ayuda a diagnosticar la LAN")

    # Reconexion coherente con el servidor
    ed.set("ReconnectTime", "300000")

    # Zona horaria de Argentina
    ed.set("TIME_GMT", "-03", "estaba en -05, la hora de los eventos no coincidia")

    # Nombres de servidor en la lista
    ed.set("ServerName1", "Server 1")
    ed.set("ServerName2", "Mantenimiento")
    ed.set("ServerName3", "Premium No-PVP")
    ed.set("ServerName4", "Server 1")

    # Coherencia con el 97x: sin master skill tree
    ed.set("DisableMasterSkillTree", "1")

    procesar(ed)


# =============================================================================
#  6. Resto de servidores: puertos e IPs coherentes
# =============================================================================

def otros_servidores():
    # --- ConnectServer -------------------------------------------------------
    ed = Editor("ConnectServer/ConnectServer.ini")
    ed.set("ConnectServerPortTCP", "44405")
    ed.set("ConnectServerPortUDP", "55557")
    ed.set("MaxIpConnection", "4", "era 3, alineado con el GameServer")
    procesar(ed)

    # --- DataServer ----------------------------------------------------------
    ed = Editor("DataServer/DataServer.ini")
    ed.set("DataServerODBC", "MuOnline")
    ed.set("DataServerPort", "55960")
    procesar(ed)

    # --- JoinServer ----------------------------------------------------------
    ed = Editor("JoinServer/JoinServer.ini")
    ed.set("JoinServerODBC", "MuOnline")
    ed.set("JoinServerPort", "55970")
    ed.set("ConnectServerAddress", IP_INTERNA)
    ed.set("ConnectServerPort", "55557")
    ed.set("CaseSensitive", "0")
    procesar(ed)

    # --- MHPServer (anticheat): version y serial IDENTICOS al cliente --------
    ed = Editor("MHPServern/MHPServer.ini")
    ed.set("ClientVersion", VERSION_CLIENTE)
    ed.set("ClientSerial", SERIAL_CLIENTE)
    ed.set("MHPServerPort", "55619")
    ed.set("MaxIpConnection", "4", "era 10, alineado con el resto")
    procesar(ed)


# =============================================================================
#  main
# =============================================================================

def main():
    print("=" * 78)
    print(" Actualizacion de configuracion [Mu]Misiones - perfil HARD / CLASICO")
    print(" LAN: %s   |   Version: %s" % (IP_LAN, VERSION_CLIENTE))
    print("=" * 78)

    common()
    command()
    custom()
    event()
    maininfo()
    otros_servidores()

    for r in REPORTES:
        print(r)

    total = sum(len(r.splitlines()) - 1 for r in REPORTES)
    print("\n" + "=" * 78)
    print(" Listo. %d ajustes aplicados en %d archivos." % (total, len(REPORTES)))
    print("=" * 78)
    return 0


if __name__ == "__main__":
    sys.exit(main())
