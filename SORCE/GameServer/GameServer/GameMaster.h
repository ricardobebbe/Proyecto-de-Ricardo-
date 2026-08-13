// GameMaster.h: interface for the CGameMaster class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct GAME_MASTER_INFO
{
	char Account[11];
	char Name[11];
	int Level;
	int ShowSlogan;
};

class CGameMaster
{
public:
	CGameMaster();
	virtual ~CGameMaster();
	void Load(char* path);
	void SetGameMasterLevel(LPOBJ lpObj,int level);
	int GetGameMasterLevel(LPOBJ lpObj);
	void CheckGameMasterSlogan(LPOBJ lpObj);
private:
	std::map<std::string, GAME_MASTER_INFO> m_GameMasterInfo;
};

extern CGameMaster gGameMaster;
