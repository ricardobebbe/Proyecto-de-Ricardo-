// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum MESSAGE_TYPE
{
MSG_BLOOD_CASTLE = 0,
	MSG_BONUS_MANAGER = 1,
	MSG_CHAOS_CASTLE = 2,
	MSG_DEVIL_SQUARE = 3,
	MSG_COMMAND_MANAGER = 4,
	MSG_LOREN_DEEP = 5,
	MSG_ILLUSION_TEMPLE = 6,
	MSG_DOUBLE_GOER = 7,
	MSG_IMPERIAL_GUARDIAN = 8,
	MSG_KANTURU = 9,
	MSG_RAKLION = 10,
	MSG_MOSS_MERCHANT = 11,
	MSG_CUSTOM_REWARD_MASTER = 12,
	MSG_CUSTOM_GOLDEN_ARCHER = 13,
	MSG_DUEL = 14,
	MSG_MOVE = 15,
	MSG_CRYWOLF = 16,
	MSG_CASTLE_SIEGE = 17,
	MSG_GUILD = 18,
	MSG_SHOP = 19,
	MSG_CUSTOM_MONSTER = 20,
	MSG_GENERAL = 21,
	MSG_SERVER = 22,
	MSG_CUSTOM_STORE = 23,
	MSG_CUSTOM_ARENA = 24,
	MSG_CUSTOM_ATTACK = 25,
	MSG_CUSTOM_DROP_EVENT = 26,
	MSG_GIFT_SYSTEM = 27,
	MSG_MARRIAGE = 28,
	MSG_CUSTOM_BUY_VIP = 29,
	MSG_BOTS_SYSTEM = 30,

};

struct MESSAGE_INFO
{
	int Index;
	char Message[128];
	
};

class CMessage
{
public:
	CMessage();
	virtual ~CMessage();
	void Load(char* path);
	char* GetMessage(int index);
private:
	char m_DefaultMessage[128];
	std::map<int,MESSAGE_INFO> m_MessageInfo;
};

extern CMessage gMessage;
