




/*
     FILE ARCHIVED ON 12:23:40 ene 21, 2015 AND RETRIEVED FROM THE
     INTERNET ARCHIVE ON 6:00:14 mar 20, 2017.
     JAVASCRIPT APPENDED BY WAYBACK MACHINE, COPYRIGHT INTERNET ARCHIVE.

     ALL OTHER CONTENT MAY ALSO BE PROTECTED BY COPYRIGHT (17 U.S.C.
     SECTION 108(a)(3)).
*/
var activado=0;

function setBoton(num){
	for (var i=0;i<10;i++)
		document.getElementById("td"+i.toString()).className='topmenu';
	document.getElementById(num).className='topmenuhov';
	activado=num[2];
}

function getBoton(num)	{
	if(activado!=num.id[2])
		num.className='topmenu';
}

// funcion de jquery efectos
function runEffect(efecto,tiempo) {
	// get effect type from 
	var selectedEffect = efecto;
	
	// most effect types need no options passed by default
	var options = {};
	// run the effect
	$( "#effect" ).toggle( selectedEffect, options, tiempo );
};
