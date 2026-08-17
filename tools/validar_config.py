#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
validar_config.py - Chequeo de coherencia de la configuracion [Mu]Misiones
---------------------------------------------------------------------------
Verifica que los distintos servidores esten de acuerdo entre si ANTES de
levantarlos. Correlo cada vez que toques un .ini o un .dat a mano.

Uso:  python3 tools/validar_config.py
Sale con codigo 1 si hay algun ERROR.
"""

import os
import re
import sys

RAIZ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

ERRORES = []
AVISOS = []
OK = []


def leer(ruta):
    """Devuelve dict clave->valor de un archivo INI/DAT."""
    ruta_abs = os.path.join(RAIZ, ruta)
    if not os.path.exists(ruta_abs):
        ERRORES.append("Falta el archivo: %s" % ruta)
        return {}
    datos = {}
    with open(ruta_abs, "rb") as fh:
        texto = fh.read().decode("utf-8", errors="replace")
    for linea in texto.splitlines():
        limpia = linea.split(";")[0]
        if "=" not in limpia:
            continue
        k, _, v = limpia.partition("=")
        datos[k.strip()] = v.strip()
    return datos


def igualdad(nombre, pares):
    """pares = [(etiqueta, valor), ...] - todos deben coincidir."""
    valores = {v for _, v in pares}
    detalle = ", ".join("%s=%r" % (e, v) for e, v in pares)
    if len(valores) == 1:
        OK.append("%-34s coherente  (%s)" % (nombre, pares[0][1]))
    else:
        ERRORES.append("%s NO coincide entre archivos -> %s" % (nombre, detalle))


def sin_espacios(etiqueta, valor):
    if valor != valor.strip():
        ERRORES.append("%s tiene espacios sobrantes: %r" % (etiqueta, valor))
    elif "  " in valor:
        AVISOS.append("%s tiene espacios dobles: %r" % (etiqueta, valor))


def creciente(nombre, valores):
    """Los rangos VIP deben mejorar (o al menos no empeorar) de AL0 a AL3."""
    nums = [int(v) for v in valores]
    if nums != sorted(nums):
        ERRORES.append(
            "%s no es creciente por rango VIP: %s "
            "(un VIP mas alto rendiria menos)" % (nombre, nums)
        )
    else:
        OK.append("%-34s creciente  %s" % (nombre, nums))


def familia(datos, plantilla):
    return [datos.get(plantilla % i, "0") for i in range(4)]


def main():
    print("=" * 74)
    print(" Validacion de configuracion [Mu]Misiones")
    print("=" * 74)

    gs = leer("GameServer/Data/GameServerInfo - Common.dat")
    cmd = leer("GameServer/Data/GameServerInfo - Command.dat")
    cs = leer("ConnectServer/ConnectServer.ini")
    ds = leer("DataServer/DataServer.ini")
    js = leer("JoinServer/JoinServer.ini")
    mhp = leer("MHPServern/MHPServer.ini")
    main_ini = leer("Ferramentas/MAIN_INFO/Maininfo.ini")

    # --- 1. Version y serial: los 3 lados deben ser identicos ---------------
    igualdad("ClientVersion / ServerVersion", [
        ("GameServer", gs.get("ServerVersion", "")),
        ("Maininfo", main_ini.get("ClientVersion", "")),
        ("MHPServer", mhp.get("ClientVersion", "")),
    ])
    igualdad("ClientSerial / ServerSerial", [
        ("GameServer", gs.get("ServerSerial", "")),
        ("Maininfo", main_ini.get("ClientSerial", "")),
        ("MHPServer", mhp.get("ClientSerial", "")),
    ])

    # --- 2. Espacios invisibles (la causa clasica de "version incorrecta") --
    sin_espacios("Maininfo/ClientVersion", main_ini.get("ClientVersion", ""))
    sin_espacios("Maininfo/ClientSerial", main_ini.get("ClientSerial", ""))
    sin_espacios("Maininfo/IpAddress", main_ini.get("IpAddress", ""))

    # --- 3. Puertos: el cliente debe apuntar al ConnectServer ---------------
    igualdad("Puerto ConnectServer (TCP)", [
        ("ConnectServer.ini", cs.get("ConnectServerPortTCP", "")),
        ("Maininfo/IpAddressPort", main_ini.get("IpAddressPort", "")),
    ])
    igualdad("Puerto ConnectServer (UDP)", [
        ("ConnectServer.ini", cs.get("ConnectServerPortUDP", "")),
        ("GameServer", gs.get("ConnectServerPort", "")),
        ("JoinServer.ini", js.get("ConnectServerPort", "")),
    ])
    igualdad("Puerto DataServer", [
        ("DataServer.ini", ds.get("DataServerPort", "")),
        ("GameServer", gs.get("DataServerPort", "")),
    ])
    igualdad("Puerto JoinServer", [
        ("JoinServer.ini", js.get("JoinServerPort", "")),
        ("GameServer", gs.get("JoinServerPort", "")),
    ])

    # --- 4. Puertos unicos ---------------------------------------------------
    usados = {
        "ConnectServer TCP": cs.get("ConnectServerPortTCP"),
        "ConnectServer UDP": cs.get("ConnectServerPortUDP"),
        "DataServer": ds.get("DataServerPort"),
        "JoinServer": js.get("JoinServerPort"),
        "GameServer": gs.get("ServerPort"),
        "MHPServer": mhp.get("MHPServerPort"),
    }
    vistos = {}
    for nombre, puerto in usados.items():
        if puerto in vistos:
            ERRORES.append("Puerto %s duplicado entre %s y %s"
                           % (puerto, vistos[puerto], nombre))
        vistos[puerto] = nombre
    if len(vistos) == len(usados):
        OK.append("%-34s sin duplicados  %s"
                  % ("Puertos", sorted(int(p) for p in usados.values() if p)))

    # --- 5. Base de datos ----------------------------------------------------
    igualdad("Nombre ODBC de la base", [
        ("DataServer.ini", ds.get("DataServerODBC", "")),
        ("JoinServer.ini", js.get("JoinServerODBC", "")),
    ])

    # --- 6. Nivel maximo vs Master Reset ------------------------------------
    max_lvl = int(gs.get("MaxUserLevel", "0") or 0)
    mr_lvl = max(int(v) for v in familia(cmd, "CommandMasterResetLevel_AL%d"))
    r_lvl = max(int(v) for v in familia(cmd, "CommandResetLevel_AL%d"))
    if mr_lvl > max_lvl:
        ERRORES.append("MasterReset exige nivel %d pero MaxUserLevel es %d: "
                       "nadie podria hacerlo nunca" % (mr_lvl, max_lvl))
    else:
        OK.append("%-34s nivel %d <= MaxUserLevel %d"
                  % ("Master Reset alcanzable", mr_lvl, max_lvl))
    if r_lvl > max_lvl:
        ERRORES.append("Reset exige nivel %d pero MaxUserLevel es %d"
                       % (r_lvl, max_lvl))
    else:
        OK.append("%-34s nivel %d <= MaxUserLevel %d"
                  % ("Reset alcanzable", r_lvl, max_lvl))

    # --- 6b. El nivel maximo no puede superar el limite del SOURCE ----------
    # SORCE/GameServer/GameServer/User.h define MAX_CHARACTER_LEVEL 350 bajo
    # SEASON_97X, y gLevelExperience[] tiene MAX_CHARACTER_LEVEL+1 entradas.
    limite_source = 350
    ruta_userh = os.path.join(RAIZ, "SORCE/GameServer/GameServer/User.h")
    if os.path.exists(ruta_userh):
        with open(ruta_userh, "rb") as fh:
            txt = fh.read().decode("utf-8", errors="replace")
        m = re.search(r"#ifdef\s+SEASON_97X\s*\n\s*#define\s+MAX_CHARACTER_LEVEL\s+(\d+)", txt)
        if m:
            limite_source = int(m.group(1))
    if max_lvl > limite_source:
        ERRORES.append("MaxUserLevel %d supera MAX_CHARACTER_LEVEL %d del source: "
                       "el servidor leeria fuera de gLevelExperience[]"
                       % (max_lvl, limite_source))
    else:
        OK.append("%-34s %d <= %d (limite del source)"
                  % ("MaxUserLevel dentro del source", max_lvl, limite_source))

    # --- 7. Progresion VIP coherente ----------------------------------------
    creciente("Experiencia por rango VIP", familia(gs, "AddExperienceRate_AL%d"))
    creciente("Drop de items por rango VIP", familia(gs, "ItemDropRate_AL%d"))
    creciente("Drop excelente por rango VIP",
              familia(gs, "ExcellentItemDropRate_AL%d"))
    creciente("Resets diarios por rango VIP",
              familia(cmd, "CommandResetLimitDay_AL%d"))

    # El costo del reset deberia BAJAR a medida que sube el VIP
    costos = [int(v) for v in familia(cmd, "CommandResetMoney_AL%d")]
    if costos != sorted(costos, reverse=True):
        AVISOS.append("El costo del reset no baja con el rango VIP: %s" % costos)
    else:
        OK.append("%-34s decreciente %s" % ("Costo de reset por VIP", costos))

    # --- 8. Master Reset no puede ser gratis --------------------------------
    mr_money = [int(v) for v in familia(cmd, "CommandMasterResetMoney_AL%d")]
    if all(m == 0 for m in mr_money):
        AVISOS.append("El Master Reset no cuesta nada de zen")
    else:
        OK.append("%-34s %s" % ("Master Reset con costo", mr_money))

    # --- 9. Offline no debe rendir mas que online ---------------------------
    on = [int(v) for v in familia(gs, "AddExperienceRate_AL%d")]
    off = [int(v) for v in familia(gs, "OffAttackExperienceRate_AL%d")]
    if any(o > n for o, n in zip(off, on)):
        ERRORES.append("La exp offline supera a la online: off=%s on=%s "
                       "(conviene no jugar)" % (off, on))
    else:
        OK.append("%-34s off=%s <= on=%s" % ("Offline por debajo de online",
                                             off, on))

    # --- 10. Encoding: los .dat deben ser ASCII limpio ----------------------
    for ruta in [
        "GameServer/Data/GameServerInfo - Common.dat",
        "GameServer/Data/GameServerInfo - Command.dat",
        "GameServer/Data/GameServerInfo - Custom.dat",
        "GameServer/Data/GameServerInfo - Event.dat",
        "Ferramentas/MAIN_INFO/Maininfo.ini",
    ]:
        with open(os.path.join(RAIZ, ruta), "rb") as fh:
            datos = fh.read()
        malos = [b for b in datos if b > 127]
        if malos:
            AVISOS.append("%s tiene %d bytes no-ASCII (riesgo de mojibake)"
                          % (ruta, len(malos)))
    if not any("no-ASCII" in a for a in AVISOS):
        OK.append("%-34s todos los archivos en ASCII" % "Encoding")

    # --- Reporte -------------------------------------------------------------
    print("\n-- Comprobaciones correctas -----------------------------------")
    for o in OK:
        print("  [ok]  %s" % o)

    if AVISOS:
        print("\n-- Avisos -----------------------------------------------------")
        for a in AVISOS:
            print("  [!]   %s" % a)

    if ERRORES:
        print("\n-- ERRORES ----------------------------------------------------")
        for e in ERRORES:
            print("  [XX]  %s" % e)

    print("\n" + "=" * 74)
    print(" %d correctas | %d avisos | %d errores"
          % (len(OK), len(AVISOS), len(ERRORES)))
    print("=" * 74)
    return 1 if ERRORES else 0


if __name__ == "__main__":
    sys.exit(main())
