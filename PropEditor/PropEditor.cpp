#include "stdafx.h"
#include "NObjEnums.h"
#include "WM_APP_messages.h"
#include "NCenums.h"
#include "PAbstractObject.h"
#include "PropList.h"
#include "PropPane.h"
#include "PropEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PropEditor::PropEditor(PropPane *pPane)
{
	pList = new PropList();
	pPropPane = pPane;
	Clear();
}
PropEditor::~PropEditor()
{
	pList->Clear();
	delete pList;
}

void PropEditor::Put(PAbstractObject * Obj)
{
	pabstractObject = Obj;
	if (Obj != NULL)
	{
		PropList *NewList = new PropList;
		Obj->Create(NewList);
		if(*NewList == *pList)
		{
			NewList->Clear();
			delete NewList;
		}
		else
		{
			pList->Clear();
			delete pList;
			pList = NewList;
			pPropPane->Paste(pList, NULL, 0);
		}
	}
	else 
	{
		pList->Clear();
		pPropPane->Clear();
	}
}

void PropEditor::Change(int id, const CString &value)
{
	PropSingle * pProp = NULL;
	
	bool LimitsReached = false;
	for(int i = 0; i < pList->GetSize(); ++i)
	{
		int ID = pList->GetAt(i)->ID;
		if (pList->GetAt(i)->ID == id)
		{
			pProp = pList->Copy(pList->GetAt(i));
			break;
		}
	}
	if(!pProp)
		return;
	if(pProp->Type == 1)
	{
		for (int y = 0; y < pProp->For_Enum.GetSize(); ++y)
		{
			CString k;
			pPropPane->LoadStringID(k,  pProp->For_Enum.GetAt(y));
			k = k.SpanExcluding(_T("\n"));
			if (k == value)
				pProp->ST_Value = pProp->For_Enum.GetAt(y);
		}
	}
	else if(pProp->Type == 2)
	{
		pProp->Value = value;
	}
	else if(pProp->Type == 3)
	{
		pProp->V_Value = atof(value);
	}
	else if (pProp->Type == 4)
	{
		CString j = value;
		j.Replace(",", ".");
		if(pProp->Max != 0 || pProp->Min != 0)
		{
			if(pProp->Max < atof(j))
			{
				LimitsReached = true;
				pProp->V_Value = pProp->Max;
			}
			else if(pProp->Min > atof(j))
			{
				LimitsReached = true;
				pProp->V_Value = pProp->Min;
			}
			else
				pProp->V_Value = atof(j);
		}
		else
			pProp->V_Value=atof(j);
	}
	else if(pProp->Type == 5)
	{
		if(pProp->Max != 0 || pProp->Min != 0)
		{
			if(pProp->Max < atof(value))
			{
				LimitsReached = true;
				pProp->V_Value = pProp->Max;
			}
			else if(pProp->Min > atof(value))
			{
				LimitsReached = true;
				pProp->V_Value = pProp->Min;
			}
			else
				pProp->V_Value = atof(value);
		}
		else
			pProp->V_Value = atof(value);
	}

	UpdateModes AddModes;
	int Group;
	pList = pabstractObject->Change(pList, pProp, id, AddModes, Group);

	// Uncomment if changing of one property can change another one
	//PropList *NewList = new PropList;
	//pabstractObject->Change(NewList, pProp, id, AddModes, Group);
	//if (*NewList == *pList)
	//{
	//	NewList->Clear();
	//	delete NewList;
	//}
	//else
	//{
	//	pList->Clear();
	//	delete pList;
	//	pList = NewList;
	//	pPropPane->Paste(pList, NULL, 0);
	//}

	if(LimitsReached)
	{
		if(!pList->IsEmpty())
			pPropPane->Paste(pList, pProp, id);
	}
	delete pProp;
	AfxGetMainWnd()->SendMessage(WM_APP_REGENERATE, N_OGL | N_TOOLCHANGED | N_LEFT | AddModes, Group); // N_SETTINGS required for text view regeneration with new tool color
}
void PropEditor::Clear()
{
	pList->Clear();
	pabstractObject = NULL;
}
