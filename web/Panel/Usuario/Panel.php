<?php
/**
 * Panel de usuario de MuMisiones (Mi Cuenta).
 * Reescrito 2026: personajes, datos de la cuenta con email oculto,
 * banco, VIP, últimos movimientos y accesos rápidos.
 */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';

if (!isset($_SESSION['login'])) {
    at_head('Mi cuenta');
    at_msg('Para entrar al panel necesitás iniciar sesión desde la web.', 'error');
    at_foot();
    return;
}

$cuenta = at_id($_SESSION['login']);

$campos = array('memb___id', 'mail_addr');
foreach (array('vip', 'diasdevip', 'vencimiento', 'banco', 'CuentaGM', 'appl_days', 'bloc_code') as $c) {
    if (at_col('MEMB_INFO', $c)) { $campos[] = $c; }
}
$info = mssql_fetch_assoc(mssql_query('SELECT ' . implode(', ', $campos)
    . " FROM MEMB_INFO WHERE memb___id='" . at_esc($cuenta) . "'"));

/* Personajes: primero por AccountID (funciona en cualquier serverfile) */
$chars = array();
$extra = at_col('Character', 'Resets') ? ', ISNULL(Resets,0) AS Resets' : ', 0 AS Resets';
$q = mssql_query("SELECT Name, Class, cLevel, Money, ISNULL(LevelUpPoint,0) AS LevelUpPoint{$extra}
                  FROM Character WHERE AccountID='" . at_esc($cuenta) . "' ORDER BY cLevel DESC");
while ($c = mssql_fetch_assoc($q)) { $chars[] = $c; }

$stat = mssql_fetch_assoc(mssql_query("SELECT ConnectStat, ServerName FROM MEMB_STAT WHERE memb___id='" . at_esc($cuenta) . "'"));
$online = $stat && (int) $stat['ConnectStat'] > 0;
$esVip  = at_vip($cuenta);
$banco  = at_banco($cuenta);
$gm     = isset($info['CuentaGM']) ? (int) $info['CuentaGM'] : 0;

$movs = array();
if (at_tabla('WEB_Movimientos')) {
    $qm = mssql_query("SELECT TOP 10 Tipo, Detalle, Zen, Personaje, Fecha FROM WEB_Movimientos
                       WHERE Cuenta='" . at_esc($cuenta) . "' ORDER BY Fecha DESC");
    while ($m = mssql_fetch_assoc($qm)) { $movs[] = $m; }
}

at_head('Mi cuenta');
?>
<div class="at-grid c2">
  <div class="at-card">
    <h2>Información de la cuenta</h2>
    <div class="at-stat"><span>Cuenta</span><b><?= at_h($info['memb___id']) ?>
      <?php if ($gm > 0): ?><span class="tag-gm">STAFF</span><?php endif; ?></b></div>
    <div class="at-stat"><span>Email</span><b><?= at_h(at_mail_oculto($info['mail_addr'] ?? '')) ?></b></div>
    <?php if (isset($info['appl_days'])): ?>
      <div class="at-stat"><span>Fecha de creación</span><b><?= at_h(at_fecha($info['appl_days'], 'd/m/Y')) ?></b></div>
    <?php endif; ?>
    <div class="at-stat"><span>Estado</span><b><i class="on-dot <?= $online ? 'si' : '' ?>"></i>
      <?= $online ? 'En línea' . (!empty($stat['ServerName']) ? ' (' . at_h($stat['ServerName']) . ')' : '') : 'Desconectado' ?></b></div>
    <div class="at-stat"><span>VIP</span><b class="<?= $esVip ? 'green' : 'muted' ?>"><?= $esVip ? 'Activo' : 'Sin VIP' ?></b></div>
    <?php if ($esVip && isset($info['vencimiento'])): ?>
      <div class="at-stat"><span>VIP hasta</span><b><?= at_h(at_fecha($info['vencimiento'])) ?></b></div>
    <?php endif; ?>
    <div class="at-stat"><span>Zen en el WebBank</span><b class="gold"><?= at_num($banco) ?></b></div>
    <?php if (isset($info['bloc_code'])): ?>
      <div class="at-stat"><span>Cuenta</span><b class="<?= (int) $info['bloc_code'] === 0 ? 'green' : 'red' ?>">
        <?= (int) $info['bloc_code'] === 0 ? 'Habilitada' : 'Bloqueada' ?></b></div>
    <?php endif; ?>
    <p class="muted" style="margin-top:10px">Mostramos tu email parcialmente oculto por seguridad.
      El staff <b>nunca</b> te pide la contraseña.</p>
  </div>

  <div class="at-card">
    <h2>Accesos rápidos</h2>
    <p>
      <a class="at-btn" href="ir.php?amz=Password">Cambiar contraseña</a>
      <a class="at-btn sec" href="ir.php?amz=Reset">Resetear personajes</a>
      <a class="at-btn sec" href="ir.php?amz=Stats">Comprar puntos</a>
      <a class="at-btn sec" href="ir.php?amz=Shop">Tienda VIP</a>
      <a class="at-btn sec" href="ir.php?amz=MisPedidos">Mis pedidos</a>
      <a class="at-btn sec" href="ir.php?amz=Rankings">Rankings</a>
    </p>
    <h3>Reglas rápidas del servidor</h3>
    <div class="at-stat"><span>Nivel para resetear</span><b><?= at_num(at_cfgi('reset_level', 350)) ?></b></div>
    <div class="at-stat"><span>Costo del reset</span><b><?= at_num(at_cfgi('reset_costo', 0)) ?> zen</b></div>
    <div class="at-stat"><span>Puntos por reset</span><b><?= at_num(at_cfgi('reset_puntos', 0)) ?></b></div>
    <div class="at-stat"><span>Punto extra</span><b><?= at_num(at_cfgi('stats_costo_punto', 0)) ?> zen</b></div>
    <p class="muted">Para resetear, repartir puntos o mover zen tenés que estar desconectado del juego.</p>
  </div>
</div>

<h2>Mis personajes</h2>
<?php if (!$chars): ?>
  <div class="at-msg at-info">Todavía no creaste ningún personaje en esta cuenta.</div>
<?php else: ?>
  <div class="at-grid c5">
    <?php foreach ($chars as $c): ?>
      <div class="at-char">
        <?= at_clase_img($c['Class'], 70, 96) ?>
        <a class="nom" href="ir.php?amz=Personaje&amp;name=<?= urlencode($c['Name']) ?>"><?= at_h($c['Name']) ?></a>
        <span class="cls"><?= at_h(at_clase_nombre($c['Class'])) ?></span>
        <span class="cls">Nivel <?= at_num($c['cLevel']) ?> &middot; <?= at_num($c['Resets']) ?> resets</span>
        <span class="cls gold"><?= at_num($c['LevelUpPoint']) ?> puntos libres</span>
        <a class="at-btn sec" href="ir.php?amz=Personaje&amp;name=<?= urlencode($c['Name']) ?>">Ver PJ</a>
      </div>
    <?php endforeach; ?>
  </div>
<?php endif; ?>

<?php if ($movs): ?>
  <h2>Últimos movimientos</h2>
  <table class="at">
    <tr><th>Fecha</th><th>Acción</th><th>Personaje</th><th>Detalle</th><th>Zen</th></tr>
    <?php foreach ($movs as $m): ?>
      <tr>
        <td><?= at_h(at_fecha($m['Fecha'])) ?></td>
        <td><?= at_h(ucfirst((string) $m['Tipo'])) ?></td>
        <td><?= at_h($m['Personaje']) ?></td>
        <td><?= at_h($m['Detalle']) ?></td>
        <td><?= at_num($m['Zen']) ?></td>
      </tr>
    <?php endforeach; ?>
  </table>
<?php endif; ?>
<?php at_foot(); ?>
