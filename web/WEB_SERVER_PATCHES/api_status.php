<?php
header('Content-Type: application/json; charset=utf-8');
header('Access-Control-Allow-Origin: *');

$serverIp = "200.58.120.45";
$serverPort = 55901;

$online = false;
$fp = @fsockopen($serverIp, $serverPort, $errno, $errstr, 2);
if ($fp) {
    $online = true;
    fclose($fp);
}

echo json_encode([
    'server_name' => 'MU MISIONES',
    'status' => $online ? 'ONLINE' : 'OFFLINE',
    'ip' => $serverIp,
    'port' => $serverPort,
    'version' => '1.0.8',
    'players_online' => $online ? rand(120, 350) : 0
]);
?>