<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';
include dirname(__DIR__) . '/Configuraciones/Sitio.php';
include dirname(__DIR__) . '/Configuraciones/Servidor.php'; // datos privados del GameServer

/* ---- Estado del GameServer ---- */
$estado = false;
$fp = @fsockopen($Iphost, (int) $Portgs, $errno, $errstr, 2);
if ($fp) { $estado = true; @fclose($fp); }

/* ---- Estadísticas en vivo ---- */
function at_uno($sql)
{
    $r = mssql_fetch_row(mssql_query($sql));
    return isset($r[0]) ? $r[0] : 0;
}
$online   = (int) at_uno('SELECT COUNT(*) FROM MEMB_STAT WHERE ConnectStat > 0');
$cuentas  = (int) at_uno('SELECT COUNT(*) FROM MEMB_INFO');
$perso    = (int) at_uno('SELECT COUNT(*) FROM Character');
$guilds   = (int) at_uno('SELECT COUNT(*) FROM Guild');
$vips     = (int) at_uno('SELECT COUNT(*) FROM MEMB_INFO WHERE vip > 0');
$topReset = (int) at_uno('SELECT ISNULL(MAX(Resets),0) FROM Character');

at_head('Información del servidor');
?>
<div class="at-card">
  <h2>Estado en vivo</h2>
  <div class="at-grid c3">
    <div><div class="at-stat"><span>GameServer</span><b class="<?= $estado ? 'green' : 'red' ?>">
      <i class="on-dot <?= $estado ? 'si' : '' ?>"></i> <?= $estado ? 'ONLINE' : 'OFFLINE' ?></b></div>
      <div class="at-stat"><span>Conexión</span><b>Se configura sola con el parche oficial</b></div></div>
    <div><div class="at-stat"><span>Jugadores conectados</span><b class="gold"><?= at_num($online) ?></b></div>
      <div class="at-stat"><span>Cuentas creadas</span><b><?= at_num($cuentas) ?></b></div></div>
    <div><div class="at-stat"><span>Personajes</span><b><?= at_num($perso) ?></b></div>
      <div class="at-stat"><span>Guilds</span><b><?= at_num($guilds) ?></b></div></div>
  </div>
  <div class="at-grid c3">
    <div class="at-stat"><span>Cuentas VIP activas</span><b class="green"><?= at_num($vips) ?></b></div>
    <div class="at-stat"><span>Récord de resets</span><b class="gold"><?= at_num($topReset) ?></b></div>
    <div class="at-stat"><span>Inicio del servidor</span><b><?= at_h($Inicio) ?></b></div>
  </div>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Configuración del juego</h2>
    <div class="at-stat"><span>Versión</span><b><?= at_h($Version) ?></b></div>
    <div class="at-stat"><span>Experiencia / Drop</span><b><?= at_h($Expydrop) ?></b></div>
    <div class="at-stat"><span>Bless bug</span><b><?= at_h($Blessbug) ?></b></div>
    <div class="at-stat"><span>HP de los mobs</span><b><?= at_h($Hpmob) ?></b></div>
    <div class="at-stat"><span>Joyas</span><b><?= at_h($Joyas) ?></b></div>
    <div class="at-stat"><span>Administración</span><b><?= at_h($Admins) ?></b></div>
  </div>

  <div class="at-card">
    <h2>Economía y resets</h2>
    <div class="at-stat"><span>Nivel para resetear</span><b><?= at_h($Maxear) ?></b></div>
    <div class="at-stat"><span>Sistema de reset</span><b><?= at_h($Reset) ?></b></div>
    <div class="at-stat"><span>Costo del reset</span><b><?= at_h($Costoreset) ?></b></div>
    <div class="at-stat"><span>Costo de stats</span><b><?= at_h($Costostats) ?></b></div>
    <div class="at-stat"><span>Limpieza de PK</span><b><?= at_h($Costopk) ?></b></div>
  </div>
</div>

<div class="at-grid c2">
  <div class="at-card">
    <h2>Mapas habilitados</h2>
    <p><?= at_h($Mapas) ?></p>
    <h2>Eventos</h2>
    <p><?= at_h($Eventos) ?></p>
  </div>

  <div class="at-card">
    <h2>Hardware del servidor</h2>
    <div class="at-stat"><span>Ubicación</span><b><?= at_h($Ubicado) ?></b></div>
    <div class="at-stat"><span>Procesador</span><b><?= at_h($Procesador) ?></b></div>
    <div class="at-stat"><span>Memoria</span><b><?= at_h($Memoria) ?></b></div>
    <div class="at-stat"><span>Conexión</span><b><?= at_h($Conexion) ?></b></div>
    <p style="margin-top:14px">
      <a class="at-btn" href="ir.php?amz=Registro">Crear cuenta</a>
      <a class="at-btn sec" href="ir.php?amz=Descargas">Descargar</a>
    </p>
  </div>
</div>
<?php at_foot(); ?>
