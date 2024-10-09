// NCCommandLineInfo.h: interface for the NCCommandLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCCOMMANDLINEINFO_H__7F4925C0_4553_11D5_AB10_008048A96FFB__INCLUDED_)
#define AFX_NCCOMMANDLINEINFO_H__7F4925C0_4553_11D5_AB10_008048A96FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class NCCommandLineInfo : public CCommandLineInfo   
{
public:
	NCCommandLineInfo();
	virtual ~NCCommandLineInfo();

	void ParseParam(LPCTSTR lpszParam,BOOL bFlag,BOOL bLast );

//	enum cmdState CurState;
	bool m_bUnitOpen;
	bool m_bProgrammOpen;
	bool m_bPrtOpen;
	bool m_bStartInHostMode;
	CString m_strUnitName;
	CString m_strPrtName;
	CArray<CString,CString> m_strProgramsName;
};

#endif // !defined(AFX_NCCOMMANDLINEINFO_H__7F4925C0_4553_11D5_AB10_008048A96FFB__INCLUDED_)
