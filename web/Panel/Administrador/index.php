<?php
require_once __DIR__ . '/comun.php';

$archivo = __DIR__ . '/Secciones/' . $AT_SECCION . '.php';
if (!file_exists($archivo)) { $archivo = __DIR__ . '/Secciones/inicio.php'; }
require $archivo;
