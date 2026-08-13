<?php
/**
 * Configuración dinámica de la web (tabla WEB_Config) + helpers comunes.
 * -------------------------------------------------------------------
 * Todo lo que el administrador puede cambiar desde el panel vive acá.
 * Si la tabla todavía no existe (base vieja), se usan los valores por
 * defecto de $AT_CFG_DEFAULT, así la web nunca se rompe.
 */
if (defined('AT_WEBCFG')) { return; }
define('AT_WEBCFG', 1);

$AT_CFG_DEFAULT = array(
    // Sitio
    'sitio_nombre'           => 'MuMisiones',
    'sitio_guild'            => 'RDDLV',
    'sitio_discord'          => '',
    'sitio_mensaje'          => '',
    'sitio_mantenimiento'    => '0',
    // Registro
    'registro_abierto'       => '1',
    'registro_vip_tipo'      => '1',
    'registro_vip_dias'      => '7',
    'registro_max_por_ip'    => '3',
    // Reset
    'reset_abierto'          => '1',
    'reset_level'            => '350',
    'reset_costo'            => '60000000',
    'reset_costo_vip'        => '0',
    'reset_puntos'           => '500',
    'reset_max'              => '0',
    'reset_conserva_stats'   => '0',
    'reset_fuente'           => 'banco',      // banco | inventario
    'reset_offline'          => '1',
    // Stats
    'stats_abierto'          => '1',
    'stats_costo_punto'      => '400000',
    'stats_max_por_compra'   => '5000',
    'stats_fuente'           => 'banco',      // banco | inventario
    'stats_max_por_atributo' => '32767',
    // Banco
    'banco_abierto'          => '1',
    'banco_max_personaje'    => '2000000000',
    // Otros
    'pk_costo'               => '60000000',
);

/** ¿Existe la columna en la tabla? (soporta serverfiles distintos) */
function at_col($tabla, $columna)
{
    static $cache = array();
    $k = strtolower($tabla . '.' . $columna);
    if (array_key_exists($k, $cache)) { return $cache[$k]; }
    $r = @mssql_query("SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='"
        . at_esc($tabla) . "' AND COLUMN_NAME='" . at_esc($columna) . "'");
    $f = $r ? mssql_fetch_row($r) : null;
    return $cache[$k] = ($f && (int) $f[0] > 0);
}

/** ¿Existe la tabla? */
function at_tabla($tabla)
{
    static $cache = array();
    $k = strtolower($tabla);
    if (array_key_exists($k, $cache)) { return $cache[$k]; }
    $r = @mssql_query("SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME='" . at_esc($tabla) . "'");
    $f = $r ? mssql_fetch_row($r) : null;
    return $cache[$k] = ($f && (int) $f[0] > 0);
}

/** Devuelve toda la configuración (cacheada por request) */
function at_cfg_all()
{
    static $cfg = null;
    if ($cfg !== null) { return $cfg; }
    $cfg = $GLOBALS['AT_CFG_DEFAULT'];
    if (at_tabla('WEB_Config')) {
        $r = @mssql_query('SELECT Clave, Valor FROM WEB_Config');
        if ($r) {
            while ($f = mssql_fetch_assoc($r)) {
                $cfg[(string) $f['Clave']] = (string) $f['Valor'];
            }
        }
    }
    return $cfg;
}

/** Valor de configuración como texto */
function at_cfg($clave, $default = '')
{
    $c = at_cfg_all();
    return array_key_exists($clave, $c) ? (string) $c[$clave] : (string) $default;
}

/** Valor de configuración como entero */
function at_cfgi($clave, $default = 0)
{
    $v = at_cfg($clave, (string) $default);
    return (int) preg_replace('/[^0-9\-]/', '', $v);
}

/** Valor de configuración como booleano */
function at_cfgb($clave, $default = false)
{
    $v = trim(at_cfg($clave, $default ? '1' : '0'));
    return in_array(strtolower($v), array('1', 'si', 'sí', 'true', 'on'), true);
}

/** Guarda un valor de configuración (usado por el panel de administración) */
function at_cfg_set($clave, $valor)
{
    if (!at_tabla('WEB_Config')) { return false; }
    $c = at_esc($clave);
    $v = at_esc($valor);
    $ok = @mssql_query("UPDATE WEB_Config SET Valor='{$v}', Fecha=GETDATE() WHERE Clave='{$c}'");
    $r  = @mssql_query("SELECT COUNT(*) FROM WEB_Config WHERE Clave='{$c}'");
    $f  = $r ? mssql_fetch_row($r) : null;
    if (!$f || (int) $f[0] === 0) {
        $ok = @mssql_query("INSERT INTO WEB_Config(Clave, Valor) VALUES('{$c}','{$v}')");
    }
    return (bool) $ok;
}

/** IP real del visitante (soporta proxy / Cloudflare) */
function at_ip()
{
    foreach (array('HTTP_CF_CONNECTING_IP', 'HTTP_X_FORWARDED_FOR', 'REMOTE_ADDR') as $k) {
        if (!empty($_SERVER[$k])) {
            $ip = trim(explode(',', $_SERVER[$k])[0]);
            if (filter_var($ip, FILTER_VALIDATE_IP)) { return $ip; }
        }
    }
    return '0.0.0.0';
}

/** Oculta la mitad del email: rica****ta@gmail.com */
function at_mail_oculto($mail)
{
    $mail = (string) $mail;
    $at = strpos($mail, '@');
    if ($at === false || $at < 2) { return $mail === '' ? '-' : '***'; }
    $u = substr($mail, 0, $at);
    $d = substr($mail, $at);
    $ver = max(1, (int) ceil(strlen($u) / 2));
    $ini = (int) ceil($ver / 2);
    $fin = $ver - $ini;
    $oculto = str_repeat('*', max(3, strlen($u) - $ver));
    return substr($u, 0, $ini) . $oculto . ($fin > 0 ? substr($u, -$fin) : '') . $d;
}

/** Formatea una fecha que puede venir como string o DateTime */
function at_fecha($v, $formato = 'd/m/Y H:i')
{
    if (empty($v)) { return '-'; }
    if ($v instanceof DateTime) { return $v->format($formato); }
    $t = strtotime((string) $v);
    return $t ? date($formato, $t) : (string) $v;
}

/** Guarda un movimiento del usuario (auditoría) */
function at_mov($cuenta, $tipo, $detalle = '', $zen = 0, $personaje = '')
{
    if (!at_tabla('WEB_Movimientos')) { return; }
    @mssql_query("INSERT INTO WEB_Movimientos(Cuenta, Personaje, Tipo, Detalle, Zen, IP) VALUES('"
        . at_esc($cuenta) . "','" . at_esc($personaje) . "','" . at_esc($tipo) . "','"
        . at_esc(mb_substr((string) $detalle, 0, 480)) . "'," . (int) $zen . ",'" . at_esc(at_ip()) . "')");
}

/** Zen disponible en el banco de la cuenta */
function at_banco($cuenta)
{
    if (!at_col('MEMB_INFO', 'banco')) { return 0; }
    $r = mssql_fetch_row(mssql_query("SELECT ISNULL(banco,0) FROM MEMB_INFO WHERE memb___id='" . at_esc($cuenta) . "'"));
    return $r ? (int) $r[0] : 0;
}

/** ¿La cuenta está conectada al juego? */
function at_online($cuenta)
{
    $r = mssql_fetch_row(mssql_query("SELECT ConnectStat FROM MEMB_STAT WHERE memb___id='" . at_esc($cuenta) . "'"));
    return $r && (int) $r[0] >= 1;
}

/** ¿La cuenta tiene VIP activo? */
function at_vip($cuenta)
{
    if (!at_col('MEMB_INFO', 'vip')) { return false; }
    $sql = at_col('MEMB_INFO', 'vencimiento')
        ? "SELECT vip, vencimiento FROM MEMB_INFO WHERE memb___id='" . at_esc($cuenta) . "'"
        : "SELECT vip FROM MEMB_INFO WHERE memb___id='" . at_esc($cuenta) . "'";
    $r = mssql_fetch_assoc(mssql_query($sql));
    if (!$r || (int) $r['vip'] <= 0) { return false; }
    if (isset($r['vencimiento']) && !empty($r['vencimiento'])) {
        $t = $r['vencimiento'] instanceof DateTime ? $r['vencimiento']->getTimestamp() : strtotime((string) $r['vencimiento']);
        if ($t && $t < time()) { return false; }
    }
    return true;
}

/** Stats base por clase (para el reset que no conserva puntos) */
function at_stats_base($class)
{
    $base = array(
        0  => array(18, 18, 15, 30, 0), 1 => array(18, 18, 15, 30, 0), 2 => array(18, 18, 15, 30, 0), 3 => array(18, 18, 15, 30, 0),
        16 => array(28, 20, 25, 10, 0), 17 => array(28, 20, 25, 10, 0), 18 => array(28, 20, 25, 10, 0), 19 => array(28, 20, 25, 10, 0),
        32 => array(22, 25, 20, 15, 0), 33 => array(22, 25, 20, 15, 0), 34 => array(22, 25, 20, 15, 0), 35 => array(22, 25, 20, 15, 0),
        48 => array(26, 26, 26, 26, 0), 49 => array(26, 26, 26, 26, 0), 50 => array(26, 26, 26, 26, 0),
        64 => array(26, 20, 20, 15, 25), 65 => array(26, 20, 20, 15, 25),
    );
    $c = (int) $class;
    return isset($base[$c]) ? $base[$c] : array(18, 18, 15, 30, 0);
}

/** Token CSRF de la sesión */
function at_csrf()
{
    if (session_status() === PHP_SESSION_NONE) { @session_start(); }
    if (empty($_SESSION['at_csrf'])) { $_SESSION['at_csrf'] = bin2hex(random_bytes(16)); }
    return $_SESSION['at_csrf'];
}

function at_csrf_ok()
{
    return isset($_SESSION['at_csrf']) && hash_equals($_SESSION['at_csrf'], (string) ($_POST['csrf'] ?? ''));
}

function at_csrf_input()
{
    return '<input type="hidden" name="csrf" value="' . htmlspecialchars(at_csrf(), ENT_QUOTES, 'UTF-8') . '">';
}
