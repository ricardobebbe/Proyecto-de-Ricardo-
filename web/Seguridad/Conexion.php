<?php
//-----------------------------------------------
// Conexión y protección SQL
//-----------------------------------------------
if (defined('AT_CONEXION')) { return; }
define('AT_CONEXION', 1);

if (!defined('AT_ROOT')) {
    define('AT_ROOT', dirname(__DIR__));
}

require_once AT_ROOT . '/Seguridad/mssql_compat.php';

// Si la web todavía no fue instalada, mandamos al instalador.
if (!file_exists(AT_ROOT . '/Configuraciones/instalado.lock')
    && is_dir(AT_ROOT . '/instalar')
    && strpos($_SERVER['REQUEST_URI'] ?? '', '/instalar') === false) {
    // Base de la web dentro del dominio (sirve en cPanel y en subcarpetas de XAMPP)
    $at_raiz = str_replace('\\', '/', AT_ROOT);
    $at_doc  = str_replace('\\', '/', rtrim($_SERVER['DOCUMENT_ROOT'] ?? '', '/'));
    $at_base = ($at_doc !== '' && strpos($at_raiz, $at_doc) === 0) ? substr($at_raiz, strlen($at_doc)) : '';
    $at_url  = rtrim($at_base, '/') . '/instalar/index.php';
    if (!headers_sent()) { header('Location: ' . $at_url); }
    echo '<meta charset="utf-8"><a href="' . htmlspecialchars($at_url, ENT_QUOTES, 'UTF-8') . '">Instalar la web</a>';
    exit;
}

require_once AT_ROOT . '/Configuraciones/SQL.php';

if (!isset($DriverSQL)) { $DriverSQL = 'auto'; }
$GLOBALS['DriverSQL'] = $DriverSQL;

$at_db = new AT_DB();
if (!$at_db->connect($IP, $UsuarioSQL, $ContraseñaSQL, $dbsql, $DriverSQL)) {
    $detalle = htmlspecialchars($at_db->lastError, ENT_QUOTES, 'UTF-8');
    $drivers = implode(', ', AT_DB::driversDisponibles());
    if ($drivers === '') { $drivers = 'NINGUNO (falta instalar el driver de SQL Server)'; }
    die('<div style="font-family:Arial;padding:20px;background:#1b1b1b;color:#eee">'
        . '<h2 style="color:#ff5252">No se pudo conectar a SQL Server</h2>'
        . '<p>Revisá <b>Configuraciones/SQL.php</b> o volvé a correr <a style="color:#4fc3f7" href="instalar/index.php">el instalador</a>.</p>'
        . '<p><b>Drivers detectados:</b> ' . htmlspecialchars($drivers, ENT_QUOTES, 'UTF-8') . '</p>'
        . ($detalle ? '<p><b>Detalle:</b> ' . $detalle . '</p>' : '')
        . '</div>');
}
AT_DB::$instance = $at_db;
$conexion = $at_db;
$db       = true;

require_once AT_ROOT . '/Seguridad/Proteccion.php';
require_once AT_ROOT . '/Configuraciones/Tema.php';

/** Escapa un valor para usarlo dentro de comillas simples en T-SQL */
function at_esc($valor)
{
    return str_replace(array("'", "\0", "\x1a"), array("''", '', ''), (string) $valor);
}

/** Sólo letras, números y guion bajo (nombres de cuenta / personaje) */
function at_id($valor)
{
    return preg_replace('/[^A-Za-z0-9_\-]/', '', (string) $valor);
}

//-----------------------------------------------
// Endurecimiento de sesión y cabeceras de seguridad
//-----------------------------------------------
if (!headers_sent()) {
    header('X-Content-Type-Options: nosniff');
    header('X-Frame-Options: SAMEORIGIN');
    header('Referrer-Policy: strict-origin-when-cross-origin');
    header('Permissions-Policy: geolocation=(), microphone=(), camera=()');
}
if (session_status() === PHP_SESSION_NONE) {
    @ini_set('session.use_only_cookies', '1');
    @ini_set('session.cookie_httponly', '1');
    if (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off') { @ini_set('session.cookie_secure', '1'); }
    @session_start();
}

require_once AT_ROOT . '/Configuraciones/Web.php';
