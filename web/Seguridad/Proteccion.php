<?php
//-----------------------------------------------
// SISTEMA ANTI-INYECCIONES SQL (reescrito para PHP 7/8)
//-----------------------------------------------
require_once dirname(__DIR__) . '/Configuraciones/Funciones.php';

if (!function_exists('at_bloquear')) {
    function at_bloquear($valor, $codigo)
    {
        criar_log_inject("Envió: " . $valor . "\r\n");
        die('<script>alert("Entrada no permitida (' . $codigo . '). Usá solamente letras y números.");'
            . 'location="javascript:history.back()"</script>');
    }

    function at_revisar($valor, $codigo)
    {
        if (!is_string($valor)) {
            if (is_array($valor)) {
                foreach ($valor as $v) { at_revisar($v, $codigo); }
            }
            return;
        }

        // Caracteres claramente peligrosos en una web de MU
        if (preg_match('/[<>\\\\\x00\x1a]|--|\/\*|\*\/|;\s*(drop|delete|update|insert|exec)/i', $valor)) {
            at_bloquear($valor, $codigo);
        }

        // Palabras reservadas completas (no fragmentos, para no romper el uso normal)
        global $badchars;
        $tokens = preg_split('/[^a-z0-9_]+/i', strtolower($valor), -1, PREG_SPLIT_NO_EMPTY);
        if ($tokens) {
            foreach ($tokens as $t) {
                if (in_array($t, array('drop','shutdown','truncate','xp_cmdshell','union','sysobjects','information_schema'), true)) {
                    at_bloquear($valor, $codigo);
                }
            }
        }
    }
}

foreach ($_POST as $__v)   { at_revisar($__v, 'P'); }
foreach ($_GET as $__v)    { at_revisar($__v, 'G'); }
foreach ($_COOKIE as $__v) {
    if (is_string($__v) && preg_match('/[<>\x00]/', $__v)) {
        criar_log_inject("Cookie sospechosa: " . $__v . "\r\n");
    }
}
