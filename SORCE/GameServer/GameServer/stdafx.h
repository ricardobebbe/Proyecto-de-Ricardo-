#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WINXP

// 1. CONFIGURACIÓN MAESTRA DE VERSIONES (Debe ir arriba de todo)
#ifndef GAMESERVER_TYPE
    #define GAMESERVER_TYPE 0
#endif

#ifndef GAMESERVER_EXTRA
    #define GAMESERVER_EXTRA 1
#endif

#ifndef GAMESERVER_UPDATE
    #define GAMESERVER_UPDATE 100 // 100 = Base limpia para lógicas antiguas / 97x
#endif

#ifndef GAMESERVER_LANGUAGE
    #define GAMESERVER_LANGUAGE 1
#endif

#define SEASON_97X // Macro maestro global para inyectar lógica 97x en User y Protocolo

// 2. FILTRO DINÁMICO DE TEMPORADAS
#if(GAMESERVER_UPDATE==100)
    #define GAMESERVER_SEASON "SEASON 97X"
#elif(GAMESERVER_UPDATE==401)
    #define GAMESERVER_SEASON "SEASON 4"
#elif(GAMESERVER_UPDATE==603)
    #define GAMESERVER_SEASON "SEASON 6"
#elif(GAMESERVER_UPDATE==803)
    #define GAMESERVER_SEASON "SEASON 8"
#endif

#if(GAMESERVER_TYPE==0)
    #define GAMESERVER_VERSION "GS"
#else
    #define GAMESERVER_VERSION "GSCS"
#endif

// 3. CONFIGURACIONES GENERALES DEL CLIENTE
#define GAMESERVER_STATUS "DESLIGADO"
#define GAMESERVER_STATUS_MODE "ATIVADO"
#define GAMESERVER_HOST 1
#define ISPREMIUN 1
extern int Conectar;

#define GAMESERVER_NOMBRE 1
#if(GAMESERVER_NOMBRE == 0)
    #define GAMESERVER_CLIENT "VIERJA&HIGHLEVEL"
#elif(GAMESERVER_NOMBRE == 1)
    #define GAMESERVER_CLIENT "RDDLV"
#else
    #define GAMESERVER_CLIENT "REAL GAMING"
#endif

#define UPDATE_GAMING 25
#define GAMESERVER_CLIENTE_UPDATE 25
#define GAMESERVER_CLIENTE_PREMIUM 0

#ifndef PROTECT_STATE
    #define PROTECT_STATE 0
#endif

#ifndef ENCRYPT_STATE
    #define ENCRYPT_STATE 1
#endif

#define GAMESERVER_NAME "Premium"

// 4. CONTROL DE CARACTERÍSTICAS (Configurado para simular 97x)
#ifdef SEASON_97X
    #define CUSTOM_PET 0       // Desactivado: Evita crasheos de inventario en User
    #define NEW_FENRIR 0       // Desactivado: El Fenrir no existe en 97x
    #define ARCA_WAR 2         // Desactivado
    #define MASTER_HOURS 2     // Desactivado: No hay niveles Master en 97x
    #define ACHERON_GUARDIAN 2 // Desactivado
#else
    #define CUSTOM_PET 1
    #define NEW_FENRIR 1
    #define ARCA_WAR 2
    #define MASTER_HOURS 1
    #define ACHERON_GUARDIAN 2
#endif

#define OFFLINE_MODE 1
#define ADD_OFFMODE_PICKUP 0 
#define BOT_BUFFER 1
#define USE_FAKE_ONLINE TRUE
#define GOLDEN_ARCHER 2 
#define START_ITEM 1 
#define PG_CUSTOM 1
#define ALLBOTSSTRUC 1

#ifndef POINT_CUSTOM
    #define POINT_CUSTOM 1 
#endif
#define PC_POINT 2 
#define HAPPY_HOURS 2 

// 5. INCLUDES DEL SISTEMA
#include "pugixml.hpp"
#include <windows.h>
#include <winsock2.h>
#include <mswSock.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;
#include <map>
#include <vector>
#include <queue>
#include <random>
#include <Rpc.h>
#include <algorithm>
#include <string>
#include <atltime.h>
#include <dbghelp.h>
#include <Psapi.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include "..\\..\\Util\\lua\\include\\lua.hpp"

// 6. LIBRERÍAS DEL ENLAZADOR (Limpias de duplicados)
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"..\\..\\Util\\lua\\lua52.lib")

#if(GAMESERVER_UPDATE>=701)
    #if(NDEBUG==0)
        #pragma comment(lib,"..\\..\\Util\\cryptopp\\Debug\\cryptlib.lib")
    #else
        #pragma comment(lib,"..\\..\\Util\\cryptopp\\Release\\cryptlib.lib")
    #endif
    #pragma comment(lib,"..\\..\\Util\\mapm\\mapm.lib")
#endif

typedef char chr;
typedef float flt;
typedef short shrt;
typedef unsigned __int64 QWORD;
