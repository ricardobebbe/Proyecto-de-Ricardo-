<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';
include dirname(__DIR__) . '/Configuraciones/Sitio.php';

/* Noticias desde la base (tabla web_noticias). Si la tabla no existe se usan las estáticas. */
$noticias = array();
$q = @mssql_query("SELECT TOP 10 titulo, cuerpo, autor, CONVERT(varchar(19), fecha, 120) AS fecha
                   FROM web_noticias WHERE activo = 1 ORDER BY fecha DESC");
if ($q && mssql_num_rows($q) > 0) {
    while (($r = mssql_fetch_assoc($q))) {
        $noticias[] = array($r['titulo'], $r['cuerpo'], $r['autor'], $r['fecha']);
    }
}
if (!$noticias) {
    $noticias = array(
        array('Bienvenidos a ' . AT_MARCA,
              'Servidor ' . $Version . ' con experiencia ' . $Expydrop . '. Creá tu cuenta, descargá el cliente y sumate a la comunidad. Todas las novedades y eventos se anuncian también en nuestro Discord oficial.',
              'Staff', date('Y-m-d H:i:s')),
        array('Tienda con entrega automática',
              'Ya podés comprar VIP y alas desde la web con MercadoPago. La entrega es automática: el VIP se activa al instante y los items se envían a tu baúl. Si el baúl está lleno, la compra queda guardada y la podés reclamar desde "Mis pedidos".',
              'Staff', date('Y-m-d H:i:s')),
        array('Eventos de la semana',
              $Eventos,
              'Staff', date('Y-m-d H:i:s')),
    );
}

at_head('Noticias');
?>
<div class="at-grid c2">
  <div>
    <?php foreach ($noticias as $n): ?>
      <div class="at-card">
        <h2 style="margin-top:0"><?= at_h($n[0]) ?></h2>
        <p class="muted"><?= at_h($n[3]) ?> &middot; por <?= at_h($n[2]) ?></p>
        <p><?= nl2br(at_h($n[1])) ?></p>
      </div>
    <?php endforeach; ?>
  </div>

  <div>
    <div class="at-card">
      <h2>El servidor</h2>
      <div class="at-stat"><span>Versión</span><b><?= at_h($Version) ?></b></div>
      <div class="at-stat"><span>Exp / Drop</span><b><?= at_h($Expydrop) ?></b></div>
      <div class="at-stat"><span>Reset</span><b><?= at_h($Reset) ?></b></div>
      <div class="at-stat"><span>Máx. level</span><b><?= at_h($Maxear) ?></b></div>
      <p style="margin-top:14px">
        <a class="at-btn" href="ir.php?amz=Registro">Crear cuenta</a>
        <a class="at-btn sec" href="ir.php?amz=Descargas">Descargar</a>
      </p>
    </div>
    <div class="at-card">
      <h2>Comunidad</h2>
      <p>Novedades, soporte y sorteos en el Discord oficial de <?= AT_MARCA ?>.</p>
      <p><a class="at-btn sec" href="ir.php?amz=Comunidad">Ir a comunidad</a>
         <a class="at-btn sec" href="ir.php?amz=Contacto">Contacto</a></p>
    </div>
  </div>
</div>
<?php at_foot(); ?>
