#pragma once

#include "User.h"
#include <fstream>

#if USE_FAKE_ONLINE == TRUE

struct OFFEXP_DATA
{
	char Account[11];
	char Password[11];
	char Name[11];
	char IP[16];

	WORD SkillID;
	WORD UseBuffs[3];
	BYTE TuNhatItem;

	BYTE TuDongReset;
	int PartyMode; //FAKE
	BYTE MapNumber;
	short MapX;
	short MapY;

	//int Pick;
};

class CFakeOnline
{
public:
	CFakeOnline();

	void RestoreFakeOnline();
	OFFEXP_DATA* GetOffExpInfo(LPOBJ lpObj);
	OFFEXP_DATA* GetOffExpInfoByAccount(LPOBJ lpObj);
	void LoadFakeData(char* path);
	void AddToList(LPOBJ lpObj);
	void DelFromList(char* account);
	void WriteToFile();
	void FakeAttackProc(LPOBJ lpObj);
	void Attack(int UserIndex);
	void OnAttackAlreadyConnected(LPOBJ lpObj);
	void TuDongBuffSkill(int aIndex); //FAKE
	void	GuiYCParty(int aIndex, int bIndex); //FAKE
public:

	std::map<std::string, OFFEXP_DATA> m_Data;
	int AccountsRestored;

	//HANDLE File;
	//bool FileLoaded;
	//std::fstream File;
};

extern CFakeOnline s_FakeOnline;
#endif