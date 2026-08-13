<?php
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';
at_head('Parche y cliente');
?>
<div class="at-grid c2">
  <div class="at-card">
    <h2>Ingresar al juego, paso a paso</h2>
    <ol>
      <li><a href="ir.php?amz=Descargas">Descargá el parche o el cliente completo</a>.</li>
      <li>Descomprimilo dentro de la carpeta del juego, reemplazando los archivos.</li>
      <li>Borrá los archivos viejos que ya no se usan (los indica el instructivo del parche).</li>
      <li>Ejecutá <b>Launcher.exe</b> como administrador. Si aparece un error de actualización, cerralo.</li>
      <li>Volvé a abrir el Launcher e ingresá con tu cuenta. ¡Listo!</li>
    </ol>
  </div>
  <div class="at-card">
    <h2>Si algo falla</h2>
    <ul>
      <li>Agregá la carpeta del juego a las excepciones del antivirus antes de descomprimir.</li>
      <li>Instalá DirectX 9.0c y Visual C++ 2010 / 2015-2022.</li>
      <li>No instales el juego dentro de <i>Archivos de programa</i>.</li>
      <li>Si el launcher no conecta, revisá tu firewall y probá con otra red.</li>
    </ul>
    <p>
      <a class="at-btn" href="ir.php?amz=Descargas">Ir a Descargas</a>
      <a class="at-btn sec" href="ir.php?amz=Contacto">Pedir ayuda</a>
    </p>
  </div>
</div>
<?php at_foot(); ?>
