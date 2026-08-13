#include "stdafx.h"
#include "CChatClassic.h"
#include "TMemory.h"
#include "Util.h"
#include "Offset.h"
#include "Object.h"
#include "Import.h"
#include "ChatExpanded.h"
#include "WindowsStruct.h"

// ---------------------------------------------------------------------------------------------
CChatClassic gCChatClassic;
// ---------------------------------------------------------------------------------------------
int Open = 0;

void CChatClassic::RenderFrame(int This)
{
	pSetBlend(true);
	glColor4f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0, (float)1.0);
	pDrawBarForm(182.0, 407.5, 268.0, 20.0, 0.0f, 0);
	pGLSwitchBlend();
	glColor3f(1.0, 1.0, 1.0);
	pGLSwitch();
	pSetBlend(false);
	RenderBitmap(51530, 180.0, 407.5, 143.0, 29.0, 0, 0, 0.51953125, 0.90625, 1, 1, 0.0);
	RenderBitmap(51531, 322.0, 407.5, 142.0, 29.0, 0, 0, 0.51953125, 0.90625, 1, 1, 0.0);
	RenderBitmap(51532, 442.0, 412.5, 15.0, 16.0, 0, 0, 0.9375, 1.0, 1, 1, 0.0);
}

__declspec(naked) void ChatPosicion()
{
	static DWORD ChatPosicion_buff = 0x0085A44D;
	static DWORD CALL01  = 0x00785870;
	_asm
	{	
	PUSH 384 //<<Y
	PUSH 186 //<<X
	MOV EDX,DWORD PTR SS:[EBP-0x270]
	MOV EAX,DWORD PTR DS:[EDX+0x10]
	PUSH EAX
	MOV ECX,DWORD PTR SS:[EBP-0x270]
	MOV EDX,DWORD PTR DS:[ECX]
	PUSH EDX
	MOV EAX,DWORD PTR SS:[EBP-0x270]
	MOV ECX,DWORD PTR DS:[EAX+0xC]
	CALL CALL01
	jmp [ChatPosicion_buff]
	}
}

__declspec(naked) void RenderWhisperTextPosicion()
{
	static DWORD WhisperTextPosicion_buff = 0x00785A6D;
	static DWORD CALL01  = 0x00417EC0;
	_asm
	{	
	ADD EDX,32//<<Y
	PUSH EDX
	MOV EAX,DWORD PTR SS:[EBP-0x20]
	MOV ECX,DWORD PTR DS:[EAX+0x14]
	ADD ECX,200//<<X
	PUSH ECX
	MOV EDX,DWORD PTR SS:[EBP-0x20]
	MOV ECX,DWORD PTR DS:[EDX+0x28]
	CALL CALL01
	jmp [WhisperTextPosicion_buff]
	}
}

__declspec(naked) void ChatTextPosicion()
{
	static DWORD ChatTextPosicion_buff = 0x00785972;
	static DWORD CALL01  = 0x00417EC0;
	_asm
	{
		ADD EDX,32
		PUSH EDX                                 ; /Arg2
		MOV EAX,DWORD PTR SS:[EBP - 0x20]        ; |
		MOV ECX,DWORD PTR DS:[EAX + 0x14]        ; |
		ADD ECX,5                                ; |
		PUSH ECX                                 ; |Arg1
		MOV EDX,DWORD PTR SS:[EBP - 0x20]        ; |
		MOV ECX,DWORD PTR DS:[EDX + 0x24]        ; |
		CALL CALL01                              ; \main1.00417EC0
		jmp [ChatTextPosicion_buff]
	}
}

static int chatX = 0;

//----- (007889B0) --------------------------------------------------------
char ChatWindowsDrawClasic(MUChat* This)
{
	This->WindowPosX = 0;
	chatX = 0;

	if(gChatExpanded.m_separate == 0)
	{
		if(This->WindowHeight == 55)
		{
			This->WindowPosY = 95;
		}

		if(This->WindowHeight == 100)
		{
			This->WindowPosY = 140;
		}

		if(This->WindowHeight == 145)
		{
			This->WindowPosY = 185;
		}

		if(This->WindowHeight == 190)
		{
			This->WindowPosY = 235;
		}

		if(This->WindowHeight == 235)
		{
			This->WindowPosY = 280;
		}
	}
	else
	{
		This->WindowPosY = 378;
	}
	//cuando se activa se pasa al centro
	if(This->WindowsActivo)
	{
		chatX = 4; 
		This->WindowPosX = 181;
		This->WindowPosY = 407;
	}

	sub_7889B0(This);

	return 1;
}

//----- (00789110) --------------------------------------------------------
char RenderFrameChatClassic(MUChat* This)
{
	float v4; // ST30_4@5
	float v5; // ST2C_4@5
	float v6; // ST28_4@5
	float v7; // ST20_4@7
	float v8; // ST20_4@7
	float v9; // ST20_4@7
	float green; // ST08_4@8
	int i; // [sp+38h] [bp-Ch]@5
	float v16; // [sp+3Ch] [bp-8h]@2
	float v17; // [sp+40h] [bp-4h]@2
	static int A = 0;
	static int B = 0;
	static int C = 0;
	static int D = 0;
	static int E = 0;
	static int F = 0;

	if(pCursorX > 260 && pCursorX < 375 && pCursorY > 400 && pCursorY < 446)
	{
		if(gInterface.CheckWindow(ChatWindow))
		{

		}
		else
		{
			if (pCursorX > 296 && pCursorX < 312 && pCursorY > 413 && pCursorY < 429)
			{
				gInterface.DrawToolTip(273.0f, 400.0f, pGetTextLine(pTextLineThis, 122));
			
				if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
				{
					A = 1;
					B = 1;
					keybd_event(VK_F2, 0, 0, 0);
				}
				else
				{
					A = 0;
					B = 0;
					keybd_event(VK_F2, 0, KEYEVENTF_KEYUP, 0);
				}
			}
			else
			{
				A = 0;
				B = 0;
			}
			if (pCursorX > 313 && pCursorX < 329 && pCursorY > 413 && pCursorY < 429)
			{
				gInterface.DrawToolTip(289.0f, 400.0f, pGetTextLine(pTextLineThis, 123));

				if(This->WindowsActivo == 1)
				{
					if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
					{
						C = 1;
						D = 1;
						keybd_event(VK_F4, 0, 0, 0);
					}
					else
					{
						C = 0;
						D = 0;
						keybd_event(VK_F4, 0, KEYEVENTF_KEYUP, 0);
					}
				}
				else
				{
					if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
					{
						C = 1;
						D = 1;
						This->WindowsActivo = 1;
							if(gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(Store)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(OtherStore)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(Character)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(GoldenArcher1)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(GoldenArcher2)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(LuckyCoin1)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(LuckyCoin2)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(ExpandInventory)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(Warehouse)
							|| gInterface.CheckWindow(Character) && gInterface.CheckWindow(PetInfo)
							|| gInterface.CheckWindow(Character) && gInterface.CheckWindow(Quest)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(NPC_ChaosMix)
							|| gInterface.CheckWindow(Inventory) && gInterface.CheckWindow(ChaosBox)
							|| gInterface.CheckWindow(Shop)
							|| gInterface.CheckWindow(Trade))
							{
								This->WindowPosX = 0;
								This->WindowPosY = 405;
							}
							else
							{
								This->WindowPosX = 179;
								This->WindowPosY = 405;
							}
					}
					else
					{
						C = 0;
						D = 0;
					}
				}

			}
			else
			{
				C = 0;
				D = 0;
			}
			if (pCursorX > 330 && pCursorX < 346 && pCursorY > 413 && pCursorY < 429)
			{
				gInterface.DrawToolTip(308.0f, 400.0f, pGetTextLine(pTextLineThis, 124));

				if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
				{
					if(Open == 0)
					{
						E = 1;
						F = 1;
						Open = 1;
						This->AlphaColor = (1.0, 1.0, 1.0, 0.7);
						keybd_event(VK_LBUTTON, 0, KEYEVENTF_KEYUP, 0);
					}
					else if(Open == 1)
					{
						E = 1;
						F = 1;
						Open = 2;
						This->AlphaColor = (1.0, 1.0, 1.0, 0.5);
						keybd_event(VK_LBUTTON, 0, KEYEVENTF_KEYUP, 0);
					}
					else if(Open == 2)
					{
						E = 1;
						F = 1;
						Open = 3;
						This->AlphaColor = (1.0, 1.0, 1.0, 0.3);
						keybd_event(VK_LBUTTON, 0, KEYEVENTF_KEYUP, 0);
					}
					else if(Open == 3)
					{
						E = 1;
						F = 1;
						Open = 4;
						This->AlphaColor = (1.0, 1.0, 1.0, 0.0);
						keybd_event(VK_LBUTTON, 0, KEYEVENTF_KEYUP, 0);
					}
					else if(Open == 4)
					{
						E = 1;
						F = 1;
						Open = 0;
						This->AlphaColor = (1.0, 1.0, 1.0, 1.0);
						keybd_event(VK_LBUTTON, 0, KEYEVENTF_KEYUP, 0);
					}
				}
				else
				{
					E = 0;
					F = 0;
				}
			}
			else
			{
				E = 0;
				F = 0;
			}

			gInterface.DrawGUI(eOrelhinha, 296.0f + A, 413.0f + B);
			gInterface.DrawGUI(eJanelinha, 313.0f + C, 413.0f + D);
			gInterface.DrawGUI(eLuizinha,  330.0f + E, 413.0f + F);
			pSetCursorFocus = true;
		}
	}
	else
	{
		pSetCursorFocus = false;
	}

	if(This->WindowsActivo){

		if(This->Unknown340 == 4)
		{
			glColor4f((GLfloat)0.69999999, (GLfloat)0.69999999, (GLfloat)0.69999999, 1.0);
		}
		else
		{
			glColor4f(1.0, 1.0, 1.0, 1.0);
		}
		v17 = (double)This->WindowPosX;
		v16 = (double)(This->WindowPosY - This->WindowHeight);
		v9 = This->WindowPosX + This->WindowWidth - 5;
		v4 = v16 - 5.0;
		pSetBlend(true);
		
		pDrawGUI(31275, v17, v4, 281.0f, 5.0f);

		pDrawGUI(51552, v9 - 24, v4 - 4, 22.0f, 12.0f);

		pGLSwitchBlend();
		pGLSwitch();
		glColor3f(1.0, 1.0, 1.0);
		pSetBlend(false);


		pSetBlend(true);
		glColor4f(1.0, 1.0, 1.0, 1.0);

		for ( int i = 0; i < This->WindowLineCount; ++i )
		{
			v8 = v16 - 3 + (double)(40 * i + 3);

			pDrawGUI(51551, v17, v8, 5.0, 40.0f);

			pDrawGUI(51551, v9, v8, 5.0, 40.0f);
		}

		for ( int i = 0; i < This->WindowLineCount; ++i )
		{
			v7 = v16 + 3 + (double)(15 * i + 3);
			pDrawGUI(51554, v9 - 20, v7, 15.0f, 15.0f);
		}

		pDrawGUI(51553, v9 - 20, v16 + 5, 15.0f, 14.0f);
		green = (double)(This->WindowPosY - 20);
		//-- scroll flecha abajo
		pDrawGUI(51555, v9 - 20, green + 3, 15.0f, 14.0f);

		pGLSwitchBlend();
		pGLSwitch();
		glColor3f(1.0, 1.0, 1.0);
		pSetBlend(false);
	}
	return 1;
}

_declspec(naked) void RenderWisperBarSizeMove()
{
	static float D2B828 = 17.00000;
	static float D4383C = 62.00000;
	static DWORD WhisperTxt_buf2f = 0x00787744;

	_asm
	{
		FLD DWORD PTR DS:[D2B828] 
		FSTP DWORD PTR SS:[ESP]
		PUSH ECX
		FLD DWORD PTR DS:[D4383C] 
		FSTP DWORD PTR SS:[ESP]
		MOV ECX,DWORD PTR SS:[EBP-0x24]
		MOV EDX,DWORD PTR DS:[ECX+0x18]
		ADD EDX,29
		MOV DWORD PTR SS:[EBP-0x28],EDX
		FILD DWORD PTR SS:[EBP-0x28]
		PUSH ECX
		FSTP DWORD PTR SS:[ESP]
		MOV EAX,DWORD PTR SS:[EBP-0x24]
		MOV ECX,DWORD PTR DS:[EAX+0x14]
		ADD ECX,195
		MOV DWORD PTR SS:[EBP-0x2C],ECX
		FILD DWORD PTR SS:[EBP-0x2C]
		JMP [WhisperTxt_buf2f]
	}
}

__declspec(naked) void WarpMessageSystem()
{
	static DWORD ChangeMove = 4;
	static DWORD Buffer_warp = 0x007890ED;
	static DWORD CALL_MAIN1 = 0x0078C030;
	static DWORD CALL_MAIN2 = 0x00402880;
	static DWORD CALL_MAIN3 = 0x0041FE10;

	ChangeMove = chatX;

	if(gChatExpanded.m_separate == 0)
	{
		_asm
		{
			PUSH 0
			PUSH 1
			PUSH 0
			PUSH 0
			MOV ECX,DWORD PTR SS:[EBP-0x2C]
			CALL CALL_MAIN1
			MOV ECX,EAX
			CALL CALL_MAIN2
			PUSH EAX
			MOV ECX,DWORD PTR SS:[EBP-0x34]
			PUSH ECX
			MOV EDX,DWORD PTR SS:[EBP-0x38]
			ADD EDX,ChangeMove
			PUSH EDX
			CALL CALL_MAIN3
			MOV ECX,EAX                             ; |
			JMP [Buffer_warp]
		}
	}
	else
	{
			_asm
		{
			PUSH 0
			PUSH 1
			PUSH 0
			PUSH 0
			MOV ECX,DWORD PTR SS:[EBP-0x2C]
			CALL CALL_MAIN1
			MOV ECX,EAX
			CALL CALL_MAIN2
			PUSH EAX
			MOV ECX,DWORD PTR SS:[EBP-0x34]
			PUSH ECX
			MOV EDX,DWORD PTR SS:[EBP-0x38]
			MOV EDX,0x5
			PUSH EDX
			CALL CALL_MAIN3
			MOV ECX,EAX                             ; |
			JMP [Buffer_warp]
		}
	}
}

__declspec(naked) void WarpMessageChat()
{
	static DWORD ChangeMove = 4;
	static DWORD Buffer_warp = 0x007890A4;
	static DWORD CALL_MAIN1 = 0x00402880;
	static DWORD CALL_MAIN2 = 0x0041FE10;

	ChangeMove = chatX;

	_asm
	{
		PUSH 0
		PUSH 1
		PUSH 0
		PUSH 0
		LEA ECX,DWORD PTR SS:[EBP-0x54]
		CALL CALL_MAIN1
		PUSH EAX
		MOV EDX,DWORD PTR SS:[EBP-0x34]
		PUSH EDX
		MOV EAX,DWORD PTR SS:[EBP-0x38]
		ADD EAX,ChangeMove
		PUSH EAX
		CALL CALL_MAIN2
		MOV ECX,EAX                             ; |
		JMP [Buffer_warp]
	}
}

bool CheckFixAllButonChat(int x, int y, int Width, int Height)
{
	return pCheckMouseOver(x, 480, Width, Height);
}

//-- (00787BA0) //-- ChatInput   //--ZEUS UP25 Interface  FIXO SHOP MOUSE
double sub_787BA00()
{
	return 2.199999809;
}

//-- (0078B0F0) ChatDialog     //--ZEUS UP25 Interface  FIXO SHOP MOUSE
double sub_78B0F00()
{
	return 2.0999999;
}

//-- (00812120) Interfaz Principal  //--ZEUS UP25 Interface  FIXO SHOP MOUSE
double sub_8121200()
{
	return 2.6;
}

//----- (007D2E50) MuHELPER    //--ZEUS UP25 Interface  FIXO SHOP MOUSE
double sub_7D2E500()
{
	return 0.3000002;
}

//----- (007E0E90) Pet     //--ZEUS UP25 Interface  FIXO SHOP MOUSE
char sub_7E0E900()
{
	return 1.0;
}

//----- (0084C870) Party   //--ZEUS UP25 Interface  FIXO SHOP MOUSE
double sub_84C8700()
{
	return 5.4000001;
}

//----- (00857DC0) Baul //--ZEUS UP25 Interface  FIXO SHOP MOUSE
double sub_857DC00()
{
	return 3.6;
}

int GetFrameChatt(int This)  //--ZEUS UP25 Interface  FIXO SHOP MOUSE
{
  return *(DWORD *)(This + 16);
}

void CChatClassic::Load()
{
	//-- DrawBarForm F5 in character
	SetCompleteHook(0xE9,0x007867A5, 0x007867B3); //F5 independente

	MemorySet(0x00787C7D,0x90,0x5);         //F5 sempre ativo
	SetDword((PVOID)(0x00788A2B+2),314);	//DIMENSIONAMENTO DA TELA PRETA DOWNGRADE (PARA O CHAT)
	SetCompleteHook(0xE8, 0x007864B3, &CheckFixAllButonChat); //-- Chat On
	SetCompleteHook(0xE8, 0x0078641E, &CheckFixAllButonChat); //-- Chat On
	SetCompleteHook(0xE8, 0x007863A8, &CheckFixAllButonChat); //-- System
	SetCompleteHook(0xE8, 0x00786347, &CheckFixAllButonChat); //-- Wisper
	SetCompleteHook(0xE8, 0x007862E2, &CheckFixAllButonChat); //-- Basico
	SetCompleteHook(0xE8, 0x0078628E, &CheckFixAllButonChat); //-- ToolTip
	SetCompleteHook(0xE9,0x00789084,&WarpMessageChat);
	SetCompleteHook(0xE9,0x007890C6,&WarpMessageSystem);
	//Local call from 00785B44
	MemorySet(0x00785B44, 0x90, 0x5); //-- buttom alpha
	MemorySet(0x00787628, 0x90, 0x5); //-- Buttom Frame 007877B0
	MemorySet(0x00787630, 0x90, 0x5); //-- tooltip 00787960
	//-- DrawBarForm F5 in character
	SetCompleteHook(0xE8,0x0078B09A,&ChatWindowsDrawClasic);
	//-- Render FrameWindows Texture Chat
	SetCompleteHook(0xE8, 0x0078B0CF, &RenderFrameChatClassic);
	//-- Posicion Chat
	SetOp((LPVOID)0x0085A425, (LPVOID)ChatPosicion, ASM::JMP);
	SetOp((LPVOID)0x00785959, (LPVOID)ChatTextPosicion, ASM::JMP);
	
	//SetByte((PVOID)(0x00785965), 6);			// POSIÇÃO X DO TEXTO NO CHAT
	//SetByte((PVOID)(0x00785959+2), 9);			// POSIÇÃO Y DO TEXTO NO CHAT
	//-- Posicion WISPER
	SetCompleteHook(0xE9,0x0078770E, &RenderWisperBarSizeMove);
	SetOp((LPVOID)0x00785A54, (LPVOID)RenderWhisperTextPosicion, ASM::JMP);
	//00787760 chat
	SetCompleteHook(0xE8, 0x00787620,this->RenderFrame);

	SetCompleteHook(0xE9, 0x00857DC0, &sub_857DC00); //-- Interface  FIXO SHOP MOUSE
	
	SetCompleteHook(0xE9, 0x007D2E50, &sub_7D2E500); //-- Interface MUHELPER //-- Interface  FIXO SHOP MOUSE
	SetCompleteHook(0xE9, 0x00812120, &sub_8121200); //-- Interface Principal //-- Interface  FIXO SHOP MOUSE
	SetCompleteHook(0xE9, 0x0078B0F0, &sub_78B0F00); //-- ChatDialog //-- Interface  FIXO SHOP MOUSE
	SetCompleteHook(0xE9, 0x00787BA0, &sub_787BA00); //-- ChatInput //-- Interface  FIXO SHOP MOUSE
}