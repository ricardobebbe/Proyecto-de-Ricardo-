<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';
at_head('Cofres de Zen (Zen Warden)');
?>
<div class="at-grid c2">
  <div class="at-card">
    <h2>¿Para qué sirven?</h2>
    <p>El comando <b>/trade</b> sólo permite pasar 50.000.000 de zen por intercambio. Los Cofres de Zen
       son baúles de oro que sirven para comerciar items caros sin esa limitación.</p>
    <p>Podés tener tantos cofres como te entren en el inventario o en el baúl.</p>
  </div>
  <div class="at-card">
    <h2>Cómo conseguirlos</h2>
    <p>Tenés que estar en <b>Lorencia, Devias o Noria</b> y usar el comando correspondiente:</p>
    <table class="at">
      <tr><th>Comando</th><th>Cofre</th><th>Valor</th></tr>
      <tr><td>/bronce</td><td>Cofre de Bronce</td><td>500.000.000 zen</td></tr>
      <tr><td>/plata</td><td>Cofre de Plata</td><td>1.000.000.000 zen</td></tr>
      <tr><td>/oro</td><td>Cofre de Oro</td><td>2.000.000.000 zen</td></tr>
    </table>
  </div>
</div>

<div class="at-card">
  <h2>Cómo volver a convertirlos en zen</h2>
  <p>Buscá al NPC <b>Zen Warden</b>. Si tenés cofres en el inventario, deposita automáticamente
     el equivalente en tu <b>Banco Web</b>, que podés ver y retirar desde el
     <a href="ir.php?amz=Usuario">panel de usuario</a>.</p>
  <ul>
    <li><span class="muted">[AVISO] Usted no tiene cofres en el inventario</span> — no llevás ningún cofre.</li>
    <li><span class="green">[AVISO] Depositaste [ XX ] zen en el WebBank</span> — cambio exitoso.</li>
  </ul>
</div>

<div class="at-card">
  <h2>Preguntas frecuentes</h2>
  <table class="at">
    <tr><th>Pregunta</th><th>Respuesta</th></tr>
    <tr><td>¿Tengo que venderlos en un shop NPC?</td><td>No. Sólo el Zen Warden te da el equivalente exacto en zen.</td></tr>
    <tr><td>Hice el cambio, ¿dónde está mi zen?</td><td>En el Banco Web de tu cuenta, dentro del panel de usuario de <?= AT_MARCA ?>.</td></tr>
    <tr><td>¿Por qué no usar el Banco Web para los trades?</td><td>Los cofres son 100% seguros e instantáneos; el banco sólo sirve para guardar y transferir tu propio zen.</td></tr>
    <tr><td>¿Los cofres se pueden caer al piso?</td><td>No se pueden tirar a propósito, pero en estado asesino (Phonomania) podés perderlos al morir.</td></tr>
    <tr><td>¿Puedo tener dos cofres de 2.000.000.000 a la vez?</td><td>Un personaje no puede superar 2.000.000.000 de zen en el inventario.</td></tr>
  </table>
  <p><a class="at-btn sec" href="ir.php?amz=Ayuda">Volver a Ayuda</a></p>
</div>
<?php at_foot(); ?>
