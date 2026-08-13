<?php
/**
 * Panel de Administración - núcleo común
 */
if (session_status() !== PHP_SESSION_ACTIVE) { session_start(); }
if (!headers_sent()) { header('Content-Type: text/html; charset=UTF-8'); }

define('AT_ROOT', dirname(dirname(__DIR__)));
require_once AT_ROOT . '/Seguridad/Conexion.php';
require_once AT_ROOT . '/Configuraciones/Sitio.php';

// ---------------------------------------------------------------- Seguridad
if (!isset($_SESSION['login'])) {
    die('<meta charset="utf-8"><div style="font-family:Arial;background:#14161a;color:#eee;padding:30px">'
      . 'Necesitás iniciar sesión. <a style="color:#4fc3f7" href="../../index.php">Volver al inicio</a></div>');
}

$AT_CUENTA = at_id($_SESSION['login']);
$__gm = mssql_fetch_row(mssql_query("SELECT ISNULL(CuentaGM,0) FROM MEMB_INFO WHERE memb___id='" . at_esc($AT_CUENTA) . "'"));
$AT_NIVEL_GM = $__gm ? (int) $__gm[0] : 0;

if ($AT_NIVEL_GM < 3) {
    criar_log_painel("Intento de acceso al panel admin por " . $AT_CUENTA, "Administrador");
    die('<meta charset="utf-8"><div style="font-family:Arial;background:#14161a;color:#eee;padding:30px">'
      . 'No tenés permisos de administrador. <a style="color:#4fc3f7" href="../../index.php">Volver</a></div>');
}

// ---------------------------------------------------------------- CSRF
if (empty($_SESSION['at_token'])) {
    $_SESSION['at_token'] = bin2hex(random_bytes(16));
}
function at_token_input() {
    return '<input type="hidden" name="token" value="' . htmlspecialchars($_SESSION['at_token'], ENT_QUOTES, 'UTF-8') . '">';
}
function at_token_ok() {
    return isset($_POST['token']) && hash_equals($_SESSION['at_token'], $_POST['token']);
}

// ---------------------------------------------------------------- Helpers
$AT_AVISOS = array();
function at_ok($m)   { $GLOBALS['AT_AVISOS'][] = array('ok', $m); }
function at_err($m)  { $GLOBALS['AT_AVISOS'][] = array('err', $m); }
function h($v)       { return htmlspecialchars((string) $v, ENT_QUOTES, 'UTF-8'); }

function at_uno($sql) {
    $r = mssql_query($sql);
    if (!$r) return null;
    $f = mssql_fetch_row($r);
    return $f ? $f[0] : null;
}
function at_filas($sql) {
    $r = mssql_query($sql);
    $out = array();
    if (!$r) return $out;
    while ($f = mssql_fetch_assoc($r)) { $out[] = $f; }
    return $out;
}
function at_exec($sql, $accion = '') {
    $r = mssql_query($sql);
    if ($r === false) {
        criar_log_sql(mssql_get_last_message(), $sql, 'Administrador');
        at_err('Error SQL: ' . h(mssql_get_last_message()));
        return false;
    }
    if ($accion !== '') { criar_log_painel($accion . ' (por ' . $GLOBALS['AT_CUENTA'] . ')', 'Administrador'); }
    return true;
}

/** Nombre de clase de MU a partir del código */
function at_clase($c) {
    $m = array(0=>'Dark Wizard',1=>'Soul Master',16=>'Dark Knight',17=>'Blade Knight',
               32=>'Fairy Elf',33=>'Muse Elf',48=>'Magic Gladiator',64=>'Dark Lord');
    return isset($m[(int)$c]) ? $m[(int)$c] : 'Clase ' . (int)$c;
}

/** ¿Existe una columna en una tabla? (para instalaciones distintas) */
function at_hay_columna($tabla, $columna) {
    static $cache = array();
    $k = strtolower($tabla . '.' . $columna);
    if (isset($cache[$k])) return $cache[$k];
    $n = at_uno("SELECT COUNT(*) FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='" . at_esc($tabla) . "' AND COLUMN_NAME='" . at_esc($columna) . "'");
    return $cache[$k] = ((int) $n > 0);
}

$AT_SECCION = isset($_GET['s']) ? preg_replace('/[^a-z]/', '', strtolower($_GET['s'])) : 'inicio';
if ($AT_SECCION === '') { $AT_SECCION = 'inicio'; }

$AT_MENU = array(
    'inicio'      => 'Resumen',
    'cuentas'     => 'Cuentas',
    'personajes'  => 'Personajes',
    'guilds'      => 'Guilds',
    'online'      => 'En línea',
    'staff'       => 'Staff / GMs',
    'sitio'       => 'Datos del servidor',
    'web'         => 'Configuración web',
    'sistema'     => 'Sistema',
);

function at_cabecera() {
    global $AT_SECCION, $AT_MENU, $AT_CUENTA, $AT_AVISOS;
    ?><!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Panel de Administración</title>
<link rel="stylesheet" href="estilos.css">
</head>
<body>
<header class="barra">
  <div class="marca">Panel de Administración</div>
  <div class="usuario">Conectado como <b><?php echo h($AT_CUENTA); ?></b>
    <a class="btn btn-fantasma" href="../../index.php" target="_top">Ir al sitio</a>
    <a class="btn btn-peligro" href="../../ir.php?amz=Salir" target="_top">Salir</a>
  </div>
</header>
<div class="contenedor">
  <nav class="menu">
    <?php foreach ($AT_MENU as $k => $v): ?>
      <a class="<?php echo $AT_SECCION === $k ? 'activo' : ''; ?>" href="index.php?s=<?php echo $k; ?>"><?php echo h($v); ?></a>
    <?php endforeach; ?>
  </nav>
  <main class="panel">
  <?php foreach ($AT_AVISOS as $a): ?>
    <div class="aviso <?php echo $a[0]; ?>"><?php echo $a[1]; ?></div>
  <?php endforeach; ?>
<?php
}

function at_pie() {
    ?>
  </main>
</div>
</body>
</html>
<?php
}
