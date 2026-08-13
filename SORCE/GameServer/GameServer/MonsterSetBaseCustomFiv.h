// MonsterSetBase.h: interface for the CMonsterSetBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

#define MAX_MSB_MONSTER 300
#define OBJ_MAXMONSTER	(OBJ_STARTUSERINDEX-100)	//MC	//11500 //5800

struct MONSTER_SET_BASE_INFO_CUSTOM5
{
	int Type;
	int MonsterClass;
	int Map;
	int Dis;
	int X;
	int Y;
	int Dir;
	int TX;
	int TY;
	int Value;
};

class CMonsterSetBaseCustomFiv
{
public:
	CMonsterSetBaseCustomFiv();
	virtual ~CMonsterSetBaseCustomFiv();
	void Load(char* path);
	void SetInfo(MONSTER_SET_BASE_INFO_CUSTOM5 info);
	bool GetPosition(int index, short map, short* ox, short* oy);
	bool GetBoxPosition(int map, int x, int y, int tx, int ty, short* ox, short* oy);
	void SetBoxPosition(int index, int map, int x, int y, int tx, int ty);
	void SetMonsterDataCustomFiv();
	bool gObjSetPosMonster(int aIndex, int number);
public:
	MONSTER_SET_BASE_INFO_CUSTOM5 m_MonsterSetBaseInfo[MAX_MSB_MONSTER];
	MONSTER_SET_BASE_INFO_CUSTOM5 m_Mp[OBJ_MAXMONSTER];//MC
	int m_count;
};

extern CMonsterSetBaseCustomFiv gMonsterSetBaseCustomFiv;
