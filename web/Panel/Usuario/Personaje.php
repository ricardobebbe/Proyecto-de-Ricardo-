<?php
/**
 * Ficha de personaje: estadísticas, reparto de puntos, reset y banco de zen.
 * Reescrito 2026 (MuMisiones) - todas las reglas se configuran desde el panel.
 */
if (session_status() === PHP_SESSION_NONE) { @session_start(); }
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';

if (!isset($_SESSION['login'])) {
    at_head('Personaje');
    at_msg('Para entrar a esta página necesitás iniciar sesión.', 'error');
    at_foot();
    return;
}

$cuenta = at_id($_SESSION['login']);
$name   = at_id($_GET['name'] ?? ($_POST['name'] ?? ''));
at_csrf();

if ($name === '') {
    at_head('Personaje');
    at_msg('No indicaste ningún personaje.', 'error');
    at_foot();
    return;
}

/* El personaje tiene que pertenecer a la cuenta logueada */
$pertenece = mssql_fetch_row(mssql_query(
    "SELECT Name FROM Character WHERE Name='" . at_esc($name) . "' AND AccountID='" . at_esc($cuenta) . "'"));
if (empty($pertenece[0])) {
    at_head('Personaje');
    at_msg('Ese personaje no pertenece a tu cuenta.', 'error');
    at_foot();
    return;
}

/* ------------------------------------------------------------------ */
/* Configuración                                                       */
/* ------------------------------------------------------------------ */
$hayResets  = at_col('Character', 'Resets');
$hayBanco   = at_col('MEMB_INFO', 'banco');
$esVip      = at_vip($cuenta);
$online     = at_online($cuenta);

$RS_ABIERTO = at_cfgb('reset_abierto', true);
$RS_LEVEL   = at_cfgi('reset_level', 350);
$RS_COSTO   = $esVip ? at_cfgi('reset_costo_vip', 0) : at_cfgi('reset_costo', 0);
$RS_PUNTOS  = at_cfgi('reset_puntos', 500);
$RS_MAX     = at_cfgi('reset_max', 0);
$RS_CONSERVA= at_cfgb('reset_conserva_stats', false);
$RS_FUENTE  = at_cfg('reset_fuente', 'banco') === 'inventario' ? 'inventario' : 'banco';
$RS_OFFLINE = at_cfgb('reset_offline', true);

$ST_ABIERTO = at_cfgb('stats_abierto', true);
$ST_PRECIO  = at_cfgi('stats_costo_punto', 0);
$ST_MAXOP   = max(1, at_cfgi('stats_max_por_compra', 1000));
$ST_FUENTE  = at_cfg('stats_fuente', 'banco') === 'inventario' ? 'inventario' : 'banco';
$ST_TOPE    = at_cfgi('stats_max_por_atributo', 32767);

$BK_ABIERTO = at_cfgb('banco_abierto', true) && $hayBanco;
$BK_TOPE    = at_cfgi('banco_max_personaje', 2000000000);

$mensajes = array();

/** Lee el personaje fresco de la base */
function at_pj($cuenta, $name)
{
    $extra = at_col('Character', 'Resets') ? ', ISNULL(Resets,0) AS Resets' : ', 0 AS Resets';
    $lead  = at_col('Character', 'Leadership') ? ', ISNULL(Leadership,0) AS Leadership' : ', 0 AS Leadership';
    return mssql_fetch_assoc(mssql_query(
        "SELECT Name, AccountID, cLevel, Class, Strength, Dexterity, Vitality, Energy,
                Money, Life, MaxLife, Mana, MapNumber, PkCount, PkLevel, CtlCode,
                ISNULL(LevelUpPoint,0) AS LevelUpPoint, Experience{$extra}{$lead}
         FROM Character WHERE Name='" . at_esc($name) . "' AND AccountID='" . at_esc($cuenta) . "'"));
}

/** Descuenta zen del banco o del inventario del personaje */
function at_cobrar($cuenta, $name, $monto, $fuente)
{
    if ($monto <= 0) { return true; }
    if ($fuente === 'inventario') {
        $r = mssql_fetch_row(mssql_query("SELECT Money FROM Character WHERE Name='" . at_esc($name) . "' AND AccountID='" . at_esc($cuenta) . "'"));
        if (!$r || (int) $r[0] < $monto) { return false; }
        return (bool) mssql_query("UPDATE Character SET Money = Money - {$monto} WHERE Name='" . at_esc($name) . "' AND AccountID='" . at_esc($cuenta) . "'");
    }
    if (at_banco($cuenta) < $monto) { return false; }
    return (bool) mssql_query("UPDATE MEMB_INFO SET banco = ISNULL(banco,0) - {$monto} WHERE memb___id='" . at_esc($cuenta) . "'");
}

$p = at_pj($cuenta, $name);

/* ------------------------------------------------------------------ */
/* Acciones                                                            */
/* ------------------------------------------------------------------ */
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['accion'])) {

    if (!at_csrf_ok()) {
        $mensajes[] = array('error', 'Sesión vencida, intentá de nuevo.');
    } elseif ($online) {
        $mensajes[] = array('error', 'Tenés que estar <b>desconectado del juego</b> para hacer cambios desde la web.');
    } else {
        $accion = (string) $_POST['accion'];

        /* ---------------- Banco de zen ---------------- */
        if ($accion === 'depositar' || $accion === 'retirar') {
            $monto = (int) preg_replace('/[^0-9]/', '', (string) ($_POST['monto'] ?? '0'));
            if (!$BK_ABIERTO) {
                $mensajes[] = array('error', 'El banco está deshabilitado por el administrador.');
            } elseif ($monto <= 0) {
                $mensajes[] = array('error', 'Ingresá una cantidad de zen válida.');
            } elseif ($accion === 'depositar') {
                if ($monto > (int) $p['Money']) {
                    $mensajes[] = array('error', 'Tu personaje no tiene esa cantidad de zen.');
                } else {
                    mssql_query("UPDATE Character SET Money = Money - {$monto} WHERE AccountID='" . at_esc($cuenta) . "' AND Name='" . at_esc($name) . "'");
                    mssql_query("UPDATE MEMB_INFO SET banco = ISNULL(banco,0) + {$monto} WHERE memb___id='" . at_esc($cuenta) . "'");
                    at_mov($cuenta, 'deposito', 'Depósito al banco', $monto, $name);
                    $mensajes[] = array('ok', 'Depositaste <b>' . at_num($monto) . '</b> zen en tu banco.');
                }
            } else {
                if ($monto > at_banco($cuenta)) {
                    $mensajes[] = array('error', 'No tenés esa cantidad de zen en el banco.');
                } elseif (((int) $p['Money'] + $monto) > $BK_TOPE) {
                    $mensajes[] = array('error', 'Tu personaje no puede tener más de ' . at_num($BK_TOPE) . ' de zen encima.');
                } else {
                    mssql_query("UPDATE MEMB_INFO SET banco = ISNULL(banco,0) - {$monto} WHERE memb___id='" . at_esc($cuenta) . "'");
                    mssql_query("UPDATE Character SET Money = Money + {$monto} WHERE AccountID='" . at_esc($cuenta) . "' AND Name='" . at_esc($name) . "'");
                    at_mov($cuenta, 'retiro', 'Retiro del banco', $monto, $name);
                    $mensajes[] = array('ok', 'Retiraste <b>' . at_num($monto) . '</b> zen del banco.');
                }
            }
        }

        /* ---------------- Reparto de puntos ---------------- */
        elseif ($accion === 'repartir') {
            $add = array(
                'Strength'  => (int) preg_replace('/[^0-9]/', '', (string) ($_POST['add_str'] ?? '0')),
                'Dexterity' => (int) preg_replace('/[^0-9]/', '', (string) ($_POST['add_agi'] ?? '0')),
                'Vitality'  => (int) preg_replace('/[^0-9]/', '', (string) ($_POST['add_vit'] ?? '0')),
                'Energy'    => (int) preg_replace('/[^0-9]/', '', (string) ($_POST['add_ene'] ?? '0')),
            );
            if ((int) $p['Class'] >= 64 && at_col('Character', 'Leadership')) {
                $add['Leadership'] = (int) preg_replace('/[^0-9]/', '', (string) ($_POST['add_com'] ?? '0'));
            }
            $total  = array_sum($add);
            $libres = (int) $p['LevelUpPoint'];
            $comprar = max(0, $total - $libres);
            $costo   = $comprar * $ST_PRECIO;
            $disponible = $ST_FUENTE === 'inventario' ? (int) $p['Money'] : at_banco($cuenta);

            $tope = false;
            foreach ($add as $col => $v) {
                if ($v > 0 && ((int) $p[$col] + $v) > $ST_TOPE) { $tope = true; }
            }

            if ($total <= 0) {
                $mensajes[] = array('error', 'No indicaste ningún punto para repartir.');
            } elseif ($total > $ST_MAXOP) {
                $mensajes[] = array('error', 'Podés repartir como máximo ' . at_num($ST_MAXOP) . ' puntos por operación.');
            } elseif ($tope) {
                $mensajes[] = array('error', 'Ningún atributo puede pasar de ' . at_num($ST_TOPE) . ' puntos.');
            } elseif ($comprar > 0 && !$ST_ABIERTO) {
                $mensajes[] = array('error', 'Sólo podés repartir tus puntos libres: la compra de puntos está deshabilitada.');
            } elseif ($comprar > 0 && $costo > $disponible) {
                $mensajes[] = array('error', 'Te faltan zen: la operación cuesta <b>' . at_num($costo) . '</b> y tenés ' . at_num($disponible) . ' en tu ' . $ST_FUENTE . '.');
            } elseif ($comprar > 0 && !at_cobrar($cuenta, $name, $costo, $ST_FUENTE)) {
                $mensajes[] = array('error', 'No se pudo descontar el zen. Intentá de nuevo.');
            } else {
                $sets = array();
                foreach ($add as $col => $v) {
                    if ($v > 0) { $sets[] = "{$col} = {$col} + {$v}"; }
                }
                $usados = min($libres, $total);
                $sets[] = 'LevelUpPoint = ISNULL(LevelUpPoint,0) - ' . $usados;
                mssql_query('UPDATE Character SET ' . implode(', ', $sets)
                    . " WHERE Name='" . at_esc($name) . "' AND AccountID='" . at_esc($cuenta) . "'");
                at_mov($cuenta, 'stats', 'Repartió ' . $total . ' puntos (' . $comprar . ' comprados)', $costo, $name);
                $mensajes[] = array('ok', 'Repartiste <b>' . at_num($total) . '</b> puntos en <b>' . at_h($name) . '</b>'
                    . ($costo > 0 ? ' por ' . at_num($costo) . ' zen.' : '.'));
            }
        }

        /* ---------------- Reset ---------------- */
        elseif ($accion === 'resetear') {
            $resets = (int) $p['Resets'];
            $disponible = $RS_FUENTE === 'inventario' ? (int) $p['Money'] : at_banco($cuenta);

            if (!$RS_ABIERTO || !$hayResets) {
                $mensajes[] = array('error', 'Los resets desde la web están deshabilitados.');
            } elseif ((int) $p['cLevel'] < $RS_LEVEL) {
                $mensajes[] = array('error', 'Necesitás nivel <b>' . at_num($RS_LEVEL) . '</b> para resetear. Te faltan ' . at_num($RS_LEVEL - (int) $p['cLevel']) . ' niveles.');
            } elseif ($RS_MAX > 0 && $resets >= $RS_MAX) {
                $mensajes[] = array('error', 'Ya llegaste al máximo de ' . at_num($RS_MAX) . ' resets.');
            } elseif ($RS_COSTO > 0 && $disponible < $RS_COSTO) {
                $mensajes[] = array('error', 'Te faltan zen: el reset cuesta <b>' . at_num($RS_COSTO) . '</b> y se cobra de tu ' . $RS_FUENTE . '.');
            } elseif ($RS_COSTO > 0 && !at_cobrar($cuenta, $name, $RS_COSTO, $RS_FUENTE)) {
                $mensajes[] = array('error', 'No se pudo descontar el zen del reset.');
            } else {
                $b   = at_stats_base($p['Class']);
                $sql = "UPDATE Character SET cLevel = 1, Experience = 0, Resets = ISNULL(Resets,0) + 1";
                if (!$RS_CONSERVA) {
                    $sql .= ", Strength = {$b[0]}, Dexterity = {$b[1]}, Vitality = {$b[2]}, Energy = {$b[3]}";
                    if ((int) $p['Class'] >= 64 && at_col('Character', 'Leadership')) { $sql .= ", Leadership = {$b[4]}"; }
                    $sql .= ", LevelUpPoint = {$RS_PUNTOS}";
                } else {
                    $sql .= ", LevelUpPoint = ISNULL(LevelUpPoint,0) + {$RS_PUNTOS}";
                }
                $sql .= " WHERE Name='" . at_esc($name) . "' AND AccountID='" . at_esc($cuenta) . "'";
                mssql_query($sql);
                at_mov($cuenta, 'reset', 'Reset #' . ($resets + 1), $RS_COSTO, $name);
                $mensajes[] = array('ok', '¡Reset realizado! <b>' . at_h($name) . '</b> ahora tiene '
                    . at_num($resets + 1) . ' resets y ' . at_num($RS_PUNTOS) . ' puntos para repartir.');
            }
        }
    }

    $p = at_pj($cuenta, $name); // refrescamos después de cada acción
}

$guild = mssql_fetch_row(mssql_query("SELECT G_Name FROM GuildMember WHERE Name='" . at_esc($name) . "'"));
$banco = at_banco($cuenta);
$libres = (int) $p['LevelUpPoint'];
$totalStats = (int) $p['Strength'] + (int) $p['Dexterity'] + (int) $p['Vitality'] + (int) $p['Energy'];
$puedeResetear = $hayResets && $RS_ABIERTO && (int) $p['cLevel'] >= $RS_LEVEL
    && ($RS_MAX === 0 || (int) $p['Resets'] < $RS_MAX);

at_head('Personaje: ' . $name);
foreach ($mensajes as $m) { at_msg($m[1], $m[0]); }
?>
<div class="at-grid c2">
  <div class="at-card center">
    <?= at_clase_img($p['Class'], 90, 124) ?>
    <h3><?= at_h($p['Name']) ?></h3>
    <p class="muted"><?= at_h(at_clase_nombre($p['Class'])) ?></p>
    <p><b class="gold"><?= at_num($p['Resets']) ?> resets</b> &middot; nivel <b><?= at_num($p['cLevel']) ?></b></p>
    <p><i class="on-dot <?= $online ? 'si' : '' ?>"></i> <?= $online ? 'En línea' : 'Desconectado' ?></p>
    <?php if ($esVip): ?><p><span class="tag-vip">CUENTA VIP</span></p><?php endif; ?>
    <p><a class="at-btn sec" href="ir.php?amz=Usuario">&laquo; Volver a mi cuenta</a></p>
  </div>

  <div class="at-card">
    <h2>Estadísticas</h2>
    <div class="at-stat"><span>Fuerza</span><b><?= at_num($p['Strength']) ?></b></div>
    <div class="at-stat"><span>Agilidad</span><b><?= at_num($p['Dexterity']) ?></b></div>
    <div class="at-stat"><span>Vitalidad</span><b><?= at_num($p['Vitality']) ?></b></div>
    <div class="at-stat"><span>Energía</span><b><?= at_num($p['Energy']) ?></b></div>
    <?php if ((int) $p['Class'] >= 64): ?>
      <div class="at-stat"><span>Comando</span><b><?= at_num($p['Leadership']) ?></b></div>
    <?php endif; ?>
    <div class="at-stat"><span>Total repartido</span><b><?= at_num($totalStats) ?></b></div>
    <div class="at-stat"><span>Puntos para repartir</span><b class="gold"><?= at_num($libres) ?></b></div>
    <div class="at-stat"><span>Zen en el inventario</span><b><?= at_num($p['Money']) ?></b></div>
    <div class="at-stat"><span>Zen en el WebBank</span><b class="gold"><?= at_num($banco) ?></b></div>
    <div class="at-stat"><span>Estado PVP</span><b class="<?= (int) $p['PkCount'] > 0 ? 'red' : '' ?>">
      <?= (int) $p['PkCount'] > 0 ? 'Asesino (' . at_num($p['PkCount']) . ' muertes)' : 'Ciudadano' ?></b></div>
    <div class="at-stat"><span>Guild</span><b><?= at_h(($guild && $guild[0] !== '') ? $guild[0] : '-') ?></b></div>
  </div>
</div>

<?php if ($online): ?>
  <div class="at-msg at-info">Estás conectado al juego. Para repartir puntos, resetear o mover zen tenés que <b>salir del juego</b> primero.</div>
<?php endif; ?>

<div class="at-grid c2">
  <!-- ------------------------- Repartir puntos ------------------------- -->
  <div class="at-card">
    <h2>Repartir puntos</h2>
    <p class="muted">Tenés <b class="gold"><?= at_num($libres) ?></b> puntos gratis.

    </p>
    <form method="post" id="fstats">
      <?= at_csrf_input() ?>
      <input type="hidden" name="accion" value="repartir">
      <input type="hidden" name="name" value="<?= at_h($name) ?>">
      <div class="at-stats-form">
        <label>Fuerza <span class="muted">(<?= at_num($p['Strength']) ?>)</span></label>
        <input type="number" min="0" step="1" name="add_str" value="0" class="pt">
        <label>Agilidad <span class="muted">(<?= at_num($p['Dexterity']) ?>)</span></label>
        <input type="number" min="0" step="1" name="add_agi" value="0" class="pt">
        <label>Vitalidad <span class="muted">(<?= at_num($p['Vitality']) ?>)</span></label>
        <input type="number" min="0" step="1" name="add_vit" value="0" class="pt">
        <label>Energía <span class="muted">(<?= at_num($p['Energy']) ?>)</span></label>
        <input type="number" min="0" step="1" name="add_ene" value="0" class="pt">
        <?php if ((int) $p['Class'] >= 64): ?>
          <label>Comando <span class="muted">(<?= at_num($p['Leadership']) ?>)</span></label>
          <input type="number" min="0" step="1" name="add_com" value="0" class="pt">
        <?php endif; ?>
      </div>
      <p>Puntos a repartir: <b id="pt_total">0</b> &middot; Costo: <b class="gold" id="pt_costo">0</b> zen</p>
      <button class="at-btn" type="submit" <?= $online ? 'disabled' : '' ?>>Aplicar puntos</button>
    </form>
  </div>

  <!-- ------------------------- Reset ------------------------- -->
  <div class="at-card">
    <h2>Resetear personaje</h2>
    <div class="at-stat"><span>Nivel necesario</span><b><?= at_num($RS_LEVEL) ?></b></div>
    <div class="at-stat"><span>Costo</span><b><?= $RS_COSTO > 0 ? at_num($RS_COSTO) . ' zen (' . at_h($RS_FUENTE) . ')' : '' ?></b></div>
    <div class="at-stat"><span>Puntos al resetear</span><b><?= at_num($RS_PUNTOS) ?></b></div>
    <div class="at-stat"><span>Stats</span><b><?= $RS_CONSERVA ? 'se conservan' : 'vuelven a cero (base de clase)' ?></b></div>
    <div class="at-stat"><span>Máximo de resets</span><b><?= $RS_MAX > 0 ? at_num($RS_MAX) : '360' ?></b></div>
    <?php if ($esVip && at_cfgi('reset_costo_vip', 0) < at_cfgi('reset_costo', 0)): ?>
      <p class="muted">Tenés descuento VIP aplicado en el costo del reset.</p>
    <?php endif; ?>
    <?php if ($puedeResetear): ?>
      <form method="post" onsubmit="return confirm('¿Seguro que querés resetear a <?= at_h($name) ?>?')">
        <?= at_csrf_input() ?>
        <input type="hidden" name="accion" value="resetear">
        <input type="hidden" name="name" value="<?= at_h($name) ?>">
        <button class="at-btn" type="submit" <?= $online ? 'disabled' : '' ?>>Resetear ahora</button>
      </form>
    <?php else: ?>
      <p class="muted">Todavía no podés resetear este personaje.
        <?php if ((int) $p['cLevel'] < $RS_LEVEL): ?>Te faltan <b><?= at_num($RS_LEVEL - (int) $p['cLevel']) ?></b> niveles.<?php endif; ?>
      </p>
    <?php endif; ?>
  </div>
</div>

<!-- ------------------------- Banco ------------------------- -->
<div class="at-card">
  <h2>Mi banco de zen</h2>
  <?php if (!$BK_ABIERTO): ?>
    <p class="muted">El banco está deshabilitado por el administrador.</p>
  <?php else: ?>
    <div class="at-grid c2">
      <form method="post">
        <?= at_csrf_input() ?>
        <input type="hidden" name="accion" value="depositar">
        <input type="hidden" name="name" value="<?= at_h($name) ?>">
        <label>Depositar zen (tenés <?= at_num($p['Money']) ?> en el inventario)</label>
        <input type="text" name="monto" inputmode="numeric" placeholder="Sólo números">
        <button class="at-btn" type="submit" <?= $online ? 'disabled' : '' ?>>Depositar</button>
      </form>
      <form method="post">
        <?= at_csrf_input() ?>
        <input type="hidden" name="accion" value="retirar">
        <input type="hidden" name="name" value="<?= at_h($name) ?>">
        <label>Retirar zen (tenés <?= at_num($banco) ?> en el banco)</label>
        <input type="text" name="monto" inputmode="numeric" placeholder="Sólo números">
        <button class="at-btn sec" type="submit" <?= $online ? 'disabled' : '' ?>>Retirar</button>
      </form>
    </div>
    <p class="muted">Un personaje no puede tener más de <?= at_num($BK_TOPE) ?> de zen encima. El banco es por cuenta, lo comparten todos tus personajes.</p>
  <?php endif; ?>
</div>

<script>
(function () {
  var libres = <?= (int) $libres ?>, precio = <?= (int) $ST_PRECIO ?>;
  var f = document.getElementById('fstats');
  if (!f) return;
  function calc() {
    var t = 0;
    f.querySelectorAll('input.pt').forEach(function (i) { t += Math.max(0, parseInt(i.value || '0', 10) || 0); });
    document.getElementById('pt_total').textContent = t.toLocaleString('es-AR');
    document.getElementById('pt_costo').textContent = (Math.max(0, t - libres) * precio).toLocaleString('es-AR');
  }
  f.addEventListener('input', calc);
  calc();
})();
</script>
<?php at_foot(); ?>
