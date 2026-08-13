<?php
$totalCuentas   = (int) at_uno("SELECT COUNT(*) FROM MEMB_INFO");
$totalPersonajes= (int) at_uno("SELECT COUNT(*) FROM Character");
$totalGuilds    = (int) at_uno("SELECT COUNT(*) FROM Guild");
$online         = (int) at_uno("SELECT COUNT(*) FROM MEMB_STAT WHERE ConnectStat=1");
$bloqueadas     = (int) at_uno("SELECT COUNT(*) FROM MEMB_INFO WHERE bloc_code=1");
$hoy            = (int) at_uno("SELECT COUNT(*) FROM MEMB_INFO WHERE appl_days >= DATEADD(day,-1,GETDATE())");
$vips           = at_hay_columna('MEMB_INFO','vip')
                ? (int) at_uno("SELECT COUNT(*) FROM MEMB_INFO WHERE ISNULL(vip,0) > 0 AND (vencimiento IS NULL OR vencimiento > GETDATE())")
                : 0;

$ultimas = at_filas("SELECT TOP 10 memb___id, mail_addr, appl_days FROM MEMB_INFO ORDER BY appl_days DESC");
$topResets = at_hay_columna('Character','Resets')
    ? at_filas("SELECT TOP 10 Name, cLevel, Resets, Class FROM Character ORDER BY Resets DESC, cLevel DESC")
    : at_filas("SELECT TOP 10 Name, cLevel, 0 AS Resets, Class FROM Character ORDER BY cLevel DESC");

at_cabecera();
?>
<h1>Resumen del servidor</h1>
<div class="tarjetas">
  <div class="tarjeta"><div class="n"><?php echo number_format($online); ?></div><div class="t">Jugadores en línea</div></div>
  <div class="tarjeta"><div class="n"><?php echo number_format($totalCuentas); ?></div><div class="t">Cuentas</div></div>
  <div class="tarjeta"><div class="n"><?php echo number_format($totalPersonajes); ?></div><div class="t">Personajes</div></div>
  <div class="tarjeta"><div class="n"><?php echo number_format($totalGuilds); ?></div><div class="t">Guilds</div></div>
  <div class="tarjeta"><div class="n"><?php echo number_format($vips); ?></div><div class="t">VIP activos</div></div>
  <div class="tarjeta"><div class="n"><?php echo number_format($bloqueadas); ?></div><div class="t">Cuentas bloqueadas</div></div>
  <div class="tarjeta"><div class="n"><?php echo number_format($hoy); ?></div><div class="t">Registros (24 h)</div></div>
</div>

<h2>Últimas cuentas creadas</h2>
<table>
  <tr><th>Cuenta</th><th>Email</th><th>Fecha</th><th></th></tr>
  <?php foreach ($ultimas as $r): ?>
  <tr>
    <td><b><?php echo h($r['memb___id']); ?></b></td>
    <td class="suave"><?php echo h($r['mail_addr']); ?></td>
    <td class="suave"><?php echo h($r['appl_days']); ?></td>
    <td><a class="btn btn-fantasma" href="index.php?s=cuentas&c=<?php echo urlencode($r['memb___id']); ?>">Ver</a></td>
  </tr>
  <?php endforeach; ?>
</table>

<h2>Top 10 personajes</h2>
<table>
  <tr><th>#</th><th>Personaje</th><th>Clase</th><th>Level</th><th>Resets</th><th></th></tr>
  <?php $i = 1; foreach ($topResets as $r): ?>
  <tr>
    <td><?php echo $i++; ?></td>
    <td><b><?php echo h($r['Name']); ?></b></td>
    <td class="suave"><?php echo h(at_clase($r['Class'])); ?></td>
    <td><?php echo (int) $r['cLevel']; ?></td>
    <td><?php echo (int) $r['Resets']; ?></td>
    <td><a class="btn btn-fantasma" href="index.php?s=personajes&p=<?php echo urlencode($r['Name']); ?>">Editar</a></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php at_pie(); ?>
