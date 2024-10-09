#include "stdafx.h"
#include "ResInv.h"
#include "MTEditPropList.h"
#include "MTPart.h"
#include "MTCollisionArray.h"
#include "MTEditStlNameDlg.h"
#include "SPropChange.h"
#include "NExprProperty.h"
#include "MTConfig.h"
#include "MTSimpleChainAxis.h"
#include "MTEditPropList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//{{AFX_MSG_MAP(MTEditPropList)
BEGIN_MESSAGE_MAP(MTEditPropList, CMFCPropertyGridCtrl)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
//}}AFX_MSG_MAP

MTEditPropList::~MTEditPropList()
{
}

void MTEditPropList::SetMachinePath(const CString & path)
{
	MachinePath = path;

	// Получаем пункт "файл"
	CMFCPropertyGridProperty *pProp;
	pProp = GetProperty(0);

	CCustomDlgProp *pFile;
	pFile = (CCustomDlgProp*)pProp->GetSubItem(1);

	pFile->SetMachinePath(MachinePath + EditingMT->GetFileName());

	// Записываем в него новый путь.
}

void MTEditPropList::SetPropValue(const COleVariant &Value, int Index1, int Index2, int Index3)
{
	CMFCPropertyGridProperty *pProperty1, *pProperty2, *pProperty3;
	pProperty1 = GetProperty(Index1);
	if(Index2 == -1)
	{
		pProperty1->SetOriginalValue(Value);
		pProperty1->ResetOriginalValue();
		pProperty1->SetValue(Value);
		return;
	}//if
	
	pProperty2 = pProperty1->GetSubItem(Index2);
	if(Index3 == -1)
	{
		pProperty2->SetOriginalValue(Value);
		pProperty2->ResetOriginalValue();
		pProperty2->SetValue(Value);
		return;
	}//if

	pProperty3 = pProperty2->GetSubItem(Index3);

	pProperty3->SetOriginalValue(Value);
	pProperty3->ResetOriginalValue();
	pProperty3->SetValue(Value);
	return;
}

void MTEditPropList::SetPropColor(const NColor &Color, int Index1, int Index2, int Index3)
{
	COLORREF tCREF;
	tCREF = Color.GetRGB();
	
	if(Index2 == -1)
	{
		CMFCPropertyGridColorProperty *pProperty1 = (CMFCPropertyGridColorProperty*)GetProperty(Index1);
		pProperty1->SetColor(tCREF);
		RemoveHighlightValue(Index1, Index2, Index3);
		return;
	}//if
	
	else
	if(Index3 == -1)
	{
		CMFCPropertyGridProperty *pProperty1 = GetProperty(Index1);
		CMFCPropertyGridColorProperty*pProperty2 = (CMFCPropertyGridColorProperty*)pProperty1->GetSubItem(Index2);
		pProperty2->SetColor(tCREF);
		//pProperty2->SetOriginalColor();
//		((CMFCPropertyGridProperty*)pProperty2)->ResetOriginalValue();
		RemoveHighlightValue(Index1, Index2, Index3);
		return;
	}//if

	else
	{
		CMFCPropertyGridProperty *pProperty1 = GetProperty(Index1);
		CMFCPropertyGridProperty *pProperty2 = pProperty1->GetSubItem(Index2);
		CMFCPropertyGridColorProperty *pProperty3 = (CMFCPropertyGridColorProperty*)pProperty2->GetSubItem(Index3);
		pProperty3->SetColor(tCREF);
		RemoveHighlightValue(Index1, Index2, Index3);
		return;
	}//else

	//Пример как выполняется отображение цвета:
	//NColor tColor;
	//COLORREF tCR;
	//tColor = Axis.GetColor();
	//tCR = tColor.GetRGB();
	//pColor->SetColor(tCR);
}

void MTEditPropList::SetPropPoint(const BPoint &Point,int Index1, int Index2, int Index3)
{
	double x, y, z;
	x = Point.GetX();
	y = Point.GetY();
	z = Point.GetZ();

	CMFCPropertyGridProperty *pProperty1, *pProperty2, *pProperty3, *pX, *pY, *pZ;
	pProperty1 = GetProperty(Index1);
	if(Index2 == -1)
	{
		pX = pProperty1->GetSubItem(0);
		pY = pProperty1->GetSubItem(1);
		pZ = pProperty1->GetSubItem(2);

		pX->SetValue(x);
		pY->SetValue(y);
		pZ->SetValue(z);

		RemoveHighlightPoint(Index1, Index2, Index3);
		return;
	}//if
	
	pProperty2 = pProperty1->GetSubItem(Index2);
	if(Index3 == -1)
	{
		pX = pProperty2->GetSubItem(0);
		pY = pProperty2->GetSubItem(1);
		pZ = pProperty2->GetSubItem(2);

		pX->SetValue(x);
		pY->SetValue(y);
		pZ->SetValue(z);

		RemoveHighlightPoint(Index1, Index2, Index3);
		return;
	}//if

	pProperty3 = pProperty2->GetSubItem(Index3);

	pX = pProperty3->GetSubItem(0);
	pY = pProperty3->GetSubItem(1);
	pZ = pProperty3->GetSubItem(2);

	pX->SetValue(x);
	pY->SetValue(y);
	pZ->SetValue(z);

	RemoveHighlightPoint(Index1, Index2, Index3);
	return;

}

void MTEditPropList::SetPropBool(bool BValue, int Index1, int Index2, int Index3)
{
	CString BStr;
	if(BValue)
		BStr.LoadString(IDS_PROP_FLAG_TRUE);
	else
		BStr.LoadString(IDS_PROP_FLAG_FALSE);

	SetPropValue(BStr, Index1, Index2, Index3);
}

void MTEditPropList::SetPropInt  (int iValue, int Index1, int Index2 , int Index3 )
{
	COleVariant Value;
	Value.lVal = iValue;
	SetPropValue(Value,Index1, Index2, Index3);
}

const COleVariant & MTEditPropList::GetPropValue(int Index1, int Index2, int Index3)
{
	CMFCPropertyGridProperty *pProperty1, *pProperty2, *pProperty3;
	pProperty1 = GetProperty(Index1);
	if(Index2 == -1)
	{
		return pProperty1->GetValue();
	}//if
	
	pProperty2 = pProperty1->GetSubItem(Index2);
	if(Index3 == -1)
	{
		return pProperty2->GetValue();
	}//if

	pProperty3 = pProperty2->GetSubItem(Index3);

	return pProperty3->GetValue();
}

int MTEditPropList::GetPropInt(int Index1, int Index2, int Index3)
{
	COleVariant tVariant;
	tVariant = GetPropValue(Index1, Index2, Index3);
	return tVariant.intVal;
}

double MTEditPropList::GetPropDouble(int Index1, int Index2, int Index3)
{
	COleVariant tVariant;
	
	tVariant = GetPropValue(Index1, Index2, Index3);
	if (tVariant.vt == VT_BSTR)
		return atof(CString(tVariant.bstrVal));
	return tVariant.dblVal ;
}//double MTEditPropList::GetPropDouble(int Index1, int Index2, int Index3)

const NColor MTEditPropList::GetPropColor(int Index1, int Index2, int Index3)
{
	COLORREF tCREF;
	NColor Color;
	
	if(Index2 == -1)
	{
		CMFCPropertyGridColorProperty *pProperty1 = (CMFCPropertyGridColorProperty*)GetProperty(Index1);
		tCREF = pProperty1->GetColor();
		Color.SetRGB(tCREF);
		return Color;
	}//if
	
	else
	if(Index3 == -1)
	{
		CMFCPropertyGridProperty *pProperty1 = GetProperty(Index1);
		CMFCPropertyGridColorProperty *pProperty2 = (CMFCPropertyGridColorProperty*)pProperty1->GetSubItem(Index2);
		tCREF = pProperty2->GetColor();
		Color.SetRGB(tCREF);
		return Color;
	}//if

	else
	{
		CMFCPropertyGridProperty *pProperty1 = GetProperty(Index1);
		CMFCPropertyGridProperty *pProperty2 = pProperty1->GetSubItem(Index2);
		CMFCPropertyGridColorProperty *pProperty3 = (CMFCPropertyGridColorProperty*)pProperty2->GetSubItem(Index3);
		tCREF = pProperty3->GetColor();
		Color.SetRGB(tCREF);
		return Color;
	}//else
}//const NColor & MTEditPropList::GetPropColor(int Index1, int Index2, int Index3)


const BPoint MTEditPropList::GetPropPoint(int Index1, int Index2, int Index3)
{
	double x, y, z;
	BPoint tPoint(0., 0., 0., 1.);
	COleVariant tVariant;

	CMFCPropertyGridProperty *pProperty1, *pProperty2, *pProperty3;
	NExprProperty *pX, *pY, *pZ;
	pProperty1 = GetProperty(Index1);
	if(Index2 == -1)
	{
		pX = (NExprProperty *)pProperty1->GetSubItem(0);
		pY = (NExprProperty *)pProperty1->GetSubItem(1);
		pZ = (NExprProperty *)pProperty1->GetSubItem(2);

		x = pX->GetDValue();
		y = pY->GetDValue();
		z = pZ->GetDValue();

		tPoint.SetX(x);
		tPoint.SetY(y);
		tPoint.SetZ(z);
		return tPoint;
	}//if
	
	pProperty2 = pProperty1->GetSubItem(Index2);
	if(Index3 == -1)
	{
		pX = (NExprProperty *)pProperty2->GetSubItem(0);
		pY = (NExprProperty *)pProperty2->GetSubItem(1);
		pZ = (NExprProperty *)pProperty2->GetSubItem(2);

		x = pX->GetDValue();
		y = pY->GetDValue();
		z = pZ->GetDValue();

		tPoint.SetX(x);
		tPoint.SetY(y);
		tPoint.SetZ(z);
		return tPoint;
	}//if

	pProperty3 = pProperty2->GetSubItem(Index3);

	pX = (NExprProperty *)pProperty3->GetSubItem(0);
	pY = (NExprProperty *)pProperty3->GetSubItem(1);
	pZ = (NExprProperty *)pProperty3->GetSubItem(2);

	x = pX->GetDValue();
	y = pY->GetDValue();
	z = pZ->GetDValue();

	tPoint.SetX(x);
	tPoint.SetY(y);
	tPoint.SetZ(z);
	return tPoint;
}//const BPoint MTEditPropList::GetPropPoint(int Index1, int Index2, int Index3)



const BPoint MTEditPropList::GetPropVector(int Index1, int Index2, int Index3)
{
	BPoint tPoint = GetPropPoint(Index1, Index2, Index3);
	tPoint.SetH(0);
	return tPoint;
}//const BPoint MTEditPropList::GetPropVector(int Index1, int Index2, int Index3)

bool MTEditPropList::GetPropBool (int Index1, int Index2, int Index3)
{
	CString BStr,tTRUEStr,tFALSEStr;
	
	BStr = GetPropValue(Index1, Index2, Index3);

	tTRUEStr.LoadString(IDS_PROP_FLAG_TRUE);
	tFALSEStr.LoadString(IDS_PROP_FLAG_FALSE);

	if(BStr == tTRUEStr)
		return TRUE;
	else
		return FALSE;
}//bool MTEditPropList::GetPropBool (int Index1, int Index2, int Index3)

BOOL MTEditPropList::IsValueModified (int Index1, int Index2, int Index3) const
{
	CMFCPropertyGridProperty *pProperty1, *pProperty2, *pProperty3;
	pProperty1 = GetProperty(Index1);
	if(Index2 == -1)
	{
		return pProperty1->IsModified();
	}//if
	
	pProperty2 = pProperty1->GetSubItem(Index2);
	if(Index3 == -1)
	{
		return pProperty2->IsModified();
	}//if

	pProperty3 = pProperty2->GetSubItem(Index3);

	return pProperty3->IsModified();
}//BOOL MTEditPropList::IsValueModified (int Index1, int Index2, int Index3) const

BOOL MTEditPropList::IsVectorModified(int Index1, int Index2, int Index3) const
{
	CMFCPropertyGridProperty *pProperty1, *pProperty2, *pProperty3;
	CMFCPropertyGridProperty *pX, *pY, *pZ;
	pProperty1 = GetProperty(Index1);
	if(Index2 == -1)
	{
		pX = pProperty1->GetSubItem(0);
		pY = pProperty1->GetSubItem(1);
		pZ = pProperty1->GetSubItem(2);

		if(pX->IsModified())
			return TRUE;
		if(pY->IsModified())
			return TRUE;
		if(pZ->IsModified())
			return TRUE;

		return FALSE;
	}//if
	
	pProperty2 = pProperty1->GetSubItem(Index2);
	if(Index3 == -1)
	{
		pX = pProperty2->GetSubItem(0);
		pY = pProperty2->GetSubItem(1);
		pZ = pProperty2->GetSubItem(2);

		if(pX->IsModified())
			return TRUE;
		if(pY->IsModified())
			return TRUE;
		if(pZ->IsModified())
			return TRUE;

		return FALSE;
	}//if

	pProperty3 = pProperty2->GetSubItem(Index3);

	pX = pProperty3->GetSubItem(0);
	pY = pProperty3->GetSubItem(1);
	pZ = pProperty3->GetSubItem(2);

	if(pX->IsModified())
		return TRUE;
	if(pY->IsModified())
		return TRUE;
	if(pZ->IsModified())
		return TRUE;

	return FALSE;
}//BOOL MTEditPropList::IsVectorModified(int Index1, int Index2, int Index3) const

BOOL MTEditPropList::IsPointModified (int Index1, int Index2, int Index3) const
{
	return IsVectorModified(Index1, Index2, Index3);
}//BOOL MTEditPropList::IsPointModified (int Index1, int Index2 = -1, int Index3 = -1) const

void MTEditPropList::RemoveHighlightValue (int Index1, int Index2, int Index3)
{
	CMFCPropertyGridProperty *pProperty1, *pProperty2, *pProperty3;
	pProperty1 = GetProperty(Index1);
	if(Index2 == -1)
	{
		pProperty1->SetOriginalValue( pProperty1->GetValue());
		pProperty1->ResetOriginalValue();
		return;
	}//if
	
	pProperty2 = pProperty1->GetSubItem(Index2);
	if(Index3 == -1)
	{
		pProperty2->SetOriginalValue( pProperty2->GetValue());
		pProperty2->ResetOriginalValue();
		return;

	}//if

	pProperty3 = pProperty2->GetSubItem(Index3);

	pProperty3->SetOriginalValue( pProperty3->GetValue());
	pProperty3->ResetOriginalValue();
	return;
}//void MTEditPropList::RemoveHighlightValue (int Index1, int Index2, int Index3)

void MTEditPropList::RemoveHighlightVector(int Index1, int Index2, int Index3)
{
	CMFCPropertyGridProperty *pProperty1, *pProperty2, *pProperty3;
	CMFCPropertyGridProperty *pX, *pY, *pZ;
	pProperty1 = GetProperty(Index1);
	if(Index2 == -1)
	{
		pX = pProperty1->GetSubItem(0);
		pY = pProperty1->GetSubItem(1);
		pZ = pProperty1->GetSubItem(2);

		pX->SetOriginalValue(pX->GetValue());
		pX->ResetOriginalValue();
		pY->SetOriginalValue(pY->GetValue());
		pY->ResetOriginalValue();
		pZ->SetOriginalValue(pZ->GetValue());
		pZ->ResetOriginalValue();

		pProperty1->SetOriginalValue(pProperty1->GetValue());
		pProperty1->ResetOriginalValue();

		return;
	}//if
	
	pProperty2 = pProperty1->GetSubItem(Index2);
	if(Index3 == -1)
	{
		pX = pProperty2->GetSubItem(0);
		pY = pProperty2->GetSubItem(1);
		pZ = pProperty2->GetSubItem(2);

		pX->SetOriginalValue(pX->GetValue());
		pX->ResetOriginalValue();
		pY->SetOriginalValue(pY->GetValue());
		pY->ResetOriginalValue();
		pZ->SetOriginalValue(pZ->GetValue());
		pZ->ResetOriginalValue();

		pProperty2->SetOriginalValue(pProperty2->GetValue());
		pProperty2->ResetOriginalValue();

		

		return;
	}//if

	pProperty3 = pProperty2->GetSubItem(Index3);

	pX = pProperty3->GetSubItem(0);
	pY = pProperty3->GetSubItem(1);
	pZ = pProperty3->GetSubItem(2);

	pX->SetOriginalValue(pX->GetValue());
	pX->ResetOriginalValue();
	pY->SetOriginalValue(pY->GetValue());
	pY->ResetOriginalValue();
	pZ->SetOriginalValue(pZ->GetValue());
	pZ->ResetOriginalValue();


	pProperty3->SetOriginalValue(pProperty3->GetValue());
	pProperty3->ResetOriginalValue();

	
	return;
}//void MTEditPropList::RemoveHighlightVector(int Index1, int Index2, int Index3)

void MTEditPropList::RemoveHighlightPoint (int Index1, int Index2, int Index3)
{
	RemoveHighlightVector(Index1, Index2, Index3);
}//void MTEditPropList::RemoveHighlightPoint (int Index1, int Index2, int Index3)

void MTEditPropList::ShowAxisProp(int index)
{
	MTAxis *Axis = EditingMT->GetCurConfig()[index];

	CString AxisName = Axis->GetName();
	CString AxisExpr = Axis->GetExpr();
	//Заполняем список
	SetPropBool(Axis->GetIsCS(), 1, 10);
	switch(Axis->GetType())
	{
	case MTAxis::AT_FOR:
		SetTypeProp(LIST_FORWARDAXIS);
		SetPropValue(AxisName,					0, 0);
		SetPropValue(AxisExpr,					0, 6);
		SetPropPoint(Axis->GetVectorOr(),       1, 0);
		SetPropValue(Axis->GetMin(),            2, 0);
		SetPropValue(Axis->GetMax(),            2, 1);
		SetPropColor(Axis->GetColor(),          3, 0);
		SetPropBool(Axis->GetVisible(),         3, 1);
		SetPropValue(Axis->GetLength(),         3, 2);
		if (GetPropertyCount() > 5)
		{
			auto pProp5 = GetProperty(5);
			for (int i = 0; i < pProp5->GetSubItemsCount(); ++i)
				SetPropValue(static_cast<MTRelocateAxis *>(Axis)->GetSpecPtsVal(i), 5, i);
			pProp5->SetOriginalValue(pProp5->GetValue());
			pProp5->ResetOriginalValue();
		}
		break;
	case MTAxis::AT_ROT:
		SetTypeProp(LIST_ROTARYAXIS);

		SetPropValue(AxisName, 0, 0);
		SetPropValue(AxisExpr, 0, 6);
		SetPropPoint(Axis->GetVectorOr(), 1, 0);
		SetPropPoint(Axis->GetBasePointOr(), 1, 1);
		SetPropValue(Axis->GetMin(), 2, 0);
		SetPropValue(Axis->GetMax(), 2, 1);
		SetPropColor(Axis->GetColor(), 3, 0);
		SetPropBool(Axis->GetVisible(), 3, 1);
		SetPropValue(Axis->GetLength(), 3, 2);
		if (GetPropertyCount() > 5)
		{
			auto pProp5 = GetProperty(5);
			for (int i = 0; i < pProp5->GetSubItemsCount(); ++i)
				SetPropValue(static_cast<MTRelocateAxis*>(Axis)->GetSpecPtsVal(i), 5, i);
			pProp5->SetOriginalValue(pProp5->GetValue());
			pProp5->ResetOriginalValue();
		}
		break;
	case MTAxis::AT_SPIN:
		SetTypeProp(LIST_SPINAXIS);

		SetPropValue(AxisName, 0, 0);
		SetPropValue(AxisExpr, 0, 6);
		SetPropPoint(Axis->GetVectorOr(), 1, 0);
		SetPropPoint(Axis->GetBasePointOr(), 1, 1);
		SetPropValue(Axis->GetMin(), 2, 0);
		SetPropValue(Axis->GetMax(), 2, 1);
		SetPropColor(Axis->GetColor(), 3, 0);
		SetPropBool(Axis->GetVisible(), 3, 1);
		SetPropValue(Axis->GetLength(), 3, 2);
		if (GetPropertyCount() > 5)
		{
			auto pProp5 = GetProperty(5);
			for (int i = 0; i < pProp5->GetSubItemsCount(); ++i)
				SetPropValue(static_cast<MTRelocateAxis*>(Axis)->GetSpecPtsVal(i), 5, i);
			pProp5->SetOriginalValue(pProp5->GetValue());
			pProp5->ResetOriginalValue();
		}
		break;
	case MTAxis::AT_PART:
		{
		MTPart *pPart = EditingMT->GetPart(Axis->GetName());

		SetTypeProp(LIST_PARTAXIS);
		
		SetPropValue(AxisName,		         0, 0);

		if(pPart != NULL)
		{
			CString sPartPath = pPart->GetFile();
			CString sFullPath = MachinePath + EditingMT->GetFileName();
			bool IsNameLocal = (sPartPath.Left(sFullPath.GetLength()).CompareNoCase(sFullPath) == 0);
			if(IsNameLocal)
				sPartPath = sPartPath.Mid(sFullPath.GetLength() + 1); //machine name + slash

			SetPropValue(sPartPath,                 0, 1);
			SetPropValue(pPart->GetMargin(), 4, 1);
		}//if
		else
		{
			SetPropValue(_T(""), 0, 1);
			SetPropValue(0., 4, 1);
		}


		SetPropPoint(Axis->GetVectorOr(),       1, 0);
		SetPropPoint(Axis->GetBasePointOr(),    1, 1);
		SetPropPoint(Axis->GetVectorYOr(),      1, 4);
		SetPropBool(Axis->GetVisible(),         3, 1);
		SetPropValue(Axis->GetLength(),         3, 2);
		SetPropColor(Axis->GetColor(), 4, 0);
		break;
		}
	case MTAxis::AT_TOOL: //tool
		SetTypeProp(LIST_TOOLAXIS);

		SetPropValue(AxisName,		           0, 0);
		SetPropValue((long)MTMachine::GetToolTurret(Axis->GetName()), 0, 2);
		SetPropValue((long)MTMachine::GetToolAttachement(Axis->GetName()), 0, 4);
		SetPropPoint(Axis->GetVectorOr(),       1, 0);
		SetPropPoint(Axis->GetBasePointOr(),    1, 1);
		SetPropPoint(Axis->GetVectorYOr(),      1, 4);
		SetPropPoint(Axis->GetLocalDisplVec(),  1, 9);
		SetPropColor(Axis->GetColor(),          3, 0);
		SetPropBool(Axis->GetVisible(),         3, 1);
		SetPropValue(Axis->GetLength(),         3, 2);
		break;
	case MTAxis::AT_VTOOL: //virttool
		SetTypeProp(LIST_TOOLAXISVIRT);

		SetPropValue(AxisName,           0, 0);
		SetPropValue((long)MTMachine::GetToolTurret(Axis->GetName()), 0, 2);
		SetPropValue((long)MTMachine::GetToolAttachement(Axis->GetName()), 0, 4);
		SetPropValue((long)MTMachine::GetToolPosition(Axis->GetName()), 0, 5);
		SetPropPoint(Axis->GetVectorOr(),       1, 0);
		SetPropPoint(Axis->GetBasePointOr(),    1, 1);
		SetPropPoint(Axis->GetVectorYOr(),      1, 4);
		SetPropColor(Axis->GetColor(),          3, 0);
		SetPropBool(Axis->GetVisible(),         3, 1);
		SetPropValue(Axis->GetLength(),         3, 2);
		break;
	case MTAxis::AT_STOCK: //stock
		SetTypeProp(LIST_STOCKAXIS);
			
		SetPropValue(AxisName,           0, 0);
		SetPropPoint(Axis->GetVectorOr(),       1, 0);
		SetPropPoint(Axis->GetVectorYOr(),      1, 4);
		if(EditingMT->GetCurConfig().IsCompStock(index))
		{
			SetPropBool(TRUE,                            1,5);

			SetCompProp(TRUE);

			SetPropPoint(EditingMT->GetCurConfig().GetCompXVect(index), 1,6);
			SetPropPoint(EditingMT->GetCurConfig().GetCompYVect(index), 1,7);
			SetPropPoint(EditingMT->GetCurConfig().GetCompZVect(index), 1,8);
		}//if
		else
		{
			SetPropBool(FALSE,                           1,5);

			SetCompProp(FALSE);
		}//else
		//kompensation 1, 5

		SetPropColor(Axis->GetColor(),          3, 0);
		SetPropBool(Axis->GetVisible(),         3, 1);
		SetPropValue(Axis->GetLength(),         3, 2);
		break;
	case MTAxis::AT_SCHAIN:
		SetTypeProp(LIST_SCHAINAXIS);
		SetPropValue(AxisName, 0, 0);
		SetPropValue(AxisExpr, 0, 6);
		SetPropPoint(Axis->GetVectorOr(), 1, 0);
		SetPropPoint(Axis->GetBasePointOr(), 1, 1);
		SetPropPoint(Axis->GetVectorYOr(), 1, 4);
		SetPropValue(static_cast<MTSimpleChainAxis*>(Axis)->GetCycleVal(), 1, 11);
		SetPropValue(static_cast<MTSimpleChainAxis*>(Axis)->GetCenterDist(), 1, 12);
		SetPropValue(Axis->GetMin(), 2, 0);
		SetPropValue(Axis->GetMax(), 2, 1);
		SetPropColor(Axis->GetColor(), 3, 0);
		SetPropBool(Axis->GetVisible(), 3, 1);
		SetPropValue(Axis->GetLength(), 3, 2);
		if (GetPropertyCount() > 5)
		{
			auto pProp5 = GetProperty(5);
			for (int i = 0; i < pProp5->GetSubItemsCount(); ++i)
				SetPropValue(static_cast<MTRelocateAxis*>(Axis)->GetSpecPtsVal(i), 5, i);
			pProp5->SetOriginalValue(pProp5->GetValue());
			pProp5->ResetOriginalValue();
		}
		break;
	}//switch

}

void MTEditPropList::ShowColProp(int index)
{
	SetTypeProp(LIST_COLLISION);

	SCollision *Col = EditingMT->GetCollision(index);

	int AxisIndex = EditingMT->GetCurConfig().GetAxisIndex(Col->Axis1);
	int size = EditingMT->GetCurConfig().GetSize();

	//Добавляем в опции пункта "ось для контроля" все оси кроме владельца объекта столкновения
	CMFCPropertyGridProperty *pProperty1, *pProperty2;
	pProperty1 = GetProperty(0);
	pProperty2 = pProperty1->GetSubItem(3);
	pProperty2->RemoveAllOptions();

	for(int i = 0; i < size; i++)
	{
		if(i != AxisIndex && !EditingMT->GetCurConfig().IsCompAxis(i))
			pProperty2->AddOption(EditingMT->GetCurConfig()[i]->GetName());
	}//for

	SetPropValue(Col->Axis2,           0, 3);
	SetPropBool(Col->Axis2Ch == TRUE,          1, 2);
	SetPropBool(Col->Axis1Ch == TRUE,          1, 3);

}//void MTEditPropList::ShowColProp(int index)

void MTEditPropList::ShowEmptyProp()
{
	SetTypeProp(LIST_EMPTY);
}//void MTEditPropList::ShowEmptyProp()

bool MTEditPropList::MakeAxisList()
{
	int SPSize = EditingMT->GetMaxSpecPtsSize();
	CMFCPropertyGridProperty* pProp;
	CString STemp1, STemp2;

	RemoveAll();
	EnableHeaderCtrl(FALSE);
	EnableDescriptionArea();
	SetVSDotNetLook();
	MarkModifiedProperties();

	//***************************************************************************************************
	STemp1.LoadString(IDS_PROP_COMMON);
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(STemp1);
	STemp1.LoadString(IDS_PROP_COMMON_NAME);
	STemp2.LoadString(IDS_PROP_COMMON_NAME_DES);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(STemp1, (_variant_t) _T(""), STemp2));

	STemp1.LoadString(IDS_PROP_COMMON_FILE);
	STemp2.LoadString(IDS_PROP_COMMON_FILE_DES);
	CCustomDlgProp *pFile = new CCustomDlgProp(STemp1, (_variant_t) _T(""), STemp2);
	pFile->AllowEdit(FALSE);
	pGroup1->AddSubItem(pFile);


	STemp1.LoadString(IDS_PROP_COMMON_TURRET);
	STemp2.LoadString(IDS_PROP_COMMON_TURRET_DES);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(STemp1, (_variant_t) 10l, STemp2));

	STemp1.LoadString(IDS_PROP_COMMON_NAME_CONTROL);
	STemp2.LoadString(IDS_PROP_COMMON_NAME_CONTROL_DES);
	CMFCPropertyGridProperty* pControlAxis = new CMFCPropertyGridProperty(STemp1, (_variant_t) _T(""), STemp2);
	pControlAxis->AllowEdit(FALSE);
	pGroup1->AddSubItem(pControlAxis);

	STemp1.LoadString(IDS_PROP_COMMON_ATTACHMENT);
	STemp2.LoadString(IDS_PROP_COMMON_ATTACHMENT_DES);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(STemp1, (_variant_t) 10l, STemp2));

	STemp1.LoadString(IDS_PROP_COMMON_POSITION);
	STemp2.LoadString(IDS_PROP_COMMON_POSITION_DES);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(STemp1, (_variant_t)10l, STemp2));

	STemp1.LoadString(IDS_PROP_COMMON_NAME_EXPRESSION);
	STemp2.LoadString(IDS_PROP_COMMON_NAME_EXPRESSION_DES);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(STemp1, (_variant_t)_T(""), STemp2));

	AddProperty(pGroup1);

	//***************************************************************************************************
	STemp1.LoadString(IDS_PROP_KINEMAT);
	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(STemp1);
	
	//...................................................................................................
	STemp1.LoadString(IDS_PROP_KINEMAT_VECT);
	CMFCPropertyGridProperty* pVector = new CMFCPropertyGridProperty(STemp1,0, TRUE);
	
	STemp1.LoadString(IDS_PROP_KINEMAT_VECT_X);
	STemp2.LoadString(IDS_PROP_KINEMAT_VECT_X_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pVector->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_VECT_Y);
	STemp2.LoadString(IDS_PROP_KINEMAT_VECT_Y_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pVector->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_VECT_Z);
	STemp2.LoadString(IDS_PROP_KINEMAT_VECT_Z_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pVector->AddSubItem(pProp);
	
	pGroup2->AddSubItem(pVector);

	
	//...................................................................................................
	STemp1.LoadString(IDS_PROP_KINEMAT_BP);
	CMFCPropertyGridProperty* pBPoint = new CMFCPropertyGridProperty(STemp1,0, TRUE);

	STemp1.LoadString(IDS_PROP_KINEMAT_BP_X);
	STemp2.LoadString(IDS_PROP_KINEMAT_BP_X_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pBPoint->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_BP_Y);
	STemp2.LoadString(IDS_PROP_KINEMAT_BP_Y_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pBPoint->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_BP_Z);
	STemp2.LoadString(IDS_PROP_KINEMAT_BP_Z_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pBPoint->AddSubItem(pProp);

	pGroup2->AddSubItem(pBPoint);

	//...................................................................................................

	CString Yes;
	Yes.LoadString(IDS_YES_STR);
	STemp1.LoadString(IDS_PROP_KINEMAT_AXISCHILDREN);
	STemp2.LoadString(IDS_PROP_KINEMAT_AXISCHILDREN_DES);
	pProp = new CMFCPropertyGridProperty(STemp1, (_variant_t) Yes, STemp2);
	STemp1.LoadString(IDS_PROP_FLAG_TRUE);
	pProp->AddOption(STemp1);
	STemp1.LoadString(IDS_PROP_FLAG_FALSE);
	pProp->AddOption(STemp1);
	pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_MYCHILDREN);
	STemp2.LoadString(IDS_PROP_KINEMAT_MYCHILDREN_DES);
	pProp = new CMFCPropertyGridProperty(STemp1, (_variant_t) Yes, STemp2);
	STemp1.LoadString(IDS_PROP_FLAG_TRUE);
	pProp->AddOption(STemp1);
	STemp1.LoadString(IDS_PROP_FLAG_FALSE);
	pProp->AddOption(STemp1);
	pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	//...................................................................................................
	STemp1.LoadString(IDS_PROP_KINEMAT_VECTY);
	CMFCPropertyGridProperty* pVectorY = new CMFCPropertyGridProperty(STemp1,0, TRUE);
	
	STemp1.LoadString(IDS_PROP_KINEMAT_VECTY_X);
	STemp2.LoadString(IDS_PROP_KINEMAT_VECTY_X_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pVectorY->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_VECTY_Y);
	STemp2.LoadString(IDS_PROP_KINEMAT_VECTY_Y_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pVectorY->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_VECTY_Z);
	STemp2.LoadString(IDS_PROP_KINEMAT_VECTY_Z_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pVectorY->AddSubItem(pProp);
	
	pGroup2->AddSubItem(pVectorY);
    //...................................................................................................

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_DES);
	pProp = new CMFCPropertyGridProperty(STemp1, (_variant_t) Yes, STemp2);
	STemp1.LoadString(IDS_PROP_FLAG_TRUE);
	pProp->AddOption(STemp1);
	STemp1.LoadString(IDS_PROP_FLAG_FALSE);
	pProp->AddOption(STemp1);
	pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISX);
	CMFCPropertyGridProperty* pCompAxisX = new CMFCPropertyGridProperty(STemp1,0, TRUE);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISX_X);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISX_X_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisX->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISX_Y);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISX_Y_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisX->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISX_Z);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISX_Z_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisX->AddSubItem(pProp);

	pGroup2->AddSubItem(pCompAxisX);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISY);
	CMFCPropertyGridProperty* pCompAxisY = new CMFCPropertyGridProperty(STemp1,0, TRUE);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISY_X);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISY_X_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisY->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISY_Y);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISY_Y_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisY->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISY_Z);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISY_Z_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisY->AddSubItem(pProp);

	pGroup2->AddSubItem(pCompAxisY);


	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISZ);
	CMFCPropertyGridProperty* pCompAxisZ = new CMFCPropertyGridProperty(STemp1,0, TRUE);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISZ_X);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISZ_X_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisZ->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISZ_Y);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISZ_Y_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisZ->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_COMP_AXISZ_Z);
	STemp2.LoadString(IDS_PROP_KINEMAT_COMP_AXISZ_Z_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pCompAxisZ->AddSubItem(pProp);

	pGroup2->AddSubItem(pCompAxisZ);

	STemp1.LoadString(IDS_PROP_KINEMAT_LOCALDV);
	CMFCPropertyGridProperty* pLocalDV = new CMFCPropertyGridProperty(STemp1,0,TRUE);

	STemp1.LoadString(IDS_PROP_KINEMAT_LOCALDV_X);
	STemp2.LoadString(IDS_PROP_KINEMAT_LOCALDV_X_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pLocalDV->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_LOCALDV_Y);
	STemp2.LoadString(IDS_PROP_KINEMAT_LOCALDV_Y_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pLocalDV->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_KINEMAT_LOCALDV_Z);
	STemp2.LoadString(IDS_PROP_KINEMAT_LOCALDV_Z_DES);
	pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
	pLocalDV->AddSubItem(pProp);
	
	pGroup2->AddSubItem(pLocalDV);

	STemp1.LoadString(IDS_PROP_COORDSYS_BPRELATIVE);
	STemp2.LoadString(IDS_PROP_COORDSYS_BPRELATIVE_DES);
	pProp = new CMFCPropertyGridProperty(STemp1, (_variant_t)Yes, STemp2);
	STemp1.LoadString(IDS_PROP_FLAG_TRUE);
	pProp->AddOption(STemp1);
	STemp1.LoadString(IDS_PROP_FLAG_FALSE);
	pProp->AddOption(STemp1);
	pProp->AllowEdit(FALSE);
	pGroup2->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_SIMPLECHAIN_CYCLEVAL);//CYCLEVAL
	STemp2.LoadString(IDS_PROP_SIMPLECHAIN_CYCLEVAL_DES);
	pGroup2->AddSubItem(new NExprProperty(STemp1, CString(_T("0")), STemp2));

	STemp1.LoadString(IDS_PROP_SIMPLECHAIN_CENTERDIST);//CENTERDIST
	STemp2.LoadString(IDS_PROP_SIMPLECHAIN_CENTERDIST_DES);
	pGroup2->AddSubItem(new NExprProperty(STemp1, CString(_T("0")), STemp2));

	//...................................................................................................

	AddProperty(pGroup2);

	//***************************************************************************************************
	STemp1.LoadString(IDS_PROP_LIMITS);
	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(STemp1);

	STemp1.LoadString(IDS_PROP_LIMITS_MIN);
	STemp2.LoadString(IDS_PROP_LIMITS_MIN_DES);
	pGroup3->AddSubItem(new NExprProperty(STemp1, CString(_T("0")), STemp2));

	STemp1.LoadString(IDS_PROP_LIMITS_MAX);
	STemp2.LoadString(IDS_PROP_LIMITS_MAX_DES);
	pGroup3->AddSubItem(new NExprProperty(STemp1, CString(_T("0")), STemp2));
	
	AddProperty(pGroup3);

	//***************************************************************************************************
	STemp1.LoadString(IDS_PROP_COORD);
	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(STemp1);

	STemp1.LoadString(IDS_PROP_COORD_COLOR);
	STemp2.LoadString(IDS_PROP_COORD_COLOR_DES);
	CMFCPropertyGridColorProperty* pColor = new CMFCPropertyGridColorProperty(STemp1, RGB(210,192,254), NULL, STemp2);
	STemp1.LoadString(IDS_PROP_COORD_COLOR_OTHER);
	pColor->EnableOtherButton(STemp1);
	STemp1.LoadString(IDS_PROP_COORD_COLOR_DEFAULT);
	pColor->EnableAutomaticButton(STemp1, ::GetSysColor(COLOR_3DFACE));
	pGroup4->AddSubItem(pColor);

	STemp1.LoadString(IDS_PROP_COORD_VISIBILITY);
	STemp2.LoadString(IDS_PROP_COORD_VISIBILITY_DES);
	pProp = new CMFCPropertyGridProperty(STemp1, (_variant_t) Yes, STemp2);
	STemp1.LoadString(IDS_PROP_FLAG_TRUE);
	pProp->AddOption(STemp1);
	STemp1.LoadString(IDS_PROP_FLAG_FALSE);
	pProp->AddOption(STemp1);
	pProp->AllowEdit(FALSE);
	pGroup4->AddSubItem(pProp);

	STemp1.LoadString(IDS_PROP_COORD_SIZE);
	STemp2.LoadString(IDS_PROP_COORD_SIZE_DES);
	pGroup4->AddSubItem(new NExprProperty(STemp1, CString(_T("0")), STemp2));

	
	AddProperty(pGroup4);
	//***************************************************************************************************
	STemp1.LoadString(IDS_PROP_MISC);
	CMFCPropertyGridProperty* pGroup5 = new CMFCPropertyGridProperty(STemp1);
	STemp1.LoadString(IDS_PROP_MISC_COLOR);
	STemp2.LoadString(IDS_PROP_MISC_COLOR_DES);
	pColor = new CMFCPropertyGridColorProperty(STemp1, RGB(210,192,254), NULL, STemp2);
	STemp1.LoadString(IDS_PROP_MISC_COLOR_OTHER);
	pColor->EnableOtherButton(STemp1);
	STemp1.LoadString(IDS_PROP_MISC_COLOR_DEFAULT);
	pColor->EnableAutomaticButton(STemp1, ::GetSysColor(COLOR_3DFACE));
	pGroup5->AddSubItem(pColor);
	STemp1.LoadString(IDS_PROP_NEAR_MISS);
	STemp2.LoadString(IDS_PROP_NEAR_MISS_DES);
	pGroup5->AddSubItem(new NExprProperty(STemp1, CString(_T("0")), STemp2));

	AddProperty(pGroup5);
	//***************************************************************************************************
	// special points
	ChangeSpecPosList();
	return TRUE;
}

bool MTEditPropList::ChangeSpecPosList()
{
	// Add property if needed
	// special points
	int SPSize = EditingMT->GetMaxSpecPtsSize();
	auto PropCount = GetPropertyCount();
	CMFCPropertyGridProperty* pGroup6 = nullptr;
	if (SPSize > 0)
	{
		if (PropCount <= 5)
		{
			CString STemp1;
			STemp1.LoadString(IDS_PROP_MTSPECPOS);// SpecPointsEdit
			pGroup6 = new CMFCPropertyGridProperty(STemp1, 0, TRUE);
			AddProperty(pGroup6);
		}
		else
			pGroup6 = GetProperty(5);

		if (pGroup6->GetSubItemsCount() > SPSize)
		{
			// remove extra
			for (int i = pGroup6->GetSubItemsCount() - 1; i >= SPSize; --i)
			{
				auto pProp = pGroup6->GetSubItem(i);
				pGroup6->RemoveSubItem(pProp);
			}
		}
		else if (pGroup6->GetSubItemsCount() < SPSize)
		{
			// add
			for (int i = pGroup6->GetSubItemsCount(); i <  SPSize; ++i)
			{
				CString STemp1, STemp2;
				STemp1.Format("%d", i);
				STemp2.LoadString(IDS_PROP_MTSPECPOS_DES);// SpecPointsEdit
				auto pProp = new NExprProperty(STemp1, CString(_T("0")), STemp2);
				pGroup6->AddSubItem(pProp);
			}
			pGroup6->Expand(!pGroup6->IsExpanded());// a tric to ensure new subitems to appear
			pGroup6->Expand(!pGroup6->IsExpanded());
		}
	}
	else
	{
		if (PropCount > 5)
		{
			pGroup6 = GetProperty(5);
			DeleteProperty(pGroup6);
		}
	}
	return true;
}

void MTEditPropList::ExpandCollapse()
{
	/// Получаем первый элемент
	CMFCPropertyGridProperty *pProp = GetProperty(0);

	/// Если нет элементов, выходим
	if(pProp == NULL)
		return;

	/// Проверяем, свернут ли он
	if(pProp->IsExpanded())
	{
		/// Если развернут, сворачиваем всё
		ExpandAll(false);
	}//if
	else
	{
		/// Иначе разворачиваем всё
		ExpandAll();
	}//else
}//void MTEditPropList::ExpandCollapse()

void MTEditPropList::SetTypeProp(int type)
{
	CMFCPropertyGridProperty *pCommon = GetProperty(0);
	CMFCPropertyGridProperty *pName = pCommon->GetSubItem(0);
	CMFCPropertyGridProperty *pFileName = pCommon->GetSubItem(1);
	CMFCPropertyGridProperty *pTurret = pCommon->GetSubItem(2);
	CMFCPropertyGridProperty *pAxisControl = pCommon->GetSubItem(3);
	CMFCPropertyGridProperty *pAttachment = pCommon->GetSubItem(4);
	CMFCPropertyGridProperty *pPosition = pCommon->GetSubItem(5);
	CMFCPropertyGridProperty *pExpression = pCommon->GetSubItem(6);

	switch(type)
	{
		case LIST_FORWARDAXIS:
			pCommon->Show();
			pName->Show();
			pName->Enable();
			pFileName->Show(FALSE);
			pTurret->Show(FALSE);
			pAxisControl->Show(FALSE);
			pAttachment->Show(FALSE);
			pPosition->Show(FALSE);
			pExpression->Show(TRUE);
			break;
		case LIST_ROTARYAXIS:
			pCommon->Show();
			pName->Show();
			pName->Enable();
			pFileName->Show(FALSE);
			pTurret->Show(FALSE);
			pAxisControl->Show(FALSE);
			pAttachment->Show(FALSE);
			pPosition->Show(FALSE);
			pExpression->Show(TRUE);
			break;
		case LIST_SPINAXIS:
			pCommon->Show();
			pName->Show();
			pName->Enable();
			pFileName->Show(FALSE);
			pTurret->Show(FALSE);
			pAxisControl->Show(FALSE);
			pAttachment->Show(FALSE);
			pPosition->Show(FALSE);
			pExpression->Show(TRUE);
			break;
		case LIST_SCHAINAXIS:
			pCommon->Show();
			pName->Show();
			pName->Enable();
			pFileName->Show(FALSE);
			pTurret->Show(FALSE);
			pAxisControl->Show(FALSE);
			pAttachment->Show(FALSE);
			pPosition->Show(FALSE);
			pExpression->Show(TRUE);
			break;
		case LIST_PARTAXIS:
			pCommon->Show();
			pName->Show();
			pName->Enable();
			pFileName->Show();
			pTurret->Show(FALSE);
			pAxisControl->Show(FALSE);
			pAttachment->Show(FALSE);
			pPosition->Show(FALSE);
			pExpression->Show(FALSE);
			break;
		case LIST_TOOLAXIS:
			pCommon->Show();
			pName->Show();
			pName->Enable(FALSE);
			pFileName->Show(FALSE);
			pTurret->Show();
			pAxisControl->Show(FALSE);
			pAttachment->Show();
			pPosition->Show(FALSE);
			pExpression->Show(FALSE);
			break;
		case LIST_TOOLAXISVIRT:
			pCommon->Show();
			pName->Show();
			pName->Enable(FALSE);
			pFileName->Show(FALSE);
			pTurret->Show();
			pAxisControl->Show(FALSE);
			pAttachment->Show();
			pPosition->Show();
			pExpression->Show(FALSE);
			break;
		case LIST_STOCKAXIS:
			pCommon->Show();
			pName->Show();
			pName->Enable();
			pFileName->Show(FALSE);
			pTurret->Show(FALSE);
			pAxisControl->Show(FALSE);
			pAttachment->Show(FALSE);
			pPosition->Show(FALSE);
			pExpression->Show(FALSE);
			break;
		case LIST_COLLISION: 
			pCommon->Show();
			pName->Show(FALSE);
			pName->Enable();
			pFileName->Show(FALSE);
			pTurret->Show(FALSE);
			pAxisControl->Show();
			pAttachment->Show(FALSE);
			pPosition->Show(FALSE);
			pExpression->Show(FALSE);
			break;
		case LIST_EMPTY:
			pCommon->Show(false);
			break;
		default:
			pCommon->Show(FALSE);
	}//switch

	CMFCPropertyGridProperty *pKinemat = GetProperty(1);
	CMFCPropertyGridProperty *pVector = pKinemat->GetSubItem(0);
	CMFCPropertyGridProperty *pBasePoint = pKinemat->GetSubItem(1);
	CMFCPropertyGridProperty *pAxisChildren = pKinemat->GetSubItem(2);
	CMFCPropertyGridProperty *pMyChildren = pKinemat->GetSubItem(3);
	CMFCPropertyGridProperty *pVectorY = pKinemat->GetSubItem(4);
	CMFCPropertyGridProperty *pCompensat = pKinemat->GetSubItem(5);
	CMFCPropertyGridProperty *pCompVectX = pKinemat->GetSubItem(6);
	CMFCPropertyGridProperty *pCompVectY = pKinemat->GetSubItem(7);
	CMFCPropertyGridProperty *pCompVectZ = pKinemat->GetSubItem(8);
	CMFCPropertyGridProperty* pLocalDV = pKinemat->GetSubItem(9);
	CMFCPropertyGridProperty* pIsCS = pKinemat->GetSubItem(10);
	CMFCPropertyGridProperty* pCycleVal = pKinemat->GetSubItem(11);
	CMFCPropertyGridProperty* pCenterDist = pKinemat->GetSubItem(12);

	pKinemat->Show();// shold be before subitem->Show calls
	pIsCS->Show();
	pCycleVal->Show(FALSE);
	pCenterDist->Show(FALSE);
	switch(type)
	{
		case LIST_FORWARDAXIS:
			pVector->Show();
			pBasePoint->Show(FALSE);
			pAxisChildren->Show(FALSE);
			pMyChildren->Show(FALSE);
			pVectorY->Show(FALSE);
			pCompensat->Show(FALSE);
			pCompVectX->Show(FALSE);
			pCompVectY->Show(FALSE);
			pCompVectZ->Show(FALSE);
			pLocalDV->Show(FALSE);
			pIsCS->Show(FALSE);
			break;
		case LIST_ROTARYAXIS:
			pVector->Show();
			pBasePoint->Show();
			pAxisChildren->Show(FALSE);
			pMyChildren->Show(FALSE);
			pVectorY->Show(FALSE);
			pCompensat->Show(FALSE);
			pCompVectX->Show(FALSE);
			pCompVectY->Show(FALSE);
			pCompVectZ->Show(FALSE);
			pLocalDV->Show(FALSE);
			pIsCS->Show(FALSE);
			break;
		case LIST_SPINAXIS:
			pVector->Show();
			pBasePoint->Show();
			pAxisChildren->Show(FALSE);
			pMyChildren->Show(FALSE);
			pVectorY->Show(FALSE);
			pCompensat->Show(FALSE);
			pCompVectX->Show(FALSE);
			pCompVectY->Show(FALSE);
			pCompVectZ->Show(FALSE);
			pLocalDV->Show(FALSE);
			pIsCS->Show(FALSE);
			break;
		case LIST_SCHAINAXIS:
			pVector->Show();
			pBasePoint->Show();
			pAxisChildren->Show(FALSE);
			pMyChildren->Show(FALSE);
			pVectorY->Show(TRUE);
			pCompensat->Show(FALSE);
			pCompVectX->Show(FALSE);
			pCompVectY->Show(FALSE);
			pCompVectZ->Show(FALSE);
			pLocalDV->Show(FALSE);
			pCycleVal->Show(TRUE);
			pCenterDist->Show(TRUE);
			break;
		case LIST_PARTAXIS:
			pVector->Show();
			pBasePoint->Show();
			pAxisChildren->Show(FALSE);
			pMyChildren->Show(FALSE);
			pVectorY->Show();
			pCompensat->Show(FALSE);
			pCompVectX->Show(FALSE);
			pCompVectY->Show(FALSE);
			pCompVectZ->Show(FALSE);
			pLocalDV->Show(FALSE);
			break;
		case LIST_TOOLAXIS:
			pVector->Show();
			pBasePoint->Show();
			pAxisChildren->Show(FALSE);
			pMyChildren->Show(FALSE);
			pVectorY->Show();
			pCompensat->Show(FALSE);
			pCompVectX->Show(FALSE);
			pCompVectY->Show(FALSE);
			pCompVectZ->Show(FALSE);
			pLocalDV->Show();
			break;
		case LIST_TOOLAXISVIRT:
			pVector->Show();
			pBasePoint->Show();
			pAxisChildren->Show(FALSE);
			pMyChildren->Show(FALSE);
			pVectorY->Show();
			pCompensat->Show(FALSE);
			pLocalDV->Show(FALSE);
			break;
		case LIST_STOCKAXIS:
			pVector->Show();
			pBasePoint->Show(FALSE);
			pAxisChildren->Show(FALSE);
			pMyChildren->Show(FALSE);
			pVectorY->Show();
			pCompensat->Show();
			pCompVectX->Show(FALSE);
			pCompVectY->Show(FALSE);
			pCompVectZ->Show(FALSE);
			pLocalDV->Show(FALSE);
			break;
		case LIST_COLLISION: 
			pVector->Show(FALSE);
			pBasePoint->Show(FALSE);
			pAxisChildren->Show();
			pMyChildren->Show();
			pVectorY->Show(FALSE);
			pCompensat->Show(FALSE);
			pCompVectX->Show(FALSE);
			pCompVectY->Show(FALSE);
			pCompVectZ->Show(FALSE);
			pLocalDV->Show(FALSE);
			pIsCS->Show(FALSE);
			break;
		case LIST_EMPTY:
			pKinemat->Show(false);
			break;
		default:
			pKinemat->Show(FALSE);
	}//switch

	CMFCPropertyGridProperty *pLimits = GetProperty(2);
	//CMFCPropertyGridProperty *pMin = pLimits->GetSubItem(0);
	//CMFCPropertyGridProperty *pMax = pLimits->GetSubItem(1);

	switch(type)
	{
	case LIST_FORWARDAXIS:
		pLimits->Show();
		break;
	case LIST_ROTARYAXIS:
		pLimits->Show();
		break;
	case LIST_SPINAXIS:
		pLimits->Show();
		break;
	case LIST_SCHAINAXIS:
		pLimits->Show();
		break;
	case LIST_PARTAXIS:
		pLimits->Show(FALSE);
		break;
	case LIST_TOOLAXIS:
		pLimits->Show(FALSE);
		break;
	case LIST_TOOLAXISVIRT:
		pLimits->Show(FALSE);
		break;
	case LIST_STOCKAXIS:
		pLimits->Show(FALSE);
		break;
	case LIST_COLLISION: 
		pLimits->Show(FALSE);
		break;
	case LIST_EMPTY:
		pLimits->Show(false);
		break;
	default:
		pLimits->Show(FALSE);
	}//switch

	CMFCPropertyGridProperty *pCoord = GetProperty(3);
	CMFCPropertyGridColorProperty *pColor = (CMFCPropertyGridColorProperty*)pCoord->GetSubItem(0);
	CMFCPropertyGridProperty *pVisibility = pCoord->GetSubItem(1);
	CMFCPropertyGridProperty *pSize = pCoord->GetSubItem(2);

	switch(type)
	{
	case LIST_FORWARDAXIS:
		pCoord->Show();
		pColor->Show();
		pVisibility->Show();
		pSize->Show();
		break;
	case LIST_ROTARYAXIS:
		pCoord->Show();
		pColor->Show();
		pVisibility->Show();
		pSize->Show();
		break;
	case LIST_SPINAXIS:
		pCoord->Show();
		pColor->Show();
		pVisibility->Show();
		pSize->Show();
		break;
	case LIST_SCHAINAXIS:
		pCoord->Show();
		pColor->Show();
		pVisibility->Show();
		pSize->Show();
		break;
	case LIST_PARTAXIS:
		pCoord->Show();
		pColor->Show(FALSE);
		pVisibility->Show();
		pSize->Show();
		break;
	case LIST_TOOLAXIS:
		pCoord->Show();
		pColor->Show();
		pVisibility->Show();
		pSize->Show();
		break;
	case LIST_TOOLAXISVIRT:
		pCoord->Show();
		pColor->Show();
		pVisibility->Show();
		pSize->Show();
		break;
	case LIST_STOCKAXIS:
		pCoord->Show(TRUE);
		pColor->Show(TRUE);
		pVisibility->Show(TRUE);
		pSize->Show(TRUE);
		break;
	case LIST_COLLISION:
		pCoord->Show(FALSE);
		pColor->Show(FALSE);
		pVisibility->Show(FALSE);
		pSize->Show(FALSE);
		break;
	case LIST_EMPTY:
		pCoord->Show(false);
		break;
	default:
		pCoord->Show(FALSE);
	}//switch

	CMFCPropertyGridProperty *pMisc = GetProperty(4);
	CMFCPropertyGridColorProperty *pSTLColor = (CMFCPropertyGridColorProperty*)pMisc->GetSubItem(0);
	CMFCPropertyGridProperty* pMargin = pCoord->GetSubItem(1);

	switch(type)
	{
	case LIST_FORWARDAXIS:
		pMisc->Show(FALSE);
		break;
	case LIST_ROTARYAXIS:
		pMisc->Show(FALSE);
		break;
	case LIST_SPINAXIS:
		pMisc->Show(FALSE);
		break;
	case LIST_SCHAINAXIS:
		pMisc->Show(FALSE);
		break;
	case LIST_PARTAXIS:
		pMisc->Show();
		pSTLColor->Show();
		pMargin->Show(TRUE);
		break;
	case LIST_TOOLAXIS:
		pMisc->Show(FALSE);
		break;
	case LIST_TOOLAXISVIRT:
		pMisc->Show(FALSE);
		break;
	case LIST_STOCKAXIS:
		pMisc->Show(FALSE);
		break;
	case LIST_COLLISION: 
		pMisc->Show(FALSE);
		break;
	case LIST_EMPTY:
		pMisc->Show(false);
		break;
	default:
		pMisc->Show(FALSE);
	}//switch

	bool SpecPosPresent = (GetPropertyCount() > 5);
	if (SpecPosPresent)
	{
		CMFCPropertyGridProperty * pSpecPos = GetProperty(5);
		switch (type)
		{
		case LIST_FORWARDAXIS:
		case LIST_ROTARYAXIS:
		case LIST_SCHAINAXIS:
		case LIST_SPINAXIS:
			pSpecPos->Show(TRUE);
			break;
		default:
			pSpecPos->Show(FALSE);
		}//switch
	}
}//void MTEditPropList::SetTypeProp(int type)

void MTEditPropList::SetCompProp(bool show)
{
	CMFCPropertyGridProperty *pKinemat = GetProperty(1);
	CMFCPropertyGridProperty *pCompVectX = pKinemat->GetSubItem(6);
	CMFCPropertyGridProperty *pCompVectY = pKinemat->GetSubItem(7);
	CMFCPropertyGridProperty *pCompVectZ = pKinemat->GetSubItem(8);

	pCompVectX->Show(show);
	pCompVectY->Show(show);
	pCompVectZ->Show(show);
}//void MTEditPropList::SetCompProp(bool show = TRUE)

void MTEditPropList::SetMachineTool(MTMachineTool *MT)
{
	EditingMT = MT;
	MakeAxisList();
}//void MTEditPropList::SetMachineTool(MTMachineTool *MT)

bool MTEditPropList::ReadChanges(SPropChange *Change)
{
	if(IsValueModified                    (0, 0))
	{
		Change->NameChanged = true;
		Change->Name =  GetPropValue (0, 0);
	}//if
	if(IsValueModified                    (0, 1))
	{
		Change->FileNameChanged = true;
		Change->FileName = GetPropValue(   0, 1);
	}//if
	if(IsValueModified                    (0, 2))
	{
		Change->TurretChanged = true;
		Change->Turret = GetPropInt    (   0, 2);
	}//if
	if(IsValueModified(                    0, 3))
	{
		Change->EnemyAxisNameChanged = true;
		Change->EnemyAxisName =GetPropValue(0,3);
	}//if
	if(IsValueModified                    (0, 4))
	{
		Change->AttachmentChanged = true;
		Change->Attachment = GetPropInt(   0, 4);
	}//if
	if(IsValueModified                    (0, 5))
	{
		Change->PositionChanged = true;
		Change->Position = GetPropInt  (   0, 5);
	}//if
	if (IsValueModified					(0, 6))
	{
		Change->ExprChanged = true;
		Change->Expr = GetPropValue		(	0, 6);
	}//if
	if(IsVectorModified                   (1, 0))
	{
		Change->VectorChanged = true;
		Change->Vector =   GetPropVector  (1, 0);
	}//if
	if(IsPointModified                    (1, 1))
	{
		Change->BasePointChanged = true;
		Change->BasePoint = GetPropPoint  (1, 1);
	}//if
	if(IsValueModified(1, 2))
	{
		Change->EnemyAxisChildrenChanged = true;
		Change->EnemyAxisChildren=GetPropBool(1, 2);
	}//if
	if(IsValueModified                    (1, 3))
	{
		Change->MyChildrenChanged = true;
		Change->MyChildren = GetPropBool  (1, 3);
	}//if
	if(IsVectorModified                   (1, 4))
	{
		Change->VectorYChanged = true;
		Change->VectorY =  GetPropVector  (1, 4);
	}//if
	if(IsValueModified                    (1, 5))
	{
		Change->CompensationChanged = true;
		Change->Compensation = GetPropBool(1, 5);
	}//if
	if(IsValueModified                    (1, 6))
	{
		Change->CompAxisXChanged = true;
		Change->CompAxisX = GetPropVector (1, 6);
	}//if
	if(IsValueModified                    (1, 7))
	{
		Change->CompAxisYChanged = true;
		Change->CompAxisY = GetPropVector (1, 7);
	}//if
	if(IsValueModified                    (1, 8))
	{
		Change->CompAxisZChanged = true;
		Change->CompAxisZ = GetPropVector (1, 8);
	}//if
	if(IsVectorModified                   (1, 9))
	{
		Change->LocalDVChanged = true;
		Change->LocalDV =    GetPropVector(1, 9);
	}//if
	if (IsValueModified(1, 11))
	{
		Change->CycleValChanged = true;
		Change->CycleVal = GetPropDouble(1, 11);
	}//if
	if (IsValueModified(1, 12))
	{
		Change->CenterDistChanged = true;
		Change->CenterDist = GetPropDouble(1, 12);
	}//if
	if(IsValueModified					  (1, 10))
	{
		Change->IsCSChanged = true;
		Change->IsCS =		   GetPropBool(1, 10);
	}//if
	if(IsValueModified                    (2, 0))
	{
		Change->LimitMinChanged = true;
		Change->LimitMin = GetPropDouble  (2, 0);
	}//if
	if(IsValueModified                    (2, 1))
	{
		Change->LimitMaxChanged = true;
		Change->LimitMax = GetPropDouble  (2, 1);
	}//if
	if(IsValueModified                    (3, 0))
	{
		Change->ColorChanged = true;
		Change->Color =    GetPropColor   (3, 0);
	}//if
	if(IsValueModified                    (3, 1))
	{
		Change->VisibilityChanged = true;
		Change->Visibility=GetPropBool    (3, 1);
	}//if
	if(IsValueModified                    (3, 2))
	{
		Change->LengthChanged = true;
		Change->Length =   GetPropDouble  (3, 2);
	}//if	
	if (IsValueModified					  (4, 0))
	{
		Change->MiscColorChanged = true;
		Change->MiscColor = GetPropColor  (4, 0);
	}//if
	if (IsValueModified				      (4, 1))
	{
		Change->MarginChanged = true;
		Change->Margin = GetPropDouble    (4, 1);
	}//if
	if (GetPropertyCount() > 5)
	{// Spec positions present
		CMFCPropertyGridProperty *pSpecPos = GetProperty(5);
		Change->SpecialPts.resize(pSpecPos->GetSubItemsCount());
		for (int i = 0; i < pSpecPos->GetSubItemsCount(); ++i)
		{
			COleVariant tVariant;
			double Val = 0.;
			tVariant = pSpecPos->GetSubItem(i)->GetValue();
			if (tVariant.vt == VT_BSTR)
				Val = atof(CString(tVariant.bstrVal));
			else
				Val = tVariant.dblVal;
			Change->SpecialPts[i] = Val;
		}
		// rtmove last zeros
		int k = int(Change->SpecialPts.size() - 1);
		for (; k >= 0; --k)
			if (Change->SpecialPts[k] != 0.)
				break;
		Change->SpecialPts.resize(k + 1);
	}
	return TRUE;
}//bool MTEditPropList::ReadChanges(SPropChange *Change)

bool MTEditPropList::IsPropertyGridModified()
{
	int size = GetPropertyCount();
	for(int i = 0; i < size ; i++)
	{
		CMFCPropertyGridProperty *pProp = GetProperty(i);
		if (IsPropertyGridPropertyModified(pProp))
			return true;
	}//for

	return false;
}//bool MTEditPropList::IsPropertyGripModified()

void MTEditPropList::RevertChanges()
{
	int size = GetPropertyCount();
	for(int i = 0; i < size ; i++)
	{
		CMFCPropertyGridProperty *pProp = GetProperty(i);
		RevertChanges(pProp);
	}//for
}

bool MTEditPropList::IsPropertyGridPropertyModified(CMFCPropertyGridProperty *pProp)
{
	if(pProp->IsModified())
		return true;

	int childAmount = pProp->GetSubItemsCount();
	for(int i = 0; i < childAmount; i++)
	{
		CMFCPropertyGridProperty *pNextProp = pProp->GetSubItem(i);
		if (IsPropertyGridPropertyModified(pNextProp))
			return true;
	}//for

	return false;
}//bool MTEditPropList::IsPropertyGridPropertyModified(int index)

void MTEditPropList::RevertChanges(CMFCPropertyGridProperty *pProp)
{
	pProp->ResetOriginalValue();

	int childAmount = pProp->GetSubItemsCount();
	for(int i = 0; i < childAmount; i++)
	{
		CMFCPropertyGridProperty *pNextProp = pProp->GetSubItem(i);
		RevertChanges(pNextProp);
	}//for
}

void MTEditPropList::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

void MTEditPropList::OnKillFocus(CWnd* pNewWnd)
{
	//if (!IsChild(pNewWnd))
	//{
	//	if (m_pSel == NULL || m_pSel->OnKillFocus(pNewWnd))
	//	{
	//		EndEditItem();
	//		m_bFocused = FALSE;

	//		if (m_pSel != NULL)
	//		{
	//			m_pSel->Redraw();
	//		}
	//	}
	//}
	CMFCPropertyGridCtrl::OnKillFocus(pNewWnd);
}
