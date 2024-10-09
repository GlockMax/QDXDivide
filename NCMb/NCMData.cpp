// NCMData.cpp: implementation of the NCMData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCMVersion.h"
#include "InfoProgram.h"
#include "InfoTool.h"
#include "NCMData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NCMData::NCMData()
{

}

NCMData::~NCMData()
{
	for(int ip=0;ip<ProgArray.GetSize();++ip)
		delete ((InfoProgram *)ProgArray[ip]);
	for(int it=0;it<ToolArray.GetSize();++it)
		delete ((InfoTool *)ToolArray[it]);
}

void NCMData::Serialize(CArchive &ar)
{
	SerializeElements(ar,&UnitFile,1);
	SerializeElements(ar,&UnitName,1);
	SerializeElements(ar,&UnitType,1);
	SerializeElements(ar,&UnitManufacturer,1);
	SerializeElements(ar,&UnitOperation,1);
	SerializeElements(ar,&UnitAxes,1);
	Stock.Serialize(ar);
	ProgArray.Serialize(ar);
	for(int i=0;i<ProgArray.GetSize();++i)
	{
		if(ar.IsLoading())
		{
			ProgArray[i] = new InfoProgram;
		}
		((InfoProgram *)ProgArray[i])->Serialize(ar);
	}
	ToolArray.Serialize(ar);
	for(int j=0;j<ToolArray.GetSize();++j)
	{
		if(ar.IsLoading())
		{
			ToolArray[j] = new InfoTool;
		}
		((InfoTool *)ToolArray[j])->Serialize(ar);
	}
}

bool NCMData::LoadNCMFile(const CStringA& fname)
{
	CFile f;
	if(!f.Open(fname,CFile::typeBinary | CFile::modeRead ))
		return false;
	CArchive ar(&f,CArchive::load);

// Copied from CNCMDoc.Serialize
	CStringA Title("NCM File.");
	CStringA str = Title;
	ar>>str;
	ar>>str;
	if(str.Left(4) != "Ver")
	{
//		AfxMessageBox(IDS_FILE_ERR01);
		ar.Close();
		f.Close();
		return false;
	}
	else
	{
		ar>>str;
	}
	if(atof(str) > atof(NCMVersion))
	{
//		AfxMessageBox("Файл создан более новой версией NCManager");
		ar.Close();
		f.Close();
		return false;
	}
	if(atof(str) <= 2.4)
	{
//		AfxMessageBox("Файл создан слишком старой версией NCManager");
		ar.Close();
		f.Close();
		return false;
	}
	Serialize(ar);

	ar.Close();
	f.Close();
	return true;
}

const CStringA NCMData::GetTimeFromDouble(double Val)
{
	CTimeSpan t((unsigned long)Val);
	CStringA s(t.Format(":%M:%S"));
	CStringA h;
	h.Format("%d",t.GetTotalHours());
	return h + s;
}

bool NCMData::GetText(int FormatIDAll, CStringA &TextAll, int FormatIDProgs, CStringA &TextProgs,int FormatIDTools, CStringA &TextTools)
{
	double TimeAll = 0., WorkLengthAll = 0., FastLengthAll = 0., WorkTimeAll = 0., FastTimeAll = 0.;

	for(int ip = 0; ip < ProgArray.GetSize(); ++ip)
	{
		InfoProgram *pInfoProgram = (InfoProgram *)ProgArray[ip];
		CStringA Name = pInfoProgram->FileName;
		int k = Name.ReverseFind('\\');
		Name.Delete(0, k + 1);
		CStringA str;
		str.Format(FormatIDProgs, 
			Name,
			GetTimeFromDouble(pInfoProgram->Time),
			GetTimeFromDouble(pInfoProgram->WorkTime),
			GetTimeFromDouble(pInfoProgram->FastTime),
			GetTimeFromDouble(pInfoProgram->Time - pInfoProgram->WorkTime - pInfoProgram->FastTime),
			pInfoProgram->WorkLength,
			pInfoProgram->FastLength);
		TextProgs += str;
		TimeAll += pInfoProgram->Time;
		WorkLengthAll += pInfoProgram->WorkLength;
		FastLengthAll += pInfoProgram->FastLength;
		WorkTimeAll += pInfoProgram->WorkTime;
		FastTimeAll += pInfoProgram->FastTime;
	}
	TextAll.Format(FormatIDAll, GetTimeFromDouble(TimeAll), GetTimeFromDouble(WorkTimeAll), GetTimeFromDouble(FastTimeAll), GetTimeFromDouble(TimeAll - FastTimeAll - WorkTimeAll), WorkLengthAll, FastLengthAll);
	for(int ip = 0; ip < ToolArray.GetSize(); ++ip)
	{
		InfoTool *pInfoTool = (InfoTool *)ToolArray[ip];
		CStringA str;
		str.Format(FormatIDTools,
			pInfoTool->Pos,
			pInfoTool->Name,
			GetTimeFromDouble(pInfoTool->Time),
			GetTimeFromDouble(pInfoTool->WorkTime),
			GetTimeFromDouble(pInfoTool->FastTime),
			GetTimeFromDouble(pInfoTool->Time - pInfoTool->WorkTime - pInfoTool->FastTime),
			pInfoTool->WorkLength,
			pInfoTool->FastLength);
		TextTools += str;
	}

	return true;
}
