// NCCommandLineInfo.cpp: implementation of the NCCommandLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCCommandLineInfo.h"
#include "NCMDoc.h"
#include "NCUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum cmdState{
   UnitOpen = 1,
   ProgrammOpen = 2,
   ProgrammAndUnitOpen = 3,
   PrtOpen = 4,
   Nothing = 0
};

cmdState CurState;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
NCCommandLineInfo::NCCommandLineInfo()
{
	m_bUnitOpen = false;
	m_bPrtOpen = false;
	m_bStartInHostMode = false;
	m_bProgrammOpen = false;
	m_strUnitName = "";
	m_strProgramsName.RemoveAll();
	CurState = Nothing;
}

NCCommandLineInfo::~NCCommandLineInfo()
{

}

void NCCommandLineInfo::ParseParam (LPCTSTR lpszParam,BOOL bFlag,BOOL bLast)
{
	if (bFlag)
	{
		if (lstrcmp(lpszParam, "prt:") == 0)
		{
			m_bPrtOpen = true;
			CurState = PrtOpen;
		}
		else if (lstrcmp(lpszParam, "u:") == 0)
		{
			m_bUnitOpen = true;
			CurState = UnitOpen;
		}
		else if (lstrcmp(lpszParam, "pr:") == 0)
		{
			m_bProgrammOpen = true;
			CurState = ProgrammOpen;
		}
		else if (lstrcmp(lpszParam, "pru:") == 0)
		{
			m_bUnitOpen = true;
			m_bProgrammOpen = true;
			CurState = ProgrammAndUnitOpen;
		}
		else if (lstrcmp(lpszParam, "host") == 0)
		{
			m_bStartInHostMode = true;
			CurState = Nothing;
		}
		else
			CurState = Nothing;
	}
	else
	{
		if (m_bProgrammOpen && CurState == ProgrammAndUnitOpen)
		{
			CString str = lpszParam;
			m_strProgramsName.Add(str);
			int i;
			for(i=str.GetLength()-1; i >= 0 && str[i] != '.'; i--);
			if(i >= 0)
				str.Delete(0,i+1);
			m_strUnitName = str;
		}
		if (m_bUnitOpen && m_strUnitName.IsEmpty() && CurState == UnitOpen)
			m_strUnitName = lpszParam;
		bool f = false;
		if (m_bUnitOpen)
		{	
			for (int i=4; i>=1; i-- )
				if (m_strUnitName.Mid(m_strUnitName.GetLength()-i,1) == _T("."))
				{
					f = true;
					i = 1;
				}

			if (!f)
				m_strUnitName = m_strUnitName + ".sm3";
		}
		
		if (m_bProgrammOpen && CurState == ProgrammOpen)
			m_strProgramsName.Add(lpszParam);
		if (CurState == Nothing)
			CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);	
		if (m_bPrtOpen && CurState == PrtOpen)
		{
			m_strPrtName = CString(lpszParam);
			CurState = Nothing;
		}

	}
	
}