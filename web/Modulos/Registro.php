<?php
/**
 * Registro de cuentas - MuMisiones
 * ---------------------------------------------------------------
 * Reescrito 2026:
 *  - INSERT armado en base a las columnas que REALMENTE existen en
 *    MEMB_INFO (soluciona el error al registrar en serverfiles Season 6
 *    con downgrade 0.97x que no tienen vip/banco/etc).
 *  - Detecta si memb_guid es IDENTITY antes de usar IDENTITY_INSERT.
 *  - VI_CURR_INFO es opcional: si falla no cancela el registro.
 *  - Límite de cuentas por IP, honeypot anti-bot y CSRF.
 *  - Registro configurable desde el panel de administración.
 */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

$errores = array();
$ok      = '';
$val     = array('account' => '', 'email' => '', 'numero' => '');

$abierto  = at_cfgb('registro_abierto', true);
$vipTipo  = at_cfgi('registro_vip_tipo', 0);
$vipDias  = at_cfgi('registro_vip_dias', 0);
$maxPorIp = at_cfgi('registro_max_por_ip', 0);

/** Cuenta cuántas cuentas creó hoy esta IP */
function at_reg_por_ip($ip)
{
    if (!at_tabla('WEB_Registros')) { return 0; }
    $r = mssql_fetch_row(mssql_query("SELECT COUNT(*) FROM WEB_Registros WHERE IP='" . at_esc($ip)
        . "' AND Fecha > DATEADD(day,-1,GETDATE())"));
    return $r ? (int) $r[0] : 0;
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['Registrar'])) {

    if (!$abierto) {
        $errores[] = 'El registro de cuentas está cerrado temporalmente.';
    }
    if (!at_csrf_ok()) {
        $errores[] = 'Sesión vencida. Volvé a enviar el formulario.';
    }
    // Honeypot: los bots completan campos ocultos
    if (trim((string) ($_POST['website'] ?? '')) !== '') {
        $errores[] = 'No se pudo validar el formulario.';
    }

    $account    = trim((string) ($_POST['account'] ?? ''));
    $password   = (string) ($_POST['password'] ?? '');
    $repassword = (string) ($_POST['repassword'] ?? '');
    $email      = trim((string) ($_POST['email'] ?? ''));
    $numero     = trim((string) ($_POST['realname'] ?? ''));

    $val['account'] = $account;
    $val['email']   = $email;
    $val['numero']  = $numero;

    if (!preg_match('/^[A-Za-z0-9_\-]{4,10}$/', $account)) {
        $errores[] = 'La cuenta debe tener entre 4 y 10 caracteres (letras, números, _ o -), sin espacios.';
    }
    if (preg_match('/webzen|admin|gm_|staff/i', $account)) {
        $errores[] = 'No podés usar nombres reservados en la cuenta.';
    }
    if (!preg_match('/^[A-Za-z0-9]{4,10}$/', $password)) {
        $errores[] = 'La contraseña debe tener entre 4 y 10 caracteres alfanuméricos, sin espacios.';
    }
    if ($password !== $repassword) {
        $errores[] = 'La confirmación de la contraseña no coincide.';
    }
    if (!filter_var($email, FILTER_VALIDATE_EMAIL) || strlen($email) > 50) {
        $errores[] = 'El email no es válido.';
    }
    if (!preg_match('/^[0-9]{7,13}$/', $numero)) {
        $errores[] = 'El código de seguridad debe tener entre 7 y 13 números.';
    }
    if (empty($_POST['tick1']) || empty($_POST['tick2']) || empty($_POST['tick3'])) {
        $errores[] = 'Tenés que aceptar las tres condiciones para crear la cuenta.';
    }
    if (!$errores && $maxPorIp > 0 && at_reg_por_ip(at_ip()) >= $maxPorIp) {
        $errores[] = 'Ya creaste el máximo de ' . $maxPorIp . ' cuentas permitidas por día desde esta conexión.';
    }

    if (!$errores) {
        $q = mssql_query("SELECT memb___id FROM MEMB_INFO WHERE LOWER(memb___id)='" . at_esc(strtolower($account)) . "'");
        if ($q && mssql_num_rows($q) > 0) { $errores[] = 'Esa cuenta ya está registrada, elegí otra.'; }

        $q = mssql_query("SELECT mail_addr FROM MEMB_INFO WHERE LOWER(mail_addr)='" . at_esc(strtolower($email)) . "'");
        if ($q && mssql_num_rows($q) > 0) { $errores[] = 'Ese email ya está registrado.'; }
    }

    if (!$errores) {
        $eAcc = at_esc($account);
        $ePwd = at_esc($password);
        $eMai = at_esc($email);
        $eNum = at_esc($numero);

        // --- Armamos el INSERT sólo con las columnas que existen ---------
        $cols = array();
        $vals = array();
        $put = function ($col, $val) use (&$cols, &$vals) {
            if (at_col('MEMB_INFO', $col)) { $cols[] = $col; $vals[] = $val; }
        };

        // ¿memb_guid es IDENTITY? Si lo es, dejamos que SQL Server lo asigne.
        $identidad = false;
        $ri = mssql_fetch_row(mssql_query(
            "SELECT ISNULL(COLUMNPROPERTY(OBJECT_ID('dbo.MEMB_INFO'),'memb_guid','IsIdentity'),0)"));
        if ($ri) { $identidad = ((int) $ri[0] === 1); }

        $guid = 0;
        if (!$identidad && at_col('MEMB_INFO', 'memb_guid')) {
            $rg   = mssql_fetch_row(mssql_query('SELECT ISNULL(MAX(memb_guid),0) + 1 FROM MEMB_INFO'));
            $guid = $rg ? (int) $rg[0] : 1;
            $cols[] = 'memb_guid';
            $vals[] = (string) $guid;
        }

        $put('memb___id', "'{$eAcc}'");
        $put('memb__pwd', "'{$ePwd}'");
        $put('memb_name', "'{$eAcc}'");
        $put('sno__numb', "'{$eNum}'");
        $put('mail_addr', "'{$eMai}'");
        $put('fpas_ques', "''");
        $put('fpas_answ', "''");
        $put('job__code', "'1'");
        $put('appl_days', 'GETDATE()');
        $put('modi_days', 'GETDATE()');
        $put('out__days', 'GETDATE()');
        $put('true_days', 'GETDATE()');
        $put('mail_chek', "'1'");
        $put('bloc_code', "'0'");
        $put('ctl1_code', "'1'");
        $put('AccountLevel', '0');
        $put('CuentaGM', '0');
        $put('banco', '0');
        $put('WebFecha', 'GETDATE()');
        $put('WebIP', "'" . at_esc(at_ip()) . "'");

        if ($vipTipo !== 0 && $vipDias > 0) {
            $put('vip', (string) $vipTipo);
            $put('datavip', 'GETDATE()');
            $put('diasdevip', (string) $vipDias);
            $put('vencimiento', "DATEADD(day, {$vipDias}, GETDATE())");
        } else {
            $put('vip', '0');
        }

        $sql = 'INSERT INTO MEMB_INFO(' . implode(', ', $cols) . ') VALUES(' . implode(', ', $vals) . ')';

        $creada = @mssql_query($sql);
        $detalle = $creada ? '' : (string) mssql_get_last_message();

        if ($creada) {
            // VI_CURR_INFO es opcional (algunos serverfiles ni la usan).
            if (at_tabla('VI_CURR_INFO')) {
                @mssql_query("INSERT INTO VI_CURR_INFO(ends_days, chek_code, used_time, memb___id, memb_name,
                        memb_guid, sno__numb, Bill_Section, Bill_value, Bill_Hour, Surplus_Point,
                        Surplus_Minute, Increase_Days)
                    VALUES('2005','1',1234,'{$eAcc}','{$eAcc}',
                        ISNULL((SELECT memb_guid FROM MEMB_INFO WHERE memb___id='{$eAcc}'),1),
                        '{$eNum}','6','3','6','6','2003-11-23 10:36:00','0')");
            }
            if (at_tabla('WEB_Registros')) {
                @mssql_query("INSERT INTO WEB_Registros(Cuenta, IP) VALUES('{$eAcc}','" . at_esc(at_ip()) . "')");
            }
            at_mov($account, 'registro', 'Cuenta creada desde la web');

            $ok  = 'Cuenta <b>' . at_h($account) . '</b> creada con éxito. Ya podés entrar al juego y al panel de usuario.';
            if ($vipTipo !== 0 && $vipDias > 0) {
                $ok .= '<br>Te regalamos <b>' . $vipDias . ' días de VIP</b> de bienvenida.';
            }
            $val = array('account' => '', 'email' => '', 'numero' => '');
        } else {
            if (function_exists('criar_log_sql')) { @criar_log_sql($detalle, $sql, 'Registro'); }
            $errores[] = 'No se pudo crear la cuenta. El servidor rechazó el registro.'
                . ($detalle !== '' ? ' <span class="muted">(' . at_h($detalle) . ')</span>' : '')
                . '<br>Si el problema sigue, el administrador tiene que ejecutar <b>sql/03_web_actualizacion.sql</b>.';
        }
    }
}

at_head('Crear cuenta');
?>
<?php if ($ok !== ''): ?>
  <div class="at-msg at-ok"><?= $ok ?></div>
<?php endif; ?>
<?php foreach ($errores as $e): ?>
  <div class="at-msg at-error"><?= $e ?></div>
<?php endforeach; ?>

<?php if (!$abierto): ?>
  <div class="at-msg at-info">El registro de cuentas nuevas está cerrado por el momento. Volvé a intentarlo más tarde.</div>
<?php endif; ?>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Crear mi cuenta</h2>
    <form method="post" action="ir.php?amz=Registro" autocomplete="off">
      <?= at_csrf_input() ?>
      <div style="position:absolute;left:-9999px" aria-hidden="true">
        <label>No completar</label><input type="text" name="website" tabindex="-1" autocomplete="off">
      </div>
      <label>Cuenta (4 a 10 caracteres)</label>
      <input type="text" name="account" maxlength="10" required value="<?= at_h($val['account']) ?>">
      <label>Contraseña (4 a 10 caracteres)</label>
      <input type="password" name="password" maxlength="10" required>
      <label>Repetir contraseña</label>
      <input type="password" name="repassword" maxlength="10" required>
      <label>Email</label>
      <input type="email" name="email" maxlength="50" required value="<?= at_h($val['email']) ?>">
      <label>Código de seguridad (7 a 13 números)</label>
      <input type="text" name="realname" maxlength="13" required value="<?= at_h($val['numero']) ?>" inputmode="numeric">

      <p style="margin-top:16px"><label style="text-transform:none;font-size:13px;color:var(--txt)">
        <input type="checkbox" name="tick1" value="1" style="width:auto"> Leí, entendí y acepto las reglas del servidor.</label></p>
      <p><label style="text-transform:none;font-size:13px;color:var(--txt)">
        <input type="checkbox" name="tick2" value="1" style="width:auto"> Entiendo que no voy a poder recuperar mi contraseña si el email no es real.</label></p>
      <p><label style="text-transform:none;font-size:13px;color:var(--txt)">
        <input type="checkbox" name="tick3" value="1" style="width:auto"> Me hago responsable de todo lo que pase en esta cuenta y no comparto mis datos.</label></p>

      <button type="submit" name="Registrar" value="1" <?= $abierto ? '' : 'disabled' ?>>Crear cuenta</button>
    </form>
  </div>

  <div class="at-card">
    <h2>Antes de registrarte</h2>
    <div class="at-stat"><span>VIP de regalo</span><b><?= ($vipTipo !== 0 && $vipDias > 0) ? $vipDias . ' días' : 'no' ?></b></div>
    <div class="at-stat"><span>Cuentas por día / conexión</span><b><?= $maxPorIp > 0 ? (int) $maxPorIp : 'sin límite' ?></b></div>
    <div class="at-stat"><span>Nivel para resetear</span><b><?= at_num(at_cfgi('reset_level', 350)) ?></b></div>
    <div class="at-stat"><span>Soporte</span><b>Discord oficial</b></div>
    <p class="muted" style="margin-top:14px">El staff de <?= AT_MARCA ?> <b>nunca</b> te va a pedir tu contraseña.
    Guardá tu código de seguridad: se usa para recuperar la cuenta.</p>
    <p><a class="at-btn sec" href="ir.php?amz=Reglas">Ver las reglas</a>
       <a class="at-btn sec" href="ir.php?amz=Descargas">Descargar el juego</a></p>
  </div>
</div>
<?php at_foot(); ?>
