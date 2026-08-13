<?php
/** Cambio de contraseña - PHP 8, sin eregi(), con at_esc() */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';

if (!isset($_SESSION['login'])) {
    at_head('Cambiar contraseña');
    at_msg('Para entrar a esta página necesitás iniciar sesión.', 'error');
    at_foot();
    return;
}

$login = at_id($_SESSION['login']);
if (empty($_SESSION['at_csrf'])) { $_SESSION['at_csrf'] = bin2hex(random_bytes(16)); }

$errores = array();
$ok = '';

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['prossegue'])) {
    if (!hash_equals($_SESSION['at_csrf'], (string) ($_POST['csrf'] ?? ''))) {
        $errores[] = 'Sesión vencida. Recargá la página e intentá de nuevo.';
    }

    $actual = (string) ($_POST['actual'] ?? '');
    $nueva  = (string) ($_POST['nova'] ?? '');
    $repite = (string) ($_POST['repassword'] ?? '');

    if (!preg_match('/^[A-Za-z0-9]{4,10}$/', $nueva)) {
        $errores[] = 'La contraseña nueva debe tener entre 4 y 10 caracteres alfanuméricos.';
    }
    if ($nueva !== $repite) {
        $errores[] = 'La confirmación no coincide con la contraseña nueva.';
    }

    if (!$errores) {
        $r = mssql_fetch_row(mssql_query("SELECT ConnectStat FROM MEMB_STAT WHERE memb___id='" . at_esc($login) . "'"));
        if ($r && (int) $r[0] >= 1) {
            $errores[] = 'Tu cuenta está conectada al juego. Salí del juego antes de cambiar la contraseña.';
        }
    }

    if (!$errores) {
        $valida = false;
        if (isset($md5) && $md5 == 1) {
            $q = mssql_query('exec ChecaSenha "' . at_esc($actual) . '", "' . at_esc($login) . '"');
            $valida = $q && mssql_num_rows($q) > 0;
        } else {
            $q = mssql_fetch_row(mssql_query("SELECT memb___id FROM MEMB_INFO WHERE memb___id='" . at_esc($login) . "' AND memb__pwd='" . at_esc($actual) . "'"));
            $valida = !empty($q[0]);
        }
        if (!$valida) { $errores[] = 'La contraseña actual no es correcta.'; }
    }

    if (!$errores) {
        $r = @mssql_query("UPDATE MEMB_INFO SET memb__pwd='" . at_esc($nueva) . "' WHERE memb___id='" . at_esc($login) . "'");
        if ($r !== false) {
            $_SESSION['senha'] = $nueva;
            $ok = 'Contraseña cambiada correctamente.';
            if (function_exists('criar_log_painel')) { @criar_log_painel('Cambio de contraseña de ' . $login, 'Password.php'); }
        } else {
            $errores[] = 'No se pudo cambiar la contraseña. Intentá de nuevo.';
        }
    }
}

at_head('Cambiar contraseña');
foreach ($errores as $e) { at_msg(at_h($e), 'error'); }
if ($ok !== '') { at_msg(at_h($ok), 'ok'); }
?>
<div class="at-card" style="max-width:520px">
  <form method="post" action="ir.php?amz=Password" autocomplete="off">
    <input type="hidden" name="csrf" value="<?= at_h($_SESSION['at_csrf']) ?>">
    <label>Contraseña actual</label>
    <input type="password" name="actual" maxlength="20" required>
    <label>Contraseña nueva</label>
    <input type="password" name="nova" maxlength="10" required>
    <label>Repetir contraseña nueva</label>
    <input type="password" name="repassword" maxlength="10" required>
    <p style="margin-top:16px">
      <button type="submit" name="prossegue" value="1">Cambiar contraseña</button>
      <a class="at-btn sec" href="ir.php?amz=Usuario">Volver al panel</a>
    </p>
  </form>
  <p class="muted">Tenés que estar desconectado del juego para poder cambiarla.</p>
</div>
<?php at_foot(); ?>
