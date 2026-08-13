<?php
/**
 * Capa de compatibilidad mssql_* para PHP 7 / 8
 * -------------------------------------------------------------
 * La extensión mssql_* fue eliminada en PHP 7. Este archivo
 * re-implementa las funciones que usa la web sobre los drivers
 * modernos disponibles:
 *
 *   - sqlsrv       (Windows / XAMPP  -> Microsoft Drivers for PHP)
 *   - pdo_sqlsrv   (Windows / Linux  -> Microsoft Drivers for PHP)
 *   - pdo_dblib    (Linux / cPanel   -> FreeTDS)
 *   - odbc         (cualquiera con DSN ODBC)
 *
 * No hay que tocar nada: se detecta el primero disponible.
 */

if (!defined('AT_COMPAT_LOADED')) {
    define('AT_COMPAT_LOADED', true);

class AT_DB
{
    /** @var AT_DB|null */
    public static $instance = null;

    public $driver = null;
    public $conn   = null;
    public $lastError = '';
    public $lastMessage = '';

    public static function driversDisponibles()
    {
        $d = array();
        if (function_exists('sqlsrv_connect'))                          $d[] = 'sqlsrv';
        if (class_exists('PDO')) {
            $pdo = PDO::getAvailableDrivers();
            if (in_array('sqlsrv', $pdo)) $d[] = 'pdo_sqlsrv';
            if (in_array('dblib',  $pdo)) $d[] = 'pdo_dblib';
            if (in_array('odbc',   $pdo)) $d[] = 'pdo_odbc';
        }
        if (function_exists('odbc_connect'))                            $d[] = 'odbc';
        if (function_exists('mssql_connect'))                           $d[] = 'mssql';
        return $d;
    }

    public function connect($host, $user, $pass, $db, $prefer = 'auto')
    {
        $port = null;
        if (strpos($host, ',') !== false) {
            list($host, $port) = explode(',', $host, 2);
        } elseif (substr_count($host, ':') === 1) {
            list($host, $port) = explode(':', $host, 2);
        }
        $host = trim($host);
        $port = $port !== null ? (int) trim($port) : null;

        $orden = self::driversDisponibles();
        if ($prefer !== 'auto' && $prefer !== '' && in_array($prefer, $orden)) {
            $orden = array_merge(array($prefer), array_diff($orden, array($prefer)));
        }

        foreach ($orden as $drv) {
            if ($this->tryDriver($drv, $host, $port, $user, $pass, $db)) {
                $this->driver = $drv;
                self::$instance = $this;
                return true;
            }
        }
        return false;
    }

    private function tryDriver($drv, $host, $port, $user, $pass, $db)
    {
        try {
            switch ($drv) {
                case 'sqlsrv':
                    $server = $port ? $host . ',' . $port : $host;
                    $c = sqlsrv_connect($server, array(
                        'UID' => $user, 'PWD' => $pass, 'Database' => $db,
                        'CharacterSet' => 'UTF-8', 'TrustServerCertificate' => true,
                        'LoginTimeout' => 5,
                    ));
                    if ($c === false) { $this->lastError = $this->sqlsrvErr(); return false; }
                    $this->conn = $c; return true;

                case 'pdo_sqlsrv':
                    $server = $port ? $host . ',' . $port : $host;
                    $dsn = "sqlsrv:Server={$server};Database={$db};TrustServerCertificate=1";
                    $this->conn = new PDO($dsn, $user, $pass, array(
                        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
                        PDO::ATTR_TIMEOUT => 5,
                    ));
                    return true;

                case 'pdo_dblib':
                    $dsn = "dblib:host={$host}" . ($port ? ':' . $port : '') . ";dbname={$db};charset=UTF-8";
                    $this->conn = new PDO($dsn, $user, $pass, array(
                        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
                        PDO::ATTR_TIMEOUT => 5,
                    ));
                    return true;

                case 'pdo_odbc':
                    $server = $port ? $host . ',' . $port : $host;
                    $dsn = "odbc:Driver={ODBC Driver 17 for SQL Server};Server={$server};Database={$db};TrustServerCertificate=yes";
                    $this->conn = new PDO($dsn, $user, $pass, array(PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION));
                    return true;

                case 'odbc':
                    $server = $port ? $host . ',' . $port : $host;
                    $dsn = "Driver={ODBC Driver 17 for SQL Server};Server={$server};Database={$db};TrustServerCertificate=yes";
                    $c = @odbc_connect($dsn, $user, $pass);
                    if (!$c) { $this->lastError = odbc_errormsg(); return false; }
                    $this->conn = $c; return true;

                case 'mssql':
                    $server = $port ? $host . ',' . $port : $host;
                    $c = @mssql_connect($server, $user, $pass);
                    if (!$c) return false;
                    @mssql_select_db($db, $c);
                    $this->conn = $c; return true;
            }
        } catch (Exception $e) {
            $this->lastError = $e->getMessage();
            return false;
        }
        return false;
    }

    private function sqlsrvErr()
    {
        $e = sqlsrv_errors();
        return $e ? $e[0]['message'] : 'Error desconocido';
    }

    /**
     * Ejecuta una consulta. Devuelve AT_Result para SELECT,
     * true para comandos sin filas y false en caso de error.
     */
    public function query($sql)
    {
        $this->lastError = '';
        try {
            switch ($this->driver) {
                case 'sqlsrv':
                    $st = sqlsrv_query($this->conn, $sql, array(), array('Scrollable' => 'buffered'));
                    if ($st === false) { $this->lastError = $this->sqlsrvErr(); $this->lastMessage = $this->lastError; return false; }
                    $rows = array();
                    while (($r = sqlsrv_fetch_array($st, SQLSRV_FETCH_BOTH)) !== null && $r !== false) {
                        $rows[] = $this->normalize($r);
                    }
                    sqlsrv_free_stmt($st);
                    return count($rows) ? new AT_Result($rows) : new AT_Result(array());

                case 'pdo_sqlsrv':
                case 'pdo_dblib':
                case 'pdo_odbc':
                    $st = $this->conn->query($sql);
                    if ($st === false) return false;
                    $rows = array();
                    do {
                        if ($st->columnCount() > 0) {
                            foreach ($st->fetchAll(PDO::FETCH_BOTH) as $r) $rows[] = $this->normalize($r);
                        }
                    } while ($this->nextRowset($st));
                    return new AT_Result($rows);

                case 'odbc':
                    $st = @odbc_exec($this->conn, $sql);
                    if (!$st) { $this->lastError = odbc_errormsg($this->conn); return false; }
                    $rows = array();
                    while ($r = odbc_fetch_array($st)) {
                        $i = 0; $row = array();
                        foreach ($r as $k => $v) { $row[$k] = $v; $row[$i++] = $v; }
                        $rows[] = $row;
                    }
                    return new AT_Result($rows);

                case 'mssql':
                    return mssql_query($sql, $this->conn);
            }
        } catch (Exception $e) {
            $this->lastError = $e->getMessage();
            $this->lastMessage = $e->getMessage();
            return false;
        }
        return false;
    }

    private function nextRowset($st)
    {
        try { return $st->nextRowset(); } catch (Exception $e) { return false; }
    }

    /** Convierte DateTime -> string y recorta espacios de CHAR */
    private function normalize($row)
    {
        foreach ($row as $k => $v) {
            if ($v instanceof DateTime)      $row[$k] = $v->format('Y-m-d H:i:s');
            elseif (is_string($v))           $row[$k] = rtrim($v);
        }
        return $row;
    }
}

/** Resultado en memoria con cursor, imita un recurso mssql */
class AT_Result
{
    public $rows = array();
    public $pos  = 0;

    public function __construct($rows) { $this->rows = $rows; }
    public function numRows() { return count($this->rows); }
    public function fetch()
    {
        if ($this->pos >= count($this->rows)) return false;
        return $this->rows[$this->pos++];
    }
    public function seek($n) { $this->pos = (int) $n; return true; }
}

/* -------- Funciones mssql_* -------- */
if (!function_exists('mssql_connect')) {

    function mssql_connect($host, $user, $pass, $newlink = false) {
        $db = new AT_DB();
        // La base se selecciona en mssql_select_db(); guardamos credenciales.
        $GLOBALS['__at_pending'] = array($host, $user, $pass);
        return $db;
    }

    function mssql_select_db($dbname, $link = null) {
        if (!isset($GLOBALS['__at_pending'])) return false;
        list($host, $user, $pass) = $GLOBALS['__at_pending'];
        $db = ($link instanceof AT_DB) ? $link : new AT_DB();
        $prefer = isset($GLOBALS['DriverSQL']) ? $GLOBALS['DriverSQL'] : 'auto';
        $ok = $db->connect($host, $user, $pass, $dbname, $prefer);
        if ($ok) AT_DB::$instance = $db;
        return $ok;
    }

    function mssql_query($sql, $link = null) {
        $db = ($link instanceof AT_DB) ? $link : AT_DB::$instance;
        if (!$db) return false;
        $r = $db->query($sql);
        if ($r === false) return false;
        return $r;
    }

    function mssql_num_rows($res) { return ($res instanceof AT_Result) ? $res->numRows() : 0; }

    function mssql_fetch_row($res) {
        if (!($res instanceof AT_Result)) return false;
        $r = $res->fetch();
        if ($r === false) return false;
        $out = array();
        foreach ($r as $k => $v) if (is_int($k)) $out[$k] = $v;
        return $out;
    }

    function mssql_fetch_array($res, $type = 3) {
        if (!($res instanceof AT_Result)) return false;
        return $res->fetch();
    }

    function mssql_fetch_assoc($res) {
        if (!($res instanceof AT_Result)) return false;
        $r = $res->fetch();
        if ($r === false) return false;
        $out = array();
        foreach ($r as $k => $v) if (!is_int($k)) $out[$k] = $v;
        return $out;
    }

    function mssql_fetch_object($res) {
        $a = mssql_fetch_assoc($res);
        return $a === false ? false : (object) $a;
    }

    function mssql_data_seek($res, $n) { return ($res instanceof AT_Result) ? $res->seek($n) : false; }

    function mssql_result($res, $row, $field) {
        if (!($res instanceof AT_Result)) return false;
        if (!isset($res->rows[$row])) return false;
        $r = $res->rows[$row];
        return isset($r[$field]) ? $r[$field] : false;
    }

    function mssql_num_fields($res) {
        if (!($res instanceof AT_Result) || !count($res->rows)) return 0;
        return (int) (count($res->rows[0]) / 2);
    }

    function mssql_free_result($res) { if ($res instanceof AT_Result) { $res->rows = array(); } return true; }

    function mssql_close($link = null) { return true; }

    function mssql_get_last_message() {
        return AT_DB::$instance ? AT_DB::$instance->lastMessage : '';
    }

    function mssql_min_error_severity($n) { return true; }
    function mssql_min_message_severity($n) { return true; }
}

/* -------- Compatibilidad de funciones eliminadas en PHP 7 -------- */
if (!function_exists('eregi')) {
    function eregi($patron, $cadena, &$coincidencias = null) {
        $patron = '/' . str_replace('/', '\/', $patron) . '/i';
        $r = @preg_match($patron, (string) $cadena, $coincidencias);
        return $r ? $r : false;
    }
}
if (!function_exists('ereg')) {
    function ereg($patron, $cadena, &$coincidencias = null) {
        $patron = '/' . str_replace('/', '\/', $patron) . '/';
        $r = @preg_match($patron, (string) $cadena, $coincidencias);
        return $r ? $r : false;
    }
}
if (!function_exists('split')) {
    function split($patron, $cadena, $limite = -1) {
        return preg_split('/' . str_replace('/', '\/', $patron) . '/', (string) $cadena, $limite);
    }
}
if (!function_exists('get_magic_quotes_gpc')) {
    function get_magic_quotes_gpc() { return false; }
}

} // AT_COMPAT_LOADED
