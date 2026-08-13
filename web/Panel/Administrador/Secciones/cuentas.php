<?php
$hayVip = at_hay_columna('MEMB_INFO', 'vip');
$hayGM  = at_hay_columna('MEMB_INFO', 'CuentaGM');
$hayBanco = at_hay_columna('MEMB_INFO', 'banco');

$cuenta = isset($_GET['c']) ? at_id($_GET['c']) : (isset($_POST['cuenta']) ? at_id($_POST['cuenta']) : '');
$buscar = isset($_GET['q']) ? trim($_GET['q']) : '';

// ------------------------------------------------------------------ Acciones
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['accion'])) {
    if (!at_token_ok()) {
        at_err('Token inválido, volvé a intentar.');
    } elseif ($cuenta === '') {
        at_err('Cuenta no válida.');
    } else {
        $c = at_esc($cuenta);
        switch ($_POST['accion']) {

            case 'bloquear':
                if (at_exec("UPDATE MEMB_INFO SET bloc_code=1 WHERE memb___id='$c'", "Bloqueó la cuenta $cuenta")) at_ok('Cuenta bloqueada.');
                break;

            case 'desbloquear':
                if (at_exec("UPDATE MEMB_INFO SET bloc_code=0 WHERE memb___id='$c'", "Desbloqueó la cuenta $cuenta")) at_ok('Cuenta desbloqueada.');
                break;

            case 'password':
                $nueva = isset($_POST['nueva']) ? trim($_POST['nueva']) : '';
                if (strlen($nueva) < 4 || strlen($nueva) > 10) {
                    at_err('La contraseña debe tener entre 4 y 10 caracteres.');
                } elseif (at_exec("UPDATE MEMB_INFO SET memb__pwd='" . at_esc($nueva) . "' WHERE memb___id='$c'", "Cambió la contraseña de $cuenta")) {
                    at_ok('Contraseña actualizada.');
                }
                break;

            case 'email':
                $mail = isset($_POST['email']) ? trim($_POST['email']) : '';
                if (!filter_var($mail, FILTER_VALIDATE_EMAIL)) {
                    at_err('Email no válido.');
                } elseif (at_exec("UPDATE MEMB_INFO SET mail_addr='" . at_esc($mail) . "' WHERE memb___id='$c'", "Cambió el email de $cuenta")) {
                    at_ok('Email actualizado.');
                }
                break;

            case 'vip':
                if (!$hayVip) { at_err('Esta base de datos no tiene columnas VIP.'); break; }
                $tipo = (int) $_POST['tipo'];
                $dias = max(0, (int) $_POST['dias']);
                if ($dias > 0) {
                    at_exec("UPDATE MEMB_INFO SET vip=$tipo, datavip=GETDATE(), diasdevip=$dias, vencimiento=DATEADD(day,$dias,GETDATE()) WHERE memb___id='$c'", "Dio VIP $tipo x $dias días a $cuenta");
                    at_ok("VIP tipo $tipo aplicado por $dias días.");
                } else {
                    at_exec("UPDATE MEMB_INFO SET vip=0, datavip=NULL, diasdevip=NULL, vencimiento=NULL WHERE memb___id='$c'", "Quitó el VIP a $cuenta");
                    at_ok('VIP eliminado.');
                }
                break;

            case 'gm':
                if (!$hayGM) { at_err('Esta base de datos no tiene la columna CuentaGM.'); break; }
                $n = (int) $_POST['nivel'];
                if ($n < 0 || $n > 3) { at_err('Nivel no válido.'); break; }
                if (strcasecmp($cuenta, $AT_CUENTA) === 0 && $n < 3) {
                    at_err('No podés quitarte a vos mismo el nivel de administrador.'); break;
                }
                if (at_exec("UPDATE MEMB_INFO SET CuentaGM=$n WHERE memb___id='$c'", "Puso CuentaGM=$n a $cuenta")) at_ok('Nivel de staff actualizado.');
                break;

            case 'banco':
                if (!$hayBanco) { at_err('Esta base de datos no tiene la columna banco.'); break; }
                $v = (int) $_POST['creditos'];
                if (at_exec("UPDATE MEMB_INFO SET banco = ISNULL(banco,0) + ($v) WHERE memb___id='$c'", "Ajustó créditos ($v) a $cuenta")) at_ok('Créditos actualizados.');
                break;

            case 'desconectar':
                at_exec("UPDATE MEMB_STAT SET ConnectStat=0 WHERE memb___id='$c'", "Desconectó a $cuenta");
                at_exec("UPDATE Character SET CtlCode=CtlCode WHERE AccountID='$c'");
                at_ok('Cuenta marcada como desconectada (arregla el "cuenta en uso").');
                break;
        }
    }
}

// ------------------------------------------------------------------ Datos
$lista = array();
if ($cuenta === '' && $buscar !== '') {
    $b = at_esc($buscar);
    $lista = at_filas("SELECT TOP 50 memb___id, mail_addr, bloc_code" . ($hayVip ? ", vip, vencimiento" : "") . "
                       FROM MEMB_INFO
                       WHERE memb___id LIKE '%$b%' OR mail_addr LIKE '%$b%'
                          OR memb___id IN (SELECT AccountID FROM Character WHERE Name LIKE '%$b%')
                       ORDER BY memb___id");
}

$info = null;
if ($cuenta !== '') {
    $filas = at_filas("SELECT TOP 1 * FROM MEMB_INFO WHERE memb___id='" . at_esc($cuenta) . "'");
    $info = $filas ? $filas[0] : null;
    if (!$info) { at_err('No existe la cuenta ' . h($cuenta) . '.'); }
}
$personajes = array();
$estado = null;
if ($info) {
    $personajes = at_filas("SELECT Name, cLevel, Class" . (at_hay_columna('Character','Resets') ? ", Resets" : ", 0 AS Resets") . "
                            FROM Character WHERE AccountID='" . at_esc($cuenta) . "'");
    $e = at_filas("SELECT TOP 1 ConnectStat, ServerName, IP FROM MEMB_STAT WHERE memb___id='" . at_esc($cuenta) . "'");
    $estado = $e ? $e[0] : null;
}

at_cabecera();
?>
<h1>Cuentas</h1>

<div class="caja">
  <form method="get" class="fila">
    <input type="hidden" name="s" value="cuentas">
    <div style="flex:1;min-width:220px">
      <label>Buscar por cuenta, email o nombre de personaje</label>
      <input type="text" name="q" value="<?php echo h($buscar); ?>" placeholder="ej: juan / juan@mail.com / MiBK">
    </div>
    <button class="btn" type="submit">Buscar</button>
  </form>
</div>

<?php if ($lista): ?>
<table>
  <tr><th>Cuenta</th><th>Email</th><th>Estado</th><th></th></tr>
  <?php foreach ($lista as $r): ?>
  <tr>
    <td><b><?php echo h($r['memb___id']); ?></b></td>
    <td class="suave"><?php echo h($r['mail_addr']); ?></td>
    <td>
      <?php if ((int) $r['bloc_code'] === 1): ?><span class="etq ban">Bloqueada</span><?php else: ?><span class="etq on">Activa</span><?php endif; ?>
      <?php if ($hayVip && (int) $r['vip'] > 0): ?><span class="etq vip">VIP <?php echo (int) $r['vip']; ?></span><?php endif; ?>
    </td>
    <td><a class="btn btn-fantasma" href="index.php?s=cuentas&c=<?php echo urlencode($r['memb___id']); ?>">Administrar</a></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php elseif ($cuenta === '' && $buscar !== ''): ?>
  <p class="suave">Sin resultados para "<?php echo h($buscar); ?>".</p>
<?php endif; ?>

<?php if ($info): $t = at_token_input(); ?>
<h2>Cuenta: <?php echo h($info['memb___id']); ?></h2>
<div class="caja">
  <div class="tarjetas">
    <div class="tarjeta"><div class="t">Email</div><div><?php echo h($info['mail_addr']); ?></div></div>
    <div class="tarjeta"><div class="t">Registrada</div><div><?php echo h($info['appl_days']); ?></div></div>
    <div class="tarjeta"><div class="t">Estado</div><div>
      <?php echo ((int) $info['bloc_code'] === 1) ? '<span class="etq ban">Bloqueada</span>' : '<span class="etq on">Activa</span>'; ?>
      <?php echo ($estado && (int) $estado['ConnectStat'] === 1) ? '<span class="etq on">En línea</span>' : '<span class="etq off">Desconectada</span>'; ?>
    </div></div>
    <?php if ($hayVip): ?>
    <div class="tarjeta"><div class="t">VIP</div><div>
      <?php echo (int) $info['vip'] > 0 ? '<span class="etq vip">Tipo ' . (int) $info['vip'] . '</span> ' . h($info['vencimiento']) : '<span class="etq off">Sin VIP</span>'; ?>
    </div></div>
    <?php endif; ?>
    <?php if ($hayGM): ?>
    <div class="tarjeta"><div class="t">Nivel staff</div><div class="n"><?php echo (int) $info['CuentaGM']; ?></div></div>
    <?php endif; ?>
    <?php if ($hayBanco): ?>
    <div class="tarjeta"><div class="t">Créditos</div><div class="n"><?php echo (int) $info['banco']; ?></div></div>
    <?php endif; ?>
  </div>
</div>

<div class="caja">
  <h2 style="margin-top:0">Acciones rápidas</h2>
  <div class="fila">
    <form method="post"><?php echo $t; ?><input type="hidden" name="cuenta" value="<?php echo h($cuenta); ?>">
      <input type="hidden" name="accion" value="<?php echo ((int) $info['bloc_code'] === 1) ? 'desbloquear' : 'bloquear'; ?>">
      <button class="btn <?php echo ((int) $info['bloc_code'] === 1) ? 'btn-ok' : 'btn-peligro'; ?>" type="submit">
        <?php echo ((int) $info['bloc_code'] === 1) ? 'Desbloquear cuenta' : 'Bloquear cuenta'; ?>
      </button>
    </form>
    <form method="post"><?php echo $t; ?><input type="hidden" name="cuenta" value="<?php echo h($cuenta); ?>">
      <input type="hidden" name="accion" value="desconectar">
      <button class="btn btn-fantasma" type="submit">Forzar desconexión</button>
    </form>
  </div>
</div>

<div class="caja">
  <h2 style="margin-top:0">Datos de acceso</h2>
  <form method="post" class="fila"><?php echo $t; ?>
    <input type="hidden" name="cuenta" value="<?php echo h($cuenta); ?>">
    <input type="hidden" name="accion" value="password">
    <div style="flex:1;min-width:200px"><label>Nueva contraseña (4-10)</label><input type="text" name="nueva" maxlength="10"></div>
    <button class="btn" type="submit">Cambiar contraseña</button>
  </form>
  <form method="post" class="fila" style="margin-top:12px"><?php echo $t; ?>
    <input type="hidden" name="cuenta" value="<?php echo h($cuenta); ?>">
    <input type="hidden" name="accion" value="email">
    <div style="flex:1;min-width:200px"><label>Email</label><input type="email" name="email" value="<?php echo h($info['mail_addr']); ?>"></div>
    <button class="btn" type="submit">Guardar email</button>
  </form>
</div>

<?php if ($hayVip): ?>
<div class="caja">
  <h2 style="margin-top:0">VIP</h2>
  <form method="post" class="fila"><?php echo $t; ?>
    <input type="hidden" name="cuenta" value="<?php echo h($cuenta); ?>">
    <input type="hidden" name="accion" value="vip">
    <div><label>Tipo</label>
      <select name="tipo"><option value="1">1 - VIP</option><option value="2">2 - VIP Plus</option><option value="3">3 - VIP Gold</option></select>
    </div>
    <div><label>Días (0 = quitar)</label><input type="number" name="dias" value="30" min="0" max="3650"></div>
    <button class="btn" type="submit">Aplicar</button>
  </form>
</div>
<?php endif; ?>

<?php if ($hayBanco): ?>
<div class="caja">
  <h2 style="margin-top:0">Créditos / banco</h2>
  <form method="post" class="fila"><?php echo $t; ?>
    <input type="hidden" name="cuenta" value="<?php echo h($cuenta); ?>">
    <input type="hidden" name="accion" value="banco">
    <div><label>Sumar o restar (podés usar negativos)</label><input type="number" name="creditos" value="100"></div>
    <button class="btn" type="submit">Aplicar</button>
  </form>
</div>
<?php endif; ?>

<?php if ($hayGM): ?>
<div class="caja">
  <h2 style="margin-top:0">Nivel de staff</h2>
  <form method="post" class="fila"><?php echo $t; ?>
    <input type="hidden" name="cuenta" value="<?php echo h($cuenta); ?>">
    <input type="hidden" name="accion" value="gm">
    <div><label>Nivel</label>
      <select name="nivel">
        <option value="0">0 - Jugador</option>
        <option value="1">1 - Colaborador</option>
        <option value="2">2 - Game Master</option>
        <option value="3">3 - Administrador</option>
      </select>
    </div>
    <button class="btn" type="submit">Guardar</button>
  </form>
</div>
<?php endif; ?>

<h2>Personajes de la cuenta</h2>
<table>
  <tr><th>Nombre</th><th>Clase</th><th>Level</th><th>Resets</th><th></th></tr>
  <?php foreach ($personajes as $p): ?>
  <tr>
    <td><b><?php echo h($p['Name']); ?></b></td>
    <td class="suave"><?php echo h(at_clase($p['Class'])); ?></td>
    <td><?php echo (int) $p['cLevel']; ?></td>
    <td><?php echo (int) $p['Resets']; ?></td>
    <td><a class="btn btn-fantasma" href="index.php?s=personajes&p=<?php echo urlencode($p['Name']); ?>">Editar</a></td>
  </tr>
  <?php endforeach; ?>
  <?php if (!$personajes): ?><tr><td colspan="5" class="suave">Esta cuenta no tiene personajes.</td></tr><?php endif; ?>
</table>
<?php endif; ?>
<?php at_pie(); ?>
