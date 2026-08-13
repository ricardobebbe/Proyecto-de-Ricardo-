<?php
/** Webhook de Mercado Pago usando cURL (Sin SDK, sin Composer) */
header('Content-Type: text/plain; charset=utf-8');

include dirname(__DIR__) . '/Seguridad/Conexion.php';
require_once dirname(__DIR__) . '/Configuraciones/Shop.php';
require_once dirname(__DIR__) . '/Seguridad/Entregas.php';

// Obtener el ID del pago desde la notificación de MP
$paymentId = '';
if (isset($_GET['topic']) && $_GET['topic'] === 'payment' && isset($_GET['id'])) {
    $paymentId = $_GET['id'];
} elseif (isset($_GET['type']) && $_GET['type'] === 'payment' && isset($_GET['data_id'])) {
    $paymentId = $_GET['data_id'];
}

// Limpiar el ID para que solo tenga números
$paymentId = preg_replace('/[^0-9]/', '', (string) $paymentId);

if ($paymentId === '') {
    http_response_code(200);
    echo 'sin pago';
    return;
}

try {
    // Consultar el pago real a la API de Mercado Pago usando cURL
    $ch = curl_init('https://api.mercadopago.com/v1/payments/' . $paymentId);
    curl_setopt_array($ch, array(
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_TIMEOUT => 20,
        CURLOPT_HTTPHEADER => array(
            'Authorization: Bearer ' . $MP_ACCESS_TOKEN,
            'Content-Type: application/json'
        )
    ));
    $resp = curl_exec($ch);
    $httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);

    if ($httpCode !== 200) {
        echo 'error al consultar MP';
        return;
    }

    $pago = json_decode((string) $resp, true);
    $pedidoId = (int) ($pago['external_reference'] ?? 0);
    $estado   = (string) ($pago['status'] ?? '');

    if ($pedidoId <= 0) {
        echo 'sin referencia';
        return;
    }

    // Verificar el pedido en nuestra base de datos
    $row = mssql_fetch_array(mssql_query("SELECT id, estado, monto_total FROM web_pedidos WHERE id = $pedidoId"));
    if (!$row) {
        echo 'pedido inexistente';
        return;
    }

    // Si ya fue entregado, no hacemos nada
    if ($row['estado'] === 'entregado') {
        echo 'ya entregado';
        return;
    }

    // Si el pago fue rechazado o cancelado
    if ($estado === 'rejected' || $estado === 'cancelled') {
        mssql_query("UPDATE web_pedidos SET estado = 'rechazado', mp_payment_id = '" . at_esc($paymentId) . "' WHERE id = $pedidoId");
        echo 'estado rechazado';
        return;
    }

    // Si el pago está pendiente (ej: esperando transferencia o pago en efectivo)
    if ($estado !== 'approved') {
        mssql_query("UPDATE web_pedidos SET estado = 'pendiente', mp_payment_id = '" . at_esc($paymentId) . "' WHERE id = $pedidoId");
        echo 'estado pendiente';
        return;
    }

    // Verificar que el monto pagado coincida con el del pedido
    $pagado = (float) ($pago['transaction_amount'] ?? 0);
    if (round($pagado, 2) + 0.01 < round((float) $row['monto_total'], 2)) {
        mssql_query("UPDATE web_pedidos SET estado = 'rechazado', detalle = 'Importe menor al esperado', mp_payment_id = '" . at_esc($paymentId) . "' WHERE id = $pedidoId");
        echo 'importe invalido';
        return;
    }

    // ¡PAGO APROBADO Y VERIFICADO! Marcar como pagado antes de entregar
    mssql_query("UPDATE web_pedidos SET estado = 'pagado', mp_payment_id = '" . at_esc($paymentId) . "' WHERE id = $pedidoId");

    // Ejecutar la lógica de entrega automática (VIP o Item al baúl)
    list($ok, $msg) = at_procesar_pedido($pedidoId);
    
    echo $ok ? 'entregado' : 'retenido: ' . $msg;

} catch (Exception $e) {
    http_response_code(500);
    echo 'error: ' . $e->getMessage();
}
?>