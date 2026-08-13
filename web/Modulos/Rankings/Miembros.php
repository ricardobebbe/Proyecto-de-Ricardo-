<?php
if (!defined('AT_CONEXION')) { include dirname(dirname(__DIR__)) . '/Seguridad/Conexion.php'; }
require_once dirname(dirname(__DIR__)) . '/Configuraciones/Tema.php';

$g = isset($_GET['name']) ? $_GET['name'] : '';
at_head('Miembros de la guild ' . $g);
echo '<div class="at-tabs"><a href="ir.php?amz=Clanes">&laquo; Volver a guilds</a></div>';
$q = mssql_query("SELECT m.Name, m.G_Level, c.Class, c.cLevel, c.Resets, c.AccountID FROM GuildMember m LEFT JOIN Character c ON c.Name = m.Name WHERE m.G_Name='" . at_esc($g) . "' ORDER BY m.G_Level DESC, c.Resets DESC");
$rangos = array(0 => 'Miembro', 32 => 'Battle Master', 64 => 'Asistente', 128 => 'Guild Master');
echo '<table class="at"><tr><th>Personaje</th><th>Rango</th><th>Clase</th><th class="center">Nivel</th><th class="center">Resets</th><th class="center">Estado</th></tr>';
$i = 0;
while ($q && ($r = mssql_fetch_row($q))) {
    $i++;
    $on = mssql_fetch_row(mssql_query("SELECT ConnectStat FROM MEMB_STAT WHERE memb___id='" . at_esc($r[5]) . "'"));
    echo '<tr><td><b>' . at_h($r[0]) . '</b></td><td>' . at_h(isset($rangos[(int) $r[1]]) ? $rangos[(int) $r[1]] : 'Miembro')
       . '</td><td>' . at_h(at_clase_nombre($r[2])) . '</td><td class="center">' . at_num($r[3])
       . '</td><td class="center gold">' . at_num($r[4]) . '</td><td class="center"><i class="on-dot '
       . (($on && (int) $on[0] > 0) ? 'si' : '') . '"></i></td></tr>';
}
if ($i === 0) { echo '<tr><td colspan="6" class="center muted">Guild sin miembros.</td></tr>'; }
echo '</table>';
at_foot();
