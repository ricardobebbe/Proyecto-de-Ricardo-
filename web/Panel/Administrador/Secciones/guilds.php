<?php
$buscar = isset($_GET['q']) ? at_esc(trim($_GET['q'])) : '';
$where = $buscar !== '' ? "WHERE G_Name LIKE '%$buscar%' OR G_Master LIKE '%$buscar%'" : '';
$filas = at_filas("SELECT TOP 100 G_Name, G_Master, ISNULL(G_Score,0) AS G_Score,
                          (SELECT COUNT(*) FROM GuildMember m WHERE m.G_Name = g.G_Name) AS Miembros
                   FROM Guild g $where ORDER BY G_Score DESC");
at_cabecera();
?>
<h1>Guilds</h1>
<div class="caja">
  <form method="get" class="fila">
    <input type="hidden" name="s" value="guilds">
    <div style="flex:1;min-width:220px"><label>Buscar guild o master</label><input type="text" name="q" value="<?php echo h(isset($_GET['q']) ? $_GET['q'] : ''); ?>"></div>
    <button class="btn" type="submit">Buscar</button>
  </form>
</div>
<table>
  <tr><th>#</th><th>Guild</th><th>Master</th><th>Miembros</th><th>Puntos</th></tr>
  <?php $i = 1; foreach ($filas as $r): ?>
  <tr>
    <td><?php echo $i++; ?></td>
    <td><b><?php echo h($r['G_Name']); ?></b></td>
    <td><a href="index.php?s=personajes&p=<?php echo urlencode($r['G_Master']); ?>"><?php echo h($r['G_Master']); ?></a></td>
    <td><?php echo (int) $r['Miembros']; ?></td>
    <td><?php echo (int) $r['G_Score']; ?></td>
  </tr>
  <?php endforeach; ?>
  <?php if (!$filas): ?><tr><td colspan="5" class="suave">Sin guilds.</td></tr><?php endif; ?>
</table>
<?php at_pie(); ?>
