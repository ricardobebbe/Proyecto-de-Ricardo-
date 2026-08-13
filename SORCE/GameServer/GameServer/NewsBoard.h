#pragma once

#include "Protocol.h"

#define MAX_NEWS_LIST 12

struct PBMSG_HEAD4
{
public:
	void set ( LPBYTE lpBuf, BYTE head, BYTE size)
	{
		lpBuf[0] = 0xC1;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};
	void setE ( LPBYTE lpBuf, BYTE head, BYTE size)
	{
		lpBuf[0] = 0xC3;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};
	BYTE c;
	BYTE size;
	BYTE headcode;
};

#pragma pack(push, 1)
struct NEWS_TITLE
{
    char	Date[12];
    char	Time[7];
    char	Text[70];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NEWS_DATA
{
    NEWS_TITLE Title;
    char	Text[1024];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NEWS_ANS_TITLES
{
    PWMSG_HEAD h;
    BYTE	RealCount;
    NEWS_TITLE Titles[MAX_NEWS_LIST];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NEWS_REQ_NEWS
{
    PBMSG_HEAD4	h;
    BYTE		ID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NEWS_ANS_NEWS
{
    PWMSG_HEAD h;
    BYTE	ID;
    NEWS_DATA News;
};
#pragma pack(pop)

class NewsBoard
{
public:
    NewsBoard();
    ~NewsBoard();

    void	Init();
    void	ReadListData(char * File);
    void	Load();
	void	PHeadSetW(LPBYTE lpBuf, BYTE head,  int size);
    void	OpenMain(LPOBJ lpUser);
    void	OpenItem(LPOBJ lpUser, NEWS_REQ_NEWS * Request);

private:
    int		m_LoadedCount;
    NEWS_DATA m_Data[MAX_NEWS_LIST];

};
extern NewsBoard g_NewsBoard;