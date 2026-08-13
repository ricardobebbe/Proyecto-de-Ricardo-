// CustomZeusMercador.h: interface for the CCustomZeusMercador class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct GOLDER_ZeusMercador_ITEM_INFO
{
	int Index;
	int Durability;
	int Option0;
	int Option1;
	int Option2;
	int Option3;
	int Option4;
	int Option5;
	int Option6;
	int Duration;
	int Rate;
};

class CCustomZeusMercador
{
public:
	CCustomZeusMercador();
	virtual ~CCustomZeusMercador();
	void Load(char* path);
	void NpcZeusMercador(LPOBJ lpNpc,LPOBJ lpObj);
private:
	int m_ItemIndex;
	int m_ItemLevel;
	int m_ItemCount;
	int m_ItemDropCount;
	int m_SendFirework;
	std::vector<GOLDER_ZeusMercador_ITEM_INFO> m_GoldenZeusMercadorItemInfo;
};

extern CCustomZeusMercador gCustomZeusMercador;