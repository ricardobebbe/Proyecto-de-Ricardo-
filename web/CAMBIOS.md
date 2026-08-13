# MuMisiones Web - Actualización 2026

## Qué se arregló
- **Registro de cuentas**: ahora detecta dinámicamente las columnas reales de `MEMB_INFO`
  (Season 6 con downgrade 97x), así que no falla si tu base no tiene `vip`, `banco` o
  `vencimiento`. Incluye validaciones, anti-bot, límite de cuentas por IP y mensajes claros.

## Nuevo
- `Configuraciones/Web.php` + tabla `WEB_Config`: toda la web se configura desde el panel admin.
- `Panel/Administrador/Secciones/web.php`: registro abierto/cerrado, nivel y costo de reset,
  puntos por reset, precio del punto, topes por atributo, banco, aviso global y mantenimiento.
- `Panel/Usuario/Personaje.php`: ficha del personaje con stats, reparto de puntos
  (fuerza / agilidad / vitalidad / energía) cobrando zen, resets y banco de zen.
- `Panel/Usuario/Panel.php`: cuenta con email parcialmente oculto, estado VIP, online,
  grid de personajes y últimos movimientos.
- Auditoría en `WEB_Movimientos` (resets, puntos, depósitos y retiros con IP y fecha).
- Modo mantenimiento y aviso global en toda la web.

## Instalación
1. Copiá la carpeta al servidor web.
2. Ejecutá en la base **MuOnline**, en orden: `sql/01_estructura_web.sql`,
   `sql/02_webshop.sql` y `sql/03_web_actualizacion.sql`.
3. Cargá tus datos en `Configuraciones/SQL.php`.
4. Entrá al panel admin con una cuenta `CuentaGM >= 3` y ajustá todo en **Configuración web**.
