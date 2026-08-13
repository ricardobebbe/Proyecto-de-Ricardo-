<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

$msg = '';
if (isset($_POST['recuperar'])) {
    $cuenta = at_id($_POST['cuenta']);
    $mail   = trim($_POST['mail']);
    $r = mssql_fetch_array(mssql_query("SELECT memb___id, mail_addr, memb__pwd FROM MEMB_INFO WHERE memb___id='" . at_esc($cuenta) . "'"));
    if (!$r) {
        $msg = '<div class="at-msg at-error">La cuenta no existe.</div>';
    } elseif (strtolower(trim($r['mail_addr'])) !== strtolower($mail) || $mail === '') {
        $msg = '<div class="at-msg at-error">El correo no coincide con el registrado.</div>';
    } else {
        $msg = '<div class="at-msg at-ok">Datos verificados. Tu contraseña es: <b>' . at_h($r['memb__pwd']) . '</b><br>Cambiala desde el panel de usuario apenas ingreses.</div>';
    }
}

at_head('Recuperar contraseña');
echo $msg;
?>
<div class="at-card" style="max-width:520px">
  <p class="muted">Ingresá el nombre de tu cuenta y el correo con el que la registraste.</p>
  <form method="post" action="ir.php?amz=Recuperar">
    <label>Cuenta</label>
    <input type="text" name="cuenta" maxlength="10" required>
    <label>Correo registrado</label>
    <input type="email" name="mail" maxlength="60" required>
    <button type="submit" name="recuperar" value="1">Recuperar</button>
  </form>
</div>
<?php at_foot(); ?>
