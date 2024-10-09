#include "stdafx.h"
#include "PropList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

PropList::PropList()
{
}

PropList::~PropList()
{
}

PropSingle * PropList::Copy(PropSingle * Single)
{
	PropSingle * single2 = new PropSingle();
	single2->Color = Single->Color;
	single2->Comment = Single->Comment;
	single2->Group = Single->Group;
	single2->ID = Single->ID;
	single2->Max = Single->Max;
	single2->Min = Single->Min;
	single2->Name = Single->Name;
	single2->ST_Value = Single->ST_Value;
	single2->Time = Single->Time;
	single2->Type = Single->Type;
	single2->V_Value = Single->V_Value;
	single2->Value = Single->Value;
	for (int k = 0; k < Single->For_Enum.GetSize(); ++k)
	{
		single2->For_Enum.Add(Single->For_Enum.GetAt(k));
	}
	return single2;
}
void PropList::Clear()
{
	if (!IsEmpty())
	{
		for(int i = 0; i < GetSize(); ++i)
			delete GetAt(i);
		RemoveAll();
	}
}

bool PropList::operator == (const PropList &InList)
{
	if(IsEmpty() != InList.IsEmpty())
		return false;
	if(InList.GetSize() != GetSize())
		return false;
	for(int i = 0; i < GetSize(); ++i)
	{
		PropSingle *InProp = InList.GetAt(i);
		PropSingle *MyProp = GetAt(i);
		if(InProp->Type != MyProp->Type) 
			return false;
		switch(InProp->Type)
		{
		case 1:
			if(InProp->ST_Value != MyProp->ST_Value)
				return false;
			break;
		case 2:
			if(InProp->Value != MyProp->Value)
				return false;
			break;
		case 3:
		case 4:
		case 5:
			if(InProp->V_Value != MyProp->V_Value)
				return false;
			break;
		case 6:
			if(InProp->Time != MyProp->Time)
				return false;
			break;
		default:
			return false;
		}
	}
	return true;
}
