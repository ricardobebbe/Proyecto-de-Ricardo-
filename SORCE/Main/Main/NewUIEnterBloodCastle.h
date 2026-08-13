#pragma once
#define MAX_ENTER_GRADE		8
typedef struct
{
/*+00*/   char	byClass[12];
/*+12*/   DWORD	*m_pNewUIMng;
/*+16*/   POINT	m_Pos;
/*+24*/   POINT	m_BtnEnterStartPos;
/*+32*/   POINT	m_EnterUITextPos;
/*+40*/   char	m_BtnExit[172];
/*+212*/  char	m_BtnEnter[8][172];
/*+1588*/  int	m_iBloodCastleLimitLevel[MAX_ENTER_GRADE*2][2];
/*+1716*/ int	m_iNumActiveBtn;
/*+1720*/ DWORD	m_dwBtnTextColor[2];
}CBloodCastle;


#define C_EnterBloodCastle			((DWORD *(__thiscall*)(DWORD *thisa))0x00873FF0)

class CNewUIEnterBloodCastle
{
public:
	CNewUIEnterBloodCastle(void);
	virtual ~CNewUIEnterBloodCastle(void);
public:
	void Init();
	static DWORD * __thiscall EnterBloodCastle(DWORD *a1);
};

extern CNewUIEnterBloodCastle g_pNewEnterBloodCastle;