<?php
/** Cierre de sesión. NO incluye Conexion.php (evita "Cannot redeclare at_esc()"). */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
$_SESSION = array();
if (ini_get('session.use_cookies')) {
    $p = session_get_cookie_params();
    setcookie(session_name(), '', time() - 42000, $p['path'], $p['domain'], $p['secure'], $p['httponly']);
}
@session_destroy();

$raiz = str_replace('\\', '/', dirname(__DIR__));
$doc  = str_replace('\\', '/', rtrim($_SERVER['DOCUMENT_ROOT'] ?? '', '/'));
$base = ($doc !== '' && strpos($raiz, $doc) === 0) ? substr($raiz, strlen($doc)) : '';
$url  = rtrim($base, '/') . '/index.php';

if (!headers_sent()) { header('Location: ' . $url); }
echo '<meta charset="utf-8"><script>if(window.top!==window.self){window.top.location=' . json_encode($url) . ';}else{location=' . json_encode($url) . ';}</script>';
exit;
