<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

/* ==== EDITÁ TUS DATOS DE CONTACTO ==== */
$discord  = 'https://discord.gg/mumisiones';
$whatsapp = 'https://wa.me/5490000000000';
$facebook = 'https://www.facebook.com/MuMisiones';
$mailSop  = 'soporte@mumisiones.com';
$mailShop = 'ventas@mumisiones.com';

at_head('Contacto');
?>
<div class="at-grid c3">
  <div class="at-card">
    <h3>Discord (recomendado)</h3>
    <p class="muted">La forma más rápida de hablar con el staff. Soporte, reportes y novedades.</p>
    <p><a class="at-btn" href="<?= at_h($discord) ?>" target="_blank" rel="noopener">Entrar al Discord</a></p>
  </div>
  <div class="at-card">
    <h3>WhatsApp</h3>
    <p class="muted">Consultas sobre compras y pagos.</p>
    <p><a class="at-btn sec" href="<?= at_h($whatsapp) ?>" target="_blank" rel="noopener">Escribir por WhatsApp</a></p>
  </div>
  <div class="at-card">
    <h3>Facebook</h3>
    <p class="muted">Página oficial. Es la única cuenta que usamos en esa red.</p>
    <p><a class="at-btn sec" href="<?= at_h($facebook) ?>" target="_blank" rel="noopener">Ver la página</a></p>
  </div>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Correos</h2>
    <div class="at-stat"><span>Soporte general</span><b><?= at_h($mailSop) ?></b></div>
    <div class="at-stat"><span>Compras y donaciones</span><b><?= at_h($mailShop) ?></b></div>
    <p class="muted">Respondemos dentro de las 24 hs hábiles. Incluí siempre tu nombre de cuenta.</p>
  </div>
  <div class="at-card">
    <h2>Antes de escribir</h2>
    <ul>
      <li>Revisá las <a href="ir.php?amz=Reglas">reglas</a> y la <a href="ir.php?amz=Ayuda">sección de ayuda</a>.</li>
      <li>Si tu compra quedó retenida, entrá a <a href="ir.php?amz=MisPedidos">Mis pedidos</a> y usá "Reintentar entrega".</li>
      <li>El staff <b>nunca</b> te va a pedir la contraseña de tu cuenta ni de tu email.</li>
      <li>Para reportar un bug, adjuntá capturas y el nombre del personaje.</li>
    </ul>
  </div>
</div>
<?php at_foot(); ?>
