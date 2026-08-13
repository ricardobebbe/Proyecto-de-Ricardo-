<?php session_start();
	   ob_start(); 
	if(strpos(strtolower($_SERVER['REQUEST_URI']), 'phpsessid') !== false)
	{
		session_destroy();
		session_start();
		session_regenerate_id();
	}
error_reporting(0);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Mu Misiones</title><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
</head>
<?php
include 'Seguridad/Conexion.php';

@$login = $_SESSION['login'];
@$senha = $_SESSION['senha'];
if(!isset($_SESSION['login'])) 
{ 
	die("Para ingresar a esta pagina necesita estar logeado!"); 
}
else
{

	$Gms = mssql_fetch_row(mssql_query("select CuentaGM from memb_info where memb___id='".$login."'"));

	if($Gms[0] == 0) // PANEL DE USUARIO
	{ 
		header('Location: ir.php?amz=Usuario');
	}
	elseif($Gms[0] == 3) // PANEL DE ADMINISTRACION
	{ 
		header('Location: ir.php?amz=Administrador');
	}

}

?>
</html>

