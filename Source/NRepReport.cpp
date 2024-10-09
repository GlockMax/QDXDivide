#include "stdafx.h"
#include "NCMVersion.h"
#include "NEvalExp.h"
#include "NCMProject.h"
#include "nrepreport.h"

NRepReport::NRepReport(void)
{
	m_Init=false;
}

NRepReport::~NRepReport(void)
{
}

bool NRepReport::Init(CString def, const CString& FPath)
{
	int k;
	CString temp;
	m_Mode=0;
	m_Init=false;
	m_Path="";
	k=def.Find("<$Report$>");
	if(k==-1)
		return false;
	def.Delete(0,10+k);
	k=def.Find("<$Condition$>");
	if(k==-1)
		return false;
	m_Name=def.Left(k);
	def.Delete(0,13+k);
	k=def.Find("<$Description$>");
	if(k==-1)
		return false;
	m_Condition=def.Left(k);
	def.Delete(0,k+15);
	k=def.Find("<$Filds$>");
	if(k==-1)
		return false;
	m_Description=def.Left(k);
	def.Delete(0,k+9);
	m_Init=true;
	m_Path=FPath;
	while (def.GetLength()>0)
	{
		k=def.Find("|");
		if(k==-1)
		{
			if(def=="")
				return true;
			Tabs.Add(def);
			break;
		}
		Tabs.Add(def.Left(k));
		def.Delete(0,k+1);
	}
	return true;
}

bool NRepReport::SetMode(int m)
{
	if(m>2||m<0)
		return false;
	m_Mode=m;
	return true;
}

int NRepReport::GetMode(void)
{
	return m_Mode;
}

CString NRepReport::GetCondition(void)
{
	return m_Condition;
}

CString NRepReport::GetName(void)
{
	return m_Name;
}

CString NRepReport::GetPath(void)
{
	return m_Path;
}

void NRepReport::GetTabs(CStringArray& StrArr)
{
	int i;
	StrArr.RemoveAll();
	for(i=0;i<Tabs.GetSize();i++)
		StrArr.Add(Tabs[i]);
}



NRepReport::NRepReport(NRepReport& rep)
{
	m_Name=rep.m_Name;
	m_Condition=rep.m_Condition;
	m_Description=rep.m_Description;
	m_Init=rep.m_Init;
	m_Mode=rep.m_Mode;
	m_Path=rep.m_Path;
	for(int i=0;i<rep.Tabs.GetSize();i++)
	{
		Tabs.Add(rep.Tabs[i]);
	}
}

NRepReport& NRepReport::operator=(const NRepReport&  rep)
{
	m_Name=rep.m_Name;
	m_Condition=rep.m_Condition;
	m_Description=rep.m_Description;
	m_Init=rep.m_Init;
	m_Mode=rep.m_Mode;
	m_Path=rep.m_Path;
	Tabs.RemoveAll();
	for(int i=0;i<rep.Tabs.GetSize();i++)
	{
		Tabs.Add(rep.Tabs[i]);
	}
	return *this;
}

CString NRepReport::GetDescription(void)
{
	return m_Description;
}

void NRepReport::SetName(const CString& Name)
{
	m_Name=Name;
}

void NRepReport::SetCondition(const CString& Cond)
{
	m_Condition=Cond;
}

void NRepReport::SetDescription(const CString& Descr)
{
	m_Description=Descr;
}

void NRepReport::SetTabs(CStringArray& str)
{
	Tabs.RemoveAll();
	int i;
	CString ts;
	for(i=0;i<str.GetSize();i++)
	{
		ts=str[i];
		Tabs.Add(ts);
	}
}

void NRepReport::AddNote(NEvalExp* Eval)
{
	CStdioFile file;
	double t;
	CString str,temp,deb;
	str = NCM_PROJECT.GetPrototypesPath();
	str += _T("temp\\") + m_Name + _T(".tmp");
	BOOL res;
	res=file.Open(str,CFile::typeText|CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	if(!res)
		return;
	file.SeekToEnd();
	str = _T("");
	for(int i = 0; i < Tabs.GetSize(); ++i)
	{
		t = 0;
		deb = Tabs[i];
		Eval->Evaluate(deb, &t);
		temp.Format(_T("%3f"), t);
		str += temp + _T("|");
	}
	str += _T("\n");
	file.WriteString(str);
	file.Close();
}