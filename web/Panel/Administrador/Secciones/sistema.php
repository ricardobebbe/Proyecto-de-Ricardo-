<?php
$logs = array(
    'Panel'   => AT_ROOT . '/Logs/Panel.log',
    'SQL'     => AT_ROOT . '/Logs/SQL.log',
    'Accesos' => AT_ROOT . '/Logs/Accesos.log',
);
$ver = isset($_GET['log']) && isset($logs[$_GET['log']]) ? $_GET['log'] : 'Panel';

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['accion']) && $_POST['accion'] === 'limpiar') {
    if (!at_token_ok()) { at_err('Token inválido.'); }
    else {
        $f = isset($logs[$_POST['log']]) ? $logs[$_POST['log']] : '';
        if ($f && file_exists($f) && @file_put_contents($f, '') !== false) { at_ok('Log vaciado.'); }
        else { at_err('No se pudo vaciar el log (revisá permisos de la carpeta Logs).'); }
    }
}

$version   = at_uno("SELECT @@VERSION");
$driver    = AT_DB::$instance ? AT_DB::$instance->driver : 'desconocido';
$tablas    = (int) at_uno("SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE='BASE TABLE'");
$contenido = '';
if (file_exists($logs[$ver])) {
    $todo  = file($logs[$ver], FILE_IGNORE_NEW_LINES);
    $todo  = array_slice($todo, -300);
    $contenido = implode("\n", array_reverse($todo));
}
$escribibles = array(
    'Logs/'                     => AT_ROOT . '/Logs',
    'Configuraciones/Sitio.php' => AT_ROOT . '/Configuraciones/Sitio.php',
    'Configuraciones/SQL.php'   => AT_ROOT . '/Configuraciones/SQL.php',
);

at_cabecera();
?>
<h1>Sistema</h1>
<div class="caja">
  <h2 style="margin-top:0">Entorno</h2>
  <table>
    <tr><th>PHP</th><td><?php echo h(PHP_VERSION); ?></td></tr>
    <tr><th>Driver SQL en uso</th><td><?php echo h($driver); ?></td></tr>
    <tr><th>Drivers disponibles</th><td><?php echo h(implode(', ', AT_DB::driversDisponibles())); ?></td></tr>
    <tr><th>SQL Server</th><td><?php echo h(trim(preg_replace('/\s+/', ' ', (string) $version))); ?></td></tr>
    <tr><th>Tablas en la base</th><td><?php echo $tablas; ?></td></tr>
    <tr><th>Instalador</th><td><?php echo is_dir(AT_ROOT . '/instalar')
        ? '<span class="etq ban">La carpeta /instalar todavía existe: borrala del servidor</span>'
        : '<span class="etq on">Eliminado</span>'; ?></td></tr>
  </table>
</div>

<div class="caja">
  <h2 style="margin-top:0">Permisos de escritura</h2>
  <table>
    <?php foreach ($escribibles as $etq => $ruta): ?>
    <tr><th><?php echo h($etq); ?></th><td>
      <?php echo is_writable($ruta) ? '<span class="etq on">Escribible</span>' : '<span class="etq ban">Sin permisos (chmod 755 / 644)</span>'; ?>
    </td></tr>
    <?php endforeach; ?>
  </table>
</div>

<div class="caja">
  <h2 style="margin-top:0">Registros</h2>
  <div class="fila" style="margin-bottom:10px">
    <?php foreach ($logs as $k => $f): ?>
      <a class="btn <?php echo $ver === $k ? '' : 'btn-fantasma'; ?>" href="index.php?s=sistema&log=<?php echo $k; ?>"><?php echo $k; ?></a>
    <?php endforeach; ?>
    <form method="post"><?php echo at_token_input(); ?>
      <input type="hidden" name="accion" value="limpiar">
      <input type="hidden" name="log" value="<?php echo h($ver); ?>">
      <button class="btn btn-peligro" type="submit">Vaciar este log</button>
    </form>
  </div>
  <pre><?php echo $contenido !== '' ? h($contenido) : 'Sin registros.'; ?></pre>
</div>
<?php at_pie(); ?>
