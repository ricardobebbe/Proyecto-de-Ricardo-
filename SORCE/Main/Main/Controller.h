#pragma once
// ----------------------------------------------------------------------------------------------

class Controller
{
public:
	bool	Load();
	// ----
	static	LRESULT WINAPI Mouse(int Code, WPARAM wParam, LPARAM lParam);
	static	LRESULT WINAPI Keyboard(int Code, WPARAM wParam, LPARAM lParam);
	//static	LRESULT WINAPI Window(HWND Window, DWORD Message, WPARAM wParam, LPARAM lParam);
	// ----
	HINSTANCE	Instance;
private:
	HHOOK		MouseHook;
	HHOOK		KeyboardHook;
	// ----
}; extern Controller gController;
// ----------------------------------------------------------------------------------------------
extern char Notice_1[1024]; //ZEUS UP19 PANEL BEM VINDAS CLIENT
extern char Notice_2[1024];
extern char Notice_3[1024];
extern char Notice_4[1024];
extern char Notice_5[1024];
extern char Notice_6[1024];
extern BOOL isNotice;