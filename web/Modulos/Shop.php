<?php
/** Tienda web: VIP y alas con pago por MercadoPago */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';
require_once dirname(__DIR__) . '/Configuraciones/Shop.php';

$logueado = isset($_SESSION['login']);
$cuenta   = $logueado ? at_id($_SESSION['login']) : '';
$error    = '';
$aviso    = '';

if (empty($_SESSION['at_csrf'])) { $_SESSION['at_csrf'] = bin2hex(random_bytes(16)); }

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['comprar'])) {
    $pid  = isset($_POST['producto']) ? preg_replace('/[^a-z0-9_]/', '', strtolower($_POST['producto'])) : '';
    $cant = max(1, min(10, (int) ($_POST['cantidad'] ?? 1)));

    if (!$logueado) {
        $error = 'Iniciá sesión con tu cuenta del juego antes de comprar.';
    } elseif (!hash_equals($_SESSION['at_csrf'], (string) ($_POST['csrf'] ?? ''))) {
        $error = 'La sesión expiró, volvé a intentarlo.';
    } elseif (!isset($SHOP_PRODUCTOS[$pid])) {
        $error = 'Producto inválido.';
    } elseif (strpos($MP_ACCESS_TOKEN, 'COMPLETAR') !== false) {
        $error = 'La tienda todavía no está configurada. Escribinos por Contacto para comprar.';
    } else {
        $prod  = $SHOP_PRODUCTOS[$pid];
        $total = round($prod['precio'] * $cant, 2);

        mssql_query("INSERT INTO web_pedidos (account_id, tipo_producto, producto_id, cantidad, monto_total, estado)
                     VALUES ('" . at_esc($cuenta) . "', '" . at_esc($prod['tipo']) . "', '" . at_esc($pid) . "', $cant, $total, 'pendiente')");
        $r = mssql_fetch_row(mssql_query("SELECT MAX(id) FROM web_pedidos WHERE account_id='" . at_esc($cuenta) . "'"));
        $pedido = (int) $r[0];

        $pref = array(
            'items' => array(array(
                'title'       => $prod['nombre'] . ' - ' . AT_MARCA,
                'quantity'    => $cant,
                'unit_price'  => (float) $prod['precio'],
                'currency_id' => $SHOP_MONEDA,
            )),
            'external_reference' => (string) $pedido,
            'notification_url'   => $SHOP_URL_BASE . '/api/mp_webhook.php',
            'back_urls' => array(
                'success' => $SHOP_URL_BASE . '/ir.php?amz=MisPedidos',
                'failure' => $SHOP_URL_BASE . '/ir.php?amz=MisPedidos',
                'pending' => $SHOP_URL_BASE . '/ir.php?amz=MisPedidos',
            ),
            'auto_return' => 'approved',
        );

        $ch = curl_init('https://api.mercadopago.com/checkout/preferences');
        curl_setopt_array($ch, array(
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_POST           => true,
            CURLOPT_TIMEOUT        => 20,
            CURLOPT_HTTPHEADER     => array('Content-Type: application/json', 'Authorization: Bearer ' . $MP_ACCESS_TOKEN),
            CURLOPT_POSTFIELDS     => json_encode($pref),
        ));
        $resp = curl_exec($ch);
        $code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        curl_close($ch);
        $data = json_decode((string) $resp, true);

        if ($code >= 200 && $code < 300 && !empty($data['init_point'])) {
            mssql_query("UPDATE web_pedidos SET mp_preference_id='" . at_esc($data['id']) . "' WHERE id = $pedido");
            header('Location: ' . $data['init_point']);
            echo '<meta charset="utf-8"><a href="' . at_h($data['init_point']) . '">Continuar al pago</a>';
            return;
        }
        $error = 'No pudimos abrir el pago en este momento. Probá de nuevo en unos minutos.';
    }
}

at_head('Tienda ' . AT_MARCA);
if ($error !== '') { at_msg($error, 'error'); }
if (!$logueado) { at_msg('Iniciá sesión en la web para que la entrega sea automática en tu cuenta.', 'info'); }
?>
<div class="at-msg at-info">
  Las compras se entregan <b>automáticamente</b>: el VIP se activa al instante y los items caen en tu
  <b>baúl</b>. Para recibir items tenés que estar <b>desconectado del juego</b> y con un espacio libre en el baúl.
</div>

<div class="at-grid c2">
<?php foreach ($SHOP_PRODUCTOS as $id => $p): ?>
  <div class="at-card">
    <h2><?= at_h($p['nombre']) ?></h2>
    <?php if (!empty($p['img'])): ?><img src="<?= at_h($p['img']) ?>" alt="<?= at_h($p['nombre']) ?>" style="max-width:100%;border-radius:8px"><?php endif; ?>
    <p><?= at_h($p['desc']) ?></p>
    <div class="at-stat"><span>Precio</span><b class="green">$ <?= at_num($p['precio']) ?></b></div>
    <form method="post" action="ir.php?amz=Shop">
      <input type="hidden" name="csrf" value="<?= at_h($_SESSION['at_csrf']) ?>">
      <input type="hidden" name="producto" value="<?= at_h($id) ?>">
      <label>Cantidad</label>
      <input type="number" name="cantidad" value="1" min="1" max="10">
      <button class="at-btn" type="submit" name="comprar" value="1" <?= $logueado ? '' : 'disabled' ?>>Comprar con MercadoPago</button>
    </form>
  </div>
<?php endforeach; ?>
</div>

<div class="at-card">
  <h2>¿Cómo funciona?</h2>
  <ol>
    <li>Iniciás sesión en la web y elegís el producto.</li>
    <li>Pagás con MercadoPago (tarjeta, débito, dinero en cuenta o efectivo).</li>
    <li>Al acreditarse el pago la entrega es automática.</li>
    <li>Podés ver el estado en <a href="ir.php?amz=MisPedidos">Mis pedidos</a> y reintentar la entrega si tu baúl estaba lleno.</li>
  </ol>
</div>
<?php at_foot(); ?>
