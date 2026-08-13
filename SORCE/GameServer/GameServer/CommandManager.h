// CommandManager.h: interface for the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

enum eCommandNumber
{
	//Comando de players COMMON
	COMMAND_MOVE                  = 0,
	COMMAND_GUILD_WAR             = 1,
	COMMAND_BATTLE_SOCCER         = 2,
	COMMAND_POST                  = 3,
	COMMAND_DISABLEPVP            = 4,
	COMMAND_ADD_POINT1            = 5,
	COMMAND_ADD_POINT2            = 6,
	COMMAND_ADD_POINT3            = 7,
	COMMAND_ADD_POINT4            = 8,
	COMMAND_ADD_POINT5            = 9,
	COMMAND_PK_CLEAR              = 10,
	COMMAND_MONEY                 = 11,
	COMMAND_CHANGE                = 12,
	COMMAND_WARE                  = 13,
	COMMAND_RESET                 = 14,
	COMMAND_MASTER_RESET          = 15,
	COMMAND_CUSTOM_STORE          = 16,
	COMMAND_CUSTOM_STORE_OFFLINE  = 17,
	COMMAND_CUSTOM_OFFATTACK      = 18,
	COMMAND_CLEAR_INVENTORY       = 19,
	COMMAND_REDISTRIBUTE          = 20,
	COMMAND_CUSTOM_ATTACK	      = 21,
	COMMAND_PICK			      = 22,
	COMMAND_LOCK			      = 23,
	COMMAND_UNLOCK			      = 24,
	COMMAND_PACK			      = 25,
	COMMAND_UNPACK			      = 26,
	COMMAND_MARRY			      = 27,
	COMMAND_OPENWAREHOUSE	      = 28,
	COMMAND_QUIZ			      = 29,
	COMMAND_EVENTSTARTJOIN	      = 30,
	COMMAND_EVENTGETJOIN	      = 31,
	COMMAND_REI		              = 32,
	
	
	////Comando de GM
	COMMAND_GM_MOVE              = 100,
	COMMAND_GM_POST              = 101,
	COMMAND_TRACK                = 102,
	COMMAND_TRACE                = 103,
	COMMAND_DISCONNECT           = 104,
	COMMAND_FIREWORKS            = 105,
	COMMAND_MAKE                 = 106,
	COMMAND_DROP                 = 107,
	COMMAND_SKIN                 = 108,
	COMMAND_SET_MONEY            = 109,

	//Comando de Start Events
	COMMAND_EVENTROULETTE	     = 110,
	COMMAND_EVENTESC		     = 111,
	COMMAND_KILLALL			     = 112,
	COMMAND_EVENTQUICKLY	     = 113,
	COMMAND_PVP				     = 114,
	COMMAND_EVENTGET		     = 115,
	COMMAND_MOVE_ALL             = 116,
	COMMAND_MOVE_GUILD           = 117,
	COMMAND_MAKE_MOB             = 118,

};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

struct SDHP_COMMAND_MASTER_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
	UINT MasterResetDay;
	UINT MasterResetWek;
	UINT MasterResetMon;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_COMMAND_MARRY_SEND
{
	PSBMSG_HEAD header; // C1:0F:02
	WORD index;
	char name[11];
	char marryname[11];
	char mode[11];
};

struct SDHP_COMMAND_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_COMMAND_MASTER_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct COMMAND_LIST_INFO
{
	int Index;
	DWORD Delay;
	int GameMaster;
	char Command[32];
	char Description[100];
	int GMLevel; //ZEUS UP24 LUA 
	int Enable[MAX_ACCOUNT_LEVEL];
	int MinLevel[MAX_ACCOUNT_LEVEL];
	int MaxLevel[MAX_ACCOUNT_LEVEL];
	int MinReset[MAX_ACCOUNT_LEVEL];
	int MaxReset[MAX_ACCOUNT_LEVEL];
	DWORD Money[MAX_ACCOUNT_LEVEL];
	DWORD WCoinC[MAX_ACCOUNT_LEVEL];
	DWORD WCoinP[MAX_ACCOUNT_LEVEL];
	DWORD GoblinPoint[MAX_ACCOUNT_LEVEL];
};

class CCommandManager
{
public:
	CCommandManager();
	virtual ~CCommandManager();
	void Load(char* path);
	void MainProc();
	long GetNumber(char* arg, int pos);
	void GetString(char* arg, char* out, int size, int pos);
	bool GetCommandCode(char* label, COMMAND_LIST_INFO* lpInfo);

	bool GetCommandCode(int Index,COMMAND_LIST_INFO* lpInfo);
	

	bool ManagementCore(LPOBJ lpObj, char* message);

	void CommandMove(LPOBJ lpObj, char* arg);
	void CommandGuildWar(LPOBJ lpObj, char* arg);
	void CommandBattleSoccer(LPOBJ lpObj, char* arg);
	bool CommandDisablePvp(LPOBJ lpObj,char* arg);
	bool CommandPKClear(LPOBJ lpObj,char* arg);
	bool CommandChange(LPOBJ lpObj,char* arg);
	bool CommandMasterReset(LPOBJ lpObj,char* arg);
	bool CommandLock(LPOBJ lpObj,char* arg);
	bool CommandUnLock(LPOBJ lpObj,char* arg);
	bool CommandMarry(LPOBJ lpObj,char* arg);
	bool CommandOpenWarehouse(LPOBJ lpObj);
	bool CommandSkin(LPOBJ lpObj,char* arg);
	


	bool CommandPost(LPOBJ lpObj, char* arg);
	bool CommandAddPoint(LPOBJ lpObj, char* arg, int type);
	bool CommandAddPointAuto(LPOBJ lpObj, char* arg, int type);
	
	bool CommandMoney(LPOBJ lpObj, char* arg);
	
	bool CommandWare(LPOBJ lpObj, char* arg);
	void CommandReset(LPOBJ lpObj, char* arg);
	void CommandResetAuto(LPOBJ lpObj, char* arg);
	
	bool CommandClearInventory(LPOBJ lpObj);
	bool CommandRedistribute(LPOBJ lpObj);
	
	
	
	
	bool CommandGMMove(LPOBJ lpObj, char* arg);
	bool CommandGMPost(LPOBJ lpObj, char* arg);
	bool CommandTrack(LPOBJ lpObj, char* arg);
	bool CommandTrace(LPOBJ lpObj, char* arg);
	bool CommandDisconnect(LPOBJ lpObj, char* arg);
	bool CommandFireworks(LPOBJ lpObj);
	bool CommandMake(LPOBJ lpObj, char* arg);
	bool CommandDrop(LPOBJ lpObj, char* arg);
	
	bool CommandSetMoney(LPOBJ lpObj, char* arg);
	
	
	
	bool CommandSetVip(LPOBJ lpObj, char* arg);
	bool CommandMoveAll(LPOBJ lpObj, char* arg);
	bool CommandMoveGuild(LPOBJ lpObj, char* arg);
	bool CommandMakeMob(LPOBJ lpObj, char* arg);

	void CommandAddPointAutoProc(LPOBJ lpObj);
	void CommandResetAutoProc(LPOBJ lpObj);

	void DiscountRequirement(LPOBJ lpObj,int index);  //ZEUS LUA UP24

	void DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg);
	void DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg);
private:
	std::map<int, COMMAND_LIST_INFO> m_CommandListInfo;
};

extern CCommandManager gCommandManager;