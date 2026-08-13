// MonsterSetBase.cpp: implementation of the CMonsterSetBaseKalimaTwo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterSetBaseKalimaDois.h"
#include "MapServerManager.h"
#include "MonsterManager.h"
#include "380ItemType.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "CustomArena.h"
#include "CustomEventDrop.h"
#include "CustomOnlineLottery.h"
#include "CustomQuiz.h"
#include "DevilSquare.h"
#include "IllusionTemple.h"
#include "InvasionManager.h"
#include "ItemManager.h"
#include "Kalima.h"
#include "EventTvT.h"
#include "Map.h"
#include "MemScript.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "MossMerchant.h"
#include "ObjectManager.h"
#include "Raklion.h"
#include "ServerInfo.h"
#include "SocketItemType.h"
#include "User.h"
#include "Util.h"
#include "ReiDoMu.h"

CMonsterSetBaseKalimaTwo gMonsterSetBaseKalimaTwo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterSetBaseKalimaTwo::CMonsterSetBaseKalimaTwo() // OK
{
	this->m_count = 0;
}

CMonsterSetBaseKalimaTwo::~CMonsterSetBaseKalimaTwo() // OK
{

}

void CMonsterSetBaseKalimaTwo::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_count = 0;

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (strcmp("end", lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				MONSTER_SET_BASE_INFO_KALIMA2 info;

				memset(&info, 0, sizeof(info));

				info.Type = section;

				info.MonsterClass = lpMemScript->GetNumber();

				info.Map = 25;

				info.Dis = lpMemScript->GetAsNumber();

				info.X = lpMemScript->GetAsNumber();

				info.Y = lpMemScript->GetAsNumber();

				if (section == 1 || section == 3)
				{
					info.TX = lpMemScript->GetAsNumber();
					info.TY = lpMemScript->GetAsNumber();
				}
				else if (section == 2)
				{
					info.X = (info.X - 3) + GetLargeRand() % 7;
					info.Y = (info.Y - 3) + GetLargeRand() % 7;
				}

				info.Dir = lpMemScript->GetAsNumber();

				if (section == 1 || section == 3)
				{
					int count = lpMemScript->GetAsNumber();

					if (section == 3)
					{
						info.Value = lpMemScript->GetAsNumber();
					}

					for (int n = 0; n < count; n++)
					{
						this->SetInfo(info);
					}
				}
				else
				{
					this->SetInfo(info);
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CMonsterSetBaseKalimaTwo::SetInfo(MONSTER_SET_BASE_INFO_KALIMA2 info) // OK
{
	if (this->m_count < 0 || this->m_count >= MAX_MSB_MONSTER)
	{
		return;
	}

	if (gMapServerManager.CheckMapServer(info.Map) == 0)
	{
		return;
	}

	info.Dir = ((info.Dir == -1) ? (GetLargeRand() % 8) : info.Dir);

	this->m_MonsterSetBaseInfo[this->m_count++] = info;
}

bool CMonsterSetBaseKalimaTwo::GetPosition(int index, short map, short* ox, short* oy) // OK
{
	if (index < 0 || index >= MAX_MSB_MONSTER)
	{
		return 0;
	}

	MONSTER_SET_BASE_INFO_KALIMA2* lpInfo = &this->m_MonsterSetBaseInfo[index];

	if (lpInfo->Type == 0 || lpInfo->Type == 4)
	{
		(*ox) = lpInfo->X;
		(*oy) = lpInfo->Y;
		return 1;
	}
	else if (lpInfo->Type == 1 || lpInfo->Type == 3)
	{
		return this->GetBoxPosition(map, lpInfo->X, lpInfo->Y, lpInfo->TX, lpInfo->TY, ox, oy);
	}
	else if (lpInfo->Type == 2)
	{
		return this->GetBoxPosition(map, (lpInfo->X - 3), (lpInfo->Y - 3), (lpInfo->X + 3), (lpInfo->Y + 3), ox, oy);
	}

	return 0;
}

bool CMonsterSetBaseKalimaTwo::GetBoxPosition(int map, int x, int y, int tx, int ty, short* ox, short* oy) // OK
{
	for (int n = 0; n < 100; n++)
	{
		int subx = tx - x;
		int suby = ty - y;

		subx = ((subx < 1) ? 1 : subx);
		suby = ((suby < 1) ? 1 : suby);

		subx = x + (GetLargeRand() % subx);
		suby = y + (GetLargeRand() % suby);

		if (gMap[map].CheckAttr(subx, suby, 1) == 0 && gMap[map].CheckAttr(subx, suby, 4) == 0 && gMap[map].CheckAttr(subx, suby, 8) == 0)
		{
			(*ox) = subx;
			(*oy) = suby;
			return 1;
		}
	}

	return 0;
}

void CMonsterSetBaseKalimaTwo::SetBoxPosition(int index, int map, int x, int y, int tx, int ty) // OK
{
	if (index < 0 || index >= MAX_MSB_MONSTER)
	{
		return;
	}

	MONSTER_SET_BASE_INFO_KALIMA2* lpInfo = &this->m_MonsterSetBaseInfo[index];

	lpInfo->Map = map;
	lpInfo->X = x;
	lpInfo->Y = y;
	lpInfo->TX = tx;
	lpInfo->TY = ty;
}


void CMonsterSetBaseKalimaTwo::SetMonsterDataKalimaTwo() // OK
{
	for (int n = 0; n < gMonsterSetBaseKalimaTwo.m_count; n++)
	{
		MONSTER_SET_BASE_INFO_KALIMA2* lpInfo = &gMonsterSetBaseKalimaTwo.m_MonsterSetBaseInfo[n];

		if (lpInfo->Type == 3 || lpInfo->Type == 4)
		{
			continue;
		}

		int index = gObjAddMonster(lpInfo->Map);

		if (OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		if (this->gObjSetPosMonster(index, n) == 0)
		{
			gObjDel(index);
			continue;
		}

		if (gObjSetMonster(index, lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}

#if(GAMESERVER_TYPE==1)

		LPOBJ lpObj = &gObj[index];

		if (lpObj->Map == MAP_CASTLE_SIEGE)
		{
			if (lpObj->Class == 216)
			{
				gCastleSiege.SetCrownIndex(index);
			}
		}

		if (lpObj->Map == MAP_CRYWOLF)
		{
			if (lpObj->Type == OBJECT_MONSTER)
			{
				if (gCrywolf.m_ObjCommonMonster.AddObj(index) == 0)
				{
					gObjDel(index);
					continue;
				}
			}

			if (lpObj->Type == OBJECT_NPC)
			{
				if (lpObj->Class >= 204 && lpObj->Class <= 209)
				{
					if (gCrywolf.m_ObjSpecialNPC.AddObj(index) == 0)
					{
						gObjDel(index);
						continue;
					}
				}
				else
				{
					if (lpObj->Class != 406 && lpObj->Class != 407)
					{
						if (gCrywolf.m_ObjCommonNPC.AddObj(index) == 0)
						{
							gObjDel(index);
							continue;
						}
					}
				}
			}
		}

#endif
	}

	gBonusManager.Init();

	gCustomArena.Init();

	gCustomEventDrop.Init();

	gCustomOnlineLottery.Init();

	gCustomQuiz.Init();

	gTvTEvent.Init();

	

	gReiDoMU.Init();

	gMossMerchant.Init();
}

bool CMonsterSetBaseKalimaTwo::gObjSetPosMonster(int aIndex, int number) // OK
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (number < 0 || number > MAX_MSB_MONSTER)
	{
		return 0;
	}

	MONSTER_SET_BASE_INFO_KALIMA2* lpInfo = &gMonsterSetBaseKalimaTwo.m_MonsterSetBaseInfo[number];

	short x, y;

	if (gMonsterSetBaseKalimaTwo.GetPosition(number, lpInfo->Map, &x, &y) == 0)
	{
		return 0;
	}

	lpObj->PosNum = number;
	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->Dir = lpInfo->Dir;
	lpObj->Map = lpInfo->Map;
	return 1;
}