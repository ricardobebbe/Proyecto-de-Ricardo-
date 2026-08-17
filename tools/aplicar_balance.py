#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
aplicar_balance.py - Balance de combate y progresion [Mu]Misiones (97x)
------------------------------------------------------------------------
Objetivo pedido:
  * Menos dependencia del reset  -> mas puntos por nivel, menos por reset.
  * Personaje mas fuerte/aguanta -> mas vida base y por vitalidad.
  * Personaje mas lento          -> sube el divisor de velocidad.
  * Que pegue menos / controlado -> baja los rate porcentuales de dano.

CRITERIO IMPORTANTE
-------------------
Las constantes ConstA/ConstB del Character.dat son DIVISORES ENTEROS
(el source hace: dano = Fuerza / ConstA). Como valen entre 3 y 9, subirlas
en 1 ya implica bajar el dano entre 11% y 25% de golpe: no permiten el
ajuste fino de 15-30% que se pedia.

Por eso el dano se ajusta con los RATE PORCENTUALES, que son exactos:
  GeneralDamageRatePvM / PvP y los  <CLASE>DamageRatePvM / PvP.

Las constantes solo se tocan donde el paso entero es razonable:
  * velocidad (divisores grandes: 9 a 50)
  * defensa   (bajarlas SUBE la defensa, que es lo que se busca)

Ninguna constante puede quedar en 0: son divisores y provocarian
division por cero. El script lo verifica al final.

Uso:  python3 tools/aplicar_balance.py
"""

import os
import re
import sys

RAIZ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

CLASES = ["DW", "DK", "FE", "MG", "DL", "SU", "RF"]

REPORTES = []


class Editor:
    def __init__(self, ruta):
        self.ruta = ruta
        self.abs = os.path.join(RAIZ, ruta)
        with open(self.abs, "rb") as fh:
            self.texto = fh.read().decode("utf-8", errors="replace")
        self.cambios = []
        self.faltantes = []

    def get(self, clave):
        m = re.search(r"^[ \t]*%s[ \t]*=[ \t]*(.*?)[ \t]*$" % re.escape(clave),
                      self.texto, re.M)
        return m.group(1).strip() if m else None

    def set(self, clave, valor, nota=""):
        patron = re.compile(
            r"^([ \t]*)(%s)([ \t]*=[ \t]*)(.*?)([ \t]*)$" % re.escape(clave), re.M)
        hallado = {"n": 0, "antes": None}

        def _sub(m):
            hallado["n"] += 1
            if hallado["n"] > 1:
                return m.group(0)
            hallado["antes"] = m.group(4)
            return "%s%s%s%s" % (m.group(1), m.group(2), m.group(3), valor)

        nuevo = patron.sub(_sub, self.texto)
        if hallado["n"] == 0:
            self.faltantes.append(clave)
            return False
        if hallado["antes"].strip() != str(valor).strip():
            self.cambios.append((clave, hallado["antes"].strip(), str(valor), nota))
        self.texto = nuevo
        return True

    def guardar(self):
        with open(self.abs, "w", encoding="ascii", errors="replace",
                  newline="") as fh:
            fh.write(self.texto)

    def reporte(self):
        if not self.cambios and not self.faltantes:
            return ""
        out = ["\n### %s" % self.ruta]
        for k, a, d, n in self.cambios:
            l = "  %-32s %s -> %s" % (k, a, d)
            if n:
                l += "   ; %s" % n
            out.append(l)
        for k in self.faltantes:
            out.append("  [!] no encontrada: %s" % k)
        return "\n".join(out)


def procesar(ed):
    ed.guardar()
    r = ed.reporte()
    if r:
        REPORTES.append(r)


# =============================================================================
#  1. Character.dat - dano, velocidad y defensa
# =============================================================================

def character():
    ed = Editor("GameServer/Data/GameServerInfo - Character.dat")

    # --- Dano global: aca si se puede ser preciso ---------------------------
    ed.set("GeneralDamageRatePvM", "80", "PvM mas suave (era 100)")
    ed.set("GeneralDamageRatePvP", "70", "PvP mas controlado (era 100)")

    # --- Dano por clase ------------------------------------------------------
    for c in CLASES:
        ed.set("%sDamageRatePvM" % c, "85")
    for c in CLASES:
        # RF ya venia en 95: se lleva al mismo piso que el resto.
        ed.set("%sDamageRatePvP" % c, "75")

    # --- Velocidad: personaje mas lento -------------------------------------
    # Son divisores (Destreza / ConstA): subirlos = mas lento.
    # Se usan valores ABSOLUTOS de destino (no un porcentaje sobre el actual),
    # para que volver a correr el script no siga frenando al personaje.
    VELOCIDAD_DESTINO = {
        "DWPhysiSpeedConstA": 22, "DWMagicSpeedConstA": 11,
        "DKPhysiSpeedConstA": 16, "DKMagicSpeedConstA": 22,
        "FEPhysiSpeedConstA": 55, "FEMagicSpeedConstA": 55,
        "MGPhysiSpeedConstA": 16, "MGMagicSpeedConstA": 22,
        "DLPhysiSpeedConstA": 11, "DLMagicSpeedConstA": 11,
        "SUPhysiSpeedConstA": 22, "SUMagicSpeedConstA": 22,
        "RFPhysiSpeedConstA": 10, "RFMagicSpeedConstA": 10,
    }
    for clave, destino in sorted(VELOCIDAD_DESTINO.items()):
        actual = ed.get(clave)
        if actual is None:
            ed.faltantes.append(clave)
            continue
        try:
            v = int(actual)
        except ValueError:
            continue
        if v <= 0:
            ed.set(clave, "1", "estaba en %s: DIVISION POR CERO" % actual)
            continue
        if v == destino:
            continue
        ed.set(clave, str(destino),
               "mas lento (-%d%% de velocidad)" % round((1 - v / float(destino)) * 100))

    # --- Defensa: aguantar mas ----------------------------------------------
    # Son divisores: BAJARLOS sube la defensa. Con valores chicos (3 o 4) un
    # -1 dispara la defensa un 33-50%, demasiado brusco, asi que esas clases
    # se dejan como estan y el aguante se consigue por vida (DefaultClassInfo).
    DEFENSA_DESTINO = {
        "FEDefenseConstA": 9,   # 10 -> 9   (+11%)
        "DLDefenseConstA": 6,   #  7 -> 6   (+17%)
        "RFDefenseConstA": 7,   #  8 -> 7   (+14%)
    }
    for c in CLASES:
        clave = "%sDefenseConstA" % c
        actual = ed.get(clave)
        if actual is None:
            ed.faltantes.append(clave)
            continue
        try:
            v = int(actual)
        except ValueError:
            continue
        if clave not in DEFENSA_DESTINO:
            continue
        destino = DEFENSA_DESTINO[clave]
        if v == destino or destino <= 0:
            continue
        ed.set(clave, str(destino),
               "mas defensa (+%d%%)" % round((v / float(destino) - 1) * 100))

    procesar(ed)


# =============================================================================
#  2. Common.dat - progresion por nivel en vez de por reset
# =============================================================================

def common():
    ed = Editor("GameServer/Data/GameServerInfo - Common.dat")

    # Mas puntos por nivel: el personaje crece jugando, no reseteando.
    # Nunca BAJAR los puntos que ya daba una clase: MG, DL y RF venian con 7.
    for c in CLASES:
        clave = "%sLevelUpPoint" % c
        actual = ed.get(clave)
        try:
            v = int(actual)
        except (TypeError, ValueError):
            v = 0
        nuevo = max(v, 6)
        ed.set(clave, str(nuevo),
               "mas poder por nivel" if nuevo != v else "ya estaba por encima")

    ed.set("PlusStatPoint", "2", "bonus por quest de nivel 220+")

    procesar(ed)


# =============================================================================
#  3. Command.dat - menos puntos por reset
# =============================================================================

def command():
    ed = Editor("GameServer/Data/GameServerInfo - Command.dat")

    # El pedido sugiere 150 por reset. En este server los resets dan puntos
    # ACUMULATIVOS (CommandResetPoint = 0 significa "usar el acumulado"),
    # asi que se fija un valor explicito y moderado.
    for i in range(4):
        ed.set("CommandResetPoint_AL%d" % i, "150",
               "puntos fijos por reset (antes acumulativo sin tope)" if i == 0 else "")

    procesar(ed)


# =============================================================================
#  4. DefaultClassInfo.txt - mas vida base y por vitalidad
# =============================================================================

def default_class_info():
    """Mas vida base y por vitalidad.

    Formato de columnas, admite decimales (el source usa GetAsFloatNumber):
      Class Str Dex Vit Ene Lea MaxLife MaxMana LevelLife LevelMana
            VitalityToLife EnergyToMana

    IDEMPOTENTE: se escriben valores ABSOLUTOS de destino, calculados una vez
    sobre los valores originales del serverfile (+15% vida base, +15% vida por
    nivel, +10% vida por punto de vitalidad). Si se aplicara un porcentaje
    sobre el valor actual, cada corrida volveria a inflar la vida.
    """
    ruta = "Data/Character/DefaultClassInfo.txt"
    abs_ruta = os.path.join(RAIZ, ruta)

    # clase -> (MaxLife, LevelLife, VitalityToLife) de destino
    DESTINO = {
        0: (69, "1.15", "2.2"),    # Dark Wizard      60  / 1.0 / 2.0
        1: (127, "2.3", "3.3"),    # Dark Knight      110 / 2.0 / 3.0
        2: (92, "1.15", "2.2"),    # Fairy Elf        80  / 1.0 / 2.0
        3: (127, "1.15", "2.2"),   # Magic Gladiator  110 / 1.0 / 2.0
        4: (104, "1.73", "2.2"),   # Dark Lord        90  / 1.5 / 2.0
        5: (81, "1.15", "2.2"),    # Summoner         70  / 1.0 / 2.0
        6: (115, "1.5", "2.2"),    # Rage Fighter     100 / 1.3 / 2.0
    }
    NOMBRES = {0: "Dark Wizard", 1: "Dark Knight", 2: "Fairy Elf",
               3: "Magic Gladiator", 4: "Dark Lord", 5: "Summoner",
               6: "Rage Fighter"}

    with open(abs_ruta, "rb") as fh:
        texto = fh.read().decode("utf-8", errors="replace")

    salida = []
    cambios = []

    for linea in texto.splitlines():
        limpia = linea.strip()
        if (not limpia or limpia.startswith("//") or limpia == "end"
                or not limpia[0].isdigit()):
            salida.append(linea)
            continue

        campos = limpia.split()
        if len(campos) < 12:
            salida.append(linea)
            continue

        clase = int(campos[0])
        if clase not in DESTINO:
            salida.append(linea)
            continue

        vida_d, vida_nivel_d, vit_vida_d = DESTINO[clase]

        antes = (campos[6], campos[8], campos[10])
        despues = (str(vida_d), vida_nivel_d, vit_vida_d)

        if antes == despues:
            salida.append(linea)
            continue

        campos[6], campos[8], campos[10] = despues

        salida.append("%-9s %-10s %-11s %-10s %-8s %-12s %-9s %-9s %-11s "
                      "%-11s %-16s %s" % tuple(campos[:12]))

        cambios.append("  %-16s vida %s->%s  vida/nivel %s->%s  vit->vida %s->%s"
                       % (NOMBRES[clase], antes[0], despues[0],
                          antes[1], despues[1], antes[2], despues[2]))

    with open(abs_ruta, "w", encoding="ascii", errors="replace",
              newline="") as fh:
        fh.write("\n".join(salida) + "\n")

    if cambios:
        REPORTES.append("\n### %s\n%s" % (ruta, "\n".join(cambios)))


# =============================================================================
#  5. Verificacion: ningun divisor puede quedar en 0
# =============================================================================

def verificar_divisores():
    ruta = "GameServer/Data/GameServerInfo - Character.dat"
    with open(os.path.join(RAIZ, ruta), "rb") as fh:
        texto = fh.read().decode("utf-8", errors="replace")

    malos = []
    for m in re.finditer(r"^[ \t]*(\w*Const[AB])[ \t]*=[ \t]*(-?\d+)[ \t]*$",
                         texto, re.M):
        clave, valor = m.group(1), int(m.group(2))
        if valor <= 0:
            malos.append((clave, valor))

    print("\n" + "=" * 74)
    if malos:
        print(" ERROR: hay divisores en cero o negativos (causarian crash):")
        for k, v in malos:
            print("   %-34s = %d" % (k, v))
        return False

    print(" Verificacion: todas las constantes ConstA/ConstB son > 0")
    return True


def main():
    print("=" * 74)
    print(" Balance de combate y progresion - [Mu]Misiones 97x")
    print("=" * 74)

    character()
    common()
    command()
    default_class_info()

    for r in REPORTES:
        print(r)

    ok = verificar_divisores()
    print("=" * 74)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
