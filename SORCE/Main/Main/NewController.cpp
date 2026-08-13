#include "stdafx.h"
#include "NewController.h"
#include "Minimize.h"

HHOOK           m_Keyboard1 = NULL;
HHOOK           m_Keyboard2 = NULL;
HWND            Window = NULL;

LRESULT CALLBACK KeyboardProc(int Code, WPARAM wParam, LPARAM lParam)
{
	if (((lParam >> 31) & 1) && (Code == HC_ACTION))
	{

		if (GetAsyncKeyState(VK_F12))
		{
			gTrayMode.Toggle();
		}

	}

	return CallNextHookEx(m_Keyboard1, Code, wParam, lParam);
}

LRESULT CALLBACK LL_KeyboardProc(int Code, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* KeyBoard = (KBDLLHOOKSTRUCT*)lParam;

	switch (wParam)
	{
	case WM_KEYUP:
	{
		switch (KeyBoard->vkCode)
		{
		case VK_F12:
		{

			gTrayMode.Toggle();
		}
		break;
		}
	}
	break;
	default:
		return CallNextHookEx(NULL, Code, wParam, lParam);
		break;
	}

	return CallNextHookEx(m_Keyboard2, Code, wParam, lParam);
}