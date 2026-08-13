#include "stdafx.h"
#if(START_ITEM==1)
#include "Util.h"
#include "User.h"
#include "CustomStartItem.h"
#include "CashShop.h"
#include "MemScript.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "ServerInfo.h"
#include "Notice.h"
#include "Message.h"
#include "DSProtocol.h"
#include "GameMain.h"

CGift gGiftNew;

CGift::CGift(){
    this->Init();
}

CGift::~CGift(){

}

void CGift::Init(){
	this->m_StatInfo.clear();
    this->m_GiftInfo.clear();
	this->m_BuffInfo.clear();
}

void CGift::Load(char* path){
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);
    if(res.status != pugi::status_ok){
		ErrorMessageBox("File %s load fail. Error: %s", path, res.description());
		return;
	}
    this->Init();

	pugi::xml_node oStartItem = file.child("StartItem");

	this->m_CustomItemStartSwitch[0] = oStartItem.attribute("StartSwitch_AL0").as_int();
	this->m_CustomItemStartSwitch[1] = oStartItem.attribute("StartSwitch_AL1").as_int();
	this->m_CustomItemStartSwitch[2] = oStartItem.attribute("StartSwitch_AL2").as_int();
	this->m_CustomItemStartSwitch[3] = oStartItem.attribute("StartSwitch_AL3").as_int();

	pugi::xml_node xStartStats = oStartItem.child("Stat");
    for (pugi::xml_node eStartStats = xStartStats.child("Char"); eStartStats; eStartStats = eStartStats.next_sibling()){
		STATS_INFO info;
		info.Class = eStartStats.attribute("ClassChar").as_int();
		info.LevelUpPoints = eStartStats.attribute("LevelUpPoints").as_int();
		info.Resets = eStartStats.attribute("Resets").as_int();
		info.Zen = eStartStats.attribute("Zen").as_int();
		this->m_StatInfo.push_back(info);
	}

	pugi::xml_node xStartItem = oStartItem.child("Item");
    for (pugi::xml_node eStartItem = xStartItem.child("Char"); eStartItem; eStartItem = eStartItem.next_sibling()){
	    GIFT_INFO info;
		info.Class = eStartItem.attribute("ClassChar").as_int();
	    info.Session = eStartItem.attribute("ItemType").as_int();
		info.ItemID = eStartItem.attribute("ItemIndex").as_int();
		info.Level = eStartItem.attribute("ItemLevel").as_int();
		info.Duration = eStartItem.attribute("ItemDur").as_int();
		info.Skill = eStartItem.attribute("ItemSkill").as_int();
		info.Luck = eStartItem.attribute("ItemLuck").as_int();
		info.Option = eStartItem.attribute("ItemOpt").as_int();
		info.Excellent = eStartItem.attribute("ItemExc").as_int();
		info.Time = eStartItem.attribute("ItemTime").as_int();
		this->m_GiftInfo.push_back(info);
	}
	pugi::xml_node xStartBuf = oStartItem.child("Buff");
	for (pugi::xml_node eStartBuf = xStartBuf.child("Char"); eStartBuf; eStartBuf = eStartBuf.next_sibling()){
		BUFF_INFO info;
		info.Class = eStartBuf.attribute("ClassChar").as_int();
		info.EffectID = eStartBuf.attribute("EffectID").as_int();
		info.Power1 = eStartBuf.attribute("Power1").as_int();
		info.Power2 = eStartBuf.attribute("Power2").as_int();
		info.Time = eStartBuf.attribute("BuffTime").as_int();
		this->m_BuffInfo.push_back(info);
	}
}

void CGift::GiftItem(LPOBJ lpObj){
	if(this->m_CustomItemStartSwitch[lpObj->AccountLevel] == 1){
		if(lpObj->ItemStart >= 1){
			return;
		}
		lpObj->ItemStart += 1;
		GDSaveTheGiftData(lpObj->Index);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(918));
        for(std::vector<STATS_INFO>::iterator it=this->m_StatInfo.begin();it != this->m_StatInfo.end();it++){
			if (it->Class != lpObj->Class){
				continue;
			}
			lpObj->Reset += it->Resets;
			lpObj->LevelUpPoint += it->LevelUpPoints;
			GCMoneySend(lpObj->Index,lpObj->Money += it->Zen);
			GDResetInfoSaveSend(lpObj->Index,0,0,0);
			GCNewCharacterInfoSend(lpObj);
			GDCharacterInfoSaveSend(lpObj->Index);
		}
		for(std::vector<GIFT_INFO>::iterator it=this->m_GiftInfo.begin();it != this->m_GiftInfo.end();it++){
			if (it->Class != lpObj->Class){
				continue;
			}
			int Days = it->Time;
			time_t t = time(NULL);
			localtime(&t);
			DWORD iTime = (DWORD)t + Days * 86400;
			GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(it->Session,it->ItemID),it->Level,it->Duration,it->Skill,it->Luck,it->Option,-1,it->Excellent,0,0,0,0,0xFF,iTime);
		}
		for(std::vector<BUFF_INFO>::iterator it=this->m_BuffInfo.begin();it != this->m_BuffInfo.end();it++){
			if (it->Class != lpObj->Class){
				continue;
			}
			EFFECT_INFO* lpInfo = gEffectManager.GetInfo(it->EffectID);
			if(lpInfo == 0){
				continue;
			}
			int Days = it->Time;
			time_t t = time(NULL);
			localtime(&t);
			DWORD iTime = (DWORD)t + Days * 86400;
			gEffectManager.AddEffect(lpObj,1,it->EffectID,iTime,it->Power1,it->Power2,0,0);
		}
	}
}
#endif