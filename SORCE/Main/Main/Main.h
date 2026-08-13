#pragma once
#define BeginBitmap				((void(*)()) 0x637770)
#define EndOpengl				((void(*)()) 0x6366F0)
#define pDrawImage              ((void(__cdecl*)(int ImageID, float PosX, float PosY, float Width, float Height, int Arg6, float Arg7, float ScaleX, float ScaleY, bool ScaleSize, bool ScalePosition, bool ScaleAlpha)) 0x637C60)
void SelectCharS0();
void ReduceCPU();