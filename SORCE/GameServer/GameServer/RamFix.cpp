#include "stdafx.h"
#include "Protect.h"
#include "RamFix.h"

void RamFix()
{
	HANDLE v1 = NULL;
	HANDLE v2;
	while ( 1 )
	{
		//Sleep(5000);
		Sleep(6000);
		v1 = GetCurrentProcess();
		SetProcessWorkingSetSize(v1, 0xFFFFFFFF, 0xFFFFFFFF);
		v2 = GetCurrentProcess();
		SetThreadPriority(v2, -2);
	}
}