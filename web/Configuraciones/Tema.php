<?php
/**
 * Tema / helpers comunes de MuMisiones
 * Todas las páginas del sitio (módulos dentro del marco) usan at_head()/at_foot().
 */
if (defined('AT_TEMA')) { return; }
define('AT_TEMA', 1);

if (!defined('AT_MARCA'))  { define('AT_MARCA', 'MuMisiones'); }
if (!defined('AT_GUILD'))  { define('AT_GUILD', 'RDDLV'); }

/** Ruta base de la web (para que funcione en subcarpetas de XAMPP y en cPanel) */
function at_base()
{
    static $base = null;
    if ($base !== null) { return $base; }
    $raiz = str_replace('\\', '/', dirname(__DIR__));
    $doc  = str_replace('\\', '/', rtrim($_SERVER['DOCUMENT_ROOT'] ?? '', '/'));
    $base = ($doc !== '' && strpos($raiz, $doc) === 0) ? substr($raiz, strlen($doc)) : '';
    $base = rtrim($base, '/');
    return $base;
}

function at_url($ruta = '')
{
    return at_base() . '/' . ltrim($ruta, '/');
}

function at_h($v)
{
    return htmlspecialchars((string) $v, ENT_QUOTES, 'UTF-8');
}

function at_num($v)
{
    return number_format((float) $v, 0, ',', '.');
}

/** Imagen de clase de personaje */
function at_clase_img($class, $w = 64, $h = 88)
{
    $map = array(
        0 => 'DW', 1 => 'SM', 2 => 'SM', 3 => 'SM',
        16 => 'DK', 17 => 'BK', 18 => 'BK', 19 => 'BK',
        32 => 'FE', 33 => 'ME', 34 => 'ME', 35 => 'ME',
        48 => 'MG', 49 => 'MG', 50 => 'MG',
        64 => 'DL', 65 => 'DL',
    );
    $key = isset($map[(int) $class]) ? $map[(int) $class] : null;
    $img = $key && file_exists(dirname(__DIR__) . '/Imagenes/Panel/' . $key . '.png')
        ? 'Imagenes/Panel/' . $key . '.png'
        : 'Imagenes/Panel/NULL.jpg';
    return '<img src="' . at_url($img) . '" width="' . (int) $w . '" height="' . (int) $h . '" alt="clase">';
}

function at_clase_nombre($class)
{
    $n = array(
        0 => 'Dark Wizard', 1 => 'Soul Master', 2 => 'Grand Master', 3 => 'Grand Master',
        16 => 'Dark Knight', 17 => 'Blade Knight', 18 => 'Blade Master', 19 => 'Blade Master',
        32 => 'Fairy Elf', 33 => 'Muse Elf', 34 => 'High Elf', 35 => 'High Elf',
        48 => 'Magic Gladiator', 49 => 'Duel Master', 50 => 'Duel Master',
        64 => 'Dark Lord', 65 => 'Lord Emperor',
    );
    return isset($n[(int) $class]) ? $n[(int) $class] : 'Desconocido';
}

/** Cabecera estándar de un módulo (se muestra dentro del marco de contenido) */
function at_head($titulo = '', $ancho = 'normal')
{
    $t = $titulo !== '' ? at_h($titulo) . ' - ' . AT_MARCA : AT_MARCA;
    echo '<!DOCTYPE html><html lang="es"><head><meta charset="UTF-8">'
       . '<meta name="viewport" content="width=device-width, initial-scale=1">'
       . '<title>' . $t . '</title>'
       . '<link rel="shortcut icon" href="' . at_url('Imagenes/favicon.ico') . '">'
       . '<link rel="stylesheet" href="' . at_url('Imagenes/modulo.css') . '">'
       . '</head><body class="at-' . at_h($ancho) . '"><div class="at-wrap">';
    if ($titulo !== '') {
        echo '<h1 class="at-title">' . at_h($titulo) . '</h1>';
    }
    at_gate_mantenimiento();
}

/**
 * Bloquea los módulos cuando el sitio está en mantenimiento.
 * El staff (CuentaGM >= 3) sigue navegando con normalidad.
 */
function at_gate_mantenimiento()
{
    if (!function_exists('at_cfgb') || !at_cfgb('sitio_mantenimiento', false)) { return; }
    $gm = 0;
    if (isset($_SESSION['login'])) {
        $g  = mssql_fetch_row(mssql_query("SELECT CuentaGM FROM MEMB_INFO WHERE memb___id='" . at_esc($_SESSION['login']) . "'"));
        $gm = isset($g[0]) ? (int) $g[0] : 0;
    }
    if ($gm >= 3) { return; }
    echo '<div class="at-msg at-info">La web está en mantenimiento. Esta sección vuelve a estar disponible en unos minutos.</div>';
    at_foot();
    exit;
}

function at_foot()
{
    echo '</div></body></html>';
}

function at_msg($texto, $tipo = 'info')
{
    echo '<div class="at-msg at-' . at_h($tipo) . '">' . $texto . '</div>';
}
