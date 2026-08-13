<?php
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';
include dirname(dirname(__DIR__)) . '/Configuraciones/Sitio.php';
at_head('Guía intermedia');
?>
<div class="at-tabs">
  <a href="ir.php?amz=Basico">Básico</a>
  <a class="on" href="ir.php?amz=Medio">Intermedio</a>
  <a href="ir.php?amz=Quests">Quests</a>
  <a href="ir.php?amz=Chaos">Chaos Machine</a>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Resets</h2>
    <div class="at-stat"><span>Nivel necesario</span><b><?= at_h($Maxear) ?></b></div>
    <div class="at-stat"><span>Costo</span><b><?= at_h($Costoreset) ?></b></div>
    <div class="at-stat"><span>Sistema</span><b><?= at_h($Reset) ?></b></div>
    <p class="muted">Los VIP resetean gratis desde el juego. Después del reset volvés a nivel 1 conservando tus items.</p>
  </div>
  <div class="at-card">
    <h2>Stats y limpieza de PK</h2>
    <div class="at-stat"><span>Costo de agregar stats</span><b><?= at_h($Costostats) ?></b></div>
    <div class="at-stat"><span>Limpiar PK</span><b><?= at_h($Costopk) ?></b></div>
    <p class="muted">Podés depositar tu zen en el banco desde el panel de usuario y retirarlo cuando lo necesites.</p>
  </div>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Joyas</h2>
    <table class="at">
      <tr><th>Joya</th><th>Función</th><th>Éxito</th></tr>
      <tr><td>Jewel of Bless</td><td>+1 al item (hasta +6)</td><td><?= at_h($Blessbug) === 'OFF!' ? 'Estándar' : 'Especial' ?></td></tr>
      <tr><td>Jewel of Soul</td><td>+1 al item (hasta +9)</td><td><?= at_h($Joyas) ?></td></tr>
      <tr><td>Jewel of Life</td><td>Agrega/sube opción adicional</td><td>Ver configuración</td></tr>
      <tr><td>Jewel of Creation</td><td>Chaos Machine (alas y items 380)</td><td>-</td></tr>
      <tr><td>Jewel of Chaos</td><td>Combinaciones en Noria</td><td>-</td></tr>
    </table>
  </div>
  <div class="at-card">
    <h2>Excellent y sets</h2>
    <ul>
      <li>Los excellent caen en los mapas altos y en los eventos (Devil Square, Blood Castle).</li>
      <li>Las mejores opciones para PVP: <b>+Vida</b>, <b>+Daño reflejado</b> y <b>+Velocidad de ataque</b>.</li>
      <li>Para farmear conviene <b>+Recuperación de maná/vida por golpe</b>.</li>
      <li>Con set completo excellent ganás bonus adicionales de defensa.</li>
    </ul>
    <h3>Alas</h3>
    <p>Las alas de nivel 2 se arman en la Chaos Machine con Chaos + Bless + Soul + Creation.
       También podés conseguirlas en la <a href="ir.php?amz=Shop">tienda</a>.</p>
  </div>
</div>

<div class="at-card">
  <h2>Eventos activos</h2>
  <p><?= at_h($Eventos) ?></p>
  <p class="muted">Los horarios se anuncian en el Discord oficial y en <a href="ir.php?amz=Noticias">Noticias</a>.</p>
</div>
<?php at_foot(); ?>
