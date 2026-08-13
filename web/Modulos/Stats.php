<?php
/** Compra de puntos con zen (resumen por personaje). El reparto fino se hace en la ficha del PJ. */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

if (!isset($_SESSION['login'])) {
    at_head('Puntos');
    at_msg('Para comprar puntos necesitás iniciar sesión.', 'error');
    at_foot();
    return;
}

$cuenta  = at_id($_SESSION['login']);
at_csrf();

$abierto = at_cfgb('stats_abierto', true);
$precio  = at_cfgi('stats_costo_punto', 0);
//$maxop   = at_cfgi('stats_max_por_compra', 1000);
$fuente  = at_cfg('stats_fuente', 'banco') === 'inventario' ? 'inventario' : 'banco';
$online  = at_online($cuenta);
$banco   = at_banco($cuenta);

$extra = at_col('Character', 'Resets') ? ', ISNULL(Resets,0) AS Resets' : ', 0 AS Resets';
$q = mssql_query("SELECT Name, Class, cLevel, Money, ISNULL(LevelUpPoint,0) AS LevelUpPoint{$extra}
                  FROM Character WHERE AccountID='" . at_esc($cuenta) . "' ORDER BY cLevel DESC");

at_head('Puntos y estadísticas');
?>
<div class="at-card">
  <h2>Cómo funciona</h2>
  <div class="at-grid c2">
    <div>
      <div class="at-stat"><span>Precio por punto</span><b><?= at_num($precio) ?> zen</b></div>
      <div class="at-stat"><span>Máximo por operación</span><b><?= at_num($maxop) ?> puntos</b></div>
    </div>
    <div>
      <div class="at-stat"><span>Se cobra de</span><b><?= at_h($fuente) ?></b></div>
      <div class="at-stat"><span>Zen en tu banco</span><b class="gold"><?= at_num($banco) ?></b></div>
    </div>
  </div>
  <p class="muted">Entrá a la ficha de cada personaje para repartir tus puntos libres en <b>fuerza, agilidad, vitalidad y energía</b>.
    </p>
  <?php if (!$abierto): ?><div class="at-msg at-info">La compra de puntos está deshabilitada por el momento.</div><?php endif; ?>
  <?php if ($online): ?><div class="at-msg at-info">Estás conectado al juego: salí del juego para modificar tus stats.</div><?php endif; ?>
</div>

<table class="at">
  <tr><th>Personaje</th><th>Clase</th><th>Nivel</th><th>Resets</th><th>Zen</th><th>Puntos libres</th><th></th></tr>
<?php while ($c = mssql_fetch_assoc($q)): ?>
  <tr>
    <td><?= at_h($c['Name']) ?></td>
    <td><?= at_h(at_clase_nombre($c['Class'])) ?></td>
    <td><?= at_num($c['cLevel']) ?></td>
    <td class="gold"><?= at_num($c['Resets']) ?></td>
    <td><?= at_num($c['Money']) ?></td>
    <td><?= at_num($c['LevelUpPoint']) ?></td>
    <td><a class="at-btn" href="ir.php?amz=Personaje&amp;name=<?= urlencode($c['Name']) ?>">Repartir puntos</a></td>
  </tr>
<?php endwhile; ?>
</table>
<?php at_foot(); ?>
