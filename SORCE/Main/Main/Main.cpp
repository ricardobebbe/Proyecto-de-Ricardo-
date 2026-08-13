#include "stdafx.h"
#include "resource.h"
#include "Main.h"
#include "..\\..\\Util\\CCRC32.H"
#include "Common.h"
#include "Controller.h"
#include "CustomBuyVip.h"
#include "CustomCommandInfo.h"
#include "CustomEventTime.h"
#include "CustomItem.h"
#include "CustomJewel.h"
#include "CustomMap.h"
#include "CustomMonster.h"
#include "CustomNpcName.h"
#include "CustomWing.h"
#include "Fix.h"
#include "StaticEffect.h"
#include "DynamicEffect.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "Item.h"
#include "ItemShopValue.h" 
#include "ItemSmoke.h"
#include "Interface.h"
#include "OffTrade.h"
#include "PacketManager.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Protocol.h"
#include "Reconnect.h"
#include "Resolution.h"
#include "User.h"
#include "Util.h"
//--
#include "InterEx.h"
#include "SelectChar.h"
#include "postInterface.h"
#include "StatsAdvance.h"
#include "CustomBow.h"
#include "CustomGloves.h"
#include "CustomPet.h"
#include "Pet.h"
#include "PetHook.h"
#include "CustomCloak.h"
#include "cape.h"
#include "ItemTRSData.h"
#include "CustomEffectRemake.h"
#include "ItemRemake.h"
#include "ItemManager.h"
#include "ItemInfoEx.h"
#include "CSCharacterS13.h"
#include "CustomItemPrice.h"
#include "MonsterGlow.h"
#include "CustomMonsterGlow.h"
#include "ModelEffect.h"
#include "CChatEx.h"
#include "CustomPetEffect.h"
#include "CChatClassic.h"
#include "CMacroUIEx.h"
#include "Minimize.h"
#include "MiniMap.h"
#include "WindowsStruct.h"
#include "SItemOption.h"
#include "ItemTooltip.h"
#include "PetMontura.h"
#include "Central.h"
#include "BuffIcon.h"
//--
#include "Inter3.h"
#include "CChatClassic.h"
#include "cCPetRenderMesh.h"
#include "MultiSelect.h"
#include "ItemTooltip.h"
#include "JCRendernoItemGlow.h"
#include "ItemGlow.h"
#include "CScene.h"
#include "ServerSelWin.h"
#include "NewUIEnterBloodCastle.h"
#include "CustomNPC.h"
#include "NewUIButton.h"
#include "NewUIMyInventory.h"
#include "SEASON3B.h"
#include "UIControl.h"
#include "AutoLoginWin.h"
#include "PetEffectDynamic.h"
#include "PartySystem.h"
#include "StoreSearch.h"
#include "NewController.h"

//#include "InfoLog.h"

HINSTANCE hins;

void StartAddress(LPVOID lpThreadParameter)
{
	HANDLE v1;
	HANDLE v2;

	while ( TRUE )
	{
		Sleep(5000);

		v1 = GetCurrentProcess();
		SetProcessWorkingSetSize(v1, 0xFFFFFFFF, 0xFFFFFFFF);

		v2 = GetCurrentProcess();
		SetThreadPriority(v2, -2);
	}
}

__declspec(naked) void OffButtonQuestInStats()
{
	static DWORD ButtonAddress = 0x0077EBB5;
	_asm{jmp ButtonAddress}
}

__declspec(naked) void OffButtonPetsInStats()
{
	static DWORD ButtonAddress = 0x0077EC5E;
	_asm{jmp ButtonAddress}
}

__declspec(naked) void OffButtonSkillMasterInStats()
{
	static DWORD ButtonAddress = 0x0077ECE7;
	_asm{jmp ButtonAddress}
}

__declspec(naked) void OffButtonInventoryExt()
{
	static DWORD ButtonAddress = 0x00836A28;
	_asm{jmp ButtonAddress}
}

__declspec(naked) void OffButtonOpenStore()
{
	static DWORD ButtonAddress = 0x008369B4;
	_asm{jmp ButtonAddress}
}
__declspec(naked) void OffButtonRepeirInInventory()
{
	static DWORD ButtonAddress = 0x00836941;
	_asm{jmp ButtonAddress}
}

extern "C" _declspec(dllexport) void EntryProc() // OK
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartAddress, 0, 0, 0);

	if(gProtect.ReadMainFile("Data\\Local\\RDDLV\\RDDLV.Misiones") == 0)
	{
		MessageBox(0,"Data\\Local\\RDDLV\\RDDLV.Misiones corrupta!","Error",MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	m_Keyboard1 = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
	m_Keyboard2 = SetWindowsHookEx(WH_KEYBOARD_LL, LL_KeyboardProc, NULL, 0);

	if(gProtect.ReadTooltipTRSData( "Data\\Local\\TooltipTRSData.bmd" ) == 0)
	{
		MessageBox(0, "Data\\Local\\TooltipTRSData.bmd missing or File corrupt!" ,"Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if(gProtect.ReadMakeViewTRSData("Data\\Local\\MakeWorldTRSData.bmd") == 0)
	{
		MessageBox(0,"Data\\Local\\MakeWorldTRSData.bmd missing or File corrupt!","Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	if(gProtect.ReadPetEffectBMD("Data\\Local\\EffectTRSData.bmd") == 0)
	{
		MessageBox(0,"Data\\Local\\EffectTRSData.bmd missing or File corrupt!","Error", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	//gInfoLog.Load();

	SetByte(0x00E61144,0xA0); // Accent
	SetByte(0x004D1E69,0xEB); // Crack (mu.exe)
	SetByte(0x004D228D,0xE9); // Crack (GameGuard)
	SetByte(0x004D228E,0x8B); // Crack (GameGuard)
	SetByte(0x004D228F,0x00); // Crack (GameGuard)
	SetByte(0x004D2290,0x00); // Crack (GameGuard)
	SetByte(0x004D2291,0x00); // Crack (GameGuard)
	SetByte(0x004D559C,0xEB); // Crack (ResourceGuard)
	SetByte(0x00633F7A,0xEB); // Crack (ResourceGuard)
	SetByte(0x00634403,0xEB); // Crack (ResourceGuard)
	SetByte(0x0063E6C4,0xEB); // Crack (ResourceGuard)
	SetByte(0x004D2246,0xEB); // Crack (ResourceGuard)
	SetByte(0x00501163,0xEB); // Crack (ResourceGuard)
	SetByte(0x0040AF0A,0x00); // Crack (ResourceGuard)
	SetByte(0x0040B4BC,0x50); // Login Screen
	SetByte(0x0040B4C5,0x50); // Login Screen
	SetByte(0x0040B4CF,0x18); // Login Screen
	SetByte(0x0040AF0A,0x00); // Login Screen
	SetByte(0x0040AFD5,0xEB); // Login Screen
	SetByte(0x00777FD6,0x70); // Item Text Limit
	SetByte(0x00777FD7,0x17); // Item Text Limit
	SetByte(0x004EBEC7,0x3C); // Item Text Limit
	SetByte(0x005C4004,0x3C); // Item Text Limit
	SetByte(0x007E40BB,0x3C); // Item Text Limit
	SetByte(0x0081B546,0x3C); // Item Text Limit
	SetByte(0x0081B58D,0x3C); // Item Text Limit
	SetByte(0x0086E284,0x3C); // Item Text Limit
	SetByte(0x0086E44C,0x3C); // Item Text Limit
	SetByte(0x0086E573,0x3C); // Item Text Limit
	SetByte(0x0086F8FC,0x3C); // Item Text Limit
	SetByte(0x007DA373,0xB7); // Item Type Limit
	SetByte(0x007E1C44,0xB7); // Item Type Limit
	SetByte(0x0052100D,0xEB); // Ctrl Fix
	SetByte(0x009543C4,0x00); // Move Vulcanus
	SetCompleteHook(0xE9, 0x004D1CF0, 0x004D1DC2); //-- Remoce MuError.DMP
	MemorySet(0x00D20170, 0x90, 0x1B); //-- Remove Creation MuError.log
	SetByte(0x0064CBD1,((gProtect.m_MainInfo.HelperActiveAlert==0) ? 0xEB : 0x75)); //-- Helper Message Box
	SetByte(0x0064CBD0,(BYTE)gProtect.m_MainInfo.HelperActiveLevel); //-- Helper Active Level
	SetByte(0x0095CEEF,(BYTE)gProtect.m_MainInfo.HelperActiveLevel); //-- Helper Active Level
	SetByte(0x0095CF14,(BYTE)gProtect.m_MainInfo.HelperActiveLevel); //-- Helper Active Level
	SetByte(0x00E61F68,(gProtect.m_MainInfo.ClientVersion[0]+1)); //-- Version
	SetByte(0x00E61F69,(gProtect.m_MainInfo.ClientVersion[2]+2)); //-- Version
	SetByte(0x00E61F6A,(gProtect.m_MainInfo.ClientVersion[3]+3)); //-- Version
	SetByte(0x00E61F6B,(gProtect.m_MainInfo.ClientVersion[5]+4)); //-- Version
	SetByte(0x00E61F6C,(gProtect.m_MainInfo.ClientVersion[6]+5)); //-- Version
	SetWord(0x00E609E4,(gProtect.m_MainInfo.IpAddressPort)); //-- IpAddressPort
	SetDword(0x004D9D3B,(DWORD)&FrameValue);
	SetDword(0x004DAC60,(DWORD)&FrameValue);
	SetDword(0x7B55338,(DWORD)10);
	SetDword(0x004D597B,(DWORD)&MainTickCount);
	SetDword(0x004DA289,(DWORD)&MainTickCount);
	SetDword(0x004DA297,(DWORD)&MainTickCount);
	SetDword(0x004DA3A2,(DWORD)&MainTickCount);
	SetDword(0x004DA3CE,(DWORD)&MainTickCount);
	SetDword(0x004DA3D9,(DWORD)&MainTickCount);
	SetDword(0x0063D326,(DWORD)&MainTickCount);
	SetDword(0x00642112,(DWORD)&MainTickCount);
	SetDword(0x004D0E09,(DWORD)gProtect.m_MainInfo.WindowName);
	SetDword(0x004D9F55,(DWORD)gProtect.m_MainInfo.ScreenShotPath);

	SetDword(0x00954148 + 1, (DWORD)gProtect.m_MainInfo.ServerName1); //-> (Gold)
	SetDword(0x00954115 + 1, (DWORD)gProtect.m_MainInfo.ServerName2); //-> (Gold PVP)
	SetDword(0x009540E2 + 1, (DWORD)gProtect.m_MainInfo.ServerName3); //-> (Non-PVP)
	SetDword(0x009540AC + 1, (DWORD)gProtect.m_MainInfo.ServerName4); //-> Sem Nada

	SetByte(0x005C914B+3, 0x00); //-- LuckItemmove

	MemorySet(0x0063E908,0x90,20); //-- C1:F3:04

	SetCompleteHook(0xE9,0x0064452A,0x00644537); //-- Fix montura

	MemoryCpy(0x00E611B2,gProtect.m_MainInfo.IpAddress,sizeof(gProtect.m_MainInfo.IpAddress)); //-- IpAddress

	MemoryCpy(0x00E61F70,gProtect.m_MainInfo.ClientSerial,sizeof(gProtect.m_MainInfo.ClientSerial)); //-- ClientSerial

	SetCompleteHook(0xFF,0x0065FD79,&ProtocolCoreEx);

	SetCompleteHook(0xE9,0x004DA280,&CheckTickCount1);

	SetCompleteHook(0xE9,0x004DA3A1,&CheckTickCount2);

	SetCompleteHook(0xE8,0x005B96E8,&DrawNewHealthBar);

	VirtualizeOffset(0x004D9D39,12);

	VirtualizeOffset(0x004D9D45,7);

	VirtualizeOffset(0x004D9EFC,15);

	VirtualizeOffset(0x004DAC5C,8);

	VirtualizeOffset(0x005451F7,5);

	VirtualizeOffset(0x00545230,8);

	VirtualizeOffset(0x005A52E9,8);

	LoadReferenceAddressTable((HMODULE)hins, MAKEINTRESOURCE(IDR_BIN1),(DWORD)&NewAddressData1);

	LoadReferenceAddressTable((HMODULE)hins, MAKEINTRESOURCE(IDR_BIN2),(DWORD)&NewAddressData2);

	LoadReferenceAddressTable((HMODULE)hins, MAKEINTRESOURCE(IDR_BIN3),(DWORD)&NewAddressData3);

	gCustomMessage.LoadEng(gProtect.m_MainInfo.EngCustomMessageInfo);

	gCustomMessage.LoadPor(gProtect.m_MainInfo.PorCustomMessageInfo);

	gCustomMessage.LoadSpn(gProtect.m_MainInfo.SpnCustomMessageInfo);

	gCustomJewel.Load(gProtect.m_MainInfo.CustomJewelInfo);

	gCustomWing.Load(gProtect.m_MainInfo.CustomWingInfo);

	gCustomItem.Load(gProtect.m_MainInfo.CustomItemInfo);

	gCustomBuyVip.Load(gProtect.m_MainInfo.CustomBuyVipInfo);

	gCustomCommandInfo.Load(gProtect.m_MainInfo.CustomCommandInfo);

	gCustomEventTime.Load(gProtect.m_MainInfo.CustomEventInfo);

	gFog.Load(gProtect.m_MainInfo.CustomFog);

	gSmokeEffect.Load(gProtect.m_MainInfo.CustomSmokeEffect);

	gCustomMonster.Load(gProtect.m_MainInfo.CustomMonsters);

	gNPCName.Load(gProtect.m_MainInfo.CustomNPCName);
	//--
	gCustomBattleGloves.Load(gProtect.m_MainInfo.CustomGloves);

	gCustomBow.Load(gProtect.m_MainInfo.CustomBowInfo);

	gCustomPet2.Load(gProtect.m_MainInfo.CustomPetInfo);

	gCloak.Load(gProtect.m_MainInfo.m_CustomCloak);

	ItemTRSData.Load(gProtect.m_MainInfo.CustomPosition);

	gCustomRemake.Load(gProtect.m_MainInfo.CustomRemake);

	gItemManager.Load(gProtect.m_MainInfo.m_ItemInfoEx);

	gCustomModelEffect.Load(gProtect.m_MainInfo.m_CustomModelInfo);
	//--
	gCustomCEffectPet.Load(gProtect.m_ReadBMD.m_PetCEffectBMD);

	gCustomWingEffect.Load(gProtect.m_ReadBMD.CustomWingEffectInfo);

	gDynamicWingEffect.Load(gProtect.m_ReadBMD.DynamicWingEffectInfo);

	cRender.Load(gProtect.m_ReadBMD.RenderMeshPet);

	gCustomMonsterGlow.LoadGlow(gProtect.m_ReadBMD.m_CustomMonsterGlow);

	gCustomMonsterGlow.LoadBrightness(gProtect.m_ReadBMD.m_CustomMonsterbrightness);

	JCEffectMonster.Load( gProtect.m_ReadBMD.m_CustomMonsterEffect );

	g_pNewEnterBloodCastle.Init();
	
	gNPC.Load(); //NPC CUSTOM UP17

	AutoLogin.Load(); //Salva Senha UP17

	JCRemoveGlow.Load( gProtect.m_ReadBMD.m_JCWRemoveGlow );
#if UPDATE > 15
	JCRender.Load( gProtect.m_ReadBMD.m_JCWRenderMesh );
#endif
//--
	gIconBuff.LoadEng(gProtect.m_ReadTooltipTRSData.m_TooltipTRSDataEng);
	
	gIconBuff.LoadSpn(gProtect.m_ReadTooltipTRSData.m_TooltipTRSDataSpn);
	
	gIconBuff.LoadPor(gProtect.m_ReadTooltipTRSData.m_TooltipTRSDataPor);

	GInfo.loadnInformation( gProtect.m_ReadTooltipTRSData.m_TRSTooltipData );

	GInfo.loadnText( gProtect.m_ReadTooltipTRSData.m_TRSTooltipText );

	gCustomCEffectDynamicPet.Load(gProtect.m_MainInfo.m_DynamicPetEffect);

//--
#if UPDATE >= 15
	gSelectCharRamdon.LoadMultiChar(gProtect.m_MakeViewTRSData.m_RenderSelect);
#endif
//--
	gPacketManager.LoadEncryptionKey("Data\\Enc1.dat");

	gPacketManager.LoadDecryptionKey("Data\\Dec2.dat");

	gProtect.CheckLauncher();

	gProtect.CheckInstance();

	gProtect.CheckClientFile();

	gProtect.CheckHackFile();

	gProtect.CheckPluginFile1();

	gProtect.CheckPluginFile2();

	gProtect.CheckPluginFile3();

	gProtect.CheckPluginFile4();

	gProtect.CheckPluginFile5();

	gProtect.CheckCameraFile();

	//InitHackCheck();

//	gTrayMode.Load();

	InitCommon();

	JCRemoveGlow.Init();

	g_ServerSelWin.Init();

	CServerSelWin();

#if UPDATE > 15
	JCRender.Init();
#endif
	gCentral.InitCentral();//-- Control: camara||mouse

	gItemInfoEx.Load();

	//gMuHelper.LoadHelper();  // COMENTADO: Archivo huérfano MuHelper.h no disponible

	if(gProtect.m_MainInfo.SelectCharAnimate == 1)
	{
		gSelectChar.Load();
	}

	#if UPDATE >= 15
	gSelectCharRamdon.InitMultiChar();
	#endif

	if(gProtect.m_MainInfo.SelectCharS13 == 1)
	{
		gCSCharacterS13.Load();
	}

	ItemTRSData.Init();

	if(gProtect.m_MainInfo.DisableCustomCloack == 0)
	{
		gCapeAnimation.Load();
	}

	if(gProtect.m_MainInfo.DisableCustomPet == 0)
	{
		gCustomPet.Load();

		gObjCreatePetExHook();

		gCustomCEffectPet.Init();

		initmontura();
	}

	gRenderMap.MiniMapload();

	InitItem();

	InitJewel();

	InitPrintPlayer();

	InitReconnect();

	InitResolution();

	gController.Load();

	InitWing();

	gObjUser.Load();

	gInterface.RenderObjectSystem();

	if(gProtect.m_MainInfo.CustomSelectServerX700 == 1)  //SelectChar+SelectServer UP17
	{
	   gCSceneA.Load();
	}

	      
	g_PersonalShopEx.Load();   //ZEUS UP24 Mercado Negro IN-GAME STORE

	gCMacroUIEx.Load();

	switch(gProtect.m_MainInfo.CustomInterfaceType)
	{
		case 1:
		case 2:
			InitInter3();
			gCChatClassic.Load();
			break;
		case 3:
			InitSeason15();
		case 4:
			gCRenderEx700.Init();
			gCChatEx.Load();
			break;
	}
	
	if(gProtect.m_MainInfo.DisableAdvance == 0)
	{
		g_StatsAdvance.Load();	              //ZEUS UP16 <-- Advanced
	}
	gOffTrade.Init();

	gFog.Init();

	if(gProtect.m_MainInfo.CustomMonsterEnable == 1)
	{
		gCustomMonster.InitMonster();
	}

	InitloadGlow();

	if(gProtect.m_MainInfo.DisableCustomBow == 0)
	{
		gCustomBow.Init();
	}

	if(gProtect.m_MainInfo.DisableCustomClaws == 0)
	{
		gCustomBattleGloves.Init();
	}

	gCustomMap.Load();

	if(gProtect.m_MainInfo.EnableShopValueSystem == 1)
	{
		gItemPrice.Init();
	}

	if(gProtect.m_MainInfo.CsSkill == 1)
	{
		//Liberar CS Skills
		SetByte((0x005B89C3+3), 0x00);
		SetByte((0x005B89C9+3), 0x00);
		SetByte((0x005B89CF+3), 0x00);
		SetByte((0x005B89D5+3), 0x00);
		SetByte((0x005B89DB+3), 0x00);
		SetByte((0x005B89E1+3), 0x00);
		SetWord((0x005B89E7+3), 0x00);
		SetByte((0x005C6314+2), 0x00);
		SetByte((0x005C6327+2), 0x00);
		SetByte((0x005C633A+2), 0x00);
		SetByte((0x005C634D+2), 0x00);
		SetByte((0x005C6360+2), 0x00);
		SetByte((0x005C6373+2), 0x00);
		SetWord((0x005C6386+2), 0x00);
	}

    SetByte(0x004030D1+3,5);

	SetWord(0x00405B7C, gProtect.m_MainInfo.ReduceMemory);
	SetWord(0x0040CC40, gProtect.m_MainInfo.ReduceMemory);
	
	if(gProtect.m_MainInfo.DisableTree == 1)
	{
		MemorySet(0x008193F0,0x90,0xC8); //Remove Master Skill Tree Window(A Buttom)
	}

	if(gProtect.m_MainInfo.EffectPlus15 == 0)
	{
		MemorySet(0x006097B0,0x90,0x69D);
	}

	if(gProtect.m_MainInfo.DisableGens == 1)
	{
		MemorySet(0x007C5AD2, 0x90, 0x05);			//-> Disable Gens (Key B)
	}

	if(gProtect.m_MainInfo.DisableTabMap == 1)
	{
		MemorySet(0x0062F876, 0x90, 0x30);			//-> Disable ViewMap (Key TAB)
	}

	if(gProtect.m_MainInfo.DisableMmoves == 1) 		//-> Disable Button Move (M) ZEUS UP18 		
	{
		SetByte(0x007D3C6B+1,0xFF);
	}

	if(gProtect.m_MainInfo.DisableF == 1) 		//-> Disable Button AMIGOS (F) ZEUS UP18 		
	{
		SetByte((0x007D37A1+1),0xFF);
	}

	//-- disables
	if(gProtect.m_MainInfo.DisableRepeirInInventory == 1)
	{
		SetCompleteHook(0xE9, 0x0083693C, &OffButtonRepeirInInventory);
	}

	if(gProtect.m_MainInfo.DisableButtonOpenStore == 1)
	{
		SetCompleteHook(0xE9, 0x008369AF, &OffButtonOpenStore);
	}

	if(gProtect.m_MainInfo.DisableButtonInventoryExt == 1)
	{
		SetCompleteHook(0xE9, 0x00836A23, &OffButtonInventoryExt);
	}

	if(gProtect.m_MainInfo.DisableButtonQuestInStats == 1)
	{
		SetCompleteHook(0xE9, 0x0077EBB0, &OffButtonQuestInStats);
	}

	if(gProtect.m_MainInfo.DisableButtonPetsInStats == 1)
	{
		SetCompleteHook(0xE9, 0x0077EC59, &OffButtonPetsInStats);
	}

	if(gProtect.m_MainInfo.DisableButtonSkillMaster == 1)
	{
		SetCompleteHook(0xE9, 0x0077ECE2, &OffButtonSkillMasterInStats);
	}

	if(gProtect.m_MainInfo.DisableEffectRemake == 0)
	{
		gCreateEffect.Load();
	}

    if(gProtect.m_MainInfo.CustomInterfaceType == 1 || gProtect.m_MainInfo.CustomInterfaceType == 2) //97d s2
	{
		SetByte(0x005528A0, 0xC3);                      // [Fix] Remover Recuo PVP
		//MemorySet(0x008684F0, 0x90, 0x52);			//-> Disable FastMenu (Key U)
	}

	if(gProtect.m_MainInfo.DisableHelper == 1)
	{
		MemorySet(0x0085B6AC, 0x90, 0x05);			//-> Remove Helper Top Screen
		MemorySet(0x0085CC50, 0x90, 0x05);			//-> Disable Helper (Key Z)
		MemorySet(0x007D40A2, 0x90, 0x05);			//-> Disable Helper (Key Home)
	}

	if(gProtect.m_MainInfo.DisableSocketInvetori == 1)
	{
		RemoveSocket();
	}
	gPostInterface.Load();	//<-- post item

	gCItemSetOption.Load();

	SetCompleteHook(0xE9, 0x0071B1A3, 0x00720894); //-- Fix RF

	InitAttackSpeed(); //-- Fix Agilidad

	SetCompleteHook(0xE9, 0x0095DFBE, 0x0095DFD3); //-- Fix RF

	///FIX GM WING ON BLOOD CASTLE Zeus Update 16
   SetCompleteHook(0xE9,0x005880F0,0x0058811F); //Fix GM Wing on Blood Castle
   SetDword(0x00588444+3,4095);
   SetByte((0x00597691 + 4), 0x04); // Size Msg Global Zeus Update 16
   SetDword(0x007D0EF6+1,350); //POSIÇÃO Y GUILD MASTER BUTTONS PART1
   SetDword(0x007D0F5D+1,350); //POSIÇÃO Y GUILD MASTER BUTTONS PART1
   SetByte(0x007D0F05+2,35); //POSIÇÃO X GUILD MASTER BUTTONS PART1
   SetByte(0x007D0F6C+2,96); //POSIÇÃO X GUILD MASTER BUTTONS PART1
   SetDword(0x007D11DE+2,350); //POSIÇÃO Y GUILD MASTER BUTTONS PART2
   SetDword(0x007D1249+2,350); //POSIÇÃO Y GUILD MASTER BUTTONS PART2
   SetByte(0x007D11EE+2,35); //POSIÇÃO X GUILD MASTER BUTTONS PART2
   SetByte(0x007D1259+2,96); //POSIÇÃO X GUILD MASTER BUTTONS PART2
   MemorySet(0x007DB7B0, 0x90, 0x05); //Tirar Bolda Invertario
   MemorySet(0x007DB812, 0x90, 0x05); //Tirar Bolda Invertario
   MemorySet(0x007DB874, 0x90, 0x05); //Tirar Bolda Invertario
   MemorySet(0x007DB8E0, 0x90, 0x05); //Tirar Bolda Invertario
   MemorySet(0x007DB959, 0x90, 0x05); //Tirar Bolda Invertario
   MemorySet(0x007DB9A2, 0x90, 0x05); //Tirar Bolda Invertario
   MemorySet(0x007DBA20, 0x90, 0x05); //Tirar Bolda Invertario
   MemorySet(0x007DBA69, 0x90, 0x05); //Tirar Bolda Invertario
   SetDword(0x004D7A0E, 480); // Linha Preta Select Char
   SetByte(0x004D7A18, 0);    // Linha Preta Select Char
   SetDword(0x004D7179, 480); // Linha Preta Select Char
   SetByte(0x004D7183, 0);    // Linha Preta Select Char
   SetCompleteHook(0xE9, 0x008369FA, 0x00836A23); //Tirar Bau Expa?o
   SetCompleteHook(0xE9, 0x00856FAA, 0x00856FD2); //Tirar Bau Expa?o
   SetCompleteHook(0xE9, 0x007D36B8, 0x007D36BF); //Tirar Bau Expa?o
   SetCompleteHook(0xE9, 0x007D371E, 0x007D3725); //Tirar Bau Expa?o
   SetCompleteHook(0xE9, 0x007D3F9C, 0x007D3FA3); //Tirar Bau Expa?o
   //MENU D POSITION UP17
   SetByte((0x0078DDFF+2),37);         //POSITION X
   SetByte((0x0078DDF1+3),60);         //POSITION Y

   SetByte((0x00832C44+2),5);			 //BARRA AMARELA REDIMENSIONADA //ZEUS UP19
   //TELA M EM RESOLUÇÃO 800
   SetByte((0x00830C77+2),34);
   SetByte((0x00830C86+2),94);
   SetByte((0x00830C98+2),139);
   SetByte((0x00830C6A+3),148);				//LARGURA
   //TELA M EM RESOLUÇÃO 1024
   SetByte((0x00830CB6+2),29);
   SetByte((0x00830CC5+2),84);
   SetByte((0x00830CD4+2),124);
   SetByte((0x00830CA9+3),128);				//LARGURA
   SetByte((0x00830C31+2),30);
   //TELA M EM RESOLUÇÃO 1280 OK
   SetByte((0x00830CF2+2),25);
   SetByte((0x00830D01+2),69);
   SetByte((0x00830D10+1),104);
   SetByte((0x00830CE5+3),108);				//LARGURA
   SetDword((0x00830D49),500);				//DIMENSIONAMENTO DO MOVE //ZEUS UP19
   SetDword((0x00832443+2),220);			//COMPRIMENTO DO MOVE220
   //TAMANHO DO MOVE - OTIMIZAÇÃO DE TAMANHO
   //SetByte((0x00830D64+2),33);
   //SetByte((0x00830D67+2),55);						//REGULA O TAMANHO DO MOVE
   //SetByte((0x00830E55+2),33);
   //SetByte((0x0083245B+2),33);
   //SetByte((0x00830ED3+2),33);
   //SetByte((0X00830FD6+2),33);
   //RETIRA IMAGENS DO MOVE         //ZEUS UP19
	MemorySet(0x008325D4, 0x90, 0x05);
	MemorySet(0x00832639, 0x90, 0x05); 
	MemorySet(0x0083268F, 0x90, 0x05);
	MemorySet(0x008326D7, 0x90, 0x05);
	MemorySet(0x00832757, 0x90, 0x05);
	MemorySet(0x00832794, 0x90, 0x05); 
	//
	SetCompleteHook(0xE9,0x007D399F,0x007D39D8);  //ZEUS UP18 LETRA (T)
    SetCompleteHook(0xE9,0x0077EB65,0x0077EC0E);
	MemorySet(0x00944E50,0x90,0x15E);    //Remove Shop X(MonsterZone)/ZEUS UP20
    //MemorySet(0x00661700,0x90,0x145);  //Remove Shop X(SafeZone)
    MemorySet(0x0057BF78, 0x90, 0x05);   //remover lazer azul //ZEUS UP20
    MemorySet(0x0057C125, 0x90, 0x05);   //remover lazer azul //ZEUS UP20
    MemorySet(0x0057BE80, 0x90, 0x05);   //remover lazer azul //ZEUS UP20
    MemorySet(0x0057C057, 0x90, 0x05);   //remover lazer azul //ZEUS UP20
    SetByte(0x004D128F, 0x1D);           // Fix Resolucion 640x480


   if(*(DWORD*)(MAIN_SCREEN_STATE) == ObjState::SwitchCharacter)
   {
	   SetDword(0x004D7179, 480);
		SetByte(0x004D7183, 0);
   }

        SetDword(0x009218D9 + 1, (7168 + 13)); //Pet
		SetDword(0x0093953E + 2, (7168 + 13));
		SetDword(0x0093AAAE + 2, (7168 + 13));
		SetDword(0x0093EDB2 + 2, (7168 + 13));
		SetDword(0x0095F31A + 1, (7168 + 13));
		SetDword(0x0095F46D + 2, (7168 + 13));

  if (gProtect.m_MainInfo.CustomPetSafe == 1)
	  {
		if (MinimizarFenrir == 0)
		{
			//Fenrir SafeZone (Struct +14)
			SetByte(0x00501955 + 2, 0);
			MemorySet(0x0054EAE1, 0x90, 0x06);//SetPlayerStop
			MemorySet(0x0054FC92, 0x90, 0x06);//SetPlayerWalk

			//Horse SafeZone (Struct +14)
			SetByte(0x00502269 + 2, 0);
			MemorySet(0x0054ECA3, 0x90, 0x6);
			MemorySet(0x0054FCEC, 0x90, 0x6);

			//Dinorant e Uniria (Struct +14)
			SetByte(0x00502C4B + 2, 0);
			MemorySet(0x0054ED6D, 0x90, 0x6);
			MemorySet(0x0054FD5F, 0x90, 0x6);
			MemorySet(0x0054FE3E, 0x90, 0x6);

			//Raven (Struct +14)
			SetByte(0x00502C44 + 2, 0);
			MemorySet(0x0054FD23, 0x90, 0x6);

		}
		if (MinimizarFenrir == 1)
		{
		}
	}

    
   //MuunSystem
   if(gProtect.m_MainInfo.DisableMuunSystem == 1) 		//-> Disable MuunSystem ZEUS UP20		
	{
   	g_MuunSystem.Init();
	g_pRenderText.Init();
	//g_Option.Init();  // COMENTADO: Archivo huérfano MuunOption.h no disponible
	//GInfo.IniInformation();
   }

   if (gProtect.m_MainInfo.CustomInterfaceType == 1 || gProtect.m_MainInfo.CustomInterfaceType == 2)   //ZEUS UP23 party
	{
		LoadPartySystem();    
	}

}




BOOL APIENTRY DllMain(HMODULE hModule,DWORD ul_reason_for_call,LPVOID lpReserved) // OK
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			hins = (HINSTANCE)hModule;
			gController.Instance = hModule;
			gTrayMode.Init(hModule);
		}
			break;
		case DLL_PROCESS_DETACH:
		//{
		//	UnhookWindowsHookEx(m_Keyboard1);
		//	UnhookWindowsHookEx(m_Keyboard2);
		//}
			break;
		case DLL_THREAD_ATTACH:
			break;
			case DLL_THREAD_DETACH:
			break;
	}

	return 1;
}

