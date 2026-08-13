// stdafx.cpp : source file that includes just the standard includes
// GameServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <stdio.h>

// Parche definitivo para fprintf, fscanf y __iob_func en Visual Studio moderno
#if _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")

extern "C" FILE* __cdecl __iob_func(void)
{
    static FILE iobs;
    iobs = *stdin;
    iobs = *stdout;
    iobs = *stderr;
    return &iobs;
}
#endif

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
