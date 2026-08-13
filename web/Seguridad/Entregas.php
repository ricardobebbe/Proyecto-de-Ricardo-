<?php
/**
 * Motor de entregas automáticas de la tienda web.
 * Se usa desde el webhook de MercadoPago y desde "Mis pedidos" (reintento).
 */
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Shop.php';

/** ¿El personaje/cuenta está online? No se entrega nada si lo está. */
function at_cuenta_online($cuenta)
{
    $c = at_esc($cuenta);
    $r = mssql_query("SELECT ConnectStat FROM MEMB_STAT WHERE memb___id = '$c'");
    $f = $r ? mssql_fetch_array($r) : null;
    return $f && (int) $f['ConnectStat'] === 1;
}

/** Suma días de VIP a la cuenta. */
function at_entregar_vip($cuenta, $dias)
{
    $c = at_esc($cuenta);
    $d = (int) $dias;
    if ($d <= 0) { return array(false, 'Cantidad de días inválida'); }

    $r = mssql_query("SELECT vip, diasdevip, vencimiento FROM MEMB_INFO WHERE memb___id = '$c'");
    $f = $r ? mssql_fetch_array($r) : null;
    if (!$f) { return array(false, 'La cuenta no existe'); }

    $base = (!empty($f['vencimiento']) && strtotime($f['vencimiento']) > time())
        ? strtotime($f['vencimiento'])
        : time();
    $nuevo = date('Y-m-d H:i:s', $base + ($d * 86400));
    $totalDias = (int) $f['diasdevip'] + $d;

    mssql_query("UPDATE MEMB_INFO SET vip = 1, diasdevip = $totalDias, vencimiento = '$nuevo' WHERE memb___id = '$c'");
    return array(true, 'VIP activo hasta ' . $nuevo);
}

/**
 * Inserta un item (hex) en el primer slot libre del baúl.
 * Devuelve array(ok, mensaje). Si no hay espacio devuelve ok=false
 * con el mensaje "SIN_ESPACIO" para que el pedido quede retenido.
 */
function at_entregar_item($cuenta, $hex)
{
    global $SHOP_BYTES_SLOT, $SHOP_BYTES_CAPACIDAD;

    $c   = at_esc($cuenta);
    $hex = strtoupper(preg_replace('/[^0-9A-Fa-f]/', '', $hex));
    $slotHex = $SHOP_BYTES_SLOT * 2;
    if (strlen($hex) !== $slotHex) {
        return array(false, 'El item configurado no tiene ' . $SHOP_BYTES_SLOT . ' bytes');
    }
    if (at_cuenta_online($cuenta)) {
        return array(false, 'ONLINE');
    }

    $r = mssql_query("SELECT CONVERT(varchar(max), Items, 2) AS hexitems FROM warehouse WHERE AccountID = '$c'");
    $f = $r ? mssql_fetch_array($r) : null;
    if (!$f) { return array(false, 'La cuenta no tiene baúl creado (entrá una vez al juego)'); }

    $items = strtoupper($f['hexitems']);
    $total = $SHOP_BYTES_CAPACIDAD * 2;
    $items = str_pad(substr($items, 0, $total), $total, 'F');

    $libre = str_repeat('F', $slotHex);
    $pos = -1;
    for ($i = 0; $i < $total; $i += $slotHex) {
        if (substr($items, $i, $slotHex) === $libre) { $pos = $i; break; }
    }
    if ($pos < 0) { return array(false, 'SIN_ESPACIO'); }

    $nuevo = substr($items, 0, $pos) . $hex . substr($items, $pos + $slotHex);
    mssql_query("UPDATE warehouse SET Items = CONVERT(varbinary(max), '0x$nuevo', 1) WHERE AccountID = '$c'");
    return array(true, 'Item entregado en el slot ' . (int) ($pos / $slotHex));
}

/**
 * Procesa un pedido pagado y lo marca como entregado.
 * $forzar = true permite reintentar un pedido retenido.
 */
function at_procesar_pedido($pedidoId)
{
    global $SHOP_PRODUCTOS;

    $id = (int) $pedidoId;
    $r  = mssql_query("SELECT * FROM web_pedidos WHERE id = $id");
    $p  = $r ? mssql_fetch_array($r) : null;
    if (!$p) { return array(false, 'Pedido inexistente'); }
    if ($p['estado'] === 'entregado') { return array(true, 'El pedido ya fue entregado'); }
    if (!in_array($p['estado'], array('pagado', 'retenido_falta_espacio'), true)) {
        return array(false, 'El pedido todavía no está pagado');
    }

    $prod = isset($SHOP_PRODUCTOS[$p['producto_id']]) ? $SHOP_PRODUCTOS[$p['producto_id']] : null;
    if (!$prod) { return array(false, 'Producto no encontrado en el catálogo'); }

    if ($prod['tipo'] === 'vip') {
        list($ok, $msg) = at_entregar_vip($p['account_id'], $prod['dias'] * (int) $p['cantidad']);
    } else {
        $ok = true; $msg = '';
        for ($i = 0; $i < (int) $p['cantidad']; $i++) {
            list($ok, $msg) = at_entregar_item($p['account_id'], $prod['hex']);
            if (!$ok) { break; }
        }
    }

    $det = at_esc(substr($msg, 0, 250));
    if ($ok) {
        mssql_query("UPDATE web_pedidos SET estado = 'entregado', fecha_entrega = GETDATE(), detalle = '$det' WHERE id = $id");
        return array(true, $msg);
    }

    if ($msg === 'SIN_ESPACIO') {
        mssql_query("UPDATE web_pedidos SET estado = 'retenido_falta_espacio', detalle = 'Baúl lleno' WHERE id = $id");
        return array(false, 'Tu baúl está lleno. Liberá un espacio y reintentá la entrega desde Mis Pedidos.');
    }
    if ($msg === 'ONLINE') {
        mssql_query("UPDATE web_pedidos SET estado = 'retenido_falta_espacio', detalle = 'Cuenta online' WHERE id = $id");
        return array(false, 'Tenés que estar desconectado del juego. Salí y reintentá desde Mis Pedidos.');
    }

    mssql_query("UPDATE web_pedidos SET detalle = '$det' WHERE id = $id");
    return array(false, $msg);
}
