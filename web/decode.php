<?php
function Guild_hex2bin ($RawInput)
{
	$BinStr = '';
	for ($i = 0; $i < strlen ($RawInput); $i += 2)
		$BinStr .= '%'.substr ($RawInput, $i, 2);

	return rawurldecode ($BinStr);
}

function binhex($bin) {
   $hex = dechex(bindec($bin));
   return $hex;
} 

$size = 120;
$pixelSize = $size / 8;
$hex = $_GET['decode'];
function color($mark) {

	if($mark == 0)
	{
		$color = "#ffffff";
	}
    if($mark == 1)
	{
		$color = "#000000";
	}
    if($mark == 2)
	{
		$color = "#8c8a8d";
	}
    if($mark == 3)
	{
		$color = "#ffffff";
	}
    if($mark == 4)
	{
		$color = "#fe0000";
	}
    if($mark == 5)
	{
		$color = "#ff8a00";
	}
    if($mark == 6)
	{
		$color = "#00ffff";
	}
    if($mark == 7)
	{
		$color = "#8cff01";
	}
    if($mark == 8)
	{
		$color = "#00ff00";
	}
    if($mark == 9)
	{
		$color = "#01ff8d";
	}
    if($mark == 'a')
	{
		$color = "#ffff00";
	}
    if($mark == 'b')
	{
		$color = "#008aff";
	}
    if($mark == 'c')
	{
		$color = "#0000fe";
	}
    if($mark == 'd')
	{
		$color = "#8c00ff";
	}
    if($mark == 'e')
	{
		$color = "#ff00fe";
	}
    if($mark == 'f')
	{
		$color = "#ff008c";
	}
    return $color;
}
for ($y = 0; $y < 8; $y++){
    for ($x = 0; $x < 8; $x++){
        $offset = ($y * 8) + $x;
        $Cuadrilla8x8[$y][$x] = substr($hex, $offset, 1);
    }
}
$SuperCuadrilla = array();
for ($y = 1; $y <= 8; $y++) {
    for ($x = 1; $x <= 8; $x++){
        $bit = $Cuadrilla8x8[$y - 1][$x - 1];
        for ($repiteY = 0; $repiteY < $pixelSize; $repiteY++) {
            for ($repite = 0; $repite < $pixelSize; $repite++) {
                $translatedY = ((($y - 1) * $pixelSize) + $repiteY);
                $translatedX=((($x - 1) * $pixelSize) + $repite);
                $SuperCuadrilla[$translatedY][$translatedX] = $bit;
            }
        }
    }
}
$img = imagecreate($size, $size);
for ($y = 0; $y < $size; $y++) {
    for ($x = 0; $x < $size; $x++) {
        $bit = $SuperCuadrilla[$y][$x];
        $color = substr(color($bit),1);
        $r = substr($color,0,2);
        $g = substr($color,2,2);
        $b = substr($color,4,2);
        $superPixel = imagecreate(1,1);
        $cl = imagecolorallocatealpha($superPixel,hexdec($r),hexdec($g),hexdec($b),0);
        imagefilledrectangle($superPixel,0,0,1,1,$cl);
        imagecopy($img,$superPixel,$x,$y,0,0,1,1);
        imagedestroy($superPixel);
    }
}
header("Content-type: image/gif");
imagerectangle($img,0,0,$size-1,$size-1, imagecolorallocate($img,0,0,0));
imagepng($img);
?>