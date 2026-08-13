<?php
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';

$tab = isset($_GET['amz']) ? preg_replace('/[^A-Za-z]/', '', $_GET['amz']) : 'Resets';
if ($tab === 'Rankings') { $tab = 'Resets'; }

$tabs = array(
    'Resets'        => 'Resets',
    'Enlinea'       => 'En línea',
    'Clanes'        => 'Guilds',
    'Salondelafama' => 'Salón de la fama',
    'Asesinos'      => 'Asesinos (PK)',
);

at_head('Rankings de ' . AT_MARCA, 'full');

echo '<div class="at-tabs">';
foreach ($tabs as $k => $v) {
    echo '<a class="' . ($k === $tab ? 'on' : '') . '" href="ir.php?amz=' . $k . '">' . at_h($v) . '</a>';
}
echo '</div>';

function at_online_cuenta($cuenta)
{
    $r = mssql_fetch_row(mssql_query("SELECT ConnectStat FROM MEMB_STAT WHERE memb___id='" . at_esc($cuenta) . "'"));
    return ($r && (int) $r[0] > 0);
}
function at_guild_de($nombre)
{
    $r = mssql_fetch_row(mssql_query("SELECT G_Name FROM GuildMember WHERE Name='" . at_esc($nombre) . "'"));
    return ($r && $r[0] !== '') ? $r[0] : '-';
}

if ($tab === 'Enlinea') {
    $q = mssql_query("SELECT s.memb___id FROM MEMB_STAT s WHERE s.ConnectStat > 0 ORDER BY s.memb___id ASC");
    $n = $q ? mssql_num_rows($q) : 0;
    echo '<p class="muted">Jugadores conectados ahora: <b class="gold">' . at_num($n) . '</b></p>';
    echo '<table class="at"><tr><th>#</th><th>Personaje</th><th>Clase</th><th class="center">Nivel</th><th class="center">Resets</th><th>Guild</th><th class="center">VIP</th></tr>';
    for ($i = 0; $i < $n; $i++) {
        $c = mssql_fetch_row($q);
        $g = mssql_fetch_row(mssql_query("SELECT GameIdc FROM AccountCharacter WHERE Id='" . at_esc($c[0]) . "'"));
        $p = ($g && $g[0] !== '') ? mssql_fetch_row(mssql_query("SELECT Name, Class, Resets, cLevel FROM Character WHERE Name='" . at_esc($g[0]) . "'")) : null;
        $v = mssql_fetch_row(mssql_query("SELECT vip FROM MEMB_INFO WHERE memb___id='" . at_esc($c[0]) . "'"));
        if (!$p) { continue; }
        echo '<tr><td>' . ($i + 1) . '</td><td><b>' . at_h($p[0]) . '</b></td><td>' . at_h(at_clase_nombre($p[1]))
           . '</td><td class="center">' . at_num($p[3]) . '</td><td class="center gold">' . at_num($p[2])
           . '</td><td>' . at_h(at_guild_de($p[0])) . '</td><td class="center">'
           . ((isset($v[0]) && (int) $v[0] > 0) ? '<span class="green">SÍ</span>' : '<span class="muted">no</span>') . '</td></tr>';
    }
    if ($n == 0) { echo '<tr><td colspan="7" class="center muted">No hay jugadores conectados en este momento.</td></tr>'; }
    echo '</table>';

} elseif ($tab === 'Clanes') {
    $q = mssql_query("SELECT TOP 50 G_Name, G_Master, G_Score, G_Notice FROM Guild ORDER BY G_Score DESC, G_Name ASC");
    echo '<table class="at"><tr><th>#</th><th>Guild</th><th>Master</th><th class="center">Miembros</th><th class="center">Score</th></tr>';
    $i = 0;
    while ($q && ($r = mssql_fetch_row($q))) {
        $i++;
        $m = mssql_fetch_row(mssql_query("SELECT COUNT(*) FROM GuildMember WHERE G_Name='" . at_esc($r[0]) . "'"));
        echo '<tr><td>' . $i . '</td><td><b><a href="ir.php?amz=Miembros&name=' . urlencode($r[0]) . '">' . at_h($r[0]) . '</a></b></td><td>'
           . at_h($r[1]) . '</td><td class="center">' . at_num(isset($m[0]) ? $m[0] : 0) . '</td><td class="center gold">' . at_num($r[2]) . '</td></tr>';
    }
    if ($i === 0) { echo '<tr><td colspan="5" class="center muted">Todavía no hay guilds creadas.</td></tr>'; }
    echo '</table>';

} elseif ($tab === 'Asesinos') {
    $q = mssql_query("SELECT TOP 100 Name, Class, cLevel, Resets, PkCount, PkLevel, AccountID FROM Character WHERE PkCount > 0 ORDER BY PkCount DESC");
    echo '<table class="at"><tr><th>#</th><th>Personaje</th><th>Clase</th><th class="center">Nivel</th><th class="center">Resets</th><th class="center">Muertes PK</th><th class="center">Estado</th></tr>';
    $i = 0;
    while ($q && ($r = mssql_fetch_row($q))) {
        $i++;
        echo '<tr><td>' . $i . '</td><td><b>' . at_h($r[0]) . '</b></td><td>' . at_h(at_clase_nombre($r[1]))
           . '</td><td class="center">' . at_num($r[2]) . '</td><td class="center">' . at_num($r[3])
           . '</td><td class="center red">' . at_num($r[4]) . '</td><td class="center"><i class="on-dot '
           . (at_online_cuenta($r[6]) ? 'si' : '') . '"></i></td></tr>';
    }
    if ($i === 0) { echo '<tr><td colspan="7" class="center muted">Sin asesinos registrados.</td></tr>'; }
    echo '</table>';

} elseif ($tab === 'Salondelafama') {
    $q = mssql_query("SELECT TOP 10 Name, Class, cLevel, Resets, AccountID FROM Character ORDER BY Resets DESC, cLevel DESC");
    echo '<div class="at-grid c5">';
    $i = 0;
    while ($q && ($r = mssql_fetch_row($q))) {
        $i++;
        echo '<div class="at-char">' . at_clase_img($r[1], 70, 96)
           . '<span class="nom">#' . $i . ' ' . at_h($r[0]) . '</span>'
           . '<span class="cls">' . at_h(at_clase_nombre($r[1])) . '</span>'
           . '<span class="cls">Level ' . at_num($r[2]) . ' &middot; ' . at_num($r[3]) . ' resets</span>'
           . '<span class="cls"><i class="on-dot ' . (at_online_cuenta($r[4]) ? 'si' : '') . '"></i></span></div>';
    }
    echo '</div>';
    if ($i === 0) { at_msg('Todavía no hay personajes en el salón de la fama.', 'info'); }

} else { // Resets
    $q = mssql_query("SELECT TOP 100 Name, Class, Resets, cLevel, AccountID, Strength, Dexterity, Vitality, Energy FROM Character WHERE ctlcode = 0 OR ctlcode IS NULL ORDER BY Resets DESC, cLevel DESC");
    echo '<table class="at"><tr><th>#</th><th>Personaje</th><th>Clase</th><th class="center">Nivel</th><th class="center">Resets</th>'
       . '<th class="center">Fue/Agi/Vit/Ene</th><th>Guild</th><th class="center">Estado</th></tr>';
    $i = 0;
    while ($q && ($r = mssql_fetch_row($q))) {
        $i++;
        echo '<tr><td>' . $i . '</td><td><b>' . at_h($r[0]) . '</b></td><td>' . at_h(at_clase_nombre($r[1]))
           . '</td><td class="center">' . at_num($r[3]) . '</td><td class="center gold">' . at_num($r[2])
           . '</td><td class="center muted">' . at_num($r[5]) . ' / ' . at_num($r[6]) . ' / ' . at_num($r[7]) . ' / ' . at_num($r[8])
           . '</td><td>' . at_h(at_guild_de($r[0])) . '</td><td class="center"><i class="on-dot '
           . (at_online_cuenta($r[4]) ? 'si' : '') . '"></i></td></tr>';
    }
    if ($i === 0) { echo '<tr><td colspan="8" class="center muted">Todavía no hay personajes creados.</td></tr>'; }
    echo '</table>';
}

at_foot();
