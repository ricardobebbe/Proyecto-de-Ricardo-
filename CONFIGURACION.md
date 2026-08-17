# Actualización de configuración — [Mu]Misiones (97x / MuEMU S6)

**Perfil aplicado:** HARD / clásico · **Red:** LAN `192.168.1.215` · **Fecha:** 2026-08-16

---

## 1. Resumen

Se revisaron y corrigieron los **7 archivos de configuración** del GameServer, el
`Maininfo.ini` del cliente y los `.ini` de ConnectServer, DataServer, JoinServer y
MHPServer. En total **158 ajustes**, incluidos **3 bugs que impedían jugar**.

Todo se aplica con un script reproducible:

```bash
python3 tools/aplicar_config.py    # aplica la configuración (idempotente)
python3 tools/validar_config.py    # verifica coherencia entre servidores
```

---

## 2. Bugs críticos corregidos

### 2.1 El cliente no podía conectarse (espacios invisibles)

`Maininfo.ini` tenía un **espacio al final** de dos valores que el servidor compara
byte a byte:

```ini
ClientVersion = 1.04.05          ← "1.04.05 " con espacio
ClientSerial  = RDDLVMuMiSiones9 ← "RDDLVMuMiSiones9 " con espacio
IpAddress     =  192.168.1.215   ← doble espacio
```

El GameServer recibía `"1.04.05 "` y lo comparaba contra `"1.04.05"` → **rechazo de
login "versión incorrecta"**, sin ningún mensaje útil en el log. Es el error más
difícil de encontrar porque el archivo *se ve* bien.

### 2.2 El Master Reset era matemáticamente imposible

```
MaxUserLevel               = 350   ← techo del servidor
CommandMasterResetLevel_AL = 400   ← nivel exigido para el MReset
```

Ningún personaje podía llegar a 400 porque el techo era 350. **El sistema de Master
Reset estaba muerto.** Se subió `MaxUserLevel` a 400.

### 2.3 El Master Level nunca subía

```
AddMasterExperienceRate_AL0..AL3 = 0
```

Con rate 0 la experiencia de maestría **nunca se acumulaba**. Se puso 20/30/40/50.

---

## 3. Incoherencias de balance corregidas

### 3.1 El VIP más caro rendía menos que el intermedio

| Rango | Antes | Ahora |
|---|---|---|
| AL0 (free) | 100 | 20 |
| AL1 | 800 | 30 |
| AL2 | **900** | 40 |
| AL3 (el más caro) | **700** ← menos que AL2 | 50 |

Un jugador que pagaba el VIP más caro **ganaba menos experiencia** que uno del rango
inferior. Ahora la progresión es estrictamente creciente.

### 3.2 Jugar offline rendía más que jugar

```
AddExperienceRate_AL0       = 100   ← jugando de verdad
OffAttackExperienceRate_AL2 = 800   ← con el personaje en modo offline
```

Convenía dejar el personaje en off-attack antes que jugar. Ahora el offline rinde
siempre la mitad que el online.

### 3.3 Master Reset gratis y sobre-recompensado

```
CommandMasterResetMoney_AL* = 0      ← gratis
CommandMasterResetPoint_AL* = 5000-8000
CommandMasterResetLimitDay  = 10     ← 10 por día
```

Se podían hacer 10 Master Resets diarios, gratis, cobrando hasta 8.000 puntos cada
uno = **80.000 puntos por día sin gastar nada**. Ahora: 1 por día, con costo de zen,
y 500-800 puntos.

### 3.4 Resets sin límite real

`CommandResetLimitDay = 999` es lo mismo que no tener límite. Perfil hard: 3-6 por
día según VIP.

---

## 4. Perfil HARD aplicado

| Parámetro | Antes | Ahora (AL0→AL3) |
|---|---|---|
| Experiencia | 100/800/900/700 | **20 / 30 / 40 / 50** |
| Exp. master | 0 | 20 / 30 / 40 / 50 |
| Exp. offline | 100/500/800/800 | 10 / 15 / 20 / 25 |
| Drop de items | 50/60/60/70 | **10 / 12 / 14 / 16** |
| Drop excelente | 2000 (20%) | **50 / 60 / 70 / 80** |
| Zen por drop | 1000 fijo | 300 / 350 / 400 / 450 |
| Niveles por golpe | 5 | **1** |
| Resets por día | 999 | 3 / 4 / 5 / 6 |
| Nivel para resetear | 350 | **400** |
| Master Resets por día | 10 | **1** |
| Límite de PK | 9999 | **6** |

---

## 5. Otros cambios

- **Tiendas personales activadas** (`PersonalShopSwitch = 0 → 1`). Estaban apagadas,
  o sea que no existía economía entre jugadores. Se acompañó con horas de tienda
  offline escalonadas por VIP (0/2/4/6 h).
- **Zona horaria a `-03`** (estaba en `-05`): los horarios de eventos no coincidían
  con la hora de Argentina.
- **FPS de 34 → 60**: el cliente estaba limitado a 34 FPS y se veía con tirones.
- **Chaos Castle**: mínimo 2 jugadores (estaba en 1, se podía "ganar" solo).
- **Eventos que no existen en 97x apagados**: Crywolf, Kanturu, Raklion, Illusion
  Temple, Imperial Guardian, Castle Deep.
- **Clases DL / SU / RF desactivadas** en los comandos de cambio de clase: no existen
  en el 97x.
- **Textos traducidos** del portugués al español (arena, sorteos, recompensas,
  anuncios de +15) — ~35 mensajes.
- **Encoding reparado**: los `.dat` arrastraban 49 bytes corruptos (`CONEXIÓN`
  aparecía como `CONEXI<?>N`). Todo quedó en ASCII limpio.
- **Puertos e IPs unificados** entre los 5 servidores, sin duplicados.

---

## 6. Mapa de puertos

| Servicio | Puerto | Escucha |
|---|---|---|
| ConnectServer TCP | **44405** | público (es el que pone el cliente) |
| ConnectServer UDP | 55557 | interno |
| MHPServer (anticheat) | 55619 | público |
| GameServer | 55901 | interno |
| DataServer | 55960 | interno |
| JoinServer | 55970 | interno |

En el **firewall de Windows** abrí solo `44405/TCP` y `55619/TCP`. El resto es
comunicación entre procesos de la misma máquina.

---

## 7. Cómo instalarlo

1. **Frená todos los servidores** (ServerStartUp).
2. Hacé una copia de tu carpeta actual por las dudas.
3. Descomprimí `MuMisiones-Config-HARD-LAN.zip` sobre la raíz de tus serverfiles,
   respetando las rutas.
4. Verificá: `python3 tools/validar_config.py` (o revisá a mano el punto 6).
5. Levantá los servidores **en este orden**: DataServer → JoinServer →
   ConnectServer → GameServer → MHPServer.

### En las PCs cliente

El `Maininfo.ini` **no va en la PC del jugador**: se compila dentro de `main.exe`
con `GetMainInfo-Premium.exe` (en `Ferramentas/`). Después de cambiarlo:

1. Abrí `Ferramentas/GetMainInfo-Premium.exe`.
2. Cargá el `Maininfo.ini` actualizado.
3. Generá el `main.exe` nuevo y repartilo a los jugadores.

Si los clientes ya tienen el `main.exe` viejo, **van a seguir apuntando a la IP y
versión anteriores**.

---

## 8. Verificación automática

`tools/validar_config.py` chequea 18 condiciones y falla si algo no cierra:

- versión y serial idénticos en GameServer / Maininfo / MHPServer
- espacios invisibles en los valores críticos
- puertos coherentes entre archivos y sin duplicados
- mismo nombre de base ODBC en Data y Join
- que el nivel de reset y master reset sea alcanzable
- que la progresión VIP sea creciente (exp, drop, resets)
- que el costo del reset baje al subir de VIP
- que la exp offline no supere a la online
- que no queden bytes no-ASCII

Estado actual: **18 correctas, 0 avisos, 0 errores**.

---

## 9. Ajustes que quedan pendientes (no los toqué)

- **`JoinServer.ini`**: `GlobalPassword = DEuSFIeL7` está en texto plano y
  `MD5Encryption = 0`. Si el servidor sale a internet, conviene cambiar la clave y
  evaluar MD5 (ojo: activarlo obliga a migrar las contraseñas ya existentes).
- **`web/Configuraciones/SQL.php`**: usuario `sa` con contraseña `1234` commiteada
  en el repositorio.
- **`Data/Custom/CustomBuyVip.txt`**: los VIP dan +10/+20/+30 de exp y drop, que se
  suma a los rates nuevos. Revisalo si querés que el perfil hard se mantenga con VIP.
- **Volcados de memoria**: hay dos `.dmp` en `GameServer/` (crashes del 9 y 10 de
  agosto) que convendría analizar.
