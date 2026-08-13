<?php
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';
at_head('Chaos Machine');
?>
<div class="at-tabs">
  <a href="ir.php?amz=Basico">Básico</a>
  <a href="ir.php?amz=Medio">Intermedio</a>
  <a href="ir.php?amz=Quests">Quests</a>
  <a class="on" href="ir.php?amz=Chaos">Chaos Machine</a>
</div>

<div class="at-msg at-info">
  La Chaos Machine está con <b>Chaos Goblin</b> en Noria (180, 100). Si la combinación falla,
  se pierden los materiales. Combiná siempre con la cuenta desconectada de otros dispositivos.
</div>

<div class="at-card">
  <h2>Combinaciones principales</h2>
  <table class="at">
    <tr><th>Combinación</th><th>Materiales</th><th>Éxito base</th></tr>
    <tr><td>Chaos Weapon</td><td>Item +4 o más + Jewel of Chaos + zen</td><td>50% - 80%</td></tr>
    <tr><td>Alas nivel 1</td><td>Chaos Weapon + Jewel of Chaos + 1 item</td><td>60%</td></tr>
    <tr><td>Alas nivel 2</td><td>Alas nivel 1 +9 +Luck + Chaos + Bless + Soul + Creation</td><td>60% - 70%</td></tr>
    <tr><td>Item 380</td><td>Certificate of Strength + Chaos + Bless + Soul + Life</td><td>70%</td></tr>
    <tr><td>Subir a +10 / +11</td><td>Item +9 + Chaos + Bless/Soul (según nivel)</td><td>+10: 55% &middot; +11: 50%</td></tr>
    <tr><td>Subir a +12 / +13</td><td>Item +11 + Chaos + Bless + Soul</td><td>+12: 45% &middot; +13: 40%</td></tr>
  </table>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Cómo mejorar el porcentaje</h2>
    <ul>
      <li>Agregá <b>zen extra</b> en la combinación: cada tramo suma porcentaje.</li>
      <li>Los items con <b>+Luck</b> suman 25% de éxito.</li>
      <li>Combinar con cuenta <b>VIP</b> da un bonus adicional en este servidor.</li>
      <li>Nunca combines items que no estés dispuesto a perder.</li>
    </ul>
  </div>
  <div class="at-card">
    <h2>Errores comunes</h2>
    <ul>
      <li>Combinar sin espacio libre en el inventario: el item resultante se pierde.</li>
      <li>Usar joyas apiladas: la máquina puede tomar toda la pila.</li>
      <li>Intentar +13 sin +Luck: el porcentaje real queda muy bajo.</li>
    </ul>
    <p><a class="at-btn sec" href="ir.php?amz=Shop">Ver alas en la tienda</a></p>
  </div>
</div>
<?php at_foot(); ?>
