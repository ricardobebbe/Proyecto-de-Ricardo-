<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';
include dirname(__DIR__) . '/Configuraciones/Sitio.php';

/* ==== EDITÁ ACÁ TUS LINKS DE DESCARGA ==== */
$descargas = array(
    array(
        'nombre' => 'Cliente completo ' . AT_MARCA . ' (Full)',
        'peso'   => '1.4 GB',
        'desc'   => 'Cliente listo para jugar. Ya viene con el parche más reciente aplicado.',
        'links'  => array(
            'MEGA'          => 'https://mega.nz/',
            'MediaFire'     => 'https://www.mediafire.com/',
            'Google Drive'  => 'https://drive.google.com/',
        ),
    ),
    array(
        'nombre' => 'Parche / Actualización',
        'peso'   => '35 MB',
        'desc'   => 'Solo si ya tenés el cliente instalado. Descomprimir dentro de la carpeta del juego y reemplazar.',
        'links'  => array(
            'MEGA'      => 'https://mega.nz/',
            'MediaFire' => 'https://www.mediafire.com/',
        ),
    ),
    array(
        'nombre' => 'Launcher / Conector',
        'peso'   => '4 MB',
        'desc'   => 'Ejecutable de conexión al servidor. Abrir siempre como administrador.',
        'links'  => array('Descarga directa' => 'https://mega.nz/'),
    ),
);

at_head('Descargas');
?>
<div class="at-msg at-info">
  Antes de instalar, desactivá temporalmente el antivirus y <b>ejecutá el juego como administrador</b>.
  El cliente de MU suele ser detectado como falso positivo.
</div>

<div class="at-grid c3">
  <?php foreach ($descargas as $d): ?>
    <div class="at-card">
      <h3><?= at_h($d['nombre']) ?></h3>
      <p class="muted"><?= at_h($d['desc']) ?></p>
      <div class="at-stat"><span>Tamaño</span><b><?= at_h($d['peso']) ?></b></div>
      <p style="margin-top:12px">
        <?php foreach ($d['links'] as $t => $u): ?>
          <a class="at-btn sec" style="margin:3px 0" href="<?= at_h($u) ?>" target="_blank" rel="noopener"><?= at_h($t) ?></a>
        <?php endforeach; ?>
      </p>
    </div>
  <?php endforeach; ?>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Requisitos mínimos</h2>
    <div class="at-stat"><span>Sistema</span><b>Windows 7 / 8 / 10 / 11 (64 bits)</b></div>
    <div class="at-stat"><span>Procesador</span><b>Dual Core 2.0 GHz</b></div>
    <div class="at-stat"><span>Memoria</span><b>2 GB RAM (recomendado 4 GB)</b></div>
    <div class="at-stat"><span>Video</span><b>128 MB compatible DirectX 9</b></div>
    <div class="at-stat"><span>Disco</span><b>3 GB libres</b></div>
    <div class="at-stat"><span>Internet</span><b>1 Mbps estable</b></div>
  </div>

  <div class="at-card">
    <h2>Cómo instalar (paso a paso)</h2>
    <ol>
      <li>Descargá el <b>cliente completo</b> desde cualquiera de los links.</li>
      <li>Descomprimí con WinRAR o 7-Zip en una carpeta fuera de <i>Archivos de programa</i>, por ejemplo <code>C:\<?= AT_MARCA ?></code>.</li>
      <li>Agregá esa carpeta a las excepciones del antivirus y de Windows Defender.</li>
      <li><a href="ir.php?amz=Registro">Creá tu cuenta</a> en la web si todavía no la tenés.</li>
      <li>Abrí el <b>launcher</b> como administrador, ingresá tu cuenta y jugá.</li>
    </ol>
    <h3>Datos del servidor</h3>
    <div class="at-stat"><span>Versión</span><b><?= at_h($Version) ?></b></div>
    <div class="at-stat"><span>Exp / Drop</span><b><?= at_h($Expydrop) ?></b></div>
  </div>
</div>

<div class="at-card">
  <h2>Problemas frecuentes</h2>
  <table class="at">
    <tr><th>Error</th><th>Solución</th></tr>
    <tr><td>El juego cierra al abrir</td><td>Ejecutar como administrador y en modo compatibilidad Windows 7.</td></tr>
    <tr><td>"Connection fail" / no conecta</td><td>Revisá tu firewall y probá con otra conexión. El servidor puede estar en mantenimiento.</td></tr>
    <tr><td>Falta un archivo .dll</td><td>Instalá DirectX 9.0c y los Visual C++ Redistributables 2010 y 2015-2022.</td></tr>
    <tr><td>El antivirus borra el .exe</td><td>Es falso positivo: agregá la carpeta del juego a las excepciones y volvé a descomprimir.</td></tr>
    <tr><td>Pantalla negra al entrar</td><td>Editá la resolución en las opciones del launcher o borrá el archivo de configuración del cliente.</td></tr>
  </table>
  <p class="muted">¿Seguís con problemas? Escribinos por <a href="ir.php?amz=Contacto">Contacto</a> o en el Discord oficial.</p>
</div>
<?php at_foot(); ?>
