<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

$secciones = array(
    'Basico'          => array('Guía básica', 'Primeros pasos: crear cuenta, personaje, comandos y niveles iniciales.'),
    'Medio'           => array('Guía intermedia', 'Resets, stats, joyas, excellent, alas y sets de nivel medio.'),
    'Quests'          => array('Quests', 'Todas las misiones del servidor y cómo completarlas.'),
    'Chaos'           => array('Chaos Machine', 'Combinaciones, porcentajes y consejos para no perder items.'),
    'CofresDeZen'     => array('Cofres de zen', 'Qué son, dónde aparecen y qué pueden dar.'),
    'Parche'          => array('Parche y cliente', 'Descargas, instalación y solución de problemas.'),
    'Actualizaciones' => array('Actualizaciones', 'Historial de cambios del servidor.'),
);

at_head('Ayuda y guías');
?>
<p class="muted">Todo lo que necesitás para empezar y para sacarle el jugo a <?= AT_MARCA ?>.</p>
<div class="at-grid c3">
  <?php foreach ($secciones as $k => $s): ?>
    <div class="at-card">
      <h3><?= at_h($s[0]) ?></h3>
      <p class="muted"><?= at_h($s[1]) ?></p>
      <p><a class="at-btn sec" href="ir.php?amz=<?= at_h($k) ?>">Abrir guía</a></p>
    </div>
  <?php endforeach; ?>
</div>

<div class="at-card">
  <h2>¿No encontrás lo que buscás?</h2>
  <p>Escribinos por <a href="ir.php?amz=Contacto">Contacto</a> o entrá al Discord oficial: el staff y la comunidad responden todos los días.</p>
</div>
<?php at_foot(); ?>
