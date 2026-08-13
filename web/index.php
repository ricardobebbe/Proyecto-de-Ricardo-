<?php
@session_start();
@ob_start();

include 'Seguridad/Conexion.php';
include 'Configuraciones/Sitio.php';
require_once 'Configuraciones/Tema.php';

//---------------------------------------------
// SISTEMA DE CUENTAS
//---------------------------------------------
$at_error_login = '';
if (isset($_POST['prossegue_login'])) {
    $login = at_id(isset($_POST['login']) ? $_POST['login'] : '');
    $senha = isset($_POST['senha']) ? (string) $_POST['senha'] : '';

    if ($login === '' || $senha === '') {
        $at_error_login = 'Completá usuario y contraseña.';
    } else {
        $ok = false;
        if (isset($md5) && $md5 == 1) {
            $q  = mssql_query('exec ChecaSenha "' . at_esc($senha) . '", "' . at_esc($login) . '"');
            $ok = $q && mssql_num_rows($q) > 0;
        } else {
            $r  = mssql_fetch_array(mssql_query("SELECT memb___id, memb__pwd FROM MEMB_INFO WHERE memb___id='" . at_esc($login) . "' AND memb__pwd='" . at_esc($senha) . "'"));
            $ok = !empty($r['memb___id']);
        }

        if ($ok) {
            $_SESSION['login'] = $login;
            $_SESSION['senha'] = $senha;
            header('Location: ' . at_url('index.php'));
            exit;
        }
        $at_error_login = 'Cuenta o contraseña incorrecta.';
        session_unset();
    }
}

$at_logueado = isset($_SESSION['login']);
$at_gm = 0;
$at_online = 0;
if ($at_logueado) {
    $g = mssql_fetch_row(mssql_query("SELECT CuentaGM FROM MEMB_INFO WHERE memb___id='" . at_esc($_SESSION['login']) . "'"));
    $at_gm = isset($g[0]) ? (int) $g[0] : 0;
}
$qOn = mssql_query("SELECT COUNT(*) FROM MEMB_STAT WHERE ConnectStat>0");
if ($qOn) { $rOn = mssql_fetch_row($qOn); $at_online = isset($rOn[0]) ? (int) $rOn[0] : 0; }
$at_aviso = function_exists('at_cfg') ? trim(at_cfg('sitio_mensaje', '')) : '';
$qCt = mssql_query("SELECT COUNT(*) FROM MEMB_INFO");
$at_cuentas = 0;
if ($qCt) { $rCt = mssql_fetch_row($qCt); $at_cuentas = isset($rCt[0]) ? (int) $rCt[0] : 0; }

// Modo mantenimiento: sólo el staff puede navegar el sitio
if (function_exists('at_cfgb') && at_cfgb('sitio_mantenimiento', false) && $at_gm < 3) {
    http_response_code(503);
    echo '<!DOCTYPE html><html lang="es"><head><meta charset="UTF-8">'
       . '<meta name="viewport" content="width=device-width, initial-scale=1">'
       . '<title>' . AT_MARCA . ' - Mantenimiento</title>'
       . '<link rel="stylesheet" href="Imagenes/modulo.css"></head><body class="at-normal"><div class="at-wrap">'
       . '<h1 class="at-title">Estamos en mantenimiento</h1>'
       . '<div class="at-card"><p>La web y el servidor están en tareas de mantenimiento. Volvé en unos minutos.</p>'
       . ($at_aviso !== '' ? '<p class="muted">' . at_h($at_aviso) . '</p>' : '')
       . '</div></div></body></html>';
    exit;
}

$at_menu = array(
    ''             => 'Inicio',
    'Reglas'       => 'Reglas',
    'Informacion'  => 'Información',
    'Registro'     => 'Crear cuenta',
    'Recuperar'    => 'Recuperar pass',
    'Ayuda'        => 'Ayuda / Guías',
    'Rankings'     => 'Rankings',
    'Comunidad'    => 'Comunidad',
    'Staff'        => 'Staff',
    'Shop'         => 'Tienda VIP',
    'Descargas'    => 'Descargas',
    'Contacto'     => 'Contacto',
);
?>
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title><?= AT_MARCA ?> Online - Servidor MU temporada clásica</title>
<meta name="description" content="<?= AT_MARCA ?> Online: servidor MU con rankings en vivo, panel de usuario, VIP y descargas. Guild oficial <?= AT_GUILD ?>.">
<link href="Imagenes/favicon.ico" rel="shortcut icon" type="image/x-icon">
<link rel="stylesheet" type="text/css" href="Imagenes/styles.css">
</head>
<body>
<div class="app">

  <header class="topbar">
    <div class="brand">
      <span class="logo"><?= AT_MARCA ?></span>
      <span class="tag">Guild oficial <?= AT_GUILD ?> &middot; <?= at_h($Version) ?></span>
    </div>
    <nav class="topnav">
      <?php foreach ($at_menu as $k => $v): ?>
        <a href="<?= $k === '' ? 'ir.php?amz=Noticias' : 'ir.php?amz=' . at_h($k) ?>" target="marco"><?= at_h($v) ?></a>
      <?php endforeach; ?>
    </nav>
    <div class="statusbar">
      <span class="pill"><span class="dot"></span> <?= at_num($at_online) ?> en línea</span>
      <span class="pill"><?= at_num($at_cuentas) ?> cuentas</span>
    </div>
  </header>

  <?php if ($at_aviso !== ''): ?>
    <div class="aviso-global"><?= at_h($at_aviso) ?></div>
  <?php endif; ?>

  <div class="layout">

    <aside class="side">
      <?php if (!$at_logueado): ?>
        <div class="box">
          <div class="box-h">Acceso de jugadores</div>
          <div class="box-b">
            <?php if ($at_error_login !== ''): ?>
              <p class="err"><?= at_h($at_error_login) ?></p>
            <?php endif; ?>
            <form method="post" action="index.php">
              <label>Usuario</label>
              <input name="login" type="text" maxlength="10" autocomplete="username">
              <label>Contraseña</label>
              <input name="senha" type="password" maxlength="20" autocomplete="current-password">
              <button name="prossegue_login" type="submit" value="1">Conectar</button>
            </form>
            <div class="links">
              <a href="ir.php?amz=Registro" target="marco">Crear cuenta</a>
              <a href="ir.php?amz=Recuperar" target="marco">Olvidé mi pass</a>
            </div>
          </div>
        </div>
      <?php else: ?>
        <div class="box">
          <div class="box-h">Mi cuenta</div>
          <div class="box-b">
            <p class="cuenta"><?= at_h($_SESSION['login']) ?>
              <?php if ($at_gm > 0): ?><span class="gm">GM</span><?php endif; ?>
            </p>
            <a class="btn" href="ir.php?amz=Usuario" target="marco">Panel de usuario</a>
            <?php if ($at_gm >= 3): ?>
              <a class="btn sec" href="Panel/Administrador/index.php" target="_blank">Administración</a>
            <?php endif; ?>
            <a class="btn sec" href="ir.php?amz=Reset" target="marco">Resetear personaje</a>
            
            <a class="btn sec" href="ir.php?amz=MisPedidos" target="marco">Mis pedidos</a>
            <a class="btn sec" href="ir.php?amz=Salir">Cerrar sesión</a>
          </div>
        </div>
      <?php endif; ?>

      <div class="box">
        <div class="box-h">Servidor</div>
        <div class="box-b info">
          <div><span>Versión</span><b><?= at_h($Version) ?></b></div>
          <div><span>Exp / Drop</span><b><?= at_h($Expydrop) ?></b></div>
          <div><span>Reset</span><b><?= at_h($Reset) ?></b></div>
          <div><span>Máx. level</span><b><?= at_h($Maxear) ?></b></div>
        </div>
      </div>

      <div class="box promos">
        <a href="ir.php?amz=Shop" target="marco"><img src="Imagenes/haztevip.jpg" alt="Hazte VIP"></a>
        <a href="ir.php?amz=Shop" target="marco"><img src="Imagenes/alasfull.jpg" alt="Comprar alas"></a>
        <a href="ir.php?amz=Descargas" target="marco"><img src="Imagenes/download.jpg" alt="Descargas"></a>
      </div>
    </aside>

    <main class="content">
      <iframe src="ir.php?amz=Noticias" name="marco" title="Contenido"></iframe>
    </main>

  </div>

  <footer class="foot">
    <span><?= at_h($Copyright) ?></span>
    <span><?= AT_MARCA ?> &middot; <?= AT_GUILD ?></span>
  </footer>

</div>
</body>
</html>
