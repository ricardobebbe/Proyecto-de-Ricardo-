<?php
if (!at_hay_columna('MEMB_INFO', 'CuentaGM')) {
    at_cabecera();
    echo '<h1>Staff / GMs</h1><div class="aviso err">La tabla MEMB_INFO no tiene la columna <b>CuentaGM</b>. '
       . 'Ejecutá <b>sql/01_estructura_web.sql</b> para agregarla.</div>';
    at_pie();
    return;
}
$filas = at_filas("SELECT memb___id, mail_addr, CuentaGM FROM MEMB_INFO WHERE ISNULL(CuentaGM,0) > 0 ORDER BY CuentaGM DESC, memb___id");
$niveles = array(1 => 'Colaborador', 2 => 'Game Master', 3 => 'Administrador');
at_cabecera();
?>
<h1>Staff / GMs</h1>
<p class="suave">Para agregar o quitar staff, buscá la cuenta en <a href="index.php?s=cuentas">Cuentas</a> y cambiá su nivel.</p>
<table>
  <tr><th>Cuenta</th><th>Email</th><th>Nivel</th><th></th></tr>
  <?php foreach ($filas as $r): $n = (int) $r['CuentaGM']; ?>
  <tr>
    <td><b><?php echo h($r['memb___id']); ?></b></td>
    <td class="suave"><?php echo h($r['mail_addr']); ?></td>
    <td><span class="etq vip"><?php echo $n; ?> - <?php echo h(isset($niveles[$n]) ? $niveles[$n] : 'Otro'); ?></span></td>
    <td><a class="btn btn-fantasma" href="index.php?s=cuentas&c=<?php echo urlencode($r['memb___id']); ?>">Administrar</a></td>
  </tr>
  <?php endforeach; ?>
  <?php if (!$filas): ?><tr><td colspan="4" class="suave">Todavía no hay cuentas con nivel de staff.</td></tr><?php endif; ?>
</table>
<?php at_pie(); ?>
