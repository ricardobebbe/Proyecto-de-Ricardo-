#pragma once

LRESULT CALLBACK KeyboardProc(int Code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LL_KeyboardProc(int Code, WPARAM wParam, LPARAM lParam);

extern HWND         Window;
extern HHOOK        m_Keyboard1;
extern HHOOK        m_Keyboard2;