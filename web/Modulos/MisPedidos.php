<?php
/** Historial de compras y reintento de entrega */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';
require_once dirname(__DIR__) . '/Seguridad/Entregas.php';

if (!isset($_SESSION['login'])) {
    at_head('Mis pedidos');
    at_msg('Iniciá sesión para ver tus compras.', 'error');
    at_foot();
    return;
}

$cuenta = at_id($_SESSION['login']);
if (empty($_SESSION['at_csrf'])) { $_SESSION['at_csrf'] = bin2hex(random_bytes(16)); }
$aviso = ''; $tipoAviso = 'ok';

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['reintentar'])) {
    if (!hash_equals($_SESSION['at_csrf'], (string) ($_POST['csrf'] ?? ''))) {
        $aviso = 'La sesión expiró, recargá la página.'; $tipoAviso = 'error';
    } else {
        $id = (int) $_POST['reintentar'];
        $q  = mssql_fetch_array(mssql_query("SELECT account_id FROM web_pedidos WHERE id = $id"));
        if (!$q || strcasecmp($q['account_id'], $cuenta) !== 0) {
            $aviso = 'Ese pedido no es tuyo.'; $tipoAviso = 'error';
        } else {
            list($ok, $msg) = at_procesar_pedido($id);
            $aviso = $msg; $tipoAviso = $ok ? 'ok' : 'error';
        }
    }
}

$res = mssql_query("SELECT id, producto_id, cantidad, monto_total, estado, detalle, fecha_creacion, fecha_entrega
                    FROM web_pedidos WHERE account_id = '" . at_esc($cuenta) . "' ORDER BY id DESC");

$etiquetas = array(
    'pendiente' => array('Pendiente de pago', 'muted'),
    'pagado'    => array('Pagado, entregando', 'green'),
    'entregado' => array('Entregado', 'green'),
    'retenido_falta_espacio' => array('Retenido (baúl lleno u online)', 'red'),
    'rechazado' => array('Pago rechazado', 'red'),
);

at_head('Mis pedidos');
if ($aviso !== '') { at_msg($aviso, $tipoAviso); }
?>
<div class="at-card">
  <h2>Historial de compras</h2>
  <table class="at">
    <tr><th>#</th><th>Producto</th><th>Cant.</th><th>Importe</th><th>Estado</th><th>Fecha</th><th></th></tr>
    <?php $hay = false; while ($res && ($f = mssql_fetch_array($res))): $hay = true;
      $e = isset($etiquetas[$f['estado']]) ? $etiquetas[$f['estado']] : array($f['estado'], 'muted'); ?>
      <tr>
        <td><?= (int) $f['id'] ?></td>
        <td><?= at_h(isset($SHOP_PRODUCTOS[$f['producto_id']]) ? $SHOP_PRODUCTOS[$f['producto_id']]['nombre'] : $f['producto_id']) ?></td>
        <td><?= (int) $f['cantidad'] ?></td>
        <td>$ <?= at_num($f['monto_total']) ?></td>
        <td><span class="<?= $e[1] ?>"><?= at_h($e[0]) ?></span>
            <?php if (!empty($f['detalle'])): ?><br><small class="muted"><?= at_h($f['detalle']) ?></small><?php endif; ?></td>
        <td><?= at_h(substr((string) $f['fecha_creacion'], 0, 16)) ?></td>
        <td>
          <?php if (in_array($f['estado'], array('pagado', 'retenido_falta_espacio'), true)): ?>
          <form method="post" action="ir.php?amz=MisPedidos">
            <input type="hidden" name="csrf" value="<?= at_h($_SESSION['at_csrf']) ?>">
            <button class="at-btn sec" type="submit" name="reintentar" value="<?= (int) $f['id'] ?>">Reintentar entrega</button>
          </form>
          <?php endif; ?>
        </td>
      </tr>
    <?php endwhile; ?>
    <?php if (!$hay): ?><tr><td colspan="7" class="muted">Todavía no hiciste ninguna compra.</td></tr><?php endif; ?>
  </table>
  <p><a class="at-btn" href="ir.php?amz=Shop">Ir a la tienda</a></p>
</div>
<?php at_foot(); ?>
