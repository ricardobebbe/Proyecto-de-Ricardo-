<?php
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';
at_head('Quests del servidor');
?>
<div class="at-tabs">
  <a href="ir.php?amz=Basico">Básico</a>
  <a href="ir.php?amz=Medio">Intermedio</a>
  <a class="on" href="ir.php?amz=Quests">Quests</a>
  <a href="ir.php?amz=Chaos">Chaos Machine</a>
</div>

<div class="at-card">
  <h2>Quests de evolución</h2>
  <table class="at">
    <tr><th>#</th><th>Quest</th><th>Nivel</th><th>Requisitos</th><th>Recompensa</th></tr>
    <tr><td>1</td><td>Find the Scroll of Emperor</td><td>150</td><td>Scroll of Emperor + 10.000.000 zen</td><td>10 puntos + habilita 2ª quest</td></tr>
    <tr><td>2</td><td>Treasure of Mine</td><td>180</td><td>Broken Sword + Tear of Elf + Soul Shard of Wizard</td><td>10 puntos + cambio de clase</td></tr>
    <tr><td>3</td><td>Gain the Certificate of Strength</td><td>380</td><td>Flame of Death Beam Knight + Horn of Hell Maine + Feather of Dark Phoenix</td><td>Habilita items 380</td></tr>
  </table>
  <p class="muted">Todas las quests se inician hablando con <b>Sebina the Priestess</b> en Devias (182, 30).</p>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Dónde conseguir los items</h2>
    <table class="at">
      <tr><th>Item</th><th>Mob / Lugar</th></tr>
      <tr><td>Scroll of Emperor</td><td>Mobs de Lost Tower y Atlans</td></tr>
      <tr><td>Broken Sword</td><td>Lost Tower / Tarkan</td></tr>
      <tr><td>Tear of Elf</td><td>Atlans</td></tr>
      <tr><td>Soul Shard of Wizard</td><td>Icarus</td></tr>
      <tr><td>Flame of Death Beam Knight</td><td>Death Beam Knight (Kalima / Icarus)</td></tr>
      <tr><td>Horn of Hell Maine</td><td>Hell Maine</td></tr>
      <tr><td>Feather of Dark Phoenix</td><td>Dark Phoenix / invasiones</td></tr>
    </table>
  </div>
  <div class="at-card">
    <h2>Consejos</h2>
    <ul>
      <li>Guardá siempre un espacio libre en el inventario antes de entregar la quest.</li>
      <li>Si fallás la quest perdés los items: no la inicies sin tener todo.</li>
      <li>Los puntos de quest se agregan automáticamente al terminar.</li>
      <li>Con VIP conseguís mejor drop de los items de quest.</li>
    </ul>
    <p><a class="at-btn sec" href="ir.php?amz=Chaos">Ir a Chaos Machine</a></p>
  </div>
</div>
<?php at_foot(); ?>
