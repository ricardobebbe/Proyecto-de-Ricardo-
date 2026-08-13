<?php
$ruta = AT_ROOT . '/Configuraciones/Sitio.php';

$rutaSrv = AT_ROOT . '/Configuraciones/Servidor.php';

$campos = array(
    'Version'    => 'Versión del servidor',
    'Inicio'     => 'Fecha de apertura',
    'Admins'     => 'Administración / país',
    'Expydrop'   => 'Experiencia y drop',
    'Blessbug'   => 'Bless bug',
    'Hpmob'      => 'HP de los mobs',
    'Joyas'      => 'Joyas',
    'Mapas'      => 'Mapas habilitados',
    'Eventos'    => 'Eventos',
    'Ubicado'    => 'Ubicación del servidor',
    'Procesador' => 'Procesador',
    'Memoria'    => 'Memoria',
    'Conexion'   => 'Conexión',
    'Copyright'  => 'Copyright del pie',
    'Costoreset' => 'Costo del reset',
    'Reset'      => 'Level para resetear',
    'Maxear'     => 'Level máximo',
    'Costostats' => 'Costo de stats',
    'Costopk'    => 'Costo de limpiar PK',
);
$largos = array('Mapas', 'Eventos', 'Costopk', 'Joyas', 'Ubicado', 'Procesador');

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['accion']) && $_POST['accion'] === 'guardar') {
    if (!at_token_ok()) {
        at_err('Token inválido.');
    } elseif (!is_writable($ruta)) {
        at_err('No se puede escribir en <b>Configuraciones/Sitio.php</b>. Dale permisos 644 (o 664) al archivo.');
    } else {
        $out = "<?php\n// Generado desde el Panel de Administración - " . date('Y-m-d H:i:s') . "\n// Podés editarlo a mano respetando el formato.\n\n";
        foreach ($campos as $k => $etq) {
            $v = isset($_POST['c_' . $k]) ? trim($_POST['c_' . $k]) : '';
            $v = str_replace(array("\r", "\n"), ' ', $v);
            $out .= '$' . str_pad($k, 12) . '= "' . str_replace(array('\\', '"', '$'), array('\\\\', '\\"', '\\$'), $v) . "\";\n";
        }
        @copy($ruta, AT_ROOT . '/Configuraciones/Sitio.bak.php');
        if (file_put_contents($ruta, $out) !== false) {
            at_ok('Configuración del sitio guardada. Se dejó una copia en <b>Configuraciones/Sitio.bak.php</b>.');
            include $ruta;
        } else {
            at_err('No se pudo guardar el archivo.');
        }
    }
}

/* --- Datos privados del GameServer (archivo aparte, protegido) --- */
$Iphost = ''; $Portgs = '';
if (file_exists($rutaSrv)) { include $rutaSrv; }

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['accion']) && $_POST['accion'] === 'guardar_servidor') {
    if (!at_token_ok()) {
        at_err('Token inválido.');
    } else {
        $ip = preg_replace('/[^A-Za-z0-9\.\-]/', '', (string) ($_POST['c_Iphost'] ?? ''));
        $pt = preg_replace('/[^0-9]/', '', (string) ($_POST['c_Portgs'] ?? ''));
        $out = "<?php\n// Datos privados del GameServer - " . date('Y-m-d H:i:s') . "\n"
             . "// Protegido por Configuraciones/.htaccess. No se muestra en la web pública.\n\n"
             . '$Iphost = "' . $ip . "\";\n" . '$Portgs = "' . $pt . "\";\n";
        if (@file_put_contents($rutaSrv, $out) !== false) {
            $Iphost = $ip; $Portgs = $pt;
            at_ok('Datos del GameServer guardados en <b>Configuraciones/Servidor.php</b>.');
        } else {
            at_err('No se pudo escribir <b>Configuraciones/Servidor.php</b>. Dale permisos 644.');
        }
    }
}

at_cabecera();
?>
<h1>Configuración del sitio</h1>
<p class="suave">Estos datos son los que muestra la web pública (información del servidor, rates, eventos y pie de página).</p>
<form method="post">
  <?php echo at_token_input(); ?>
  <input type="hidden" name="accion" value="guardar">
  <div class="caja">
    <div class="campos">
      <?php foreach ($campos as $k => $etq): if (in_array($k, $largos, true)) continue; ?>
        <div><label><?php echo h($etq); ?></label>
          <input type="text" name="c_<?php echo $k; ?>" value="<?php echo h(isset($$k) ? $$k : ''); ?>"></div>
      <?php endforeach; ?>
    </div>
    <?php foreach ($largos as $k): ?>
      <div style="margin-bottom:12px"><label><?php echo h($campos[$k]); ?></label>
        <textarea name="c_<?php echo $k; ?>" rows="2"><?php echo h(isset($$k) ? $$k : ''); ?></textarea></div>
    <?php endforeach; ?>
    <button class="btn" type="submit">Guardar configuración</button>
  </div>
</form>
<h1>GameServer (privado)</h1>
<p class="suave">Estos datos se guardan en <b>Configuraciones/Servidor.php</b> y sólo se usan para chequear si el server está online. Nunca se muestran en las páginas públicas.</p>
<form method="post">
  <?php echo at_token_input(); ?>
  <input type="hidden" name="accion" value="guardar_servidor">
  <div class="caja">
    <div class="campos">
      <div><label>IP del GameServer</label><input type="text" name="c_Iphost" value="<?php echo h($Iphost); ?>"></div>
      <div><label>Puerto del GameServer</label><input type="text" name="c_Portgs" value="<?php echo h($Portgs); ?>"></div>
    </div>
    <button class="btn" type="submit">Guardar datos del GameServer</button>
  </div>
</form>
<?php at_pie(); ?>
