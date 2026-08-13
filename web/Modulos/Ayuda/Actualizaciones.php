<?php
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';

/* ==== HISTORIAL DE CAMBIOS (editar acá) ==== */
$cambios = array(
    array('29/07/2026', 'Web modernizada', array(
        'Registro y cambio de contraseña reescritos para PHP 8.',
        'Panel de usuario y ficha de personaje nuevos, con banco de zen.',
        'Tienda web con entrega automática de VIP y alas.',
        'Sistema de retención de compras cuando el baúl está lleno.',
    )),
    array('15/07/2026', 'Ajustes de balance', array(
        'Revisión de drop en mapas altos.',
        'Nuevos horarios de eventos.',
    )),
);

at_head('Actualizaciones');
?>
<p class="muted">Historial de cambios del servidor y de la web de <?= AT_MARCA ?>.</p>
<?php foreach ($cambios as $c): ?>
  <div class="at-card">
    <h2 style="margin-top:0"><?= at_h($c[1]) ?></h2>
    <p class="muted"><?= at_h($c[0]) ?></p>
    <ul><?php foreach ($c[2] as $i): ?><li><?= at_h($i) ?></li><?php endforeach; ?></ul>
  </div>
<?php endforeach; ?>
<p><a class="at-btn sec" href="ir.php?amz=Ayuda">Volver a Ayuda</a></p>
<?php at_foot(); ?>
