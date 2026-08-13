<?php
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';
include dirname(dirname(__DIR__)) . '/Configuraciones/Sitio.php';
at_head('Guía básica');
?>
<div class="at-tabs">
  <a class="on" href="ir.php?amz=Basico">Básico</a>
  <a href="ir.php?amz=Medio">Intermedio</a>
  <a href="ir.php?amz=Quests">Quests</a>
  <a href="ir.php?amz=Chaos">Chaos Machine</a>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>1. Empezar a jugar</h2>
    <ol>
      <li><a href="ir.php?amz=Registro">Creá tu cuenta</a> en la web.</li>
      <li><a href="ir.php?amz=Descargas">Descargá el cliente</a> y descomprimilo.</li>
      <li>Abrí el launcher como administrador e ingresá con tu cuenta.</li>
      <li>Creá tu personaje y elegí clase.</li>
    </ol>
  </div>
  <div class="at-card">
    <h2>2. Clases disponibles</h2>
    <table class="at">
      <tr><th>Clase</th><th>Rol</th><th>Nivel de evolución</th></tr>
      <tr><td>Dark Wizard</td><td>Magia a distancia</td><td>150 (Soul Master)</td></tr>
      <tr><td>Dark Knight</td><td>Cuerpo a cuerpo, tanque</td><td>150 (Blade Knight)</td></tr>
      <tr><td>Fairy Elf</td><td>Soporte y arco</td><td>150 (Muse Elf)</td></tr>
      <tr><td>Magic Gladiator</td><td>Híbrido (nivel 220 de cuenta)</td><td>-</td></tr>
      <tr><td>Dark Lord</td><td>Comandante y cuervo</td><td>-</td></tr>
    </table>
  </div>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>3. Comandos dentro del juego</h2>
    <table class="at">
      <tr><th>Comando</th><th>Para qué sirve</th></tr>
      <tr><td>/reset</td><td>Resetear el personaje (nivel <?= at_h($Maxear) ?>)</td></tr>
      <tr><td>/addstr /addagi /addvit /addene</td><td>Agregar puntos rápido</td></tr>
      <tr><td>/post mensaje</td><td>Mensaje global (requiere item o zen)</td></tr>
      <tr><td>/pkclear</td><td>Limpiar estado de asesino</td></tr>
      <tr><td>/requests off</td><td>Bloquear invitaciones molestas</td></tr>
    </table>
  </div>
  <div class="at-card">
    <h2>4. Consejos para empezar bien</h2>
    <ul>
      <li>Farmeá en Lorencia y Noria hasta nivel 40 antes de pasar a Davias.</li>
      <li>Guardá tus joyas: valen más que el zen en la economía del server.</li>
      <li>Usá el <b>banco de zen</b> del panel de usuario para no perder zen si te matan.</li>
      <li>Sumate al Discord: ahí se avisan los eventos e invasiones.</li>
      <li>Nunca compartas tu cuenta ni tu contraseña, ni siquiera con "amigos".</li>
    </ul>
    <p><a class="at-btn sec" href="ir.php?amz=Medio">Seguir con la guía intermedia</a></p>
  </div>
</div>
<?php at_foot(); ?>
