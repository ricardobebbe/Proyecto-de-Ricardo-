<?php
$filas = at_filas("SELECT TOP 200 s.memb___id, s.ServerName, s.IP, s.ConnectTM,
                          (SELECT TOP 1 Name FROM Character c WHERE c.AccountID = s.memb___id ORDER BY c.cLevel DESC) AS Personaje
                   FROM MEMB_STAT s WHERE s.ConnectStat = 1 ORDER BY s.memb___id");
at_cabecera();
?>
<h1>Jugadores en línea (<?php echo count($filas); ?>)</h1>
<table>
  <tr><th>Cuenta</th><th>Personaje</th><th>Servidor</th><th>IP</th><th>Conectado desde</th><th></th></tr>
  <?php foreach ($filas as $r): ?>
  <tr>
    <td><b><?php echo h($r['memb___id']); ?></b></td>
    <td><?php echo h($r['Personaje']); ?></td>
    <td class="suave"><?php echo h($r['ServerName']); ?></td>
    <td class="suave"><?php echo h($r['IP']); ?></td>
    <td class="suave"><?php echo h($r['ConnectTM']); ?></td>
    <td><a class="btn btn-fantasma" href="index.php?s=cuentas&c=<?php echo urlencode($r['memb___id']); ?>">Administrar</a></td>
  </tr>
  <?php endforeach; ?>
  <?php if (!$filas): ?><tr><td colspan="6" class="suave">No hay nadie conectado en este momento.</td></tr><?php endif; ?>
</table>
<?php at_pie(); ?>
