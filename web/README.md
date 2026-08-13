# AT_Web — Guía de instalación (cPanel y XAMPP)

Web para servidor **MU Online** que se conecta a **Microsoft SQL Server**.
Actualizada para funcionar en **PHP 7 y PHP 8** (el código original sólo servía en PHP 5).

---

## 1. Qué necesitás

- Apache con **PHP 7.2 o superior** (probado en PHP 8).
- **Microsoft SQL Server** con la base `MuOnline` de tus serverfiles.
- Un **driver de SQL Server para PHP**. Sirve cualquiera de estos:
  - `sqlsrv` + `pdo_sqlsrv` (Microsoft, el recomendado en Windows/XAMPP)
  - `pdo_dblib` (FreeTDS, el habitual en hostings Linux/cPanel)
  - `odbc` / `pdo_odbc`

> Sin driver la web no puede conectarse. El instalador te avisa en el paso 1 si falta.

---

## 2. Instalación en XAMPP (Windows)

1. Copiá la carpeta del sitio en `C:\xampp\htdocs\mu` (o directo en `htdocs`).
2. Instalá los drivers de Microsoft:
   - Descargá *Microsoft Drivers for PHP for SQL Server* de la versión de tu PHP.
   - Copiá `php_sqlsrv_XX_ts_x64.dll` y `php_pdo_sqlsrv_XX_ts_x64.dll` en `C:\xampp\php\ext`.
   - Agregá en `C:\xampp\php\php.ini`:
     ```
     extension=php_sqlsrv_XX_ts_x64.dll
     extension=php_pdo_sqlsrv_XX_ts_x64.dll
     ```
   - Instalá también el *ODBC Driver 17/18 for SQL Server*.
   - Reiniciá Apache desde el panel de XAMPP.
3. En SQL Server habilitá **autenticación mixta (SQL Server y Windows)** y **TCP/IP**
   (SQL Server Configuration Manager → Protocolos → TCP/IP → Habilitado → reiniciar servicio).
4. Abrí `http://localhost/mu/instalar/` y seguí los pasos.

---

## 3. Instalación en cPanel

1. Subí el ZIP al **Administrador de archivos** dentro de `public_html` y extraelo.
2. Permisos: carpetas `755`, archivos `644`. `Logs/` y `Configuraciones/` deben poder escribirse.
3. En **Seleccionar versión de PHP** elegí 7.4 u 8.x y activá `pdo_dblib` (o `sqlsrv` si lo ofrecen)
   y `mbstring`. Si no aparece, pedile al soporte del hosting que lo habilite.
4. Tu SQL Server debe aceptar conexiones **remotas** desde la IP del hosting
   (puerto 1433 abierto en el firewall del VPS y en SQL Server).
5. Entrá a `https://tudominio.com/instalar/` y seguí los pasos.

---

## 4. Pasos del instalador

1. **Requisitos** — comprueba PHP, drivers y permisos.
2. **Base de datos** — servidor, base, usuario, contraseña y driver; prueba la conexión.
3. **Estructura** — aplica `sql/01_estructura_web.sql` (columnas VIP, créditos, `CuentaGM`,
   `Resets`, tablas `WEB_Noticias` y `WEB_Log`). No borra datos y se puede repetir.
4. **Administrador** — le da `CuentaGM = 3` a una cuenta del juego ya existente.
5. **Listo** — escribe `Configuraciones/SQL.php` y crea `Configuraciones/instalado.lock`.

> **Borrá la carpeta `/instalar` cuando termines.**

Si preferís hacerlo a mano: ejecutá `sql/01_estructura_web.sql` en SQL Server Management Studio
y editá `Configuraciones/SQL.php`.

---

## 5. Panel de administración

Entrá al sitio con una cuenta que tenga `CuentaGM = 3` y abrí:

```
/Panel/Administrador/index.php
```

Incluye:

- **Resumen**: online, cuentas, personajes, guilds, VIPs, últimos registros y top 10.
- **Cuentas**: buscar, bloquear/desbloquear, cambiar contraseña y email, dar/quitar VIP,
  ajustar créditos, nivel de staff y **forzar desconexión** (arregla el "cuenta en uso").
- **Personajes**: editar level, resets, stats, puntos y zen; limpiar PK; mover a Lorencia.
- **Guilds**, **En línea**, **Staff / GMs**.
- **Configuración del sitio**: edita rates, eventos, IP del GameServer y copyright sin tocar código.
- **Sistema**: versión de PHP y SQL, driver en uso, permisos y visor de logs.

Todas las acciones quedan registradas en `Logs/Panel.log` y están protegidas con token CSRF.

---

## 6. Qué se actualizó respecto del original

- Compatibilidad **PHP 7/8**: capa `Seguridad/mssql_compat.php` que reimplementa `mssql_*`
  sobre `sqlsrv` / `pdo_sqlsrv` / `pdo_dblib` / `odbc`, más reemplazos de `eregi`, `ereg`,
  `split` y `get_magic_quotes_gpc`.
- Etiquetas cortas `<?` convertidas a `<?php` y todos los archivos pasados a **UTF-8**.
- Conexión centralizada con detección automática de driver y mensajes de error claros.
- Protección anti-inyección reescrita y funciones de escape (`at_esc`, `at_id`).
- Instalador web nuevo y panel de administración nuevo.
- `.htaccess` que bloquea el listado de directorios y el acceso a `.log`, `.lock`, `.sql` y `.bak`.

---

## 7. Problemas frecuentes

| Síntoma | Solución |
|---|---|
| "No se pudo conectar a SQL Server" | Revisá IP/puerto, usuario `sa`, TCP/IP habilitado y firewall. |
| "Drivers detectados: NINGUNO" | Falta instalar/activar `sqlsrv` o `pdo_dblib`. |
| Login correcto pero dice contraseña incorrecta | Cambiá la opción *Contraseñas del GameServer* (texto plano vs MD5). |
| Acentos raros | Los archivos ya están en UTF-8; asegurate de no reabrirlos con editores en ANSI. |
| El panel dice "sin permisos" | La cuenta necesita `CuentaGM = 3` en `MEMB_INFO`. |
