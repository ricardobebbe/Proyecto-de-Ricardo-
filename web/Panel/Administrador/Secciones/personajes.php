<?php
$hayResets = at_hay_columna('Character', 'Resets');
$nombre = isset($_GET['p']) ? at_id($_GET['p']) : (isset($_POST['personaje']) ? at_id($_POST['personaje']) : '');
$buscar = isset($_GET['q']) ? trim($_GET['q']) : '';

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['accion'])) {
    if (!at_token_ok()) { at_err('Token inválido.'); }
    elseif ($nombre === '') { at_err('Personaje no válido.'); }
    else {
        $n = at_esc($nombre);
        switch ($_POST['accion']) {
            case 'guardar':
                $sets = array(
                    'cLevel='        . max(1, min(400, (int) $_POST['level'])),
                    'LevelUpPoint='  . max(0, (int) $_POST['puntos']),
                    'Money='         . max(0, (int) $_POST['zen']),
                    'Strength='      . max(0, (int) $_POST['fuerza']),
                    'Dexterity='     . max(0, (int) $_POST['agilidad']),
                    'Vitality='      . max(0, (int) $_POST['vitalidad']),
                    'Energy='        . max(0, (int) $_POST['energia']),
                );
                if ($hayResets) { $sets[] = 'Resets=' . max(0, (int) $_POST['resets']); }
                if (at_exec("UPDATE Character SET " . implode(',', $sets) . " WHERE Name='$n'", "Editó el personaje $nombre")) {
                    at_ok('Personaje actualizado.');
                }
                break;

            case 'pk':
                if (at_exec("UPDATE Character SET PkCount=0, PkLevel=3, PkTime=0 WHERE Name='$n'", "Limpió el PK de $nombre")) at_ok('PK limpiado.');
                break;

            case 'mover':
                if (at_exec("UPDATE Character SET MapNumber=0, MapPosX=125, MapPosY=125 WHERE Name='$n'", "Movió a Lorencia a $nombre")) at_ok('Personaje movido a Lorencia.');
                break;
        }
    }
}

$lista = array();
if ($nombre === '' && $buscar !== '') {
    $b = at_esc($buscar);
    $lista = at_filas("SELECT TOP 50 Name, AccountID, cLevel, Class" . ($hayResets ? ", Resets" : ", 0 AS Resets") . "
                       FROM Character WHERE Name LIKE '%$b%' OR AccountID LIKE '%$b%' ORDER BY Name");
}

$per = null;
if ($nombre !== '') {
    $f = at_filas("SELECT TOP 1 * FROM Character WHERE Name='" . at_esc($nombre) . "'");
    $per = $f ? $f[0] : null;
    if (!$per) at_err('No existe el personaje ' . h($nombre) . '.');
}

at_cabecera();
?>
<h1>Personajes</h1>
<div class="caja">
  <form method="get" class="fila">
    <input type="hidden" name="s" value="personajes">
    <div style="flex:1;min-width:220px"><label>Buscar por nombre de personaje o cuenta</label>
      <input type="text" name="q" value="<?php echo h($buscar); ?>"></div>
    <button class="btn" type="submit">Buscar</button>
  </form>
</div>

<?php if ($lista): ?>
<table>
  <tr><th>Personaje</th><th>Cuenta</th><th>Clase</th><th>Level</th><th>Resets</th><th></th></tr>
  <?php foreach ($lista as $r): ?>
  <tr>
    <td><b><?php echo h($r['Name']); ?></b></td>
    <td class="suave"><?php echo h($r['AccountID']); ?></td>
    <td class="suave"><?php echo h(at_clase($r['Class'])); ?></td>
    <td><?php echo (int) $r['cLevel']; ?></td>
    <td><?php echo (int) $r['Resets']; ?></td>
    <td><a class="btn btn-fantasma" href="index.php?s=personajes&p=<?php echo urlencode($r['Name']); ?>">Editar</a></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php endif; ?>

<?php if ($per): $t = at_token_input(); ?>
<h2><?php echo h($per['Name']); ?> — <?php echo h(at_clase($per['Class'])); ?>
  <span class="suave">(cuenta <a href="index.php?s=cuentas&c=<?php echo urlencode($per['AccountID']); ?>"><?php echo h($per['AccountID']); ?></a>)</span>
</h2>
<div class="caja">
  <form method="post"><?php echo $t; ?>
    <input type="hidden" name="personaje" value="<?php echo h($nombre); ?>">
    <input type="hidden" name="accion" value="guardar">
    <div class="campos">
      <div><label>Level</label><input type="number" name="level" value="<?php echo (int) $per['cLevel']; ?>"></div>
      <?php if ($hayResets): ?><div><label>Resets</label><input type="number" name="resets" value="<?php echo (int) $per['Resets']; ?>"></div><?php endif; ?>
      <div><label>Puntos libres</label><input type="number" name="puntos" value="<?php echo (int) $per['LevelUpPoint']; ?>"></div>
      <div><label>Zen</label><input type="number" name="zen" value="<?php echo (int) $per['Money']; ?>"></div>
      <div><label>Fuerza</label><input type="number" name="fuerza" value="<?php echo (int) $per['Strength']; ?>"></div>
      <div><label>Agilidad</label><input type="number" name="agilidad" value="<?php echo (int) $per['Dexterity']; ?>"></div>
      <div><label>Vitalidad</label><input type="number" name="vitalidad" value="<?php echo (int) $per['Vitality']; ?>"></div>
      <div><label>Energía</label><input type="number" name="energia" value="<?php echo (int) $per['Energy']; ?>"></div>
    </div>
    <button class="btn" type="submit">Guardar cambios</button>
  </form>
</div>
<div class="caja">
  <h2 style="margin-top:0">Herramientas</h2>
  <div class="fila">
    <form method="post"><?php echo $t; ?><input type="hidden" name="personaje" value="<?php echo h($nombre); ?>">
      <input type="hidden" name="accion" value="pk"><button class="btn btn-fantasma" type="submit">Limpiar PK</button></form>
    <form method="post"><?php echo $t; ?><input type="hidden" name="personaje" value="<?php echo h($nombre); ?>">
      <input type="hidden" name="accion" value="mover"><button class="btn btn-fantasma" type="submit">Mover a Lorencia</button></form>
  </div>
  <p class="suave" style="margin-bottom:0">Pedile al jugador que esté desconectado antes de editar, o el GameServer puede pisar los cambios al salir.</p>
</div>
<?php endif; ?>
<?php at_pie(); ?>
