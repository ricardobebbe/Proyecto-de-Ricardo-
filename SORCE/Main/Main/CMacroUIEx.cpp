#include "stdafx.h"
#include "CMacroUIEx.h"
#include "Util.h"
#include "Offset.h"
#include "Import.h"
#include "User.h"
#include "Interface.h"
#include "Defines.h"
#include "Protocol.h"
#include "Common.h"
#include "MiniMap.h"
#include "InfoLog.h"

// ---------------------------------------------------------------------------------------------
CMacroUIEx gCMacroUIEx;
// ---------------------------------------------------------------------------------------------

void CMacroUIEx::RenderFrame(int* This)
{
    char Cord[256];
    float X = (float)*((DWORD*)This + 5);
    float Y = (float)*((signed int*)This + 6);
    float renderMenu = 15.0f;

    EnableAlphaTest(1);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    renderMenu = 10.0f;

    //-- Fondos del Helper e Interfaz Desactivados
    // RenderBitmap(51550, X + 85.0 + renderMenu, Y, 124.0, 24.0, 0.0, 0.285, 0.725, 0.28, 1, 1, 0.0); 
    // RenderBitmap(51550, X, Y, 124.0, 24.0, 0.0, 0.0, 0.725, 0.28, 1, 1, 0.0); 

    pSetFont(pTextThis(), (int)pFontNormal);

    //-- Dejamos activo el sistema para seguir viendo el Mapa y las Coordenadas en pantalla
    if (World >= 82 && World <= 100)
    {
        wsprintf(Cord, "%s %d,%d", pGetTextLine(pTextLineThis, (3810 + World - 82)), gObjUser.CoordenadaX, gObjUser.CoordenadaY);
    }
    else
    {
        wsprintf(Cord, "%s %d,%d", pGetMapName(World), gObjUser.CoordenadaX, gObjUser.CoordenadaY);
    }

    pSetBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
    pSetTextColor(pTextThis(), 240, 240, 240, 255);
    pDrawText(pTextThis(), X - 35, Y + 4, Cord, 190, 0, (LPINT)3, 0);

    //-- Código desactivado (comentado de forma segura)
    /*
    DWORD Color = Color4f(255, 189, 25, 255);
    gInterface.DrawFormat(Color, X + 154 + renderMenu, Y + 2, 27.5, 3, "0");

    renderMenu = 15.0f;
    gInterface.DrawButtonRender(ButtonSettings, 119 + renderMenu, 7.0, 0, 0);
    if (gInterface.IsWorkZone(ButtonSettings)) {
        gInterface.DrawToolTip(X + 119 + renderMenu, Y + 50, "Config Helper");
        if (gInterface.Data[ButtonSettings].OnClick) {
            RenderBitmap(51546, X + 119 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.451f, 0.878, 0.225, 1, 1, 0.0);
        } else {
            RenderBitmap(51546, X + 119 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.2255f, 0.878, 0.225, 1, 1, 0.0);
        }
    } else {
        RenderBitmap(51546, X + 119 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.0, 0.878, 0.225, 1, 1, 0.0);
    }

    if (!*(BYTE*)(*(DWORD*)(MUHELPER_STRUC)+0x08)) {
        gInterface.DrawButtonRender(ButtonStart, 138 + renderMenu, 7.0 , 0, 0.0f);
        if (gInterface.IsWorkZone(ButtonStart)) {
            gInterface.DrawToolTip(X + 138 + renderMenu, Y + 50,"Play Helper");
            if (gInterface.Data[ButtonStart].OnClick) {
                RenderBitmap(51547, X + 138 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.451f, 0.878, 0.225, 1, 1, 0.0);
            } else {
                RenderBitmap(51547, X + 138 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.2255f, 0.878, 0.225, 1, 1, 0.0);
            }
        } else {
            RenderBitmap(51547, X + 138 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.0, 0.878, 0.225, 1, 1, 0.0);
        }
    } else {
        // Bloque del else que quedó cortado al final
    }
    */

    //-- Código activo restaurado y corregido dentro de la función
    gInterface.DrawButtonRender(ButtonStop, 138 + renderMenu, 7.0, 0, 0.0f);

    if (offhelper == 1)
    {
        //gMuHelper.RunningOffHelper();  // ELIMINADO: gMuHelper no está definido
    }

    //-- Pause
    if (gInterface.IsWorkZone(ButtonStop))
    {
        gInterface.DrawToolTip(X + 138 + renderMenu, Y + 50, "Pause Helper");

        if (gInterface.Data[ButtonStop].OnClick)
        {
            RenderBitmap(51548, X + 138 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.451f, 0.878, 0.225, 1, 1, 0.0);
            //gMuHelper.ClearOffHelper();
        }
        else
        {
            RenderBitmap(51548, X + 138 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.2255f, 0.878, 0.225, 1, 1, 0.0);
        }
    }
    else
    {
        RenderBitmap(51548, X + 138 + renderMenu, Y + 7.0, 19, 19, 0.0, 0.0, 0.878, 0.225, 1, 1, 0.0);
    }

    gInterface.DrawButtonRender(eMenu, X + 100.0f + renderMenu, Y + 7.0, 0, 0.0f);

    if (gInterface.IsWorkZone(eMenu))
    {
        gInterface.DrawToolTip(X + 100.0f + renderMenu, Y + 50, "Especial Menu"); // Corregido: Y + 50 en lugar de solo 50

        if (gInterface.Data[eMenu].OnClick)
        {
            RenderBitmap(51553, X + 100.0f + renderMenu, Y + 7.0, 19, 19, 0.0, 0.451f, 0.878, 0.225, 1, 1, 0.0);
        }
        else
        {
            RenderBitmap(51553, X + 100.0f + renderMenu, Y + 7.0, 19, 19, 0.0, 0.2255f, 0.878, 0.225, 1, 1, 0.0);
        }
    }
    else
    {
        RenderBitmap(51553, X + 100.0f + renderMenu, Y + 7.0, 19, 19, 0.0, 0.0, 0.878, 0.225, 1, 1, 0.0);
    }

    pGLSwitch();
    EnableAlphaTest(0);
}

void CMacroUIEx::Button(DWORD Event)
{
    DWORD CurrentTick = GetTickCount();

    if (gInterface.IsWorkZone(ButtonSettings))
    {
        DWORD Delay = (CurrentTick - gInterface.Data[ButtonSettings].EventTick);

        if (Event == WM_LBUTTONDOWN)
        {
            gInterface.Data[ButtonSettings].OnClick = true;
            return;
        }

        gInterface.Data[ButtonSettings].OnClick = false;

        if (Delay < 500)
        {
            return;
        }

        gInterface.Data[ButtonSettings].EventTick = GetTickCount();
    }
    else if (gInterface.IsWorkZone(ButtonStart))
    {
        DWORD Delay = (CurrentTick - gInterface.Data[ButtonStart].EventTick);

        if (Event == WM_LBUTTONDOWN)
        {
            gInterface.Data[ButtonStart].OnClick = true;
            return;
        }

        gInterface.Data[ButtonStart].OnClick = false;

        if (Delay < 500)
        {
            return;
        }

        gInterface.Data[ButtonStart].EventTick = GetTickCount();
    }
    else if (gInterface.IsWorkZone(ButtonStop))
    {
        DWORD Delay = (CurrentTick - gInterface.Data[ButtonStop].EventTick);

        if (Event == WM_LBUTTONDOWN)
        {
            gInterface.Data[ButtonStop].OnClick = true;
            return;
        }

        gInterface.Data[ButtonStop].OnClick = false;

        if (Delay < 500)
        {
            return;
        }

        gInterface.Data[ButtonStop].EventTick = GetTickCount();
    }
}

char BarTime(int This)
{
    float v3;
    float v4;
    float v5;
    int v13;

    v13 = This;

    if (*(BYTE*)(v13 + 72) == 1)
    {
        // CORREGIDO: Se asignan valores a v4 y v5 antes de usarlos (antes eran v1 y v2, causando error de variable no inicializada)
        v5 = (float)*(signed int*)(This + 20);
        v4 = (float)*(signed int*)(This + 16);
        RenderImage2(51522, v5, v4);

        if (*(DWORD*)(v13 + 88) == 1)
        {
            v3 = *(float*)(v13 + 92) * 50.0f;
            v4 = (float)(*(DWORD*)(v13 + 20) + 2);
            v5 = (float)(*(DWORD*)(v13 + 16) + 2);
            RenderImage(51549, v5 + 108.0f, v4 - 16.0f, v3, 3.0f);
        }
        else if (*(DWORD*)(v13 + 88) > 1)
        {
            RenderImage2(51522, v5, v4);
            v3 = *(float*)(v13 + 92) * 50.0f;
            v4 = (float)(*(DWORD*)(v13 + 20) + 2);
            v5 = (float)(*(DWORD*)(v13 + 16) + 2);
            RenderImage(51549, v5 + 108.0f, v4 - 16.0f, v3, 3.0f);
        }
    }
    return 1;
}

//-- (007D25C0)
__declspec(naked) void MuHelperRemake_01()  //<- ConfigHelper
{
    static DWORD Remake_Buffer = 0x007D26DA;
    static DWORD W = 19;
    static DWORD H = 19;
    static float X = 119.0f + 15.0f; // CORREGIDO: Cambiado de DWORD a float para evitar truncamiento y empujar correctamente en la pila
    static float Y = 7.0f;           // CORREGIDO: Cambiado de DWORD a float

    _asm
    {
        PUSH H; | Arg6
        PUSH W; | Arg5
        MOV EDX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EAX, DWORD PTR DS : [EDX + 0x18] ; |
        PUSH Y; | Arg4
        MOV ECX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EDX, DWORD PTR DS : [ECX + 0x14] ; |
        MOV EAX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV ECX, DWORD PTR DS : [EAX + 0xC] ; |
        LEA EDX, DWORD PTR DS : [EDX + ECX + 0x29] ; |
        PUSH X; | Arg3
        JMP[Remake_Buffer]
    }
}

//-- (007D25C0)
__declspec(naked) void MuHelperRemake_02()  //<- PlayHelper
{
    static DWORD Remake_Buffer = 0x007D2776;
    static DWORD W = 19;
    static DWORD H = 19;
    static float X = 138.0f + 15.0f; // CORREGIDO: float
    static float Y = 7.0f;           // CORREGIDO: float

    _asm
    {
        PUSH H; | Arg6
        PUSH W; | Arg5
        MOV EAX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV ECX, DWORD PTR DS : [EAX + 0x18] ; |
        PUSH Y; | Arg4
        MOV EDX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EAX, DWORD PTR DS : [EDX + 0x14] ; |
        MOV ECX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EDX, DWORD PTR DS : [ECX + 0xC] ; |
        LEA EAX, DWORD PTR DS : [EAX + EDX + 0x3B] ; |
        PUSH X; | Arg3
        JMP[Remake_Buffer]
    }
}

//-- (007D25C0)
__declspec(naked) void MuHelperRemake_03()  //<- PauseHelper
{
    static DWORD Remake_Buffer = 0x007D2821;
    static DWORD W = 19;
    static DWORD H = 19;
    static float X = 138.0f + 15.0f; // CORREGIDO: float
    static float Y = 7.0f;           // CORREGIDO: float

    _asm
    {
        PUSH H; | Arg6
        PUSH W; | Arg5
        MOV EAX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV ECX, DWORD PTR DS : [EAX + 0x18] ; |
        PUSH Y; | Arg4
        MOV EDX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EAX, DWORD PTR DS : [EDX + 0x14] ; |
        MOV ECX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EDX, DWORD PTR DS : [ECX + 0xC] ; |
        LEA EAX, DWORD PTR DS : [EAX + EDX + 0x3B] ; |
        PUSH X; | Arg3
        JMP[Remake_Buffer]
    }
}

//==========================================================================================

void CMacroUIEx::RenderHelperFrame(int* This)
{
    char Cord[256];
    float X = (float)*((DWORD*)This + 5);
    float Y = (float)*((signed int*)This + 6);
    float RenderX;

    EnableAlphaTest(1);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    RenderImage(31602, X, Y, 22.0, 25.0);

    for (int i = 0; i < 5; i++)
    {
        RenderX = X + 22.0f + (i * 26.0f);
        RenderImage(31603, RenderX, Y, 26.0, 25.0);
    }

    RenderImage(31604, RenderX, Y, 73.0, 20.0);

    // Mapa y Coordenadas
    if (World >= 82 && World <= 100)
    {
        wsprintf(Cord, "%s  (%d , %d)", pGetTextLine(pTextLineThis, (3810 + World - 82)), gObjUser.CoordenadaX, gObjUser.CoordenadaY);
    }
    else
    {
        wsprintf(Cord, "%s  (%d , %d)", pGetMapName(World), gObjUser.CoordenadaX, gObjUser.CoordenadaY);
    }

    pSetBackgroundTextColor(pTextThis(), 0, 0, 0, 0);
    pSetTextColor(pTextThis(), 240, 240, 240, 255);
    pDrawText(pTextThis(), X - 30, Y + 4, Cord, 190, 0, (LPINT)3, 0);
    pGLSwitch();
    EnableAlphaTest(0);

    //-- Config
    gInterface.DrawButtonRender(ButtonSettings, 146, 1, 0, 0);

    if (gInterface.IsWorkZone(ButtonSettings))
    {
        gInterface.DrawToolTip(146, Y + 50, "Config Helper");

        if (gInterface.Data[ButtonSettings].OnClick)
        {
            pDrawButton(31761, 146, 1.6, 18.0f, 13.3, 0, 26.9);
        }
        else
        {
            pDrawButton(31761, 146, 1.6, 18.0f, 13.3, 0, 13.6);
        }
    }
    else
    {
        pDrawButton(31761, 146, 1.0, 18.0f, 13.3, 0, 0);
    }

    if (!*(BYTE*)(*(DWORD*)(MUHELPER_STRUC)+0x08))
    {
        gInterface.DrawButtonRender(ButtonStart, 146 + 18, 1, 0, 0);
        //-- Play
        if (gInterface.IsWorkZone(ButtonStart))
        {
            gInterface.DrawToolTip(146 + 18, Y + 50, "Play Helper");

            if (gInterface.Data[ButtonStart].OnClick)
            {
                pDrawButton(31762, 146 + 18, 1.6, 18.0f, 13.3, 0, 26.9);
            }
            else
            {
                pDrawButton(31762, 146 + 18, 1.6, 18.0f, 13.3, 0, 13.6);
            }
        }
        else
        {
            pDrawButton(31762, 146 + 18, 1.0, 18.0f, 13.3, 0, 0);
        }
    }
    else
    {
        gInterface.DrawButtonRender(ButtonStop, 146 + 18, 1, 0, 0);

        if (offhelper == 1)
        {
            //gMuHelper.RunningOffHelper();  // ELIMINADO: gMuHelper no está definido
        }
        //-- Pause
        if (gInterface.IsWorkZone(ButtonStop))
        {
            gInterface.DrawToolTip(146 + 18, Y + 50, "Pause Helper");

            if (gInterface.Data[ButtonStop].OnClick)
            {
                pDrawButton(31763, 146 + 18, 1.6, 18.0f, 13.3, 0, 26.9);
            }
            else
            {
                pDrawButton(31763, 146 + 18, 1.6, 18.0f, 13.3, 0, 13.6);
            }
        }
        else
        {
            pDrawButton(31763, 146 + 18, 1.0, 18.0f, 13.3, 0, 0);
        }
    }

    //=======================================================
    DWORD Color;
    if (gProtect.m_MainInfo.CustomMenuSwitch != 0)
    {
        pDrawButton(30983, 146.0f + 37.2f, 0.0, 36, 18, 0, 0);

        gInterface.DrawButtonRender(eMenu, 146.0f + 37.2f, Y + 1.0, 0, 0.0f);

        if (gInterface.IsWorkZone(eMenu))
        {
            gInterface.DrawToolTip(146.0f + 37.2f, Y + 50, "Especial Menu"); // CORREGIDO: Y + 50

            if (gInterface.Data[eMenu].OnClick)
            {
                Color = eGray150;
                gInterface.DrawColoredGUI(eMenu, 146.0f + 37.2f, Y + 1.0, Color);
            }
            else
            {
                gInterface.DrawButtonRender(eMenu, 146.0f + 37.2f, Y + 1.0, 0, 0.0f);
            }
        }
        else
        {
            gInterface.DrawButtonRender(eMenu, 146.0f + 37.2f, Y + 1.0, 0, 0.0f);
        }
    }
} // CORREGIDO: Se añadió la llave de cierre que faltaba para esta función

__declspec(naked) void MuHelperRemake2_01()  //<- ConfigHelper
{
    static DWORD Addr_JMP = 0x007D26DA;
    static DWORD W = 18;
    static DWORD H = 13.3; // Nota: Se mantiene como DWORD si el macro original lo espera así, pero idealmente sería float
    static float X = 146.0f;       // CORREGIDO: float
    static float Y = 1.0f;         // CORREGIDO: float

    _asm
    {
        PUSH H; | Arg6
        PUSH W; | Arg5
        MOV EDX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EAX, DWORD PTR DS : [EDX + 0x18] ; |
        PUSH Y; | Arg4
        MOV ECX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EDX, DWORD PTR DS : [ECX + 0x14] ; |
        MOV EAX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV ECX, DWORD PTR DS : [EAX + 0xC] ; |
        LEA EDX, DWORD PTR DS : [EDX + ECX + 0x29] ; |
        PUSH X; | Arg3
        JMP[Addr_JMP]
    }
}

__declspec(naked) void MuHelperRemake2_02()  //<- PlayHelper
{
    static DWORD Addr_JMP = 0x007D2776;
    static DWORD W = 18;
    static DWORD H = 13.3;
    static float X = 146.0f + 18.0f; // CORREGIDO: float
    static float Y = 1.0f;           // CORREGIDO: float

    _asm
    {
        PUSH H; | Arg6
        PUSH W; | Arg5
        MOV EAX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV ECX, DWORD PTR DS : [EAX + 0x18] ; |
        PUSH Y; | Arg4
        MOV EDX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EAX, DWORD PTR DS : [EDX + 0x14] ; |
        MOV ECX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EDX, DWORD PTR DS : [ECX + 0xC] ; |
        LEA EAX, DWORD PTR DS : [EAX + EDX + 0x3B] ; |
        PUSH X; | Arg3
        JMP[Addr_JMP]
    }
}

__declspec(naked) void MuHelperRemake2_03()  //<- PauseHelper
{
    static DWORD Addr_JMP = 0x007D2821;
    static DWORD W = 18;
    static DWORD H = 13.3;
    static float X = 146.0f + 18.0f; // CORREGIDO: float
    static float Y = 1.0f;           // CORREGIDO: float

    _asm
    {
        PUSH H; | Arg6
        PUSH W; | Arg5
        MOV EAX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV ECX, DWORD PTR DS : [EAX + 0x18] ; |
        PUSH Y; | Arg4
        MOV EDX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EAX, DWORD PTR DS : [EDX + 0x14] ; |
        MOV ECX, DWORD PTR SS : [EBP - 0xB8] ; |
        MOV EDX, DWORD PTR DS : [ECX + 0xC] ; |
        LEA EAX, DWORD PTR DS : [EAX + EDX + 0x3B] ; |
        PUSH X; | Arg3
        JMP[Addr_JMP]
    }
}

void OffHelper(DWORD id, float x, float y, float w, float h)
{
    pDrawGUI(id, x, y, w, h);

    if (gInterface.IsWorkZone(ButtonStartAttack))
    {
        if (gInterface.Data[ButtonStartAttack].OnClick)
        {
            if (offhelper == 0)
            {
                offhelper = 1;
            }
            else
            {
                offhelper = 0;
            }
            gInterface.Data[ButtonStartAttack].OnClick = false;
        }
    }

    if (offhelper == 1)
    {
        gInterface.DrawButtonRender(ButtonStartAttack, x + 50, y + 8, 0, 0);
    }
    else
    {
        gInterface.DrawButtonRender(ButtonStartAttack, x + 50, y + 8, 0, 15);
    }

    gInterface.DrawFormat(eGold, x + 50 + 16, y + 12, 60, 1, "MuOffHelper");
}

#define pHelperMouseClick ((char(__thiscall*)(int a1)) 0x007D2920)

char FixHelperMouseClick(int a1)
{
    if (gInterface.CheckWindow(ChaosBox) || gInterface.CheckWindow(Warehouse))
        return 0;

    return pHelperMouseClick(a1);
}

void CMacroUIEx::Load()
{
    SetCompleteHook(0xE8, 0x007D2B0C, &FixHelperMouseClick);
    SetCompleteHook(0xE8, 0x007F76CD, &OffHelper);

    if (gProtect.m_MainInfo.CustomInterfaceType == 3 || gProtect.m_MainInfo.CustomInterfaceType == 4)
    {
        // CORREGIDO: Se añade cast a (DWORD) para evitar errores de compilación con punteros a funciones miembro
        SetCompleteHook(0xE9, 0x007D2BC0, (DWORD)&CMacroUIEx::RenderFrame);
        SetCompleteHook(0xE9, 0x0080B8E0, (DWORD)&BarTime);
        SetCompleteHook(0xE9, 0x007D26B5, (DWORD)&MuHelperRemake_01);
        SetCompleteHook(0xE9, 0x007D2751, (DWORD)&MuHelperRemake_02);
        SetCompleteHook(0xE9, 0x007D27FC, (DWORD)&MuHelperRemake_03);
    }
    else
    {
        SetCompleteHook(0xE9, 0x007D26B5, (DWORD)&MuHelperRemake2_01);
        SetCompleteHook(0xE9, 0x007D2751, (DWORD)&MuHelperRemake2_02);
        SetCompleteHook(0xE9, 0x007D27FC, (DWORD)&MuHelperRemake2_03);
        SetCompleteHook(0xE9, 0x007D2BC0, (DWORD)&CMacroUIEx::RenderHelperFrame);
    }
}