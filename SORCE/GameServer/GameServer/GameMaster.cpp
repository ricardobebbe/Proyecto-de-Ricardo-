// GameMaster.cpp: implementation of the CGameMaster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameMaster.h"
#include "EffectManager.h"
#include "Util.h"

CGameMaster gGameMaster;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameMaster::CGameMaster() // OK
{
	
}

CGameMaster::~CGameMaster() // OK
{

}

void CGameMaster::Load(char* path) // OK
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("File %s load fail. Error: %s", path, res.description());
		return;
	}

	this->m_GameMasterInfo.clear();

	pugi::xml_node GameMasterList = file.child("GameMasterList");

	for (pugi::xml_node GameMaster = GameMasterList.child("GameMaster"); GameMaster; GameMaster = GameMaster.next_sibling())
	{
		GAME_MASTER_INFO info;

		strcpy_s(info.Account, GameMaster.attribute("Account").as_string());

		strcpy_s(info.Name, GameMaster.attribute("Character").as_string());

		info.Level = GameMaster.attribute("Level").as_int();

		info.ShowSlogan = GameMaster.attribute("ShowSlogan").as_int();

		this->m_GameMasterInfo.insert(std::pair<std::string, GAME_MASTER_INFO>(info.Name, info));
	}
}

void CGameMaster::SetGameMasterLevel(LPOBJ lpObj,int level) // OK
{
	GAME_MASTER_INFO info;

	memcpy(info.Account,lpObj->Account,sizeof(info.Account));

	memcpy(info.Name,lpObj->Name,sizeof(info.Name));

	info.Level = level;

	info.ShowSlogan = 1;

	this->m_GameMasterInfo.insert(std::pair<std::string, GAME_MASTER_INFO>(info.Name, info));
}

int CGameMaster::GetGameMasterLevel(LPOBJ lpObj) // OK
{
	std::map<std::string, GAME_MASTER_INFO>::iterator it = this->m_GameMasterInfo.find(lpObj->Name);

	if (it != this->m_GameMasterInfo.end())
	{
		if (strcmp(it->second.Account, lpObj->Account) == 0 && strcmp(it->second.Name, lpObj->Name) == 0)
		{
			return it->second.Level;
		}
	}
	
	return -1;
}

void CGameMaster::CheckGameMasterSlogan(LPOBJ lpObj) // OK
{
	std::map<std::string, GAME_MASTER_INFO>::iterator it = this->m_GameMasterInfo.find(lpObj->Name);

	if (it != this->m_GameMasterInfo.end())
	{
		if(it->second.ShowSlogan == 1)
		{
			gEffectManager.AddEffect(lpObj, 0, EFFECT_GAME_MASTER, 0, 0, 0, 0, 0);
		}
	}
}