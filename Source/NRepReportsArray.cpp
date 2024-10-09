#include "stdafx.h"
#include "NEvalExp.h"
#include "direct.h"
#include "NCUnit.h"
#include "NCMVersion.h"
#include "NCMProject.h"
#include "nrepreportsarray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NRepReportsArray::NRepReportsArray(void)
{
	Eval=NULL;
}

NRepReportsArray::~NRepReportsArray(void)
{
	if(Eval)
		delete Eval;
}

bool NRepReportsArray::Init(CString FPath)
{
	CStdioFile F;
	int i;
	CString str,reptxt;
	//*******************************************************
	//если файл не открылся по какойлибо причине
	//*******************************************************
	if(!F.Open(FPath,CFile::typeText|CFile::modeRead|CFile::modeCreate|CFile::modeNoTruncate))
	{
		CStringArray temp;
		int k;
		str.LoadString(IDS_REPORT_DEF);
		k=FPath.Find(str);
		if(k==-1)
			return false;
		str=FPath.Left(k);
		while(_chdir(str)==-1)
		{
			k=str.ReverseFind('\\');
			if(k==-1)
				return false;
			temp.InsertAt(0,str.Right(str.GetLength()-k));
			str.Delete(k,str.GetLength()-k);
		}
		for(i=0;i<temp.GetSize();i++)
		{
			str.Append(temp[i]);
			if(_mkdir(str)==-2)
				return false;
		}
		if(!F.Open(FPath,CFile::typeText|CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate))
			return false;
		F.Close();
		return true;
	}
	//*******************************************************
	bool flag=false;
	while(F.ReadString(str))
	{
		if(str=="<$Report$>")
		{
			if(!flag)
				flag=true;
			else
				flag=false;
		}
		if(str=="<$End$>")
		{
			flag=false;
			NRepReport rep;
			if(!rep.Init(reptxt,FPath))
			{
				AfxMessageBox("Файл описания отчётов: "+FPath+" - содержит ошибки!");
				continue;
			}
			reptxt="";
			if(FindReport(rep.GetName())==-1)
				RepArray.Add(rep);
		}
		if(flag)
			reptxt+=str;
	}//while
	F.Close();
	if(flag)
		return false;
	return true;
}

int NRepReportsArray::FindReport(CString Name)
{
	int i;
	for(i=0;i<RepArray.GetSize();i++)
	{
		if(Name==RepArray[i].GetName())
			return i;
	}
	return -1;
}

NRepReport& NRepReportsArray::operator[](int i)
{
	return RepArray[i];
}

bool NRepReportsArray::SaveReportsDefinition(CString FPath)
{
	CStdioFile file;
	CString str;
	//*******************************************************
	//если файл не открылся по какойлибо причине
	//*******************************************************
	if(!file.Open(FPath,CFile::typeText|CFile::modeWrite|CFile::modeCreate))
	{
		CStringArray temp;
		int k,i;
		str.LoadString(IDS_REPORT_DEF);
		k=FPath.Find(str);
		if(k==-1)
			return false;
		str=FPath.Left(k);
		while(_chdir(str)==-1)
		{
			k=str.ReverseFind('\\');
			if(k==-1)
				return false;
			temp.InsertAt(0,str.Right(str.GetLength()-k));
			str.Delete(k,str.GetLength()-k);
		}
		for(i=0;i<temp.GetSize();i++)
		{
			str.Append(temp[i]);
			if(_mkdir(str)==-2)
				return false;
		}
		if(!file.Open(FPath,CFile::typeText|CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate))
			return false;
		//F.Close();
		//return true;
	}//
	/*if(!file.Open(FPath,CFile::typeText|CFile::modeCreate|CFile::modeReadWrite))
		return false;*/
	int i,j;
	for(i=0;i<RepArray.GetSize();i++)
	{
		CStringArray sarr;
		str="\n<$Report$>\n"+RepArray[i].GetName();
		file.WriteString(str);
		str="\n<$Condition$>\n"+RepArray[i].GetCondition();
		file.WriteString(str);
		str="\n<$Description$>\n"+RepArray[i].GetDescription();
		file.WriteString(str);
		str="\n<$Filds$>\n";
		RepArray[i].GetTabs(sarr);
		if(sarr.GetSize()>0)
			str+=sarr[0];
		for(j=1;j<sarr.GetSize();j++)
			str+="|"+sarr[j];
		file.WriteString(str);
		str="\n<$End$>\n";
		file.WriteString(str);

	}
	file.Close();
	return true;
}

void NRepReportsArray::RemoveExtraReps(void)
{
	int i=0;
	while(i<RepArray.GetSize())
	{
		if(RepArray[i].GetMode()==0)
			RepArray.RemoveAt(i);
		else
			i++;
	}
}

void NRepReportsArray::RemoveAll(void)
{
	RepArray.RemoveAll();
}

int NRepReportsArray::GetSize(void)
{
	return int(RepArray.GetSize());
}

void NRepReportsArray::RemoveAt(int i)
{
	RepArray.RemoveAt(i);
}

void NRepReportsArray::AddReport(NRepReport nrep)
{
	RepArray.Add(nrep);
}

void NRepReportsArray::Evaluate()
{
	bool NeedWork = false;
	for(int k = 0; k < RepArray.GetSize(); ++k)
		if(RepArray[k].GetMode() == 1)
			NeedWork = true;
	if(!NeedWork || Eval == NULL)
		return;
	double t;
	CString str, deb;
	str = NCM_PROJECT.GetSettingsPath();
	str += _T("\\temp");
	if(_chdir(str) == -1)
	{
		if(_mkdir(str) == -1)
			return;
	}
	for(int i = 0; i < RepArray.GetSize(); ++i)
	{
		if(RepArray[i].GetMode() == 1)
		{
			deb=RepArray[i].GetCondition();
			if(Eval->Evaluate(deb, &t))
			{
				if(t == 1)
					RepArray[i].AddNote(Eval);
			}
		}//if mode 1
	}//for
}

void NRepReportsArray::SetUnit(NCUnit* NUnit)
{
	delete Eval;
	Eval=new NEvalExp(NUnit);
}
