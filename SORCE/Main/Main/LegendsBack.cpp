#include "stdafx.h"
#include "Import.h"
#include "Defines.h"
#include "Util.h"
#include "TMemory.h"
#include "ToolKit.h"
#include "LegendsBack.h"
#include "StatsAdvance.h"
#include "Offset.h"
#include "OffTrade.h"
#include "Interface.h"

void WindowsInventory(DWORD id, float x, float y, float w, float h)
{
	pDrawGUI(61522, x, y, w, h);
}

void WindowsBack01(DWORD id, float x, float y, float w, float h)
{
	pDrawGUI(61522, x, y, w, h);
}

void WindowsStore(DWORD id, float x, float y, float w, float h)
{
	pDrawGUI(61522, x, y, w, h);

	gOffTrade.DrawOffTradeWindow(x, y);
}

void WindowsEmpty(DWORD id, float x, float y, float w, float h)
{
	pDrawGUI(51522, x, y, w, h);
}

__declspec(naked) void BackInventory()
{
	static DWORD BoxColorContinue_buff = 0x0077FDAC;

	_asm
	{
	PUSH 255
	PUSH 199
	PUSH 110
	PUSH 30
	JMP [BoxColorContinue_buff]
	}
}

__declspec(naked) void PointInventory()
{
	static DWORD BoxColorContinue_buff = 0x0077FD98;

	_asm
	{
	PUSH 255
	PUSH 0
	PUSH 0
	PUSH 0
	JMP [BoxColorContinue_buff]
	}
}

//Character
int RemakeCharacterWindow(int a1)
{
  int v42; // ST24_4@1
  float green; // ST08_4@1
  float red; // ST04_4@1
  
  v42 = a1;
  green = (double)*(signed int *)(a1 + 20);
  red =   (double)*(signed int *)(a1 + 16);
  pDrawGUI(31352, red, green, 190.0, 433.0);
  		if(*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x74) > 0)
		{
		pDrawGUI(0x700020, 558, 57, 65.0, 11.0);
		}
//
  int DL = pGetCharClass(*(BYTE *)(oUserPreviewStruct + 19));
  if ( DL == 4 )
  {
  green = (double)*(signed int *)(a1 + 20);
  red = (double)*(signed int *)(a1 + 16);
  pDrawGUI(51291, red, green, 190.0, 433.0);
  		if(*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x74) > 0)
		{
		pDrawGUI(0x700020, 558, 57, 65.0, 11.0);
		}
  }

  int Class = pGetCharClass(*(BYTE *)(oUserPreviewStruct + 19));
  if ( Class == 3 || Class == 5 || Class == 6)
  {
  green = (double)*(signed int *)(a1 + 20);
  red = (double)*(signed int *)(a1 + 16);
  pDrawGUI(51292, red, green, 190.0, 433.0);
  		if(*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x74) > 0)
		{
		pDrawGUI(0x700020, 558, 57, 65.0, 11.0);
		}
  }
//
  return a1;
}

DWORD Slot;
__declspec(naked) void ShadowCheck()
{
	static DWORD jmpBack = 0x007DB30F;

	__asm
	{
		mov     eax, [ebp - 0x1C]
		movzx   ecx, byte ptr[eax + 0x60]
		mov		Slot, ecx
		jmp     [jmpBack]
	}
}

void ItemBox(DWORD ID, float x, float y, float w, float h)
{
	RenderBitmap(31072, x, y, w, h, 0, 0, 1.0, 1.0, 1, 1, 0.0);

	switch (Slot)
	{
		case 0:
		break;
		case 1:
			glColor4f(1.0, 1.0, 0.0, 0.30000001);//glColor4f(1.0, 1.0, 0.0, 0.60000001);
		break;
		case 2:
			glColor4f(1.0, 0.66000003, 0.0, 0.30000001);//glColor4f(1.0, 0.66000003, 0.0, 0.60000001);
		break;
		case 3:
			glColor4f(0.89000001, 0.10900001, 0.10900001, 0.30000001);//glColor4f(1.0, 0.33000001, 0.0, 0.60000001);
		break;
		case 4:
			glColor4f(1.0, 0.0, 0.0, 0.40000001);
		break;
		case 5:
			glColor4f(1.0, 0.2, 0.1, 0.40000001);
		break;
		default:
			RenderBitmap(31071, x, y, w, h, 0, 0, 1.0, 1.0, 1, 1, 0.0);
		break;
	}

	if(Slot >= 1 && Slot <= 5)
	{
		pSetBlend(true);
		pDrawBarForm(x, y, w, h, 0.0, 0);
		pGLSwitchBlend();
		pGLSwitch();
		glColor3f(1.0, 1.0, 1.0);
	}

	Slot = -1;
}

bool isWorkZone(int a1, int a2, int a3, int a4)
{
	
	if (gInterface.CheckWindow(ObjWindow::Inventory)){
		a3 -= 190;
		
	}
	if (gInterface.CheckWindow(ObjWindow::ExpandInventory)
		|| gInterface.CheckWindow(ObjWindow::ExpandWarehouse)){
		a3 -= 190;
	}
	if (gInterface.CheckWindow(ObjWindow::Warehouse)
		|| gInterface.CheckWindow(ObjWindow::Trade)
		|| gInterface.CheckWindow(ObjWindow::ChaosBox)){
		a3 -= 190;
	}
	else if (gInterface.CheckWindow(ObjWindow::Character)){
		a3 -= 190;
	}
	return pCursorX < a3 && pCursorY < a4;
}


void Master_back(DWORD a1, float a2, float a3, float a4, float a5) 
{
	pDrawGUI(a1, a2, a3, a4, 428);
}


bool WindowsCheck()
{
	if (gInterface.CheckWindow(Character) ||
		gInterface.CheckWindow(CommandWindow) ||
		gInterface.CheckWindow(GensInfo) ||
		gInterface.CheckWindow(Guild) ||
		gInterface.CheckWindow(NPC_Dialog) ||
		gInterface.CheckWindow(NPC_Julia) ||
		gInterface.CheckWindow(NPC_Titus) ||
		gInterface.CheckWindow(Party) ||
		gInterface.CheckWindow(PetInfo) ||
		gInterface.CheckWindow(MuHelper) ||
		gInterface.CheckWindow(Quest) ||
		gInterface.CheckWindow(Lugard) ||
		gInterface.CheckWindow(Jerint) ||
		gInterface.CheckWindow(GoldenArcher1) ||
		gInterface.CheckWindow(GoldenArcher2) ||
		gInterface.CheckWindow(LuckyCoin1) ||
		gInterface.CheckWindow(LuckyCoin2) ||
		gInterface.CheckWindow(NPC_Duel) ||
		gInterface.CheckWindow(NPC_Devin) ||
		gInterface.CheckWindow(GuardNPC) ||
		gInterface.CheckWindow(SeniorNPC) ||
		gInterface.CheckWindow(GuardNPC2) ||
		gInterface.CheckWindow(CastleGateSwitch) ||
		gInterface.CheckWindow(CatapultNPC) ||
		gInterface.CheckWindow(CreateGuild) ||
		gInterface.CheckWindow(ObjWindow::Inventory))
		return true;
	return false;
}

bool WindowsCombined2Checks()
{
	if ((gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(Store))
		||	(gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(Warehouse))
		||	(gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(ExpandInventory))
		||	(gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(OtherStore))
		||	(gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(Character))
		||	(gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(Shop))
		||	(gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(ObjWindow::Trade))
		)
		return true;
	return false;
}

bool WindowsCombinedChecks()
{
	if ((gInterface.CheckWindow(Inventory)&& gInterface.CheckWindow(ExpandInventory)&& gInterface.CheckWindow(Store))
		||	(gInterface.CheckWindow(Inventory)&& gInterface.CheckWindow(Warehouse)&& gInterface.CheckWindow(ExpandWarehouse))
		||	(gInterface.CheckWindow(Inventory)&& gInterface.CheckWindow(ExpandInventory)&& gInterface.CheckWindow(OtherStore))
		||	(gInterface.CheckWindow(Inventory)&& gInterface.CheckWindow(ExpandInventory)&& gInterface.CheckWindow(Warehouse))
		||	(gInterface.CheckWindow(Character)&& gInterface.CheckWindow(ExpandInventory)&& gInterface.CheckWindow(Inventory))
		||	(gInterface.CheckWindow(ObjWindow::Trade)&& gInterface.CheckWindow(ExpandInventory)&& gInterface.CheckWindow(Inventory))
		)
		return true;
	return false;
}


int sub_5C6E80()
{
	signed int v48;

	v48 = 640;

	return v48;
}

signed int JCGetScreenWidth()
{
	signed int v52; // [sp+0h] [bp-4h]@11

	v52 = CGetScreenWidth2( );

	return v52;
}

signed int JCWinIngame()
{
	return 640;
}

bool CheckMouseWindow(int x, int y, int w, int h)
{
	if( pCheckWindow(pWindowThis(), 78) )
	{
		x = 70;
		w = 380;
	}
	return pCheckMouseOver(x, y, w, h);
}

void LegendsLoad()
{
	SetCompleteHook(0xE8, 0x00857746, &CheckMouseWindow); //-- baul check
	//--
	SetCompleteHook(0xE8, 0x005B6180, &JCGetScreenWidth);
	//--
	SetCompleteHook(0xE8, 0x0040F2E9, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x004D714E, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x004D79E7, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x004D9756, &JCWinIngame); //-- Camara General Renderizado
	//--
	SetCompleteHook(0xE8, 0x005DB4D5, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB1AA, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB22F, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB322, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB3A7, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB46D, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB4AD, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB534, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB5C4, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB628, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB68C, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x005DB6ED, &JCWinIngame); //-- Camara General Renderizado
	SetCompleteHook(0xE8, 0x008605BF, &JCWinIngame); //-- load character
	//-
	SetCompleteHook(0xE8, 0x005B8344, &JCGetScreenWidth); //-- check cursor attack
	SetCompleteHook(0xE8, 0x0047FFF4, &JCGetScreenWidth); //-- Load MapName In-game
	SetCompleteHook(0xE8, 0x0048007C, &JCGetScreenWidth); //-- Load MapName In-game
	SetCompleteHook(0xE8, 0x004F2A6C, &JCGetScreenWidth); //-- algo del raven
	SetCompleteHook(0xE8, 0x004F2B55, &JCGetScreenWidth); //-- algo del raven
	SetCompleteHook(0xE8, 0x005B4956, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x005B9E22, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x005BAFE5, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x007D453E, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x007DF2D9, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x007DFD4B, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x007DFD54, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x007DFD68, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x007F3F87, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x007F4A63, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x007FAC46, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x008346F6, &JCGetScreenWidth);
	SetCompleteHook(0xE8, 0x0085F90C, &JCGetScreenWidth);

	//-- mueve la cámara
	//SetCompleteHook(0xE9, 0x005C6E80, &sub_5C6E80);
	//SetDword(0x005C7281+3,640);

	//SetOp((LPVOID)0x0081A146, (LPVOID)Master_back, ASM::CALL); //0081A146 1.04e	//00819FF6 1.04d
	//SetOp((LPVOID)0x0081A199, (LPVOID)Master_back, ASM::CALL); //0081A199 1.04e //0081A049 1.04d
	////
	//SetOp((LPVOID)0x00834700, (LPVOID)isWorkZone, ASM::CALL); //00834700 1.04e	//008345B0 1.04d
	//
	Slot = -1;
	SetCompleteHook(0xE9, 0x007DB308, &ShadowCheck); //Get Slot Type
	SetCompleteHook(0xE8, 0x007DB5D0, &ItemBox);

	SetCompleteHook(0xE9, 0x0077FDA4, &BackInventory);
	SetCompleteHook(0xE9, 0x0077FD87, &PointInventory);
	SetCompleteHook(0xE8, 0x0077F7EE, &RemakeCharacterWindow);
	SetDword((PVOID)(0x0077E935 + 1), 51290);
	SetByte((PVOID)(0x0077E94A + 1), 24);
	SetByte((PVOID)(0x0077E94C + 1), 24);
	SetByte((PVOID)(0x0077E957 + 2), 117);
	SetByte((PVOID)(0x0077E964 + 1), 130);
	SetDword((PVOID)(0x0077E97E + 1), 51290);
	SetByte((PVOID)(0x0077E996 + 1), 24);
	SetByte((PVOID)(0x0077E998 + 1), 24);
	SetByte((PVOID)(0x0077E9A3 + 2), 172);
	SetByte((PVOID)(0x0077E9B3 + 2), 130);
	SetDword((PVOID)(0x0077E9D1 + 1), 51290);
	SetByte((PVOID)(0x0077E9E9 + 1), 24);
	SetByte((PVOID)(0x0077E9EB + 1), 24);
	SetByte((PVOID)(0x0077E9F6 + 1), 237);
	SetByte((PVOID)(0x0077EA05 + 2), 130);
	SetDword((PVOID)(0x0077EA23 + 1), 51290);
	SetByte((PVOID)(0x0077EA3B + 1), 24);
	SetByte((PVOID)(0x0077EA3D + 1), 24);
	SetByte((PVOID)(0x0077EA48 + 2), 292);
	SetByte((PVOID)(0x0077EA58 + 1), 130);
	SetDword((PVOID)(0x0077EA75 + 1), 51290);
	SetByte((PVOID)(0x0077EA8D + 1), 24);
	SetByte((PVOID)(0x0077EA8F + 1), 24);
	SetByte((PVOID)(0x0077EA9A + 2), 347);
	SetByte((PVOID)(0x0077EAAA + 2), 130);
	SetByte((PVOID)(0x007801D9 + 1), 4);
	SetByte((PVOID)(0x007801DD + 1), 42);
	SetByte((PVOID)(0x00780217 + 1), 2);
	SetByte((PVOID)(0x0078023A + 2), 56);
	SetByte((PVOID)(0x0078190D + 1), 4);
	SetByte((PVOID)(0x00781911 + 1), 42);
	SetByte((PVOID)(0x0078194E + 1), 2);
	SetByte((PVOID)(0x00781973 + 2), 56);
	SetByte((PVOID)(0x007830EF + 1), 4);
	SetByte((PVOID)(0x007830F3 + 1), 42);
	SetByte((PVOID)(0x00783130 + 1), 2);
	SetByte((PVOID)(0x00783155 + 2), 56);
	SetByte((PVOID)(0x00782D15 + 1), 4);
	SetByte((PVOID)(0x00782D19 + 1), 42);
	SetByte((PVOID)(0x00782D56 + 1), 2);
	SetByte((PVOID)(0x00782D7C + 2), 56);
	SetByte((PVOID)(0x00782D15 + 1), 4);
	SetByte((PVOID)(0x00782D19 + 1), 42);
	SetByte((PVOID)(0x00782D56 + 1), 2);
	SetByte((PVOID)(0x00782D7C + 2), 56);
	SetByte((PVOID)(0x00784B6D + 1), 4);
	SetByte((PVOID)(0x00784B71 + 1), 42);
	SetByte((PVOID)(0x00784BAE + 1), 2);
	SetByte((PVOID)(0x00784BD4 + 2), 56);

	//--
	//-- Windows Command
	SetCompleteHook(0xE8, 0x0078E6D6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0078E70A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0078E74A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0078E79C, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0078E7E2, &WindowsEmpty);

	//-- Windows Helper
	SetCompleteHook(0xE8, 0x007F65A6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x007F65E6, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007F6638, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007F66A2, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007F66FA, &WindowsEmpty);
	//-- Windows MuHelper2
	SetCompleteHook(0xE8, 0x0080C848, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0080C87C, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0080C8BC, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0080C90E, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0080C954, &WindowsEmpty);
	//-- Windows Desconocido1
	SetCompleteHook(0xE8, 0x00845956, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0084598A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008459C7, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00845A10, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00845A4F, &WindowsEmpty);
	//-- Windows Desconocido2
	SetCompleteHook(0xE8, 0x007C6BCC, &WindowsBack01);
	SetCompleteHook(0xE8, 0x007C6C06, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007C6C49, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007C6C98, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007C6CDD, &WindowsEmpty);
	//-- Windows Desconocido3
	SetCompleteHook(0xE8, 0x007C7D0C, &WindowsBack01);
	SetCompleteHook(0xE8, 0x007C7D46, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007C7D89, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007C7DD8, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007C7E1D, &WindowsEmpty);
	//-- Windows Desconocido4
	SetCompleteHook(0xE8, 0x00850966, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0085099A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008509D7, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00850A20, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00850A5F, &WindowsEmpty);
	//-- Windows Desconocido5
	SetCompleteHook(0xE8, 0x00852476, &WindowsBack01);
	SetCompleteHook(0xE8, 0x008524AA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008524E7, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00852530, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0085256F, &WindowsEmpty);
	//-- Windows Desconocido6
	SetCompleteHook(0xE8, 0x008540B6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x008540EA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00854127, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00854170, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008541AF, &WindowsEmpty);
	//-- Windows Desconocido7
	SetCompleteHook(0xE8, 0x00868052, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0086808C, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008680D5, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00868130, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0086817B, &WindowsEmpty);
	//-- Windows Desconocido8
	SetCompleteHook(0xE8, 0x0087F886, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0087F8BA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0087F8F7, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0087F940, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0087F97F, &WindowsEmpty);
	//-- Windows Desconocido9 NPC CASTLE SIEGE
	SetCompleteHook(0xE8, 0x00882F52, &WindowsBack01);
	SetCompleteHook(0xE8, 0x00882F8C, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00882FD5, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00883030, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0088307B, &WindowsEmpty);
	//-- Windows NPC Lugar
	SetCompleteHook(0xE8, 0x00893102, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0089313C, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00893185, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008931E0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0089322B, &WindowsEmpty);
	//-- Windows Guard NPC
	SetCompleteHook(0xE8, 0x008812A2, &WindowsBack01);
	SetCompleteHook(0xE8, 0x008812DC, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00881325, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00881380, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008813CB, &WindowsEmpty);
	//-- Windows NPC Senior
	SetCompleteHook(0xE8, 0x00879EE2, &WindowsBack01);
	SetCompleteHook(0xE8, 0x00879F1C, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00879F65, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00879FC0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0087A00B, &WindowsEmpty);
	//-- Windows BloodCastle
	SetCompleteHook(0xE8, 0x008747B6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x008747F0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0087483C, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008748A0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008748F2, &WindowsEmpty);
	//-- Windows DevilSquare
	SetCompleteHook(0xE8, 0x008781BC, &WindowsBack01);
	SetCompleteHook(0xE8, 0x008781F0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00878230, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00878282, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008782C8, &WindowsEmpty);
	//-- Windows Pet
	SetCompleteHook(0xE8, 0x0084D01A, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0084D05A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084D0AC, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084D116, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084D16E, &WindowsEmpty);
	//-- Windows Party
	SetCompleteHook(0xE8, 0x0084A546, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0084A57A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084A5BA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084A60C, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084A652, &WindowsEmpty);
	//-- Windows ChaosMachine
	SetCompleteHook(0xE8, 0x0082CA22, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0082CB4B, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0082CB00, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0082CAA5, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0082CA5C, &WindowsEmpty);
	//-- Windows Store
	SetCompleteHook(0xE8, 0x00841E49, &WindowsStore);
	SetCompleteHook(0xE8, 0x00841E7D, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00841EBA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00841F03, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00841F42, &WindowsEmpty);
	//-- Windows Store Other PJ
	SetCompleteHook(0xE8, 0x0084F406, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0084F43A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084F477, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084F4C0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084F4FF, &WindowsEmpty);
	//-- Windows Baul
	SetCompleteHook(0xE8, 0x00857946, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0085797A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008579B7, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00857A00, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00857A3F, &WindowsEmpty);
	//-- Windows Baul Ext.
	SetCompleteHook(0xE8, 0x008567E6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0085681A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00856857, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008568A0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008568DF, &WindowsEmpty);
	//-- Windows Inventory
	SetCompleteHook(0xE8, 0x00836EF6, &WindowsInventory);
	SetCompleteHook(0xE8, 0x00836F2A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00836F67, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00836FB0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00836FEF, &WindowsEmpty);
	//-- Windows Inventory Ext.
	SetCompleteHook(0xE8, 0x007D55A6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x007D55DA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007D5617, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007D5660, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007D569F, &WindowsEmpty);
	//-- Windows Shop.
	SetCompleteHook(0xE8, 0x008470A6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x008470DA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00847117, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00847160, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084719F, &WindowsEmpty);
	//-- Windows GuildMarker
	SetCompleteHook(0xE8, 0x007D1786, &WindowsBack01);
	SetCompleteHook(0xE8, 0x007D17BA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007D17F7, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007D1840, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007D187F, &WindowsEmpty);
	//-- Windows Guild User
	SetCompleteHook(0xE8, 0x007CB6F6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x007CB72A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007CB767, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007CB7B0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x007CB7EF, &WindowsEmpty);
	
	//-- Windows Gens-Quest
	SetCompleteHook(0xE8, 0x00843826, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0084385A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00843897, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008438E0, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0084391F, &WindowsEmpty);
	//-- Windows Quest (T)
	SetCompleteHook(0xE8, 0x0083EA46, &WindowsBack01);
	SetCompleteHook(0xE8, 0x0083EA7A, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0083EAB7, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0083EB00, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x0083EB3F, &WindowsEmpty);
	//-- Windows Trade
	SetCompleteHook(0xE8, 0x008643B6, &WindowsBack01);
	SetCompleteHook(0xE8, 0x008643EA, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00864427, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x00864470, &WindowsEmpty);
	SetCompleteHook(0xE8, 0x008644AF, &WindowsEmpty);
}