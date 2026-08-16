# Balance de GameServer y correcciones de C++ — [Mu]Misiones (97x)

**Fecha:** 2026-08-16 · **Perfil:** menos reset, más fuerte, más lento, daño controlado

---

## 1. Lo primero: qué del pedido NO hacía falta

Verifiqué cada punto contra el código real antes de tocar nada. Tres de las
correcciones "obligatorias" del documento **no correspondían a este repositorio**:

| Pedido | Realidad verificada |
|---|---|
| §3.1–3.3 Reemplazar `& &` por `&&` | **0 ocurrencias** de `& &` en todo el source (`grep -rn '& &' *.cpp *.h`). Era un artefacto de pegar el código en un documento de texto, no un bug del repo. Las líneas citadas ya estaban correctas. |
| §3.4 Llamar `gObjSetExperienceTable()` en `ReadUtilInfo` | Ya se llama, en `GameMain.cpp:55`, **después** de `gServerInfo.ReadInit()` (línea 49). El orden es correcto. Agregarla otra vez habría duplicado el trabajo. |
| §3.5 Llamar `SetMasterLevelExperienceTable()` en `ReadSkillInfo` | Ya se llama, en `GameMain.cpp:202`. |
| §14 "faltan SkillManager/SkillDamage" | **Sí están** en el repo (`SkillManager.cpp/.h`, `SkillDamage.cpp/.h`). |

Si hubiera aplicado §3.4 y §3.5 tal cual, el resultado no sería incorrecto pero sí
redundante. Lo importante es que **el diagnóstico de fondo era acertado**: la tabla de
experiencia efectivamente estaba rota, sólo que por otra causa.

---

## 2. Los bugs reales que sí encontré

### 2.1 🔴 `CheckMasterLevel()` sin `return` — comportamiento indefinido

`MasterSkillTree.cpp`. El cuerpo entero de la función estaba dentro de
`#if(GAMESERVER_UPDATE>=401)`, pero **este build compila con `GAMESERVER_UPDATE 100`**
(`stdafx.h:16`). Resultado: la función quedaba literalmente vacía.

```cpp
bool CMasterSkillTree::CheckMasterLevel(LPOBJ lpObj)
{
    #if(GAMESERVER_UPDATE>=401)
    ...todos los return...
    #endif
}   // <-- ningún return: devuelve basura del stack
```

Compilado con `-Wall` esto da `warning: no return statement in function returning
non-void`. Es **undefined behavior**: el valor devuelto es lo que quedó en el registro.

Por qué importa tanto: `CharacterLevelUp()` decide con este valor si el personaje sube
**nivel normal** o **master level**. Con un retorno impredecible, el servidor podía
mandar a un personaje común por la rama de master level, donde `MasterNextExperience`
vale 0 → **la barra de experiencia se traba**. Es la causa más probable del síntoma que
reportaste.

**Corregido:** `return` garantizado en todos los caminos, y una rama `#else` explícita
para builds sin master skill tree.

### 2.2 🔴 La tabla de experiencia desbordaba en el nivel 204

`User.cpp`, `gObjSetExperienceTable()`. La fórmula 97x acumula `n³ × 10` en un `DWORD`
de 32 bits:

```
Total teórico a nivel 350 : 37.730.306.250
Máximo de un DWORD        :  4.294.967.295
```

A partir del nivel 204 el valor **daba la vuelta**:

| Nivel | Experiencia requerida |
|---|---|
| 203 | 4.287.384.360 |
| 204 | **77.313.704** ← menos que el nivel anterior |

La tabla dejaba de crecer en **8 puntos** (204, 242, 268, 288, 304, 319, 331, 342).
Verificado numéricamente: antes **8 niveles con la tabla decreciendo**, ahora **0**.

Además, la rama Season 6+ usaba `gLevelExperience[n] += ...` (sobre memoria sin
inicializar) en vez de `=` sobre el nivel anterior.

**Corregido:** acumulación en `QWORD` (64 bits) con saturación en el máximo del `DWORD`,
de modo que la tabla nunca decrece ni vale 0.

### 2.3 🔴 Se descartaba la experiencia sobrante en cada level up

`ObjectManager.cpp`, `CharacterLevelUp()`. Dentro del bucle de **nivel normal** había
este corte:

```cpp
if (lpObj->MasterLevel >= gServerInfo.m_MasterSkillTreeMaxLevel)
{
    AddExperience = 0;   // <-- tira toda la experiencia restante
    break;
}
```

Con el master desactivado, `MasterLevel = 0` y `MasterSkillTreeMaxLevel = 0`, así que
`0 >= 0` es **siempre verdadero**. El personaje subía **un solo nivel** y el resto de la
experiencia se perdía. Esa comprobación pertenece al bucle de master level, no al de
nivel normal.

Medido con simulación (`MaxLevelUp = 5`):

| Escenario | Antes | Ahora |
|---|---|---|
| Un golpe de 500.000 exp desde nivel 1 | nivel **2** | nivel **6** |
| Kills chicos (200 exp × 5000) | nivel 25 | nivel 25 |
| Kills grandes (100.000 exp × 2000) | nivel 94 | nivel 95 |
| Premios de evento (5.000.000 × 300) | nivel **143** | nivel **156** |

El impacto **escala con el tamaño del premio**: casi invisible matando bichos chicos,
brutal con recompensas de evento o quest. Coincide con "la barra se traba".

### 2.4 🟠 `gObjCalcExperience()` leía `gLevelExperience[-1]`

Con `Level = 0` la expresión `gLevelExperience[lpObj->Level - 1]` indexa **antes del
array**. `gObjCharZeroSet()` deja `Level = 0`, así que el caso era alcanzable.
**Corregido** con acotación del nivel al rango válido.

### 2.5 🟠 Posible bucle sin corte con `MaxLevelUp <= 0`

El bucle usaba `(--MaxLevelUp) == 0` como condición de salida. Con `MaxLevelUp = 0` (el
valor por defecto que leía el `.dat`), el contador se volvía negativo y **nunca llegaba
a 0**. **Corregido** con un contador propio validado.

### 2.6 🟠 `AccountLevel` sin validar como índice de array

`ResetTable.cpp`. `Level[]`, `Money[]` y `Point[]` tienen tamaño `MAX_ACCOUNT_LEVEL`
(4), y se indexaban con `lpObj->AccountLevel` **directo de la base de datos**, sin
comprobar rango. **Corregido** en las 3 funciones con la variable acotada `accountLevel`
(esta parte sí estaba bien identificada en §3.1 del pedido).

### 2.7 🟠 Validaciones de configuración (§3.6–3.8 del pedido)

Aplicadas en `ServerInfo.cpp`: `MaxUserLevel`, `MaxLevelUp`, `MaxLevelUpEvent`,
`MaxLevelUpQuest` y `MasterSkillTreeMaxLevel` ahora se validan al cargar, con aviso en
el log si el `.dat` trae un valor imposible.

---

## 3. ⚠️ Corregí un error MÍO del turno anterior

El turno pasado puse **`MaxUserLevel = 400`** razonando que el Master Reset pedía nivel
400. **Estaba mal.** Al leer el source:

```c
#ifdef SEASON_97X
#define MAX_CHARACTER_LEVEL 350
```

y `gLevelExperience[]` se declara con `MAX_CHARACTER_LEVEL + 1` entradas (**0 a 350**).
Con `MaxUserLevel = 400`, `CharacterLevelUp` habría dejado subir hasta 400 y
`gObjCalcExperience` habría leído **fuera del array** en los niveles 351-400.

Corregido a **350** en `MaxUserLevel`, `DifferenceMaxLevel`, `CommandResetLevel_AL0..3`
y `CommandMasterResetLevel_AL0..3`. Además agregué:

- **Defensa en el código**: `CharacterLevelUp` ahora acota `maxNormalLevel` a
  `MAX_CHARACTER_LEVEL` aunque el `.dat` diga otra cosa.
- **Chequeo #19 en el validador**: lee `MAX_CHARACTER_LEVEL` directamente de `User.h` y
  falla si el `.dat` lo supera. El mismo error no puede repetirse en silencio.

---

## 4. Balance aplicado

### 4.1 Por qué NO toqué las constantes de daño

El pedido (§7.2) sugiere subir `PhysiDamageMinConstA` y compañía un 15-30%. Pero en el
source son **divisores enteros**:

```cpp
lpObj->PhysiDamageMinRight = Strength / gServerInfo.m_DWPhysiDamageMinConstA;
```

y valen entre **3 y 9**. Con enteros, el paso mínimo (+1) ya es enorme:

| Constante | +1 | Efecto real |
|---|---|---|
| `DWPhysiDamageMinConstA` | 6 → 7 | −14% |
| `DWPhysiDamageMaxConstA` | 4 → 5 | −20% |
| `DKDefenseConstA` | 3 → 2 | **+50% defensa** |

No permiten el ajuste fino que se buscaba. **El daño se ajustó con los rate
porcentuales**, que son exactos y hacen justo lo que se pedía.

### 4.2 Daño — más controlado

| Parámetro | Antes | Ahora |
|---|---|---|
| `GeneralDamageRatePvM` | 100 | **80** |
| `GeneralDamageRatePvP` | 100 | **70** |
| `<clase>DamageRatePvM` (×7) | 100 | **85** |
| `<clase>DamageRatePvP` (×7) | 100 (RF 95) | **75** |

Efecto combinado: PvM al **68%** del daño original, PvP al **52%**.

### 4.3 Velocidad — más lento

Divisores de velocidad (`Destreza / ConstA`) subidos ~10%, con valores **absolutos** de
destino: DW 20→22, DK 15→16, FE 50→55, MG 15→16, DL 10→11, SU 20→22, RF 9→10 (físico y
mágico).

### 4.4 Aguante — más fuerte

**Vida** (`DefaultClassInfo.txt`, admite decimales — el source usa `GetAsFloatNumber()`):
+15% vida base, +15% vida por nivel, +10% vida por vitalidad.

| Clase | Vida base | Vida/nivel | Vitalidad→vida |
|---|---|---|---|
| Dark Wizard | 60 → 69 | 1.0 → 1.15 | 2.0 → 2.2 |
| Dark Knight | 110 → 127 | 2.0 → 2.3 | 3.0 → 3.3 |
| Fairy Elf | 80 → 92 | 1.0 → 1.15 | 2.0 → 2.2 |
| Magic Gladiator | 110 → 127 | 1.0 → 1.15 | 2.0 → 2.2 |
| Dark Lord | 90 → 104 | 1.5 → 1.73 | 2.0 → 2.2 |
| Summoner | 70 → 81 | 1.0 → 1.15 | 2.0 → 2.2 |
| Rage Fighter | 100 → 115 | 1.3 → 1.5 | 2.0 → 2.2 |

**Defensa**: sólo se bajó donde el salto entero queda dentro del 10-20% buscado —
FE 10→9 (+11%), DL 7→6 (+17%), RF 8→7 (+14%). En DW, DK, MG y SU la constante vale 3 o 4
y un −1 daría +33% o +50%: **se dejaron sin tocar** y el aguante se consigue por vida.

### 4.5 Menos dependencia del reset

| Parámetro | Antes | Ahora |
|---|---|---|
| `CommandResetPoint_AL0..3` | 0 (acumulativo, sin tope) | **150** fijos |
| `DW/DK/FE/SULevelUpPoint` | 5 | **6** |
| `MG/DL/RFLevelUpPoint` | 7 | **7** (no se bajaron) |
| `PlusStatPoint` | 1 | **2** |

Nota: en la primera versión del script bajé MG/DL/RF de 7 a 6 siguiendo el "6 para
todos" del pedido. Eso era un **nerf** a tres clases, contrario al objetivo. Lo corregí:
el script nunca baja los puntos que una clase ya tenía.

---

## 5. Cómo se verificó

Sin Visual Studio no puedo compilar el GameServer completo (es un proyecto Windows), así
que verifiqué de estas formas:

1. **Sintaxis real con g++**: extraje las 7 funciones editadas del source y las compilé
   con declaraciones equivalentes → `-Wall -Wextra -Wreturn-type` sin errores.
2. **Prueba del bug de `return`**: compilé la versión original y la corregida.
   Antes: `warning: no return statement`. Ahora: limpio.
3. **Simulación numérica del desbordamiento**: tabla antes vs. después →
   8 niveles decrecientes → **0**.
4. **Simulación del level up**: ambas versiones lado a lado, 4 escenarios de exp.
5. **Balance de llaves y `#if`/`#endif`** en los 5 archivos editados.
6. **Validador de configuración**: 19 chequeos → 19 correctas, 0 errores.
7. **Idempotencia**: los dos scripts corridos dos veces → 0 cambios en la segunda.

> ⚠️ **Falta la prueba definitiva: compilar en Visual Studio.** La verificación anterior
> cubre sintaxis y lógica, pero no sustituye un build real ni las pruebas en vivo de la
> §12 del pedido.

---

## 6. Archivos modificados

**C++** (`SORCE/GameServer/GameServer/`)
- `MasterSkillTree.cpp` — `CheckMasterLevel` con return garantizado
- `User.cpp` — tabla de experiencia en 64 bits; `gObjCalcExperience` acotado
- `ObjectManager.cpp` — `CharacterLevelUp` reescrito (ambas ramas)
- `ServerInfo.cpp` — validación de `MaxUserLevel`, `MaxLevelUp*`, `MasterSkillTreeMaxLevel`
- `ResetTable.cpp` — `AccountLevel` acotado en 3 funciones

**Configuración**
- `GameServer/Data/GameServerInfo - Common.dat`, `- Command.dat`, `- Character.dat`
- `Data/Character/DefaultClassInfo.txt`

**Herramientas**
- `tools/aplicar_balance.py` — balance de combate y progresión (idempotente)
- `tools/validar_config.py` — +1 chequeo: `MaxUserLevel` vs. `MAX_CHARACTER_LEVEL`
- `tools/aplicar_config.py` — corregido a 350; ya no toca `CommandResetPoint`

**Backup**: `entrega/backup/` con los 5 `.cpp` y los `.dat` originales.

---

## 7. Lo que queda pendiente

- **Compilar en Visual Studio** y correr las 20 pruebas de la §12 del pedido.
- **Skills** (§8): `Skill.txt`, `SkillDamage.txt` y `MasterSkillTree.txt` no los toqué.
  Los archivos están en el repo; conviene medir primero cuánto pega cada skill con el
  balance nuevo antes de ajustar a ciegas.
- **`Util/ResetTable.txt`** (§6.3): revisar que la columna `Point` no vuelva a repartir
  valores altos ahora que `CommandResetPoint` es 150.
- **Logs de diagnóstico** (§11): no los agregué para no ensuciar el log en producción.
  Si querés medir la experiencia en vivo, se agregan en 5 minutos.
- **Los dos `.dmp`** de `GameServer/` (crashes del 9 y 10 de agosto) siguen sin analizar.
  Ahora que sabemos del `return` faltante, vale la pena revisarlos: encaja con un crash
  no determinista.
