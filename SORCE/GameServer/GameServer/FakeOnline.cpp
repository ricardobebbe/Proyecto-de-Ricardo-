#include "stdafx.h"
#include "FakeOnline.h"
#include "ItemManager.h"
#include "Map.h"
#include "MasterSkillTree.h"
#include "Notice.h"
#include "SkillManager.h"
#include "SocketManager.h"
#include "Viewport.h"
#include "User.h"
#include "Util.h"
#include "MemScript.h"
#include "Path.h"
#include "Party.h"
#include "EffectManager.h"
#include "MapManager.h"
#include "Message.h"
#include "OfflineMode.h"
//#include "OfflineData.h"
#include "Monster.h"
#include "DSProtocol.h"
//#include "LogToFile.h"
//#include <iomanip>
//#include <stdlib.h>
#include <list>
#include <string>
#include "JSProtocol.h"
#include "ObjectManager.h"
#include "OfflineMode.h"
#include "Move.h"
#include "ServerInfo.h"
#include "stdafx.h"
#include "Attack.h"

#if USE_FAKE_ONLINE == TRUE
//========================================================================================================================
CFakeOnline s_FakeOnline;

CFakeOnline::CFakeOnline() // OK
{

}



OFFEXP_DATA* CFakeOnline::GetOffExpInfo(LPOBJ lpObj)
{
	std::map<std::string, OFFEXP_DATA>::iterator it = this->m_Data.find(lpObj->Account);

	if (it != this->m_Data.end())
	{
		if (strcmp(lpObj->Name, it->second.Name) == 0)
		{
			return &it->second;
		}
	}

	return 0;
}

OFFEXP_DATA* CFakeOnline::GetOffExpInfoByAccount(LPOBJ lpObj)
{
	std::map<std::string, OFFEXP_DATA>::iterator it = this->m_Data.find(lpObj->Account);

	if (it != this->m_Data.end())
	{
		return &it->second;
	}

	return 0;
}


void CFakeOnline::LoadFakeData(char* path)
{
	this->m_Data.clear();

	pugi::xml_document file;    
	pugi::xml_parse_result res = file.load_file(path);
	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("File %s load fail. Error: %s", path, res.description());
		return;
	}
	pugi::xml_node oFakeOnlineData = file.child("FakeOnlineData");
	for (pugi::xml_node rInfoData = oFakeOnlineData.child("Info"); rInfoData; rInfoData = rInfoData.next_sibling()){
		OFFEXP_DATA info;

		strncpy(info.Account, rInfoData.attribute("Account").as_string(), sizeof(info.Account));
		strncpy(info.Password, rInfoData.attribute("Password").as_string(), sizeof(info.Password));
		strncpy(info.Name, rInfoData.attribute("Name").as_string(), sizeof(info.Name));
		strncpy(info.IP, rInfoData.attribute("IP").as_string(), sizeof(info.Name));
		info.SkillID = rInfoData.attribute("SkillID").as_int();
		info.UseBuffs[0] = rInfoData.attribute("UseBuffs_0").as_int();
		info.UseBuffs[1] = rInfoData.attribute("UseBuffs_1").as_int();
		info.UseBuffs[2] = rInfoData.attribute("UseBuffs_2").as_int();
		info.TuNhatItem = rInfoData.attribute("TuNhatItem").as_int();
		info.TuDongReset = rInfoData.attribute("TuDongReset").as_int();
		info.PartyMode = rInfoData.attribute("PartyMode").as_int();
		info.MapNumber = rInfoData.attribute("MapNumber").as_int();
		info.MapX = rInfoData.attribute("MapX").as_int();
		info.MapY = rInfoData.attribute("MapY").as_int();

		this->m_Data.insert(std::pair<std::string, OFFEXP_DATA>(info.Account, info));

	}
	LogAdd(LOG_BLUE, "[FakeOnline] Load Data OK");

}

//========================================================================================================================

void CFakeOnline::RestoreFakeOnline()
{
	
	for (std::map<std::string, OFFEXP_DATA>::iterator it = this->m_Data.begin(); it != this->m_Data.end(); it++)
	{
		if (gObjFindByAcc(it->second.Account) != 0)
		{
			continue;
		}

		int aIndex = gObjAddSearch(0, it->second.IP);

		if (aIndex >= 0)
		{
			gObjAdd(0, it->second.IP, aIndex);
			gObj[aIndex].LoginMessageSend++;
			gObj[aIndex].LoginMessageSend++;
			gObj[aIndex].LoginMessageCount++;
			gObj[aIndex].ConnectTickCount = GetTickCount();
			gObj[aIndex].ClientTickCount = GetTickCount();
			gObj[aIndex].ServerTickCount = GetTickCount();

			gObj[aIndex].MapServerMoveRequest = 0;
			gObj[aIndex].LastServerCode = -1;
			gObj[aIndex].DestMap = -1;
			gObj[aIndex].DestX = 0;
			gObj[aIndex].DestY = 0;

			GJConnectAccountSend(aIndex, it->second.Account, it->second.Password, it->second.IP);
			
			gObj[aIndex].Socket = INVALID_SOCKET;

			LogAdd(LOG_TEST, "[ FakeOnline ]  [Conta: %s Char: %s] Online Server", it->second.Account, it->second.Name);
		}
	}
}

void FakeAnimationMove(int aIndex, int x, int y)
{
	LPOBJ lpObj = &gObj[aIndex];

	int map_num = gObj[aIndex].Map;

	BYTE path[8];

	if (lpObj->Teleport != 0)
	{
		return;
	}

	if (gObjCheckMapTile(lpObj, 1) != 0)
	{
		return;
	}

	if ((GetTickCount() - lpObj->LastMoveTime) < 100)
	{
		return;
	}

	if (gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	if (lpObj->SkillSummonPartyTime != 0)
	{
		lpObj->SkillSummonPartyTime = 0;
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(272));
	}

	lpObj->Dir = path[0] >> 4;
	lpObj->Rest = 0;
	lpObj->PathCur = 0;
	lpObj->PathCount = path[0] & 0x0F;
	lpObj->LastMoveTime = 50;
	lpObj->LastMoveTime = GetTickCount();

	memset(lpObj->PathX, 0, sizeof(lpObj->PathX));

	memset(lpObj->PathY, 0, sizeof(lpObj->PathY));

	memset(lpObj->PathOri, 0, sizeof(lpObj->PathOri));

	lpObj->TX = x;
	lpObj->TY = y;
	lpObj->PathCur = ((lpObj->PathCount > 0) ? 1 : 0);
	lpObj->PathCount = ((lpObj->PathCount > 0) ? (lpObj->PathCount + 1) : lpObj->PathCount);
	lpObj->PathStartEnd = 1;
	lpObj->PathX[0] = x;
	lpObj->PathY[0] = y;
	lpObj->PathDir[0] = lpObj->Dir;

	for (int n = 1; n < lpObj->PathCount; n++)
	{
		if ((n % 2) == 0)
		{
			lpObj->TX = lpObj->PathX[n - 1] + RoadPathTable[((path[((n + 1) / 2)] & 0x0F) * 2) + 0];
			lpObj->TY = lpObj->PathY[n - 1] + RoadPathTable[((path[((n + 1) / 2)] & 0x0F) * 2) + 1];
			lpObj->PathX[n] = lpObj->PathX[n - 1] + RoadPathTable[((path[((n + 1) / 2)] & 0x0F) * 2) + 0];
			lpObj->PathY[n] = lpObj->PathY[n - 1] + RoadPathTable[((path[((n + 1) / 2)] & 0x0F) * 2) + 1];
			lpObj->PathOri[n - 1] = path[((n + 1) / 2)] & 0x0F;
			lpObj->PathDir[n + 0] = path[((n + 1) / 2)] & 0x0F;
		}
		else
		{
			lpObj->TX = lpObj->PathX[n - 1] + RoadPathTable[((path[((n + 1) / 2)] / 0x10) * 2) + 0];
			lpObj->TY = lpObj->PathY[n - 1] + RoadPathTable[((path[((n + 1) / 2)] / 0x10) * 2) + 1];
			lpObj->PathX[n] = lpObj->PathX[n - 1] + RoadPathTable[((path[((n + 1) / 2)] / 0x10) * 2) + 0];
			lpObj->PathY[n] = lpObj->PathY[n - 1] + RoadPathTable[((path[((n + 1) / 2)] / 0x10) * 2) + 1];
			lpObj->PathOri[n - 1] = path[((n + 1) / 2)] / 0x10;
			lpObj->PathDir[n + 0] = path[((n + 1) / 2)] / 0x10;
		}
	}

	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = lpObj->TX;
	lpObj->TY = lpObj->TY;
	lpObj->OldX = lpObj->TX;
	lpObj->OldY = lpObj->TY;
	lpObj->ViewState = 0;

	gMap[lpObj->Map].SetStandAttr(lpObj->TX, lpObj->TY);

	PMSG_MOVE_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE1, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);

	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	pMsg.x = (BYTE)lpObj->TX;

	pMsg.y = (BYTE)lpObj->TY;

	pMsg.dir = lpObj->Dir << 4;

	lpObj->PathCur = 0;
	lpObj->PathCount = 0;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	pMsg.x = (BYTE)lpObj->X;
	pMsg.y = (BYTE)lpObj->Y;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if (lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index, (BYTE*)&pMsg, pMsg.header.size);
			}
		}
	}
}
void FakeAutoRepair(int aIndex)
{
	if (!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < INVENTORY_WEAR_SIZE; ++n)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			int money = gItemManager.RepairItem(lpObj, &lpObj->Inventory[n], n, 1);

			if (money != 0)
			{
				gObjectManager.CharacterCalcAttribute(aIndex);
			}
		}
	}
}

void CFakeOnline::FakeAttackProc(LPOBJ lpObj) // OK
{
	if (lpObj->IsFakeOnline != 0)
	{
		lpObj->CheckSumTime = GetTickCount();
		lpObj->ConnectTickCount = GetTickCount();
	}
}
void CFakeOnline::OnAttackAlreadyConnected(LPOBJ lpObj) // OK
{
	OFFEXP_DATA *info = s_FakeOnline.GetOffExpInfo(lpObj);
	if (info != 0 && lpObj->Socket == INVALID_SOCKET)
	{
		lpObj->Map = info->MapNumber;
		lpObj->X = info->MapX;
		lpObj->Y = info->MapY;
	}
	if (lpObj->IsFakeOnline != 0)
	{
		lpObj->IsFakeOnline = 0;
		gObjDel(lpObj->Index);
	}
}
void CFakeOnline::Attack(int aIndex)
{
	int range = 0;

	if (OBJMAX_RANGE(aIndex) == FALSE)
	{
		return;
	}

	if (!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->IsFakeOnline == 0)
	{
		return;
	}

	OFFEXP_DATA *info = s_FakeOnline.GetOffExpInfo(lpObj);
	if (info != 0 && lpObj->Socket == INVALID_SOCKET)
	{

		if (lpObj->Map == info->MapNumber && gServerInfo.InSafeZone(lpObj->Index) == true)
		{

			FakeAnimationMove(lpObj->Index, info->MapX, info->MapY);
			LogAdd(LOG_TEST, "FakeOnline Debug Di Chuyen");

		}

		else if (lpObj->Map != info->MapNumber && gServerInfo.InSafeZone(lpObj->Index) == true)
		{
			LogAdd(LOG_TEST, "FakeOnline Debug Move Map OK");
			gObjTeleport(lpObj->Index, info->MapNumber, info->MapX, info->MapY);

		}
	}


	if (gServerInfo.InSafeZone(aIndex) == true)
	{
		return;
	}


	lpObj->MoveRange = 15; // 15
	
	g_OfflineMode.RenderAutoPote(aIndex);

	g_OfflineMode.RenderAutoBuffParty(aIndex);

	g_OfflineMode.RenderAttack(aIndex);

	FakeAutoRepair(aIndex);


}
#endif

void CFakeOnline::TuDongBuffSkill(int aIndex)	//-- OK
{
	if (!gObjIsConnectedGP(aIndex))
	{
		return;
	}
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTarget = NULL;

	if (gServerInfo.InSafeZone(aIndex) == true)
	{
		return;
	}

	//-- AUTOBUFF ONLY
	if (lpObj->BuffOn != 0)
	{
		CSkill* RenderBuff;
		for (int n = 0; n < 3; n++)
		{
			if (lpObj->BuffSkill[n] > 0)
			{
				RenderBuff = gSkillManager.GetSkill(lpObj, lpObj->BuffSkill[n]);

				if (RenderBuff != 0)
				{
					if (gEffectManager.CheckEffect(lpObj, gSkillManager.GetSkillEffect(RenderBuff->m_index)) == 0)
					{
						gSkillManager.UseAttackSkill(lpObj->Index, lpObj->Index, RenderBuff); //Buff Your self
					}
				}
			}
		}
	}

	//-- PARTY MODE ON
	if (lpObj->PartyModeOn != 0 && lpObj->PartyNumber >= 0)
	{
		//-- PARTY HEAL ELF
		if (lpObj->PartyModeHealOn != 0 && lpObj->Class == CLASS_FE)
		{
			CSkill* RenderPartyHealing;

			for (int i = 0; i < MAX_PARTY_USER; i++)
			{

				if (OBJECT_RANGE(gParty.m_PartyInfo[lpObj->PartyNumber].Index[i]) != 0 && gObjCalcDistance(lpObj, &gObj[gParty.m_PartyInfo[lpObj->PartyNumber].Index[i]]) < MAX_PARTY_DISTANCE)
				{
					RenderPartyHealing = gSkillManager.GetSkill(lpObj, SKILL_HEAL);

					if (RenderPartyHealing != 0)
					{
						lpTarget = &gObj[gParty.m_PartyInfo[lpObj->PartyNumber].Index[i]];

						if (lpTarget->Index == lpObj->Index)
						{
							continue;
						}
						if (lpTarget->Life < ((lpTarget->MaxLife * lpObj->PartyModeHealPercent) / 100))
						{
							if (gEffectManager.CheckEffect(lpTarget, gSkillManager.GetSkillEffect(RenderPartyHealing->m_index)) == 0)
							{
								gSkillManager.UseAttackSkill(lpObj->Index, lpTarget->Index, RenderPartyHealing);
							}
						}
					}
				}
			}
		}
		//-- PARTY BUFF
		if (lpObj->PartyModeBuffOn != 0 && lpObj->PartyNumber >= 0)
		{
			CSkill* RenderPartyBuff;

			for (int i = 0; i < MAX_PARTY_USER; i++)
			{
				if (OBJECT_RANGE(gParty.m_PartyInfo[lpObj->PartyNumber].Index[i]) != 0 && gObjCalcDistance(lpObj, &gObj[gParty.m_PartyInfo[lpObj->PartyNumber].Index[i]]) < MAX_PARTY_DISTANCE)
				{
					for (int n = 0; n < 3; n++)
					{
						if (lpObj->BuffSkill[n] > 0)
						{
							RenderPartyBuff = gSkillManager.GetSkill(lpObj, lpObj->BuffSkill[n]);

							if (RenderPartyBuff != 0)
							{
								lpTarget = &gObj[gParty.m_PartyInfo[lpObj->PartyNumber].Index[i]];

								if (gEffectManager.CheckEffect(lpTarget, gSkillManager.GetSkillEffect(RenderPartyBuff->m_index)) == 0)
								{
									gSkillManager.UseAttackSkill(lpObj->Index, gParty.m_PartyInfo[lpObj->PartyNumber].Index[i], RenderPartyBuff);
								}
							}
						}
					}
				}
			}
		}
	}
}

void CFakeOnline::GuiYCParty(int aIndex, int bIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (gObjIsConnectedGP(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
	{
		return;
	}

	//-- PatyLevel
	if (gServerInfo.m_PartyRestrict == 1 && gParty.IsParty(lpTarget->PartyNumber) == 0)
	{
		if (gObj[aIndex].PartyNumber >= 0)
		{
			if (gParty.GetLevel(gObj[aIndex].PartyNumber) == TRUE)
			{
				int limmaxlevel = 0;
				int limmaxlevel2 = 0;

				if (gParty.maxlevel > gObj[bIndex].Level)
				{
					limmaxlevel = gParty.maxlevel;
					limmaxlevel2 = gObj[bIndex].Level;
				}
				else
				{
					limmaxlevel = gObj[bIndex].Level;
					limmaxlevel2 = gParty.maxlevel;
				}

				if ((limmaxlevel - limmaxlevel2) > gServerInfo.m_DifferenceMaxLevelParty)
				{
					gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 2, 0, gMessage.GetMessage(861), gServerInfo.m_DifferenceMaxLevelParty);
					return;
				}

				int limminlevel = 0;
				int limminlevel2 = 0;

				if (gParty.minlevel > gObj[bIndex].Level)
				{
					limminlevel = gParty.minlevel;
					limminlevel2 = gObj[bIndex].Level;
				}
				else
				{
					limminlevel = gObj[bIndex].Level;
					limminlevel2 = gParty.minlevel;
				}
				if ((limminlevel - limminlevel2) > gServerInfo.m_DifferenceMaxLevelParty)
				{
					gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 2, 0, gMessage.GetMessage(861), gServerInfo.m_DifferenceMaxLevelParty);
					return;
				}
			}
		}
		else
		{
			short sMaxMinLevel[2];
			if (gObj[aIndex].Level > gObj[bIndex].Level)
			{
				sMaxMinLevel[1] = gObj[aIndex].Level;
				sMaxMinLevel[0] = gObj[bIndex].Level;
			}
			else
			{
				sMaxMinLevel[1] = gObj[bIndex].Level;
				sMaxMinLevel[0] = gObj[aIndex].Level;
			}
			if ((sMaxMinLevel[1] - sMaxMinLevel[0]) >gServerInfo.m_DifferenceMaxLevelParty)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 2, 0, gMessage.GetMessage(861), gServerInfo.m_DifferenceMaxLevelParty);
				return;
			}
		}
	}
	//--FinPartyLevel
	if (CA_MAP_RANGE(lpTarget->Map) != 0 || CC_MAP_RANGE(lpTarget->Map) != 0 || IT_MAP_RANGE(lpTarget->Map) != 0 || DA_MAP_RANGE(lpTarget->Map) != 0 || DG_MAP_RANGE(lpTarget->Map) != 0 || IG_MAP_RANGE(lpTarget->Map) != 0)
	{
		gParty.GCPartyResultSend(aIndex, 0);
		return;
	}

	if (OBJECT_RANGE(lpObj->PartyTargetUser) != 0 || OBJECT_RANGE(lpTarget->PartyTargetUser) != 0)
	{
		gParty.GCPartyResultSend(aIndex, 0);
		return;
	}

	if (gServerInfo.m_GensSystemPartyLock != 0 && lpObj->GensFamily != lpTarget->GensFamily)
	{
		gParty.GCPartyResultSend(aIndex, 6);
		return;
	}

	if (gParty.AutoAcceptPartyRequest(lpObj, lpTarget) != 0)
	{
		return;
	}

	if (gParty.IsParty(lpObj->PartyNumber) != 0 && gParty.IsLeader(lpObj->PartyNumber, aIndex) == 0)
	{
		gParty.GCPartyResultSend(aIndex, 0);
		return;
	}

	if ((lpTarget->Option & 1) == 0)
	{
		gParty.GCPartyResultSend(aIndex, 1);
		return;
	}

	if (gParty.IsParty(lpTarget->PartyNumber) != 0)
	{
		gParty.GCPartyResultSend(aIndex, 4);
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_PARTY;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = GetTickCount();
	lpObj->TargetNumber = bIndex;
	lpObj->PartyTargetUser = bIndex;

	lpTarget->Interface.use = 1;
	lpTarget->Interface.type = INTERFACE_PARTY;
	lpTarget->Interface.state = 0;
	lpTarget->InterfaceTime = GetTickCount();
	lpTarget->TargetNumber = aIndex;

	PMSG_PARTY_REQUEST_SEND pMsg;

	pMsg.header.set(0x40, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	DataSend(bIndex, (BYTE*)&pMsg, pMsg.header.size);
}

