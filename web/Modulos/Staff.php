<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

/**
 * Staff dinámico: toma los GM directamente de la base
 *  CuentaGM: 1 = Ayudante, 2 = Moderador, 3+ = Administrador
 */
$niveles = array(1 => 'Ayudante', 2 => 'Moderador', 3 => 'Administrador', 4 => 'Administrador');

$staff = array();
$q = mssql_query("SELECT memb___id, CuentaGM FROM MEMB_INFO WHERE CuentaGM > 0 ORDER BY CuentaGM DESC, memb___id ASC");
if ($q) {
    while ($r = mssql_fetch_row($q)) {
        $cuenta = $r[0];
        $nivel  = (int) $r[1];
        $per = mssql_fetch_row(mssql_query("SELECT TOP 1 Name, Class FROM Character WHERE AccountID='" . at_esc($cuenta) . "' ORDER BY cLevel DESC"));
        $gld = $per ? mssql_fetch_row(mssql_query("SELECT G_Name FROM GuildMember WHERE Name='" . at_esc($per[0]) . "'")) : null;
        $on  = mssql_fetch_row(mssql_query("SELECT ConnectStat FROM MEMB_STAT WHERE memb___id='" . at_esc($cuenta) . "'"));
        $staff[] = array(
            'nombre' => $per ? $per[0] : $cuenta,
            'clase'  => $per ? (int) $per[1] : -1,
            'rango'  => isset($niveles[$nivel]) ? $niveles[$nivel] : 'Staff',
            'guild'  => ($gld && $gld[0] !== '') ? $gld[0] : AT_GUILD,
            'online' => ($on && (int) $on[0] > 0),
        );
    }
}

at_head('Staff de ' . AT_MARCA);

if (!$staff) {
    at_msg('Todavía no hay cuentas marcadas como staff. Asigná <b>CuentaGM</b> (1 ayudante, 2 moderador, 3 admin) en MEMB_INFO desde el panel de administración.', 'info');
} else {
    foreach (array('Administrador', 'Moderador', 'Ayudante', 'Staff') as $rango) {
        $grupo = array_values(array_filter($staff, function ($s) use ($rango) { return $s['rango'] === $rango; }));
        if (!$grupo) { continue; }
        echo '<h2>' . at_h(strtoupper($rango) . 'ES') . '</h2><div class="at-grid c5">';
        foreach ($grupo as $s) {
            echo '<div class="at-char">'
               . at_clase_img($s['clase'], 70, 96)
               . '<span class="nom">' . at_h($s['nombre']) . '</span>'
               . '<span class="cls">' . at_h($s['rango']) . '</span>'
               . '<span class="cls">Guild: ' . at_h($s['guild']) . '</span>'
               . '<span class="cls"><i class="on-dot ' . ($s['online'] ? 'si' : '') . '"></i> '
               . ($s['online'] ? 'En línea' : 'Desconectado') . '</span>'
               . '</div>';
        }
        echo '</div>';
    }
}

echo '<div class="at-card"><h3>¿Necesitás ayuda de un GM?</h3><p class="muted">Escribinos desde la sección '
   . '<a href="ir.php?amz=Contacto">Contacto</a> o en la comunidad oficial. Nunca pidas ni entregues tu contraseña: el staff de '
   . AT_MARCA . ' jamás te la va a pedir.</p></div>';

at_foot();
