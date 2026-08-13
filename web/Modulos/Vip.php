<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

// Verificar si el usuario está logueado para mostrarle su cuenta
$logueado = isset($_SESSION['login']);
$cuenta = $logueado ? at_id($_SESSION['login']) : '';

at_head('Comprar VIP - ' . AT_MARCA);
?>

<div class="at-card">
    <h2>💎 Hazte VIP - 1 Mes</h2>
    <p>Obtén acceso a beneficios exclusivos: resets gratis, mejor drop, mapas VIP, soporte prioritario y más.</p>

    <?php if ($logueado): ?>
        <div class="at-msg at-info">
            ✅ Estás comprando para la cuenta: <b><?= at_h($cuenta) ?></b>. 
            <br>Recuerda enviar el comprobante indicando este nombre de cuenta.
        </div>
    <?php else: ?>
        <div class="at-msg at-err">
            ⚠️ No has iniciado sesión. Te recomendamos <a href="ir.php?amz=Registro">iniciar sesión</a> antes de comprar para que podamos vincular el pago a tu cuenta correctamente.
        </div>
    <?php endif; ?>

    <div style="text-align: center; margin: 30px 0; padding: 20px; background: rgba(46, 204, 113, 0.1); border-radius: 8px; border: 1px solid #2ecc71;">
        <h3 style="color: #2ecc71; font-size: 36px; margin: 10px 0;">$3.500 ARS</h3>
        <p class="muted">Pago único por 30 días de beneficios VIP.</p>

        <!-- AQUÍ ESTÁ TU LINK DE MERCADO PAGO -->
        <a href="https://mpago.la/2e1TLwC" target="_blank" class="at-btn" style="background-color: #009ee3; color: white; font-size: 18px; font-weight: bold; padding: 15px 30px; text-decoration: none; display: inline-block; border-radius: 6px; margin-top: 15px; box-shadow: 0 4px 6px rgba(0,0,0,0.2);">
            💳 PAGAR AHORA CON MERCADO PAGO
        </a>
    </div>

    <div class="at-card" style="background: var(--panel2); border: 1px solid var(--acento);">
        <h3>📌 Pasos obligatorios después de pagar:</h3>
        <ol style="line-height: 1.8;">
            <li>Realiza el pago a través del botón azul de arriba.</li>
            <li>Guarda la <b>captura de pantalla</b> o el PDF del comprobante de pago.</li>
            <li>Envía el comprobante por <b>Discord</b> o <b>WhatsApp</b> al staff del servidor.</li>
            <li>Indica claramente tu nombre de cuenta: <b><?= $logueado ? at_h($cuenta) : '(Debes iniciar sesión para verla aquí)' ?></b>.</li>
            <li>¡Listo! Te activaremos el VIP en un plazo máximo de 1 a 2 horas.</li>
        </ol>
        <p style="color: #ff5252; font-size: 13px; margin-top: 15px;">
            <b>Nota:</b> El staff NUNCA te pedirá tu contraseña. Solo necesitamos el comprobante y tu nombre de cuenta.
        </p>
    </div>
</div>

<?php at_foot(); ?>