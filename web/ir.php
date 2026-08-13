<?php
include 'Seguridad/Conexion.php';

$amz = isset($_GET['amz']) ? preg_replace('/[^A-Za-z]/', '', $_GET['amz']) : 'Noticias';

switch ($amz) {
//-------------------------------------------------------------------------------------
// Sitio Web
//-------------------------------------------------------------------------------------
case 'Inicio':         include 'Modulos/Noticias.php';                  break;
case 'Informacion':    include 'Modulos/Informacion.php';               break;
case 'Noticias':       include 'Modulos/Noticias.php';                  break;
case 'Reglas':         include 'Modulos/Reglas.php';                    break;
case 'Staff':          include 'Modulos/Staff.php';                     break;
case 'Contacto':       include 'Modulos/Contacto.php';                  break;
case 'Registro':       include 'Modulos/Registro.php';                  break;
case 'Recuperar':      include 'Modulos/Recuperar.php';                 break;
case 'Comunidad':      include 'Modulos/Comunidad.php';                 break;
case 'Foro':           include 'Modulos/Comunidad.php';                 break;
case 'Vip':            include 'Modulos/Shop.php';                      break;
case 'Alas':           include 'Modulos/Shop.php';                      break;
case 'Shop':           include 'Modulos/Shop.php';                      break;
case 'Tienda':         include 'Modulos/Shop.php';                      break;
case 'MisPedidos':     include 'Modulos/MisPedidos.php';                break;
case 'CofresDeZen':    include 'Modulos/CofresDeZen.php';               break;
case 'Reset':          include 'Modulos/Reset.php';                     break;
case 'Stats':          include 'Modulos/Stats.php';                     break;
case 'Parche':         include 'Modulos/Ayuda/Parche.php';              break;
case 'Descargas':      include 'Modulos/Descargas.php';                 break;
case 'Ayuda':          include 'Modulos/Ayuda.php';                     break;
case 'Actualizaciones':include 'Modulos/Ayuda/Actualizaciones.php';     break;
case 'Vip':            include 'Modulos/Vip.php';                       break;
case 'Shop':           include 'Modulos/Vip.php';                       break; 
//-------------------------------------------------------------------------------------
// RANKING
//-------------------------------------------------------------------------------------
case 'Rankings':       include 'Modulos/Rankings/Rankings.php';         break;
case 'Enlinea':        include 'Modulos/Rankings/Rankings.php';         break;
case 'Clanes':         include 'Modulos/Rankings/Rankings.php';         break;
case 'Resets':         include 'Modulos/Rankings/Rankings.php';         break;
case 'Salondelafama':  include 'Modulos/Rankings/Rankings.php';         break;
case 'Asesinos':       include 'Modulos/Rankings/Rankings.php';         break;
case 'Miembros':       include 'Modulos/Rankings/Miembros.php';         break;

//-------------------------------------------------------------------------------------
// PANEL
//-------------------------------------------------------------------------------------
case 'Panel':          include 'Panel/Panel.php';                       break;
case 'Usuario':        include 'Panel/Usuario/Panel.php';               break;
case 'Salir':          include 'Panel/Salir.php';                       break;
case 'Personaje':      include 'Panel/Usuario/Personaje.php';           break;
case 'Password':       include 'Panel/Usuario/Password.php';            break;

default:               include 'Modulos/Noticias.php';                  break;
}
