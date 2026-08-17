// ResetTable.cpp: implementation of the CResetTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResetTable.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CResetTable gResetTable;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResetTable::CResetTable() // OK
{
	this->m_ResetTableInfo.clear();
}

CResetTable::~CResetTable() // OK
{

}

void CResetTable::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_ResetTableInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			RESET_TABLE_INFO info;

			info.MinReset = lpMemScript->GetNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.Level[0] = lpMemScript->GetAsNumber();

			info.Level[1] = lpMemScript->GetAsNumber();

			info.Level[2] = lpMemScript->GetAsNumber();

			info.Level[3] = lpMemScript->GetAsNumber();

			info.Money[0] = lpMemScript->GetAsNumber();

			info.Money[1] = lpMemScript->GetAsNumber();

			info.Money[2] = lpMemScript->GetAsNumber();

			info.Money[3] = lpMemScript->GetAsNumber();

			info.Point[0] = lpMemScript->GetAsNumber();

			info.Point[1] = lpMemScript->GetAsNumber();

			info.Point[2] = lpMemScript->GetAsNumber();

			info.Point[3] = lpMemScript->GetAsNumber();

			this->m_ResetTableInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CResetTable::GetResetLevel(LPOBJ lpObj) // OK
{
	// FIX: AccountLevel viene de la base de datos y se usa como indice de
	// arrays de tamano MAX_ACCOUNT_LEVEL. Un valor corrupto o fuera de rango
	// leia memoria ajena. Se acota antes de usarlo.
	int accountLevel = ((lpObj == 0 || lpObj->AccountLevel < 0 || lpObj->AccountLevel >= MAX_ACCOUNT_LEVEL) ? 0 : lpObj->AccountLevel);

	for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Level[accountLevel] == -1)
			{
				return gServerInfo.m_CommandResetLevel[accountLevel];
			}
			else
			{
				return it->Level[accountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetLevel[accountLevel];
}

int CResetTable::GetResetMoney(LPOBJ lpObj) // OK
{
	// FIX: AccountLevel viene de la base de datos y se usa como indice de
	// arrays de tamano MAX_ACCOUNT_LEVEL. Un valor corrupto o fuera de rango
	// leia memoria ajena. Se acota antes de usarlo.
	int accountLevel = ((lpObj == 0 || lpObj->AccountLevel < 0 || lpObj->AccountLevel >= MAX_ACCOUNT_LEVEL) ? 0 : lpObj->AccountLevel);

	for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Money[accountLevel] == -1)
			{
				return gServerInfo.m_CommandResetMoney[accountLevel];
			}
			else
			{
				return it->Money[accountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetMoney[accountLevel];
}

int CResetTable::GetResetPoint(LPOBJ lpObj) // OK
{
	// FIX: AccountLevel viene de la base de datos y se usa como indice de
	// arrays de tamano MAX_ACCOUNT_LEVEL. Un valor corrupto o fuera de rango
	// leia memoria ajena. Se acota antes de usarlo.
	int accountLevel = ((lpObj == 0 || lpObj->AccountLevel < 0 || lpObj->AccountLevel >= MAX_ACCOUNT_LEVEL) ? 0 : lpObj->AccountLevel);

	int point = 0;

	for(int n=1;n <= lpObj->Reset;n++)
	{
		int AddPoint = gServerInfo.m_CommandResetPoint[accountLevel];

		for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
		{
			if(n >= it->MinReset && n <= it->MaxReset)
			{
				if(it->Point[accountLevel] == -1)
				{
					AddPoint = gServerInfo.m_CommandResetPoint[accountLevel];
					break;
				}
				else
				{
					AddPoint = it->Point[accountLevel];
					break;
				}
			}
		}

		point += AddPoint;
	}

	return point;
}
