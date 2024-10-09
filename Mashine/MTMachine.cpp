// MTMachineTool.cpp: implementation of the MTMachineTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTPartAxis.h"
#include "MTPart.h"
#include "MTRelocateAxis.h"
#include "NCMProject.h"
#include "PropList.h"
#include "MTConfig.h"
#include "MTMachine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(MTMachine, MTBase, 0)
MTMachine::MTMachine()
: Visible(true), Part(*this)
{

	SetDrawParams();
	DrawSharpEdges = true;
	DrawSmoothSurf = true;
}

MTMachine::~MTMachine()
{
	MTBase::RemoveAll();
	Part.RemoveAll();
}

void MTMachine::RemoveAll()
{
	Part.RemoveAll();// Should be the first to ensure MTMachine correctness during parts deleting
	MTBase::RemoveAll();
}//void MTMachine::RemoveAll()

void MTMachine::Serialize(CArchive &ar)
{		
}

const int MTMachine::GetToolTurret(const CString& ToolName)
{
	int iTool = ToolName.Find(_T("tool"));

	CString suffix = ToolName.Mid(iTool + 4);
	if(suffix.GetLength() == 0)
		return 0;
	
	if(suffix[0] == _T(':'))
		return 0;

	return _ttoi(suffix);
}//const int MTMachine::GetToolTurret(CString ToolName)

const int MTMachine::GetToolAttachement(const CString& ToolName)
{
	int iipos = ToolName.Find(_T(":"));
	if(iipos == -1)
		return 0;
	if(iipos == (ToolName.GetLength()-1))
		return -1;
	CString sAtt = ToolName.Mid(iipos+1);

	int iAtt = _ttoi(sAtt);
	return iAtt;
}//const int MTMachine::GetToolAttachement(CString ToolName)

const int MTMachine::GetToolPosition(const CString& ToolName)
{
	int iPos = _ttoi(ToolName);

	return iPos;
}
const CString MTMachine::GetStockAxisName(int StPos)
{
	if (StPos < 0 )
		return CString("");

	CString key("stock");
	if (StPos > 0)
	{
		CString n;
		n.Format("%d", StPos);
		key += n;
	}
	return key;
}
const CString MTMachine::GetStockMovedAxisName(int StPos)
{
	if (StPos < 0)
		return CString("");

	CString key("stock_moved");
	if (StPos > 0)
	{
		CString n;
		n.Format("%d", StPos);
		key += n;
	}
	return key;
}
//const int MTMachine::GetToolPosition(CString ToolName)

MTPartAxis * MTMachine::GetToolAxis(int Tur, int Att) const
{
	return GetCurConfig().GetToolAxis(Tur, Att);
}

BPoint MTMachine::GetToolVector(int Tur, int Att) const
{
	int i = GetCurConfig().GetAxisIndex(MTConfig::GetToolAxisName(Tur, Att));
	if( i < 0)
		return BPoint(0., 0., 0., -1.);
	MTAxis *pAxis = GetCurConfig()[i];
	if(!pAxis->IsKindOf(RUNTIME_CLASS(MTPartAxis)))
		return BPoint(0., 0., 0., -1.);
	return ((MTPartAxis *)pAxis)->GetVectorY();
}

MTPartAxis * MTMachine::GetStockAxis(int StPos) const
{
	int i = GetStockAxisInd(StPos);
	if( i < 0)
		return NULL;
	MTAxis *pAxis = GetCurConfig()[i];
	if(!pAxis->IsKindOf(RUNTIME_CLASS(MTPartAxis)))
		return NULL;
	return (MTPartAxis *)pAxis;
}

MTPartAxis * MTMachine::GetStockMovedAxis(int StockPosition) const
{
	int i = GetStockMovedAxisInd(StockPosition);
	if( i < 0)
		return NULL;
	MTAxis *pAxis = GetCurConfig()[i];
	if(!pAxis->IsKindOf(RUNTIME_CLASS(MTPartAxis)))
		return NULL;
	return (MTPartAxis *)pAxis;
}

int MTMachine::GetStockAxisInd(int StPos) const
{
	return GetCurConfig().GetAxisIndex(GetStockAxisName(StPos));
}

int MTMachine::GetStockMovedAxisInd(int StPos) const
{
	auto Ind = GetCurConfig().GetAxisIndex(GetStockMovedAxisName(StPos));
	if(Ind < 0)
		Ind = GetStockAxisInd(StPos);
	return Ind;
}

int MTMachine::GetStockMovedAxisIndSGr(BStockGrID StGr) const
{
	
	return GetStockMovedAxisInd(Conv2StPos(StGr));
}

void MTMachine::FullCopyWOVis(const MTMachine& MT)
{
	MTBase::FullCopyWOVis(MT);
	Part.Size = MT.Part.Size;
	Part.Part.Copy(MT.Part.Part);
}

void MTMachine::DelFullCopyWOVis()
{
	ClearFullCopyWOVis();
	MTBase::DelFullCopyWOVis();
}

void MTMachine::ClearFullCopyWOVis()
{
	Part.Size = 0;
	Part.Part.RemoveAll();
}

const MTPartArray & MTMachine::GetParts(void) const
{
	return Part;
}

MTPartArray & MTMachine::GetParts(void)
{
	return Part;
}

void MTMachine::RemovePart(int PartIndex)
{
	delete Part.Part[PartIndex];
	Part.Part.RemoveAt(PartIndex);
	Part.Size = int(Part.Part.GetSize());
}

MTPart* MTMachine::GetPart(const CString& AxisName, int StartInd /*= 0*/) const
{
	int PartSize = Part.GetSize();

	for(int i = StartInd; i < PartSize; i++)
	{
		if(Part[i]->GetAxisName() == AxisName)
			return Part[i];
	}//for

	return NULL;
}

int MTMachine::GetPartIndex(int AxisIndex) const
{
	CString AxisName = GetCurConfig()[AxisIndex]->GetName();

	int PartSize = Part.GetSize();

	for (int i = 0; i < PartSize; i++)
	{
		if (Part[i]->GetAxisName() == AxisName)
			return i;
	}//for

	return -1;
}

int MTMachine::GetMaxSpecPtsSize() const
{
	int AxSize = GetCurConfig().GetSize();
	int SPSize = 0;
	for (int i = 0; i < AxSize; i++)
	{
		MTAxis *pAxis = GetCurConfig()[i];
		MTRelocateAxis *pRelocateAxis = dynamic_cast<MTRelocateAxis *>(GetCurConfig()[i]);
		if (pRelocateAxis == nullptr)
			continue;
		SPSize = max(SPSize, int(pRelocateAxis->GetSpecPtsSize()));
	}//for

	return SPSize;
}//if

void MTMachine::SetDrawParams(bool iDrawAxis, bool iDrawPartAxis, bool iDrawPart)
{
	DrawAxis = iDrawAxis;
	DrawPartAxis = iDrawPartAxis;
	DrawPart = iDrawPart;
}

void MTMachine::SelectAxis(const CString & AxisName)
{
	SelectedAxis = AxisName;

	int AxisIndex = GetCurConfig().GetAxisIndex(SelectedAxis);
	if(SelectedAxis.IsEmpty() || AxisIndex < 0)
	{
		Part.SelectPart(-1);
		return;
	}
	if(GetCurConfig()[AxisIndex]->GetType() == MTAxis::AT_PART)
	{
		int PartIndex = GetPartIndex(AxisIndex);
		Part.SelectPart(PartIndex);
	}//if
	else
		Part.SelectPart(-1);
}//void MTMachine::SelectAxis(int AxisIndex)

