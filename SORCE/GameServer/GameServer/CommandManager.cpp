// CommandManager.cpp: implementation of the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "BonusManager.h"
#include "CashShop.h"
#include "CustomAttack.h"
#include "CustomBuyVip.h"
#include "CustomStore.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "GameMaster.h"
//#include "GiftSystem.h"
#include "Guild.h"
#include "GuildClass.h"
#include "ItemManager.h"
#include "JSProtocol.h"
#include "Log.h"
//#include "MarriageSystem.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"
#include "Message.h"
#include "Monster.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Protocol.h"
#include "Quest.h"
#include "QuestReward.h"
//#include "ResetReward.h"
#include "ResetTable.h"
#include "ServerInfo.h"
#include "Util.h"
#include "LuaFunction.h"
#include "ScriptLoader.h"
#include "RewardSystem.h"
#include "CustomRankUser.h"
#include "ReiDoMU.h"
#include "Filter.h"
#include "EventPvP.h"
#include "CustomPick.h"
#include "ItemManager.h"
#include "JewelMix.h"
#include "Warehouse.h"
#include "CustomQuiz.h"
#include "EventRunAndCatch.h"
#include "EventRussianRoulette.h"
#include "EventKillAll.h"
#include "EventHideAndSeek.h"
#include "EventQuickly.h"
#include "EventStart.h"

CCommandManager gCommandManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandManager::CCommandManager() // OK
{

}

CCommandManager::~CCommandManager() // OK
{

}

void CCommandManager::Load(char* path) // OK
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("File %s load fail. Error: %s", path, res.description());
		return;
	}

	this->m_CommandListInfo.clear();

	pugi::xml_node CommandManager = file.child("CommandManager");

	for (pugi::xml_node Command = CommandManager.child("Command"); Command; Command = Command.next_sibling())
	{
		COMMAND_LIST_INFO info;

		info.Index = Command.attribute("Index").as_int();
		info.Delay = Command.attribute("Delay").as_int();
		info.GameMaster = Command.attribute("GameMaster").as_int();
		strcpy_s(info.Command, Command.attribute("Syntax").as_string());
		strcpy_s(info.Description, Command.attribute("Description").as_string());

		for (pugi::xml_node Setting = Command.child("Setting"); Setting; Setting = Setting.next_sibling())
		{
			int index = Setting.attribute("AccountLevel").as_int();

			info.Enable[index] = Setting.attribute("Enable").as_int();
			info.MinLevel[index] = Setting.attribute("MinLevel").as_int();
			info.MaxLevel[index] = Setting.attribute("MaxLevel").as_int();
			info.MinReset[index] = Setting.attribute("MinReset").as_int();
			info.MaxReset[index] = Setting.attribute("MaxReset").as_int();
			info.Money[index] = Setting.attribute("Money").as_int();
			info.WCoinC[index] = Setting.attribute("WCoinC").as_int();
			info.WCoinP[index] = Setting.attribute("WCoinP").as_int();
			info.GoblinPoint[index] = Setting.attribute("GoblinPoint").as_int();
		}

		this->m_CommandListInfo.insert(std::pair<int, COMMAND_LIST_INFO>(info.Index, info));
	}
}

void CCommandManager::MainProc() // OK
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		if (gObj[n].AutoAddPointCount > 0)
		{
			this->CommandAddPointAutoProc(&gObj[n]);
		}

		if (gObj[n].AutoResetEnable != 0)
		{
			this->CommandResetAutoProc(&gObj[n]);
		}
	}
}

long CCommandManager::GetNumber(char* arg, int pos) // OK
{
	int count = 0, p = 0;

	char buffer[60] = { 0 };

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for (int n = 0; n < len; n++)
	{
		if (arg[n] == 0x20)
		{
			count++;
		}
		else if (count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	return atoi(buffer);
}

void CCommandManager::GetString(char* arg, char* out, int size, int pos) // OK
{
	int count = 0, p = 0;

	char buffer[60] = { 0 };

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for (int n = 0; n < len; n++)
	{
		if (arg[n] == 0x20)
		{
			count++;
		}
		else if (count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	memcpy(out, buffer, (size - 1));
}

bool CCommandManager::GetCommandCode(int Index,COMMAND_LIST_INFO* lpInfo) //ZEUS UP24  LUA 
{
	std::map<int,COMMAND_LIST_INFO>::iterator it = this->m_CommandListInfo.find(Index);

	if(it != this->m_CommandListInfo.end())
	{
		(*lpInfo) = it->second;
		return 1;
	}

	return 0;
}

bool CCommandManager::GetCommandCode(char* label, COMMAND_LIST_INFO* lpInfo) // OK
{
	for (std::map<int, COMMAND_LIST_INFO>::iterator it = this->m_CommandListInfo.begin(); it != this->m_CommandListInfo.end(); it++)
	{
		if (strcmp(label, it->second.Command) == 0)
		{
			(*lpInfo) = it->second;
			return 1;
		}
	}

	return 0;
}

bool CCommandManager::ManagementCore(LPOBJ lpObj, char* message) // OK
{
	if(lpObj->Interface.type == INTERFACE_TRADE)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Você não pode usar comandos no trade");
		return 0;
	}

	char command[32] = { 0 };

	memset(command, 0, sizeof(command));

	this->GetString(message, command, sizeof(command), 0);

	char* argument = &message[strlen(command)];

	if (argument[0] == 0x20)
	{
		argument++;
	}

	COMMAND_LIST_INFO lpInfo;

	if (this->GetCommandCode(command, &lpInfo) == 0)
	{
		return 0;
	}

	if (lpInfo.Enable[lpObj->AccountLevel] == 0)
	{
		return 0;
	}

	if (argument[0] == 0x3F)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, lpInfo.Description);
		return 0;
	}

	if (lpObj->Level < lpInfo.MinLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(65),lpInfo.MinLevel[lpObj->AccountLevel]);
		return 0;
	}

	if (lpInfo.MaxLevel[lpObj->AccountLevel] != -1 && lpInfo.MaxLevel[lpObj->AccountLevel] > lpObj->Level)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(765),lpInfo.MaxLevel[lpObj->AccountLevel], lpInfo.Command);
		return 0;
	}

	if (lpObj->Reset < lpInfo.MinReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(66),lpInfo.MinReset[lpObj->AccountLevel], lpInfo.Command);
		return 0;
	}

	if (lpInfo.MaxReset[lpObj->AccountLevel] != -1 && lpInfo.MaxReset[lpObj->AccountLevel] > lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(232),lpInfo.MaxReset[lpObj->AccountLevel], lpInfo.Command);
		return 0;
	}

	DWORD tick = (GetTickCount() - lpObj->CommandCheckTime[lpInfo.Index]) / 1000;

	if (tick < lpInfo.Delay)
	{
		
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(68),(lpInfo.Delay-tick));
		return 0;
	}


	if (lpInfo.GameMaster != -1 && lpInfo.GameMaster > gGameMaster.GetGameMasterLevel(lpObj))
	{
		
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(64), lpInfo.Command);

		return 0;
	}

	if (lpInfo.Money[lpObj->AccountLevel] > lpObj->Money)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(67),lpInfo.Money[lpObj->AccountLevel]);
		return 0;
	}

	/*if (lpObj->WCoinC < lpInfo.WCoinC[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(MSG_COMMAND_MANAGER, 7), lpInfo.WCoinC[lpObj->AccountLevel], lpInfo.Command);
		return 0;
	}*/

	/*if (lpObj->WCoinP < lpInfo.WCoinP[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(MSG_COMMAND_MANAGER, 8), lpInfo.WCoinP[lpObj->AccountLevel], lpInfo.Command);
		return 0;
	}*/

	/*if (lpObj->GoblinPoint < lpInfo.GoblinPoint[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(MSG_COMMAND_MANAGER, 9), lpInfo.GoblinPoint[lpObj->AccountLevel], lpInfo.Command);
		return 0;
	}*/

	if(gScriptLoader.OnCommandManager(lpObj->Index,lpInfo.Index,argument) != 0)    //ZEUS LUA UP24
	{
		this->DiscountRequirement(lpObj,lpInfo.Index);
		return 0;
	}

	bool Result = 0;

	switch (lpInfo.Index)
	{
	case COMMAND_MOVE:
		this->CommandMove(lpObj, argument);
		break;
	case COMMAND_GUILD_WAR:
		this->CommandGuildWar(lpObj, argument);
		break;
	case COMMAND_BATTLE_SOCCER:
		this->CommandBattleSoccer(lpObj, argument);
		break;
	case COMMAND_POST:
		Result = this->CommandPost(lpObj, argument);
		break;
	case COMMAND_DISABLEPVP:
		Result = CommandDisablePvp(lpObj,argument);
		break;
	case COMMAND_ADD_POINT1:
		Result = this->CommandAddPoint(lpObj, argument, 0);
		break;
	case COMMAND_ADD_POINT2:
		Result = this->CommandAddPoint(lpObj, argument, 1);
		break;
	case COMMAND_ADD_POINT3:
		Result = this->CommandAddPoint(lpObj, argument, 2);
		break;
	case COMMAND_ADD_POINT4:
		Result = this->CommandAddPoint(lpObj, argument, 3);
		break;
	case COMMAND_ADD_POINT5:
		Result = this->CommandAddPoint(lpObj, argument, 4);
		break;
	case COMMAND_PK_CLEAR:
		Result = CommandPKClear(lpObj,argument);
		break;
	case COMMAND_MONEY:
		Result = this->CommandMoney(lpObj, argument);
		break;
	case COMMAND_CHANGE:
		Result = CommandChange(lpObj,argument);
		break;
	case COMMAND_WARE:
		Result = this->CommandWare(lpObj, argument);
		break;
	case COMMAND_RESET:
		this->CommandReset(lpObj, argument);
		break;
	case COMMAND_MASTER_RESET:
		Result = CommandMasterReset(lpObj,argument);
		break;
	case COMMAND_CUSTOM_STORE:
		Result = gCustomStore.CommandCustomStore(lpObj, argument);
		break;
	case COMMAND_CUSTOM_STORE_OFFLINE:
		Result = gCustomStore.CommandCustomStoreOffline(lpObj, argument);
		break;
	case COMMAND_CLEAR_INVENTORY:
		Result = this->CommandClearInventory(lpObj);
		break;
	case COMMAND_REDISTRIBUTE:
		Result = this->CommandRedistribute(lpObj);
		break;
	case COMMAND_LOCK:
		Result = CommandLock(lpObj,argument);
		break;
	case COMMAND_UNLOCK:
		Result = CommandUnLock(lpObj,argument);
		break;
	case COMMAND_PACK:
		Result = gJewelMix.CommandJewelMix(lpObj,argument);
		break;
	case COMMAND_UNPACK:
		Result = gJewelMix.CommandJewelUnMix(lpObj,argument);
		break;
	case COMMAND_MARRY:
         Result = CommandMarry(lpObj,argument);
         break;
	case COMMAND_OPENWAREHOUSE:
         Result = CommandOpenWarehouse(lpObj);
         break;
	case COMMAND_QUIZ:
		 gCustomQuiz.CommandQuiz(lpObj,argument);
		 Result = 0;
		 break;
	case COMMAND_EVENTSTARTJOIN:
		gEventStart.CommandEventStartJoin(lpObj,argument);
		Result = 0;
		break;
	case COMMAND_EVENTGET:
		 gEventRunAndCatch.CommandEventRunAndCatch(lpObj,argument);
		 Result = 0;
		 break;
	case COMMAND_EVENTGETJOIN:
		{
		gEventRunAndCatch.CommandEventRunAndCatchJoin(lpObj,argument);
		gEventRussianRoulette.CommandEventRussianRouletteJoin(lpObj,argument);
		gEventKillAll.CommandEventKillAllJoin(lpObj,argument);
		Result = 0;
		}
		break;
		// -----------------------------------------------------------------------------------
		// COMMANDS FOR GAME MASTERS
		// -----------------------------------------------------------------------------------
	case COMMAND_GM_MOVE:
		Result = this->CommandGMMove(lpObj, argument);
		break;
	case COMMAND_GM_POST:
		Result = this->CommandGMPost(lpObj, argument);
		break;
	case COMMAND_TRACK:
		Result = this->CommandTrack(lpObj, argument);
		break;
	case COMMAND_TRACE:
		Result = this->CommandTrace(lpObj, argument);
		break;
	case COMMAND_DISCONNECT:
		Result = this->CommandDisconnect(lpObj, argument);
		break;
	case COMMAND_FIREWORKS:
		Result = this->CommandFireworks(lpObj);
		break;
	case COMMAND_MAKE:
		Result = this->CommandMake(lpObj, argument);
		break;
	case COMMAND_DROP:
		Result = this->CommandDrop(lpObj, argument);
		break;
	case COMMAND_SKIN:
		Result = this->CommandSkin(lpObj, argument);
		break;
	case COMMAND_SET_MONEY:
		Result = this->CommandSetMoney(lpObj, argument);
		break;
	case COMMAND_EVENTROULETTE:
		gEventRussianRoulette.CommandEventRussianRoulette(lpObj,argument);
	    Result = 0;
	    break;
    case COMMAND_EVENTESC:
		gEventHideAndSeek.CommandEventHideAndSeek(lpObj,argument);
		Result = 0;
		break;
	case COMMAND_KILLALL:
		 Result = gEventKillAll.CommandEventKillAll(lpObj,argument);
		 break;
	case COMMAND_EVENTQUICKLY:
		gEventQuickly.CommandEventQuickly(lpObj,argument);
		Result = 0;
		break;
	case COMMAND_PVP:
		Result = gEventPvP.CommandEventPvP(lpObj,argument);
		break;
	case COMMAND_CUSTOM_ATTACK:
		Result = gCustomAttack.CommandCustomAttack(lpObj,argument);
		break;
	case COMMAND_CUSTOM_OFFATTACK:
		Result = gCustomAttack.CommandCustomAttackOffline(lpObj,argument);
		break;
	case COMMAND_PICK:
		Result = gCustomPick.CommandPick(lpObj,argument);
		break;
	case COMMAND_REI:
		gReiDoMU.CommandReiDoMU(lpObj,argument);
		Result = 0;
		break;
	case COMMAND_MOVE_ALL:
		Result = this->CommandMoveAll(lpObj, argument);
		break;
	case COMMAND_MOVE_GUILD:
		Result = this->CommandMoveGuild(lpObj, argument);
		break;
	case COMMAND_MAKE_MOB:
		Result = this->CommandMakeMob(lpObj, argument);
		break;
	default:
		return 0;
	}

	if (Result == 1)
	{
		if (lpInfo.Money[lpObj->AccountLevel] > 0)
		{
			lpObj->Money -= lpInfo.Money[lpObj->AccountLevel];

			GCMoneySend(lpObj->Index, lpObj->Money);
		}

		if (lpInfo.WCoinC[lpObj->AccountLevel] > 0 || lpInfo.WCoinP[lpObj->AccountLevel] > 0 || lpInfo.GoblinPoint[lpObj->AccountLevel] > 0)
		{
		//	gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, lpInfo.WCoinC[lpObj->AccountLevel], lpInfo.WCoinP[lpObj->AccountLevel], lpInfo.GoblinPoint[lpObj->AccountLevel]);
		}

		lpObj->CommandCheckTime[lpInfo.Index] = GetTickCount();
	}

	return 1;
}

void CCommandManager::DiscountRequirement(LPOBJ lpObj,int index) //ZEUS LUA UP24
{
	COMMAND_LIST_INFO lpInfo;

	if(this->GetCommandCode(index,&lpInfo) == 0)
	
	{
		return;
	}

	if(lpInfo.Money[lpObj->AccountLevel] > 0)
	{
		lpObj->Money -= lpInfo.Money[lpObj->AccountLevel];

		GCMoneySend(lpObj->Index,lpObj->Money);
	}

	/*if(lpInfo.Coin1[lpObj->AccountLevel] > 0 || lpInfo.Coin2[lpObj->AccountLevel] > 0 || lpInfo.Coin3[lpObj->AccountLevel] > 0)
	{
		gObjCoinSub(lpObj->Index,lpInfo.Coin1[lpObj->AccountLevel],lpInfo.Coin2[lpObj->AccountLevel],lpInfo.Coin3[lpObj->AccountLevel]);
	}*/

	lpObj->CommandCheckTime[lpInfo.Index] = GetTickCount();

	gScriptLoader.OnCommandDone(lpObj->Index,index);
}

void CCommandManager::CommandMove(LPOBJ lpObj, char* arg) // OK
{
	if(lpObj->Permission[12] != 0)  //ZEUS UP24 LUA
	{
		return;
	}

	MOVE_INFO MoveInfo;

	if (gMove.GetInfoByName(arg, &MoveInfo) != 0)
	{
		gMove.Move(lpObj, MoveInfo.Index);

	this->DiscountRequirement(lpObj,COMMAND_MOVE);

	}
}

void CCommandManager::CommandGuildWar(LPOBJ lpObj, char* arg) // OK
{
	char GuildName[9] = { 0 };

	this->GetString(arg, GuildName, sizeof(GuildName), 0);

	if (strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName, lpObj->Index, 0);

		this->DiscountRequirement(lpObj,COMMAND_GUILD_WAR);
	}
}

void CCommandManager::CommandBattleSoccer(LPOBJ lpObj, char* arg) // OK
{
	char GuildName[9] = { 0 };

	this->GetString(arg, GuildName, sizeof(GuildName), 0);

	if (strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName, lpObj->Index, 1);

		this->DiscountRequirement(lpObj,COMMAND_BATTLE_SOCCER);
	}
}

bool CCommandManager::CommandPost(LPOBJ lpObj,char* arg) // OK
{

	gFilter.CheckSyntax(arg);

	if(lpObj->AccountLevel > 0)
	{
		if(gServerInfo.m_CommandPostTypeVip == 0)
		{
			PostMessage1(lpObj->Name,gMessage.GetMessage(880),arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 1)
		{
			PostMessage2(lpObj->Name,gMessage.GetMessage(880),arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 2)
		{
			PostMessage3(lpObj->Name,gMessage.GetMessage(880),arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 3)
		{
			PostMessage4(lpObj->Name,gMessage.GetMessage(880),arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 4)
		{
			PostMessage5(lpObj->Name,gMessage.GetMessage(880),arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 5)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),5,lpObj->Name,arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 6)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),6,lpObj->Name,arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 7)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),7,lpObj->Name,arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 8)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),8,lpObj->Name,arg);
		}
		else if(gServerInfo.m_CommandPostTypeVip == 9)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),9,lpObj->Name,arg);
		}
	}
	else
	{
		if(gServerInfo.m_CommandPostType == 0)
		{
			PostMessage1(lpObj->Name,gMessage.GetMessage(69),arg);
		}
		else if(gServerInfo.m_CommandPostType == 1)
		{
			PostMessage2(lpObj->Name,gMessage.GetMessage(69),arg);
		}
		else if(gServerInfo.m_CommandPostType == 2)
		{
			PostMessage3(lpObj->Name,gMessage.GetMessage(69),arg);
		}
		else if(gServerInfo.m_CommandPostType == 3)
		{
			PostMessage4(lpObj->Name,gMessage.GetMessage(69),arg);
		}
		else if(gServerInfo.m_CommandPostType == 4)
		{
			PostMessage5(lpObj->Name,gMessage.GetMessage(69),arg);
		}
		else if(gServerInfo.m_CommandPostType == 5)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),0,lpObj->Name,arg);
		}
		else if(gServerInfo.m_CommandPostType == 6)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),1,lpObj->Name,arg);
		}
		else if(gServerInfo.m_CommandPostType == 7)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),2,lpObj->Name,arg);
		}
		else if(gServerInfo.m_CommandPostType == 8)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),3,lpObj->Name,arg);
		}
		else if(gServerInfo.m_CommandPostType == 9)
		{
			GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),4,lpObj->Name,arg);
		}
	}
	gLog.Output(LOG_COMMAND,"[CommandPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);

	this->DiscountRequirement(lpObj,COMMAND_POST);

	return 1;
}

bool CCommandManager::CommandLock(LPOBJ lpObj,char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 7)
	if(lpObj->Lock > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(775));
		return 0;
	}

	int password = this->GetNumber(arg,0);

	if(password <= 0)
	{
		return 0;
	}

	if(password > 99999999)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(774));
		return 0;
	}

	lpObj->Lock = password;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(773));

	gLog.Output(LOG_COMMAND,"[CommandLock][%s][%s] - Password: %d",lpObj->Account,lpObj->Name, password);

	GJAccountLockSaveSend(lpObj->Index,password);

	return 1;

#endif
	return 0;
}

bool CCommandManager::CommandUnLock(LPOBJ lpObj,char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 7)
	if(lpObj->Lock <= 0)
	{
		return 0;
	}

	int password = this->GetNumber(arg,0);

	if(password <= 0)
	{
		return 0;
	}

	if(password > 99999999)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(774));
		return 0;
	}

	if(password != lpObj->Lock)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(777));
		return 0;
	}

	lpObj->Lock = 0;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(776));

	gLog.Output(LOG_COMMAND,"[CommandUnLock][%s][%s] - Password: %d",lpObj->Account,lpObj->Name, password);

	GJAccountLockSaveSend(lpObj->Index,0);

	return 1;

#endif
	return 0;
}

bool CCommandManager::CommandAddPoint(LPOBJ lpObj,char* arg,int type) // OK
{

	if(lpObj->Class != CLASS_DL && type == 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(71));
		return 0;
	}

	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandAddPointAuto(lpObj,arg,type);
		return 1;
	}

	int amount = this->GetNumber(arg,0);

	if(amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(72));
		return 0;
	}

	if(gObjectManager.CharacterLevelUpPointAdd(lpObj,type,amount) == 0)
	{
		return 0;
	}

	GCNewCharacterInfoSend(lpObj);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(74),amount,lpObj->LevelUpPoint);

	gLog.Output(LOG_COMMAND,"[CommandAddPoint][%s][%s] - (Type: %d, Amount: %d)",lpObj->Account,lpObj->Name,type,amount);

	return 1;
}

bool CCommandManager::CommandAddPointAuto(LPOBJ lpObj, char* arg, int type) // OK
{
	int amount = this->GetNumber(arg, 1);

	if (amount > 0)
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = amount;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(MSG_COMMAND_MANAGER/*, 17*/), amount);

		gLog.Output(LOG_COMMAND, "[CommandAddPointAuto][%s][%s] - (Type: %d, Amount: %d)", lpObj->Account, lpObj->Name, type, amount);

		return 1;
	}
	else
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = 0;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(MSG_COMMAND_MANAGER/*, 18*/));

		this->DiscountRequirement(lpObj,(COMMAND_ADD_POINT1+type));

		return 0;
	}
}

bool CCommandManager::CommandPKClear(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->PKLevel <= 3)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(76));

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(76));
		}
		return 0;
	}

	lpObj->PKLevel = 3;

	lpObj->PKCount = 0;
	
	GCPKLevelSend(lpObj->Index,lpObj->PKLevel);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(78));

	//if (Npc >= 0)
	{
		//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(78));
	}

	gLog.Output(LOG_COMMAND,"[CommandPKClear][%s][%s] - (PKLevel: %d)",lpObj->Account,lpObj->Name,lpObj->PKLevel);

	return 1;
}

bool CCommandManager::CommandMoney(LPOBJ lpObj, char* arg) // OK
{
	int money = this->GetNumber(arg, 0);

	if (money <= 0)
	{
		lpObj->Money = 0;
	}
	else if (gObjCheckMaxMoney(lpObj->Index, money) == 0)
	{
		lpObj->Money = MAX_MONEY;
	}
	else
	{
		lpObj->Money += money;
	}

	GCMoneySend(lpObj->Index, lpObj->Money);

	gLog.Output(LOG_COMMAND, "[CommandMoney][%s][%s] - (Money: %d)", lpObj->Account, lpObj->Name, money);

	this->DiscountRequirement(lpObj,COMMAND_MONEY);

	return 1;
}

bool CCommandManager::CommandChange(LPOBJ lpObj,char* arg) // OK
{
	int ChangeUp = lpObj->ChangeUp;

	if(lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL || lpObj->Class == CLASS_RF)
	{
		ChangeUp++;
	}

	if(ChangeUp >= gServerInfo.m_CommandChangeLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(83));

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(83));
		}
		return 0;
	}

	for(int n=0;n <= ((ChangeUp==0)?3:6);n++)
	{
		if(gQuest.CheckQuestListState(lpObj,n,QUEST_FINISH) == 0)
		{
			if(n != 3 || lpObj->Class == CLASS_DK)
			{
				gQuest.AddQuestList(lpObj,n,QUEST_ACCEPT);
				gQuestReward.InsertQuestReward(lpObj,n);
				gQuest.AddQuestList(lpObj,n,QUEST_FINISH);
			}
		}
	}

	lpObj->SendQuestInfo = 0;

	gQuest.GCQuestInfoSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(84));

	//if (Npc >= 0)
	{
		//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(84));
	}

	gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);

	gLog.Output(LOG_COMMAND,"[CommandChange][%s][%s] - (ChangeUp: %d)",lpObj->Account,lpObj->Name,ChangeUp);

	return 1;
}


bool CCommandManager::CommandWare(LPOBJ lpObj,char* arg) // OK
{

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->LoadWarehouse != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(86));
		return 0;
	}

	int number = this->GetNumber(arg,0);

	if(number < 0 || number >= gServerInfo.m_CommandWareNumber[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(87));
		return 0;
	}

	lpObj->WarehouseMoney = 0;

	memset(lpObj->WarehouseMap,0xFF,WAREHOUSE_SIZE);

	for(int n=0;n < WAREHOUSE_SIZE;n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->LoadWarehouse = 0;

	lpObj->WarehouseNumber = number;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(88),number);

	gLog.Output(LOG_COMMAND,"[CommandWare][%s][%s] - (Number: %d)",lpObj->Account,lpObj->Name,number);

	this->DiscountRequirement(lpObj,COMMAND_WARE);

	return 1;
}

void CCommandManager::CommandReset(LPOBJ lpObj, char* arg) // OK
{
	char mode[5] = { 0 };

	this->GetString(arg, mode, sizeof(mode), 0);

	if (strcmp(mode, "auto") == 0)
	{
		this->CommandResetAuto(lpObj, arg);
		return;
	}

	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(90));
		return;
	}

	if (((gMasterSkillTree.CheckMasterLevel(lpObj) == 0) ? lpObj->Level : (lpObj->Level + lpObj->MasterLevel)) < gResetTable.GetResetLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(91), gResetTable.GetResetLevel(lpObj));
		return;
	}

	if (lpObj->Money < gResetTable.GetResetMoney(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(92), gResetTable.GetResetMoney(lpObj));
		return;
	}

	
	if (lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(93), gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);
		return;
	}

	if (gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(94));
				return;
			}
		}
	}

	if (((lpObj->CommandManagerTransaction[0] == 0) ? (lpObj->CommandManagerTransaction[0]++) : lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F, 0x00, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCommandManager::CommandResetAuto(LPOBJ lpObj,char* arg) //Reset Auto
{
	if(gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(89));

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(89));
		}
		return;
	}

	if(lpObj->AutoResetEnable == 0)
	{
		lpObj->AutoResetEnable =  ( gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0 ) ? 0 : 1;

		lpObj->AutoResetStats[0] = this->GetNumber(arg,1);

		lpObj->AutoResetStats[1] = this->GetNumber(arg,2);

		lpObj->AutoResetStats[2] = this->GetNumber(arg,3);

		lpObj->AutoResetStats[3] = this->GetNumber(arg,4);

		lpObj->AutoResetStats[4] = ((lpObj->Class==CLASS_DL)?this->GetNumber(arg,5):0);

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(116));

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(116));
		}
	}
	else
	{
		lpObj->AutoResetEnable = 0;

		lpObj->AutoResetStats[0] = 0;

		lpObj->AutoResetStats[1] = 0;

		lpObj->AutoResetStats[2] = 0;

		lpObj->AutoResetStats[3] = 0;

		lpObj->AutoResetStats[4] = 0;

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(117));

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(117));
		}
	}
}


bool CCommandManager::CommandMasterReset(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(119));

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(119));
		}
		return 0;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(120),gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel]);

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(120),gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel]);
		}
		return 0;
	}

	if(lpObj->Reset < gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(121),gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]);
		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(121),gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]);
		}
		return 0;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(122),gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]);

		//if (Npc >= 0)
		{
		//	GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(122),gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]);
		}
		return 0;
	}

	if(lpObj->MasterReset >= gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(123),gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel]);

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(123),gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel]);
		}
		return 0;
	}

	if(gServerInfo.m_CommandMasterResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(124));

				//if (Npc >= 0)
				{
					//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(124));
				}
				return 0;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return 0;
	}

	SDHP_COMMAND_MASTER_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x01,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

	return 0;
}

bool CCommandManager::CommandClearInventory(LPOBJ lpObj) // OK
{
	if(lpObj->Lock > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,2,0,gMessage.GetMessage(778));
		return 0;
	}

	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		gItemManager.InventoryDelItem(lpObj->Index, i);
		gItemManager.GCItemDeleteSend(lpObj->Index, i, 1);
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(513));

	gLog.Output(LOG_COMMAND, "[CommandClearInventory][%s][%s] - Inventory cleaned", lpObj->Account, lpObj->Name);

	this->DiscountRequirement(lpObj,COMMAND_CLEAR_INVENTORY);

	return 1;
}

bool CCommandManager::CommandRedistribute(LPOBJ lpObj) // OK
{
	int Point = 0;

	int str = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 0);
	int agi = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 1);
	int vit = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 2);
	int ene = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 3);
	int cmd = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class, 4);

	Point += (lpObj->Strength - str);
	lpObj->Strength = str;

	Point += (lpObj->Dexterity - agi);
	lpObj->Dexterity = agi;

	Point += (lpObj->Vitality - vit);
	lpObj->Vitality = vit;

	Point += (lpObj->Energy - ene);
	lpObj->Energy = ene;

	if (lpObj->Class == CLASS_DL)
	{
		Point += (lpObj->Leadership - cmd);
		lpObj->Leadership = cmd;
	}

	lpObj->LevelUpPoint += Point;

	GCNewCharacterInfoSend(lpObj);
	GDCharacterInfoSaveSend(lpObj->Index);
	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(737));

	gLog.Output(LOG_COMMAND, "[CommandRedistribute][%s][%s] - Points restored.", lpObj->Account, lpObj->Name);

	this->DiscountRequirement(lpObj,COMMAND_REDISTRIBUTE);

	return 1;
}


bool CCommandManager::CommandGMMove(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	int map = this->GetNumber(arg, 1);

	int x = this->GetNumber(arg, 2);

	int y = this->GetNumber(arg, 3);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return 0;
	}

	gObjTeleport(lpTarget->Index, map, x, y);

	gLog.Output(LOG_COMMAND, "[CommandGMMove][%s][%s] - (Name: %s, Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, name, map, x, y);

	this->DiscountRequirement(lpObj,COMMAND_GM_MOVE);

	return 1;
}

bool CCommandManager::CommandGMPost(LPOBJ lpObj, char* arg) // OK
{
	gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[%s] %s", lpObj->Name, arg);

	gLog.Output(LOG_COMMAND, "[CommandGMPost][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);

	this->DiscountRequirement(lpObj,COMMAND_GM_POST);

	return 1;
}

bool CCommandManager::CommandTrack(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return 0;
	}

	gObjTeleport(lpObj->Index, lpTarget->Map, lpTarget->X, lpTarget->Y);

	gLog.Output(LOG_COMMAND, "[CommandTrack][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);

	this->DiscountRequirement(lpObj,COMMAND_TRACK);

	return 1;
}

bool CCommandManager::CommandTrace(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return 0;
	}

	gObjTeleport(lpTarget->Index, lpObj->Map, lpObj->X, lpObj->Y);

	gLog.Output(LOG_COMMAND, "[CommandTrace][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);

	this->DiscountRequirement(lpObj,COMMAND_TRACE);

	return 1;
}

bool CCommandManager::CommandDisconnect(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return 0;
	}

	gObjUserKill(lpTarget->Index);

	gLog.Output(LOG_COMMAND, "[CommandDisconnect][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);

	this->DiscountRequirement(lpObj,COMMAND_DISCONNECT);

	return 1;
}

bool CCommandManager::CommandFireworks(LPOBJ lpObj) // OK
{
	for (int n = 0; n < 15; n++)
	{
		GCServerCommandSend(lpObj->Index, 0, (lpObj->X + (((GetLargeRand() % 5) * 2) - 4)), (lpObj->Y + (((GetLargeRand() % 5) * 2) - 4)));
	}

	gLog.Output(LOG_COMMAND, "[CommandFireworks][%s][%s] - (Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, lpObj->Map, lpObj->X, lpObj->Y);

	this->DiscountRequirement(lpObj,COMMAND_FIREWORKS);

	return 1;
}


bool CCommandManager::CommandMake(LPOBJ lpObj, char* arg) // OK
{
	int count = this->GetNumber(arg, 0);

	if (count < 0)
	{
		int section = this->GetNumber(arg, 1);
		int type = this->GetNumber(arg, 2);
		int level = this->GetNumber(arg, 3);
		int skill = this->GetNumber(arg, 4);
		int luck = this->GetNumber(arg, 5);
		int option = this->GetNumber(arg, 6);
		int exc = this->GetNumber(arg, 7);
		int set = this->GetNumber(arg, 8);
		int socket = this->GetNumber(arg, 9);

		if (gItemManager.IsValidItem(GET_ITEM(section, type)) == 0)
		{
			return 0;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

		count -= count * 2;

		for (int n = 0; n < count; n++)
		{
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc, set, 0, 0, ItemSocketOption, 0xFF, 0);
		}

		gLog.Output(LOG_COMMAND, "[CommandMake][%s][%s] - Count: %d (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d, Socket: %d)", lpObj->Account, lpObj->Name, count, section, type, level, skill, luck, option, exc, set, socket);
	}
	else
	{
		int section = count;
		int type = this->GetNumber(arg, 1);
		int level = this->GetNumber(arg, 2);
		int skill = this->GetNumber(arg, 3);
		int luck = this->GetNumber(arg, 4);
		int option = this->GetNumber(arg, 5);
		int exc = this->GetNumber(arg, 6);
		int set = this->GetNumber(arg, 7);
		int socket = this->GetNumber(arg, 8);

		if (gItemManager.IsValidItem(GET_ITEM(section, type)) == 0)
		{
			return 0;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc, set, 0, 0, ItemSocketOption, 0xFF, 0);

		gLog.Output(LOG_COMMAND, "[CommandMake][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d, Socket: %d)", lpObj->Account, lpObj->Name, section, type, level, skill, luck, option, exc, set, socket);
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(107));

	this->DiscountRequirement(lpObj,COMMAND_MAKE);

	return 1;
}

bool CCommandManager::CommandDrop(LPOBJ lpObj, char* arg) // OK
{
	int count = this->GetNumber(arg, 0);

	if (count < 0)
	{
		int section = this->GetNumber(arg, 1);
		int type = this->GetNumber(arg, 2);
		int level = this->GetNumber(arg, 3);
		int skill = this->GetNumber(arg, 4);
		int luck = this->GetNumber(arg, 5);
		int option = this->GetNumber(arg, 6);
		int exc = this->GetNumber(arg, 7);
		int set = this->GetNumber(arg, 8);
		int socket = this->GetNumber(arg, 9);

		if (gItemManager.IsValidItem(GET_ITEM(section, type)) == 0)
		{
			return 0;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

		count -= count * 2;

		int px = lpObj->X;
		int py = lpObj->Y;

		for (int n = 0; n < count; n++)
		{
			if (gObjGetRandomFreeLocation(lpObj->Map, &px, &py, 2, 2, 10) == 0)
			{
				px = lpObj->X;
				py = lpObj->Y;
			}

			GDCreateItemSend(lpObj->Index, lpObj->Map, px, py, GET_ITEM(section, type), level, 0, skill, luck, option, lpObj->Index, exc, set, 0, 0, ItemSocketOption, 0xFF, 0);
		}

		gLog.Output(LOG_COMMAND, "[CommandDrop][%s][%s] - Count: %d (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d, Socket: %d)", lpObj->Account, lpObj->Name, count, section, type, level, skill, luck, option, exc, set, socket);
	}
	else
	{
		int section = count;
		int type = this->GetNumber(arg, 1);
		int level = this->GetNumber(arg, 2);
		int skill = this->GetNumber(arg, 3);
		int luck = this->GetNumber(arg, 4);
		int option = this->GetNumber(arg, 5);
		int exc = this->GetNumber(arg, 6);
		int set = this->GetNumber(arg, 7);
		int socket = this->GetNumber(arg, 8);

		if (gItemManager.IsValidItem(GET_ITEM(section, type)) == 0)
		{
			return 0;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for (int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

		GDCreateItemSend(lpObj->Index, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, GET_ITEM(section, type), level, 0, skill, luck, option, lpObj->Index, exc, set, 0, 0, ItemSocketOption, 0xFF, 0);

		gLog.Output(LOG_COMMAND, "[CommandDrop][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d, Socket: %d)", lpObj->Account, lpObj->Name, section, type, level, skill, luck, option, exc, set, socket);
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(107));

	this->DiscountRequirement(lpObj,COMMAND_DROP);

	return 1;
}

bool CCommandManager::CommandSkin(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return 0;
	}

	lpTarget->Change = this->GetNumber(arg, 1);

	gObjViewportListProtocolCreate(lpTarget);

	gObjectManager.CharacterUpdateMapEffect(lpTarget);

	gLog.Output(LOG_COMMAND, "[CommandSkin][%s][%s] - (Name: %s, Change: %d)", lpObj->Account, lpObj->Name, name, lpTarget->Change);

	this->DiscountRequirement(lpObj,COMMAND_SKIN);

	return 1;
}

bool CCommandManager::CommandSetMoney(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return 0;
	}

	int money = this->GetNumber(arg, 1);

	if (money <= 0)
	{
		lpTarget->Money = 0;
	}
	else if (gObjCheckMaxMoney(lpTarget->Index, money) == 0)
	{
		lpTarget->Money = MAX_MONEY;
	}
	else
	{
		lpTarget->Money += money;
	}

	GCMoneySend(lpTarget->Index, lpTarget->Money);

	gLog.Output(LOG_COMMAND, "[CommandSetMoney][%s][%s] - (Name: %s, Money: %d)", lpObj->Account, lpObj->Name, name, money);

	this->DiscountRequirement(lpObj,COMMAND_SET_MONEY);

	return 1;
}


bool CCommandManager::CommandMoveAll(LPOBJ lpObj, char* arg) // OK
{
	int map = this->GetNumber(arg, 0);
	int x = this->GetNumber(arg, 1);
	int y = this->GetNumber(arg, 2);

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			gObjTeleport(n, map, x, y);
		}
	}

	gLog.Output(LOG_COMMAND, "[CommandMoveAll][%s][%s] (Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, map, x, y);

	this->DiscountRequirement(lpObj,COMMAND_MOVE_ALL);

	return 1;
}

bool CCommandManager::CommandMoveGuild(LPOBJ lpObj, char* arg) // OK
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);
	int map = this->GetNumber(arg, 1);
	int x = this->GetNumber(arg, 2);
	int y = this->GetNumber(arg, 3);

	GUILD_INFO_STRUCT * lpNode = gGuildClass.SearchGuild(name);

	if (lpNode == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(798));
		return 0;
	}

	for (int n = 0; n < MAX_GUILD_USER; n++)
	{
		int iGuildMemberIndex = lpNode->Index[n];

		if (lpNode->Use[n] > 0 && iGuildMemberIndex != -1)
		{
			LPOBJ lpObj = &gObj[iGuildMemberIndex];

			if (lpObj == 0)
			{
				continue;
			}

			if (gObjIsConnectedGP(iGuildMemberIndex) != 0)
			{
				gObjTeleport(iGuildMemberIndex, map, x, y);
			}
		}
	}

	gLog.Output(LOG_COMMAND, "[CommandMoveGuild][%s][%s] (Guild: %s Map: %d, X: %d, Y: %d)", lpNode->Name, lpObj->Account, lpObj->Name, map, x, y);

	this->DiscountRequirement(lpObj,COMMAND_MOVE_GUILD);

	return 1;
}

bool CCommandManager::CommandMakeMob(LPOBJ lpObj, char* arg) // OK
{
	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(380));
		return 0;
	}

	int index = gObjAddMonster(lpObj->Map);

	int monster = (this->GetNumber(arg, 0) < 0) ? 0 : this->GetNumber(arg, 0);

	if (OBJECT_RANGE(index) == 0)
	{
		
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(381));

		return 0;
	}

	LPOBJ lpMonster = &gObj[index];

	int px = lpObj->X;
	int py = lpObj->Y;

	lpMonster->PosNum = -1;
	lpMonster->X = px;
	lpMonster->Y = py;
	lpMonster->TX = px;
	lpMonster->TY = py;
	lpMonster->OldX = px;
	lpMonster->OldY = py;
	lpMonster->StartX = px;
	lpMonster->StartY = py;
	lpMonster->Dir = GetLargeRand() % 8;
	lpMonster->Map = lpObj->Map;

	if (gObjSetMonster(index, monster) == 0)
	{
		
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(381));
		gObjDel(index);
		return 0;
	}

	if (lpMonster->Type == OBJECT_NPC)
	{
		
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(382));
		gObjDel(index);
		return 0;
	}

	gLog.Output(LOG_COMMAND, "[CommandMakeMob][%s][%s] (Mob: %d Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, monster,lpObj->Map, lpObj->X, lpObj->Y);

	this->DiscountRequirement(lpObj,COMMAND_MAKE_MOB);

	return 1;
}

void CCommandManager::CommandAddPointAutoProc(LPOBJ lpObj) // OK
{
	if (lpObj->LevelUpPoint == 0 || lpObj->AutoAddPointCount == 0)
	{
		return;
	}

	int AddStatCount = lpObj->AutoAddPointCount;

	int* stat[5] = { &lpObj->Strength,&lpObj->Dexterity,&lpObj->Vitality,&lpObj->Energy,&lpObj->Leadership };

	for (int n = 0; n < 5; n++)
	{
		if (lpObj->AutoAddPointStats[n] > 0)
		{
			if (AddStatCount == 0)
			{
				break;
			}

			int AddStat = lpObj->LevelUpPoint / AddStatCount;

			AddStat = (((lpObj->LevelUpPoint%AddStatCount)>0) ? (AddStat + 1) : AddStat);

			AddStat = ((AddStat>lpObj->AutoAddPointStats[n]) ? lpObj->AutoAddPointStats[n] : AddStat);

//			AddStat = ((((*stat[n]) + AddStat)>gServerInfo.m_MaxStatPoint[lpObj->Class][lpObj->AccountLevel]) ? (gServerInfo.m_MaxStatPoint[lpObj->Class][lpObj->AccountLevel] - (*stat[n])) : AddStat);

			AddStatCount--;

			(*stat[n]) += AddStat;

			lpObj->LevelUpPoint -= AddStat;

	//		lpObj->AutoAddPointStats[n] -= (((*stat[n]) >= gServerInfo.m_MaxStatPoint[lpObj->Class][lpObj->AccountLevel]) ? lpObj->AutoAddPointStats[n] : AddStat);

			lpObj->AutoAddPointCount = 0;

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);
}


void CCommandManager::CommandResetAutoProc(LPOBJ lpObj) //Reset Auto
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < gResetTable.GetResetLevel(lpObj))
	{
		return;
	}

	if(lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		return;
	}

	if (gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0) //Disable when vip over
	{
		lpObj->AutoResetEnable = 0; // Sets AutoResetEnable para OFF
		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(117)); // Escolher a MSG
		return;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;
	//-- quitar party al resetear
	/*if(gServerInfo.m_PartyRestrict != 0 && gParty.IsParty(lpObj->PartyNumber) != 0)
	{
		if (gParty.GetMemberCount(lpObj->PartyNumber) <=2)
		{
			gParty.Destroy(lpObj->PartyNumber);
		}
		else
		{
			gParty.DelMember(lpObj->PartyNumber,lpObj->Index);
		}		
	}*/

	if(lpMsg->ResetDay >= ((DWORD)gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(129),gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->ResetWek >= ((DWORD)gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(130),gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->ResetMon >= ((DWORD)gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(131),gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gResetTable.GetResetMoney(lpObj);

	//lpObj->Money -= gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]==-1)?(lpObj->Level-gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]):gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level-1];

	lpObj->Reset += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetDay += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetWek += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetMon += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpObj->AutoAddPointCount = 0;

	lpObj->AutoAddPointStats[0] = 0;

	lpObj->AutoAddPointStats[1] = 0;

	lpObj->AutoAddPointStats[2] = 0;

	lpObj->AutoAddPointStats[3] = 0;

	lpObj->AutoAddPointStats[4] = 0;

	if (gServerInfo.m_CustomRankUserType == 0)
	{
		gCustomRankUser.CheckUpdate(lpObj);
	}

	//gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);

	if(gServerInfo.m_CommandResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass/16)*16);
		lpObj->ChangeUp = lpObj->DBClass%16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp*16);
		Class -= (Class/32);
		Class += (lpObj->Class*32);

		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);

		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if(gServerInfo.m_CommandResetSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if(gServerInfo.m_CommandResetType == 1)
	{
		//lpObj->Money -= gResetTable.GetResetMoney(lpObj);

		//GCMoneySend(lpObj->Index,lpObj->Money);

		int point = gResetTable.GetResetPoint(lpObj);

		point = (point*gServerInfo.m_CommandResetPointRate[lpObj->Class])/100;

		point += (lpObj->Level-1)*gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH)==0)?0:((lpObj->Level>220)?((lpObj->Level-220)*gServerInfo.m_PlusStatPoint):0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	if(gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset;

		point = (point*gServerInfo.m_CommandMasterResetPointRate[lpObj->Class])/100;

		lpObj->LevelUpPoint += point;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDResetInfoSaveSend(lpObj->Index,lpMsg->ResetDay,lpMsg->ResetWek,lpMsg->ResetMon);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(95),lpObj->Reset);

	if(gServerInfo.m_CommandResetMove[lpObj->AccountLevel] != 0)
	{
		switch(lpObj->Class)
		{
			case CLASS_DW:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DK:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_FE:
				gObjMoveGate(lpObj->Index,27);
				break;
			case CLASS_MG:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DL:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_SU:
				gObjMoveGate(lpObj->Index,267);
				break;
			case CLASS_RF:
				gObjMoveGate(lpObj->Index,17);
				break;
		}
	}

	lpObj->AutoAddPointStats[0] = lpObj->AutoResetStats[0];

	lpObj->AutoAddPointStats[1] = lpObj->AutoResetStats[1];

	lpObj->AutoAddPointStats[2] = lpObj->AutoResetStats[2];

	lpObj->AutoAddPointStats[3] = lpObj->AutoResetStats[3];

	lpObj->AutoAddPointStats[4] = lpObj->AutoResetStats[4];

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	gRewardSystem.GetRewardByReset( lpObj );

	this->CommandAddPointAutoProc(lpObj);

	gLog.Output(LOG_COMMAND,"[CommandReset][%s][%s] - (Reset: %d)",lpObj->Account,lpObj->Name,lpObj->Reset);
}

bool CCommandManager::CommandDisablePvp(LPOBJ lpObj,char* arg)
{
#if (GAMESERVER_CLIENTE_UPDATE >= 7)
	if(lpObj->DisablePvp == 1)
	{
		lpObj->DisablePvp = 0;
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(772));

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(772));
		}

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		return 0;
	}
	else
	{
		lpObj->DisablePvp = 1;

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(771));

		//if (Npc >= 0)
		{
			//GCChatTargetNewSend(lpObj,Npc,gMessage.GetMessage(771));
		}

		gLog.Output(LOG_COMMAND,"[CommandDisablePvP][%s][%s] - Used",lpObj->Account,lpObj->Name);

		gCustomAttack.DGCustomAttackResumeSaveSend(lpObj->Index);

		return 1;
	}

#endif
	return 0;
}

void CCommandManager::DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandMasterResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if(lpMsg->MasterResetDay >= ((DWORD)gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(132),gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->MasterResetWek >= ((DWORD)gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(133),gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->MasterResetMon >= ((DWORD)gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(134),gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]==-1)?(lpObj->Level-gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]):gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level-1];

	if (gServerInfo.m_CommandMasterResetRemoveReset[lpObj->AccountLevel] > 0)
	{
		lpObj->Reset = (lpObj->Reset-gServerInfo.m_CommandMasterResetRemoveReset[lpObj->AccountLevel]);
	}
	else
	{
		lpObj->Reset = ((gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]==-1)?(lpObj->Reset-gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]):gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]);
	}

	lpObj->MasterReset += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->MasterResetDay += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->MasterResetWek += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->MasterResetMon += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	if (gServerInfo.m_CustomRankUserType == 1)
	{
		gCustomRankUser.CheckUpdate(lpObj);
	}

	//gCustomRankUser.GCReqRankLevelUser(lpObj->Index, lpObj->Index);

	if(gServerInfo.m_CommandMasterResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass/16)*16);
		lpObj->ChangeUp = lpObj->DBClass%16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp*16);
		Class -= (Class/32);
		Class += (lpObj->Class*32);

		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);

		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if(gServerInfo.m_CommandMasterResetSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if(gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset;

		point = (point*gServerInfo.m_CommandMasterResetPointRate[lpObj->Class])/100;

		point += (lpObj->Level-1)*gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH)==0)?0:((lpObj->Level>220)?((lpObj->Level-220)*gServerInfo.m_PlusStatPoint):0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	if(gServerInfo.m_CommandResetType == 1)
	{
		int point = gResetTable.GetResetPoint(lpObj);

		point = (point*gServerInfo.m_CommandResetPointRate[lpObj->Class])/100;

		lpObj->LevelUpPoint += point;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	gRewardSystem.GetRewardByReset( lpObj );

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDMasterResetInfoSaveSend(lpObj->Index,lpMsg->MasterResetDay,lpMsg->MasterResetWek,lpMsg->MasterResetMon);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(125),lpObj->MasterReset);

	if(gServerInfo.m_CommandMasterResetMove[lpObj->AccountLevel] != 0)
	{
		switch(lpObj->Class)
		{
			case CLASS_DW:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DK:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_FE:
				gObjMoveGate(lpObj->Index,27);
				break;
			case CLASS_MG:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DL:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_SU:
				gObjMoveGate(lpObj->Index,267);
				break;
			case CLASS_RF:
				gObjMoveGate(lpObj->Index,17);
				break;
		}
	}

	gLog.Output(LOG_COMMAND,"[CommandMasterReset][%s][%s] - (MasterReset: %d)",lpObj->Account,lpObj->Name,lpObj->MasterReset);
}

bool CCommandManager::CommandMarry(LPOBJ lpObj,char* arg) // OK
{
	char mode[11] = {0};
	this->GetString(arg,mode,sizeof(mode),0);


	if (mode[0] == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(606));
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(623),gServerInfo.m_CommandMarryLevel,gServerInfo.m_CommandMarryCost);
		return 0;
	}

	if(strcmp(mode,"to") == 0 && gServerInfo.m_CommandMarryOnlyGM == 0)
	{
		char MarryName[11] = {0};
		this->GetString(arg,MarryName,sizeof(MarryName),1);
		LPOBJ lpTarget = gObjFind(MarryName);
		if (lpTarget == 0) 
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(627),MarryName);
			return 0;
		}
    
		if (strcmp(MarryName,lpObj->Name) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(602));
			return 0;
		}
    
		SDHP_COMMAND_MARRY_SEND pMsg;


		pMsg.header.set(0x0F,0x02,sizeof(pMsg));


		pMsg.index = lpObj->Index;


		memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));


		memcpy(pMsg.marryname,MarryName,sizeof(pMsg.marryname));


		memcpy(pMsg.mode,"marry",sizeof(pMsg.mode));


		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
	}
	else if(strcmp(mode,"to") == 0 && gServerInfo.m_CommandMarryOnlyGM == 1)
	{

		if(gGameMaster.GetGameMasterLevel(lpObj) == 0)
		{
			return 0;
		}

		char MarryName1[11] = {0};
		char MarryName2[11] = {0};
		this->GetString(arg,MarryName1,sizeof(MarryName1),1);
		this->GetString(arg,MarryName2,sizeof(MarryName2),2);

		LPOBJ lpTarget1 = gObjFind(MarryName1);
		LPOBJ lpTarget2 = gObjFind(MarryName2);

		if (lpTarget1 == 0) 
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(627),MarryName1);
			return 0;
		}

		if (lpTarget2 == 0) 
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(627),MarryName2);
			return 0;
		}
    
		if (strcmp(MarryName1,MarryName2) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(602));
			return 0;
		}
		SDHP_COMMAND_MARRY_SEND pMsg;

		pMsg.header.set(0x0F,0x02,sizeof(pMsg));

		pMsg.index = lpTarget1->Index;

		memcpy(pMsg.name,MarryName1,sizeof(pMsg.name));

		memcpy(pMsg.marryname,MarryName2,sizeof(pMsg.marryname));

		memcpy(pMsg.mode,"marry",sizeof(pMsg.mode));

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
	}
	else if(strcmp(mode,"divorce") == 0)
	{
		SDHP_COMMAND_MARRY_SEND pMsg;

		pMsg.header.set(0x0F,0x02,sizeof(pMsg));

		pMsg.index = lpObj->Index;

		memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

		memcpy(pMsg.marryname,lpObj->Name,sizeof(pMsg.marryname));

		memcpy(pMsg.mode,"divorce",sizeof(pMsg.mode));

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
	}
	else if(strcmp(mode,"track") == 0)
	{
		if (lpObj->Money < (DWORD)gServerInfo.m_CommandMarryCost) 
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(620),gServerInfo.m_CommandMarryCost);
			return 0;
		}

		SDHP_COMMAND_MARRY_SEND pMsg;

		pMsg.header.set(0x0F, 0x02,sizeof(pMsg));

		pMsg.index = lpObj->Index;

		memcpy(pMsg.name, lpObj->Name,sizeof(pMsg.name));

		memcpy(pMsg.marryname, lpObj->Name, sizeof(pMsg.marryname));

		memcpy(pMsg.mode,"track",sizeof(pMsg.mode));

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
	}
	else if(strcmp(mode,"trace") == 0)
	{
		if (lpObj->Money < (DWORD)gServerInfo.m_CommandMarryCost) 
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(620),gServerInfo.m_CommandMarryCost);
			return 0;
		}

		SDHP_COMMAND_MARRY_SEND pMsg;

		pMsg.header.set(0x0F, 0x02,sizeof(pMsg));

		pMsg.index = lpObj->Index;

		memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

		memcpy(pMsg.marryname, lpObj->Name, sizeof(pMsg.marryname));

		memcpy(pMsg.mode, "trace", sizeof(pMsg.mode));

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
	}
	else 
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(606));
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(623),gServerInfo.m_CommandMarryLevel,gServerInfo.m_CommandMarryCost);
		return 0;
	}
	return 0;
}

bool CCommandManager::CommandOpenWarehouse(LPOBJ lpObj) // OK
{

	if(lpObj->Interface.use != 0)
	{
		return 0;
	}

	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX || lpObj->Interface.type == INTERFACE_TRADE || lpObj->Interface.type == INTERFACE_PERSONAL_SHOP)
	{
		return 0;
	}
	if (gItemManager.ChaosBoxHasItem(lpObj) || gItemManager.TradeHasItem(lpObj))
	{
		return 0;
	}

	if(gServerInfo.m_CommandOpenWareOnlySafeZone == 1 && gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(631));
		return 0;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_WAREHOUSE;
	lpObj->Interface.state = 0;

	gWarehouse.GDWarehouseItemSend(lpObj->Index,lpObj->Account);

	return 1;
}