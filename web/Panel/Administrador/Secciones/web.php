<?php
/**
 * Panel de Administración - Configuración de la web (tabla WEB_Config).
 * Desde acá el admin controla registro, resets, puntos, banco y avisos
 * sin tocar ni un archivo del servidor.
 */
if (!at_tabla('WEB_Config')) {
    at_err('Falta la tabla <b>WEB_Config</b>. Ejecutá <b>sql/03_web_actualizacion.sql</b> en la base MuOnline.');
}

$grupos = array(
    'Sitio' => array(
        'sitio_nombre'        => array('texto',  'Nombre del servidor'),
        'sitio_guild'         => array('texto',  'Guild oficial'),
        'sitio_discord'       => array('texto',  'Link de Discord'),
        'sitio_mensaje'       => array('area',   'Aviso global (se muestra arriba en el sitio)'),
        'sitio_mantenimiento' => array('bool',   'Modo mantenimiento (sólo staff puede entrar)'),
    ),
    'Registro de cuentas' => array(
        'registro_abierto'    => array('bool',   'Registro abierto'),
        'registro_vip_tipo'   => array('numero', 'Tipo de VIP al registrarse (0 = sin VIP)'),
        'registro_vip_dias'   => array('numero', 'Días de VIP de regalo'),
        'registro_max_por_ip' => array('numero', 'Máximo de cuentas por IP y por día (0 = sin límite)'),
    ),
    'Resets' => array(
        'reset_abierto'        => array('bool',   'Resets habilitados en la web'),
        'reset_level'          => array('numero', 'Nivel necesario para resetear'),
        'reset_costo'          => array('numero', 'Costo del reset en zen'),
        'reset_costo_vip'      => array('numero', 'Costo del reset para cuentas VIP'),
        'reset_puntos'         => array('numero', 'Puntos que se entregan por reset'),
        'reset_max'            => array('numero', 'Máximo de resets (0 = sin límite)'),
        'reset_conserva_stats' => array('bool',   'El reset conserva los stats'),
        'reset_fuente'         => array('fuente', 'De dónde se cobra el reset'),
        'reset_offline'        => array('bool',   'Exigir estar desconectado del juego'),
    ),
    'Puntos / estadísticas' => array(
        'stats_abierto'          => array('bool',   'Compra de puntos habilitada'),
        'stats_costo_punto'      => array('numero', 'Zen por cada punto extra'),
        'stats_max_por_compra'   => array('numero', 'Máximo de puntos por operación'),
        'stats_max_por_atributo' => array('numero', 'Tope por atributo (fuerza, agilidad, etc.)'),
        'stats_fuente'           => array('fuente', 'De dónde se cobran los puntos'),
    ),
    'Banco de zen' => array(
        'banco_abierto'       => array('bool',   'Banco habilitado'),
        'banco_max_personaje' => array('numero', 'Zen máximo que puede llevar un personaje'),
        'pk_costo'            => array('numero', 'Costo de limpiar PK'),
    ),
);

if ($_SERVER['REQUEST_METHOD'] === 'POST' && ($_POST['accion'] ?? '') === 'guardar_web') {
    if (!at_token_ok()) {
        at_err('Token inválido.');
    } else {
        $n = 0;
        foreach ($grupos as $campos) {
            foreach ($campos as $clave => $def) {
                if ($def[0] === 'bool') {
                    $valor = empty($_POST['c_' . $clave]) ? '0' : '1';
                } elseif ($def[0] === 'numero') {
                    $valor = (string) (int) preg_replace('/[^0-9\-]/', '', (string) ($_POST['c_' . $clave] ?? '0'));
                } elseif ($def[0] === 'fuente') {
                    $valor = (($_POST['c_' . $clave] ?? 'banco') === 'inventario') ? 'inventario' : 'banco';
                } else {
                    $valor = trim((string) ($_POST['c_' . $clave] ?? ''));
                }
                if (at_cfg_set($clave, $valor)) { $n++; }
            }
        }
        at_ok('Configuración guardada (' . $n . ' valores). Los cambios ya están activos en la web.');
        criar_log_painel('Actualizó la configuración de la web', 'Administrador');
    }
}

/* Releemos con los valores nuevos */
$actual = array();
$r = mssql_query('SELECT Clave, Valor FROM WEB_Config');
if ($r) { while ($f = mssql_fetch_assoc($r)) { $actual[$f['Clave']] = (string) $f['Valor']; } }
function at_v($clave) { global $actual; return isset($actual[$clave]) ? $actual[$clave] : at_cfg($clave); }

at_cabecera();
?>
<h1>Configuración de la web</h1>
<p class="suave">Todo lo que se cambia acá impacta al instante en el registro, los resets, el reparto de puntos y el banco de zen.</p>

<form method="post">
  <?php echo at_token_input(); ?>
  <input type="hidden" name="accion" value="guardar_web">
  <?php foreach ($grupos as $titulo => $campos): ?>
    <div class="caja">
      <h2><?php echo h($titulo); ?></h2>
      <div class="campos">
        <?php foreach ($campos as $clave => $def):
            list($tipo, $etq) = $def; $v = at_v($clave); ?>
          <div>
            <label><?php echo h($etq); ?></label>
            <?php if ($tipo === 'bool'): ?>
              <select name="c_<?php echo $clave; ?>">
                <option value="1" <?php echo $v === '1' ? 'selected' : ''; ?>>Sí</option>
                <option value="0" <?php echo $v !== '1' ? 'selected' : ''; ?>>No</option>
              </select>
            <?php elseif ($tipo === 'fuente'): ?>
              <select name="c_<?php echo $clave; ?>">
                <option value="banco" <?php echo $v !== 'inventario' ? 'selected' : ''; ?>>WebBank de la cuenta</option>
                <option value="inventario" <?php echo $v === 'inventario' ? 'selected' : ''; ?>>Zen del personaje</option>
              </select>
            <?php elseif ($tipo === 'area'): ?>
              <textarea name="c_<?php echo $clave; ?>" rows="2"><?php echo h($v); ?></textarea>
            <?php else: ?>
              <input type="text" name="c_<?php echo $clave; ?>" value="<?php echo h($v); ?>">
            <?php endif; ?>
          </div>
        <?php endforeach; ?>
      </div>
    </div>
  <?php endforeach; ?>
  <button class="btn" type="submit">Guardar configuración</button>
</form>

<h1>Últimos movimientos de jugadores</h1>
<?php if (!at_tabla('WEB_Movimientos')): ?>
  <p class="suave">Ejecutá <b>sql/03_web_actualizacion.sql</b> para activar la auditoría de resets, puntos y banco.</p>
<?php else: ?>
  <table class="tabla">
    <tr><th>Fecha</th><th>Cuenta</th><th>Personaje</th><th>Acción</th><th>Detalle</th><th>Zen</th><th>IP</th></tr>
    <?php foreach (at_filas('SELECT TOP 40 * FROM WEB_Movimientos ORDER BY Fecha DESC') as $m): ?>
      <tr>
        <td><?php echo h(at_fecha($m['Fecha'])); ?></td>
        <td><?php echo h($m['Cuenta']); ?></td>
        <td><?php echo h($m['Personaje']); ?></td>
        <td><?php echo h($m['Tipo']); ?></td>
        <td><?php echo h($m['Detalle']); ?></td>
        <td><?php echo number_format((float) $m['Zen'], 0, ',', '.'); ?></td>
        <td><?php echo h($m['IP']); ?></td>
      </tr>
    <?php endforeach; ?>
  </table>
<?php endif; ?>
<?php at_pie(); ?>
