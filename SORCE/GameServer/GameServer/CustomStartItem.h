#pragma once
#if(START_ITEM==1)
#define MAX_CLASS 7

struct GIFT_INFO
{
	int Class;
	int Session;
	int ItemID;
	int Level;
	int Duration;
	int Skill;
	int Luck;
	int Option;
	int Excellent;
	int	Time;
};

struct BUFF_INFO
{
	int Class;
	int EffectID;
	int Power1;
	int Power2;
	int Time;
};

struct STATS_INFO
{
	int Class;
	int Resets;
	int LevelUpPoints;
	int Zen;
};

class CGift
{
public:
	CGift();
	virtual ~CGift();
	void Init();
	void Load(char* path);
	void GiftItem(LPOBJ lpObj);
private:
	int m_CustomItemStartSwitch[MAX_ACCOUNT_LEVEL];
	std::vector<STATS_INFO> m_StatInfo;
	std::vector<GIFT_INFO> m_GiftInfo;
	std::vector<BUFF_INFO> m_BuffInfo;
};

extern CGift gGiftNew;
#endif