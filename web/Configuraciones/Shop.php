<?php
/**
* Configuración de la tienda web (MercadoPago + catálogo)
* -----------------------------------------------------------------
* IMPORTANTE: el ACCESS TOKEN es privado. No lo compartas ni lo subas
* a ningún repositorio público. Este archivo está protegido por el
* .htaccess de la carpeta Configuraciones.
*/
/* Access Token de producción de MercadoPago (Tus integraciones > Credenciales) */
$MP_ACCESS_TOKEN = 'TEST-711ffded-a4de-4796-b626-5aa66eba3b60';
/* Public key (opcional, para checkout embebido) */
$MP_PUBLIC_KEY   = 'TEST-7580383697751675-072923-2b6b974e437673d1654f66730ec594bc-2185197547';
/* URL pública de la web (sin barra final). Ej: https://www.mumisiones.com */
$SHOP_URL_BASE   = 'https://www.mumisiones.com';
/* Moneda de cobro */
$SHOP_MONEDA     = 'ARS';
/* ------------------------------------------------------------------
* Baúl (warehouse): tamaño de cada slot y capacidad total en bytes.
* 108 slots x 10 bytes = 1080 bytes (config del plan).
* Si tu server usa otro formato, ajustá estos dos valores.
* ------------------------------------------------------------------ */
$SHOP_BYTES_SLOT     = 10;
$SHOP_BYTES_CAPACIDAD = 1080;
/* ------------------------------------------------------------------
* CATÁLOGO
*   tipo: 'vip'  -> suma días de VIP automáticamente
*         'item' -> inserta el hex en el baúl (necesita espacio)
*   hex : bytes del item en hexadecimal (sin 0x), tal como los guarda
*         tu servidor. Cambiá estos valores por los de TU serverfiles.
* ------------------------------------------------------------------ */
$SHOP_PRODUCTOS = array(
'vip_30' => array(
'nombre' => 'VIP 30 días',
'tipo'   => 'vip',
'dias'   => 30,
'precio' => 3500.00,
'img'    => 'Imagenes/haztevip.jpg',
'desc'   => 'Resets y stats gratis desde el juego, mapas VIP, zen mensual, eventos exclusivos y mejor drop.',
),
'vip_90' => array(
'nombre' => 'VIP 90 días',
'tipo'   => 'vip',
'dias'   => 90,
'precio' => 9000.00,
'img'    => 'Imagenes/haztevip.jpg',
'desc'   => 'Tres meses de VIP con todos los beneficios, al mejor precio.',
),
'box_vip_verde' => array(
'nombre' => 'Box VIP Verde (GM)',
'tipo'   => 'item',
'hex'    => '63C8000000E000FFFFFF',
'precio' => 500.00,
'img'    => 'Imagenes/boxvipverde.jpg',
'desc'   => 'Box VIP exclusiva de GM. Contiene items especiales. Se entrega en tu baúl.',
),
'ala_angel' => array(
'nombre' => 'Wings of Heaven (Ala Ángel) +11',
'tipo'   => 'item',
'hex'    => '2400DC2D2D3D000000C0',
'precio' => 6000.00,
'img'    => 'Imagenes/alasfull.jpg',
'desc'   => 'Alas de nivel 2 +11 con opciones completas. Se entregan directamente en tu baúl.',
),
);