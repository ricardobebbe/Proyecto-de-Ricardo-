#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Define la compatibilidad mínima con Windows (Windows XP en adelante)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

typedef unsigned __int64 QWORD;

// System Include
#include <windows.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Mmsystem.h>
#include <time.h>
#include <gl\GL.h>
#include "detours.h"
#include <Shellapi.h>
#include <vector>
#include <map>
#include <tlhelp32.h> // ZEUS UP20 MAXIMO DE ABRIR Main.exe
#include <tchar.h>
#include <windef.h>
#include <atltime.h>
#include <stdarg.h>    // 🛠️ Librería de argumentos cargada correctamente arriba

// 🛠️ REEMPLAZO CORRECTO PARA PARCHAR FUNCIÓN DE LOGS/CHAT EN COMPILADORES MODERNOS
#ifndef _crt_va_start
#define _crt_va_start(ap, v) va_start(ap, v)
#endif
#ifndef _crt_va_end
#define _crt_va_end(ap) va_end(ap)
#endif
#ifndef _crt_va_arg
#define _crt_va_arg(ap, t) va_arg(ap, t)
#endif

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Opengl32.lib")
#pragma comment(lib,"detours.lib")

#define PREMIUN	1
#define MAX_CHAT_TYPE1			10

#define MAX_CUSTOM_GLOVES	50
#define MAX_CUSTOM_BOW		50
#define MAX_PET_ITEM		300
#define MAX_CUSTOMCLOAK		60

#define MAX_CUSTOM_WING			100
#define MAX_REMAKE_EFFECT		100
#define MAX_CUSTOM_ITEM_INFO	1000
#define MAX_POSITION_ITEM		1000

#define MAX_CUSTOPETEFFECT			2000
#define MAX_DYNAMIC_WING_EFFECT		5000
#define MAX_CUSTOM_WING_EFFECT		5000

#define UPDATE 15

#define ADD_PARTY_LEADER 0

#define LODWORD(h)					((DWORD)(__int64(h) & __int64(0xffffffff)))
