<?php
/** Resets desde la web: lista de personajes con las reglas configuradas en el panel. */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

if (!isset($_SESSION['login'])) {
    at_head('Resets');
    at_msg('Para resetear necesitás iniciar sesión.', 'error');
    at_foot();
    return;
}

$cuenta = at_id($_SESSION['login']);
at_csrf();

$esVip     = at_vip($cuenta);
$abierto   = at_cfgb('reset_abierto', true);
$RS_LEVEL  = at_cfgi('reset_level', 350);
$RS_COSTO  = $esVip ? at_cfgi('reset_costo_vip', 0) : at_cfgi('reset_costo', 0);
$RS_PUNTOS = at_cfgi('reset_puntos', 500);
$RS_MAX    = at_cfgi('reset_max', 0);
$RS_FUENTE = at_cfg('reset_fuente', 'banco') === 'inventario' ? 'inventario' : 'banco';
$online    = at_online($cuenta);
$banco     = at_banco($cuenta);

$extra = at_col('Character', 'Resets') ? ', ISNULL(Resets,0) AS Resets' : ', 0 AS Resets';
$q = mssql_query("SELECT Name, Class, cLevel, Money, ISNULL(LevelUpPoint,0) AS LevelUpPoint{$extra}
                  FROM Character WHERE AccountID='" . at_esc($cuenta) . "' ORDER BY cLevel DESC");

at_head('Resets');
?>
<div class="at-card">
  <h2>Reglas del reset</h2>
  <div class="at-grid c2">
    <div>
      <div class="at-stat"><span>Nivel necesario</span><b><?= at_num($RS_LEVEL) ?></b></div>
      <div class="at-stat"><span>Costo</span><b><?= $RS_COSTO > 0 ? at_num($RS_COSTO) . ' zen' : 'gratis' ?><?= $esVip ? ' (VIP)' : '' ?></b></div>
      <div class="at-stat"><span>Se cobra de</span><b><?= at_h($RS_FUENTE) ?></b></div>
    </div>
    <div>
      <div class="at-stat"><span>Puntos por reset</span><b><?= at_num($RS_PUNTOS) ?></b></div>
      <div class="at-stat"><span>Máximo de resets</span><b><?= $RS_MAX > 0 ? at_num($RS_MAX) : 'sin límite' ?></b></div>
      <div class="at-stat"><span>Zen en tu banco</span><b class="gold"><?= at_num($banco) ?></b></div>
    </div>
  </div>
  <p class="muted">Tenés que estar <b>desconectado del juego</b>. Al resetear, los stats vuelven a la base de tu clase y recibís los puntos para repartir desde la ficha del personaje.</p>
  <?php if (!$abierto): ?><div class="at-msg at-info">Los resets por la web están cerrados temporalmente.</div><?php endif; ?>
  <?php if ($online): ?><div class="at-msg at-info">Estás conectado al juego: salí del juego para poder resetear.</div><?php endif; ?>
</div>

<table class="at">
  <tr><th>Personaje</th><th>Clase</th><th>Nivel</th><th>Resets</th><th>Zen</th><th>Puntos libres</th><th></th></tr>
<?php while ($c = mssql_fetch_assoc($q)):
    $puede = $abierto && !$online && (int) $c['cLevel'] >= $RS_LEVEL && ($RS_MAX === 0 || (int) $c['Resets'] < $RS_MAX); ?>
  <tr>
    <td><a href="ir.php?amz=Personaje&amp;name=<?= urlencode($c['Name']) ?>"><?= at_h($c['Name']) ?></a></td>
    <td><?= at_h(at_clase_nombre($c['Class'])) ?></td>
    <td><?= at_num($c['cLevel']) ?></td>
    <td class="gold"><?= at_num($c['Resets']) ?></td>
    <td><?= at_num($c['Money']) ?></td>
    <td><?= at_num($c['LevelUpPoint']) ?></td>
    <td>
      <?php if ($puede): ?>
        <form method="post" action="ir.php?amz=Personaje&amp;name=<?= urlencode($c['Name']) ?>"
              onsubmit="return confirm('¿Resetear a <?= at_h($c['Name']) ?>?')">
          <?= at_csrf_input() ?>
          <input type="hidden" name="accion" value="resetear">
          <input type="hidden" name="name" value="<?= at_h($c['Name']) ?>">
          <button class="at-btn" type="submit">Resetear</button>
        </form>
      <?php else: ?>
        <span class="muted">No disponible</span>
      <?php endif; ?>
    </td>
  </tr>
<?php endwhile; ?>
</table>
<?php at_foot(); ?>
