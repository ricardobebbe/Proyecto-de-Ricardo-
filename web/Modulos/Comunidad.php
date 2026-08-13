<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';
include_once dirname(__DIR__) . '/Configuraciones/Sitio.php';

at_head('Comunidad ' . AT_MARCA);
?>
<div class="at-grid c2">
  <div class="at-card">
    <h3>Guild oficial <?= AT_GUILD ?></h3>
    <p class="muted">La guild <b><?= AT_GUILD ?></b> es la casa del staff y de los jugadores veteranos de <?= AT_MARCA ?>.
    Sumate para participar de los eventos organizados, castle siege y torneos internos.</p>
  </div>
  <div class="at-card">
    <h3>Reglas de convivencia</h3>
    <ul class="muted">
      <li>Respeto entre jugadores dentro y fuera del juego.</li>
      <li>Prohibido el uso de hacks, bots externos o bugs.</li>
      <li>El staff nunca pide contraseñas.</li>
    </ul>
    <a class="at-btn" href="ir.php?amz=Reglas">Ver reglas completas</a>
  </div>
  <div class="at-card">
    <h3>Servidor</h3>
    <div class="at-stat"><span>Versión</span><b><?= at_h($Version) ?></b></div>
    <div class="at-stat"><span>Exp / Drop</span><b><?= at_h($Expydrop) ?></b></div>
    <div class="at-stat"><span>Eventos</span><b><?= at_h($Eventos) ?></b></div>
  </div>
  <div class="at-card">
    <h3>¿Dudas?</h3>
    <p class="muted">Escribinos y un moderador te responde.</p>
    <a class="at-btn" href="ir.php?amz=Contacto">Contacto</a>
  </div>
</div>
<?php at_foot(); ?>
