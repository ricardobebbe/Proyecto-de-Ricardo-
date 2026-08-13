#include "StdAfx.h"
#include "NewsBoard.h"
#include "user.h"
#include "GameMain.h"
#include "ServerInfo.h"
#include "Path.h"
#include "MemScript2.h"
#include "Log.h"
#include "Util.h"

NewsBoard g_NewsBoard;

NewsBoard::NewsBoard()
{
    this->Init();
}

NewsBoard::~NewsBoard()
{
}

void NewsBoard::Init()
{
    this->m_LoadedCount = 0;

    for( int i = 0; i < MAX_NEWS_LIST; i++ )
    {
        this->m_Data[i].Title.Date[0]	= 0;
        this->m_Data[i].Title.Time[0]	= 0;
        this->m_Data[i].Title.Text[0]	= 0;
        this->m_Data[i].Text[0]			= 0;
    }
}

void NewsBoard::ReadListData(char * File)
{
	CMemScript2* lpScript = new(std::nothrow) CMemScript2;

	if(lpScript == 0)
	{
		return;
	}

	if(lpScript->SetBuffer(File) == 0)
	{
		delete lpScript;
		return;
	}

	try
	{
		while(true)
		{
			if(lpScript->GetToken() == 2)
			{
				break;
			}

			if(this->m_LoadedCount >= MAX_NEWS_LIST)
				break;

			memcpy(this->m_Data[this->m_LoadedCount].Title.Date, lpScript->GetString(),   11);
			memcpy(this->m_Data[this->m_LoadedCount].Title.Time, lpScript->GetAsString(), 6);
			memcpy(this->m_Data[this->m_LoadedCount].Title.Text, lpScript->GetAsString(), 69);
			memcpy(this->m_Data[this->m_LoadedCount].Text,       lpScript->GetAsString(), 1023);

			this->m_LoadedCount++;			
		}
	}
	catch(...)
	{
		MessageBox(0,File,"Error reading file",0);
	}

	delete lpScript;

    //LogAddTD("[NewsSystem] [%d] news loaded from list", this->m_LoadedCount);
}
// -------------------------------------------------------------------------------

void NewsBoard::Load()
{
    this->Init();

	//this->ReadListData(gPath.GetFullPath("..\\Data\\Custom\\Painel Noticia\\NewsList.txt"));
	this->ReadListData(gPath.GetFullPath("Custom\\Painel Noticia\\NewsList.txt"));

}

void NewsBoard::PHeadSetW(LPBYTE lpBuf, BYTE head,  int size) 
{
	lpBuf[0] = 0xC2;
	lpBuf[1]= SET_NUMBERHB(size);
	lpBuf[2]= SET_NUMBERLB(size);
	lpBuf[3]= head;
}

void NewsBoard::OpenMain(LPOBJ lpUser)
{
    if( lpUser->Connected != OBJECT_ONLINE )
    {
        return;
    }

    NEWS_ANS_TITLES pAnswer = { 0 };

	this->PHeadSetW((LPBYTE)&pAnswer, 0xFD, sizeof(NEWS_ANS_TITLES));

    pAnswer.RealCount = this->m_LoadedCount;

    for( int i = 0; i < this->m_LoadedCount; i++ )
    {

        strcpy(pAnswer.Titles[i].Date, this->m_Data[i].Title.Date);
        strcpy(pAnswer.Titles[i].Time, this->m_Data[i].Title.Time);
        strcpy(pAnswer.Titles[i].Text, this->m_Data[i].Title.Text);
    }

	DataSend(lpUser->Index, (LPBYTE)&pAnswer, sizeof(NEWS_ANS_TITLES));
}

void NewsBoard::OpenItem(LPOBJ lpUser, NEWS_REQ_NEWS * Request)
{
    if( lpUser->Connected != OBJECT_ONLINE )
    {
        return;
    }

    if( Request->ID < 0 || Request->ID >= MAX_NEWS_LIST )
    {
        return;
    }

    NEWS_ANS_NEWS pAnswer = { 0 };

	this->PHeadSetW((LPBYTE)&pAnswer, 0xFE, sizeof(NEWS_ANS_TITLES));

    pAnswer.ID = Request->ID;

    strcpy(pAnswer.News.Title.Date, this->m_Data[Request->ID].Title.Date);
    strcpy(pAnswer.News.Title.Time, this->m_Data[Request->ID].Title.Time);
    strcpy(pAnswer.News.Title.Text, this->m_Data[Request->ID].Title.Text);
    strcpy(pAnswer.News.Text, this->m_Data[Request->ID].Text);

    DataSend(lpUser->Index, (LPBYTE)&pAnswer, sizeof(NEWS_ANS_TITLES));
}