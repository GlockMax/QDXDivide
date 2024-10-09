// NProgram.cpp: implementation of the NProgram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NGeomArray.h"
#include "Gl\gl.h"
#include "NCUnit.h"
#include "NCadr.h"
#include "PropList.h"
#include "NCMProject.h"
#include "NProgram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
IMPLEMENT_SERIAL(NProgram, BProgram, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NProgram::NProgram() : BProgram()
{
	pSciDoc = nullptr;
}

NProgram::~NProgram()
{
}

CString NProgram::NLoad(const CString& filename, const NCUnit& ThisUnit)
{
	CString Ret = BLoad(filename);
	if(!Ret.IsEmpty())
	{
		// Translit
		CString Val;
		if(ThisUnit.GetOtherValue("Translit", Val))
		{
			if(Val == "Yes")
			{
				if (ThisUnit.GetWordValue("CommentStart", Val))
				{
					char Cs = Val[0];
					if (ThisUnit.GetWordValue("CommentEnd", Val))
					{
						char Ce = Val[0];
						TranslitText(true, Cs, Ce);
					}
				}
			}
		}
		// Add additional text if any
		if(!ThisUnit.GetSCycleDef().IsEmpty())
		{
			CString Symbol;
			ThisUnit.GetWordValue("AdditionalTextStart", Symbol);
			CadrArray.Add(new NCadr(Symbol));
			for(int i = 0; i < ThisUnit.GetSCycleDef().GetSize(); ++i)
				CadrArray.Add(new NCadr(ThisUnit.GetSCycleDef().GetAt(i)));
		}
	}
	return Ret;
}

void NProgram::TranslitText(bool Tr2Ru, char Cs, char Ce)
{
	CString Val;
	for(int i = 0; i < CadrArray.GetSize(); ++i)
	{
		CString &Str = CadrArray[i]->GetTextMod();
				
		for(int Is = Str.Find(Cs), Ie = 0; Is >= 0; Is = Str.Find(Cs, Is + 1))
		{
			Ie = Str.Find(Ce, Is);
			if(Ie < 0)
				Ie = Str.GetLength();
			CString Comment = Str.Mid(Is + 1, Ie - Is - 1);
			Str.Delete(Is + 1, Ie - Is - 1);
			Str.Insert(Is + 1, TranslitRus(Comment, Tr2Ru));
		}
	}
}

void NProgram::SetSciDoc(void* pDoc)
{
	pSciDoc = pDoc;
}

void NProgram::DrawInt(int StockPos, bool CompOn, bool ProgOn, bool Draw4Pick, const std::vector<BProgVisInterval> & VisInt
	, int BoldCadr /*= -1*/, double t, bool UseAxes /*= false*/, bool DrawNodes /*= false*/)
{
	if(VisInt.empty())
		return;
	bool VBOEnabled = BaseRender::IsVBOEnabled();
	if(VBOEnabled)
	{
		BProgram::DrawInt(StockPos,	CompOn, ProgOn, Draw4Pick, VisInt, BoldCadr, t, UseAxes, DrawNodes);
	}
	else
	{
		glPushAttrib(GL_CURRENT_BIT);
		for(int kg = 0; kg < VisInt.size(); ++kg)
		{
			if(CompOn)
			{
				glColor4fv(GetColor().Inv().array);
				((NGeomArray *)&GetCompGeomArrayByStPos(StockPos))->Draw(VisInt[kg].GetStart(), VisInt[kg].GetEnd(), BoldCadr == -1 ? -1 : BoldCadr + 1, t);
			}
			if(ProgOn)
			{
				glColor4fv(GetColor().array);
				((NGeomArray *)&GetGeomArrayByStPos(StockPos))->Draw(VisInt[kg].GetStart(), VisInt[kg].GetEnd(), BoldCadr == -1 ? -1 : BoldCadr + 1, t);
			}
		}
		glPopAttrib();
	}
}

PropList * NProgram::Create(PropList * List)
{
	List->Clear();

	if(!IsAux())
	{
		// Активность
		PropSingle * h = new PropSingle();
		h->ID=1;
		h->Name=IDS_ACTIVE;
		h->Type=1;
		h->Comment=IDS_PROP_PROG_ONE_ACTIVE;
		h->Group=IDS_PROP_PROG_ONE;
		h->ST_Value = (Active ? IDS_TRUE : IDS_FALSE);
		h->For_Enum.Add(IDS_TRUE);
		h->For_Enum.Add(IDS_FALSE);
		h->Color = 1;
		List->Add(h);
		// Visibility
		h = new PropSingle();
		h->ID=2;
		h->Name=IDS_VISIBILITY;
		h->Type=1;
		h->Comment=IDS_COMM_PROG_ONE;
		h->Group=IDS_PROP_PROG_ONE;
		h->ST_Value = (Visible ? IDS_TRUE : IDS_FALSE);
		h->For_Enum.Add(IDS_TRUE);
		h->For_Enum.Add(IDS_FALSE);
		h->Color = 1;
		List->Add(h);
		// Thickness
		h = new PropSingle();
		h->ID=3;
		h->Name=IDS_PROG_THIKNESS;
		h->Type=4;
		h->Comment=IDS_COMM_PROG_THICK;
		h->Group=IDS_PROP_PROG_ONE;
		h->V_Value=Thickness;
		h->Max = 10.;
		h->Min = 1.;
		h->Color = 1;
		List->Add(h);
		// Color
		h = new PropSingle();
		h->ID=4;
		h->Name=IDS_COLOR;
		h->Type=3;
		h->Comment=IDS_COMM_PROG_COLOR;
		h->Group=IDS_PROP_PROG_ONE;
		h->V_Value = GetColor().GetRGB();
		h->Color = 1;
		List->Add(h);
		// Comment
		h = new PropSingle();
		h->ID=5;
		h->Name=IDS_PROG_COMMENT;
		h->Type=2;
		h->Comment=IDS_COMM_PROG_COMMENT;
		h->Group=IDS_PROP_PROG_ONE;
		h->Value = Comment;
		h->Color = 1;
		List->Add(h);
		// Program size
		h = new PropSingle();
		h->ID=6;
		h->Name=IDS_PROG_SIZE;
		h->Type=5;
		h->Comment=IDS_COMM_PROG_SIZE;
		h->Group=IDS_PROP_PROG_ONE;
		h->V_Value=GetSize();
		h->Color = 0;
		List->Add(h);
		// Program length
		h = new PropSingle();
		h->ID=7;
		h->Name=IDS_PROG_LENGTH;
		h->Type=4;
		h->Comment=IDS_COMM_PROG_LENGTH;
		h->Group=IDS_PROP_PROG_ONE;
		h->V_Value = GetInfo().FastLength + GetInfo().WorkLength;
		h->Color = 0;
		List->Add(h);
		// Program time
		h = new PropSingle();
		h->ID=8;
		h->Name=IDS_PROG_TIME;
		h->Type=6;
		h->Comment=IDS_COMM_PROG_TIME;
		h->Group=IDS_PROP_PROG_ONE;
		h->Time = GetInfo().Time;
		h->Color = 0;
		List->Add(h);
		// File path
		h = new PropSingle(); 
		h->ID = 10;
		h->Name = IDS_PROP_STLFILE;
		h->Type = 2;
		h->Comment = IDS_COMM_PROP_STLFILE;
		h->Group = IDS_PROP_PROG_ONE;
		h->Value = GetInfo().FileName;
		h->Color = 0;
		List->Add(h);
	}
	else
	{// Is aux
		// Protected
		PropSingle * h = new PropSingle();
		h->ID = 9;
		h->Name = IDS_PROTECTED;
		h->Type = 1;
		h->Comment = IDS_COMM_PROTECTED;
		h->Group = IDS_PROP_SUBPROG;
		h->ST_Value = (Protected && NCUnit::IsFileProtectionEnabled() ? IDS_TRUE : IDS_FALSE);
		h->For_Enum.Add(IDS_TRUE);
		h->For_Enum.Add(IDS_FALSE);
		h->Color = NCUnit::IsFileProtectionEnabled() ? 1 : 0;
		List->Add(h);
		// Comment
		h = new PropSingle();
		h->ID=5;
		h->Name=IDS_PROG_COMMENT;
		h->Type=2;
		h->Comment=IDS_COMM_PROG_COMMENT;
		h->Group=IDS_PROP_SUBPROG;
		h->Value = Comment;
		h->Color = 1;
		List->Add(h);
		// Program size
		h = new PropSingle();
		h->ID=6;
		h->Name=IDS_PROG_SIZE;
		h->Type=5;
		h->Comment=IDS_COMM_PROG_SIZE;
		h->Group=IDS_PROP_SUBPROG;
		h->V_Value=GetSize();
		h->Color = 0;
		List->Add(h);
		// File path
		h = new PropSingle();
		h->ID = 10;
		h->Name = IDS_PROP_STLFILE;
		h->Type = 2;
		h->Comment = IDS_COMM_PROP_STLFILE;
		h->Group = IDS_PROP_SUBPROG;
		h->Value = GetInfo().FileName;
//		h->Value = ((NProgram *)this)->GetInfo().FileName;
		h->Color = 0;
		List->Add(h);
	}

	return List;
}

PropList * NProgram::Change(PropList *List, PropSingle *Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;
	switch(id)
	{
	case 1: // IDS_ACTIVE
		{
			bool NewActive = (Single->ST_Value == IDS_TRUE);
			if(Active != NewActive)
			{
				Group = OG_PROGRAMS;
				Active = NewActive;
			}
		}
		break;
	case 2: // IDS_VISIBILITY
		Visible = (Single->ST_Value == IDS_TRUE);
		break;
	case 3: // Thickness
		Thickness = Single->V_Value;
		if(Thickness <= 0)
			Thickness = 1;
		break;
	case 4: // Color
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			SetColor(Col);
			PrepRenders(NCM_PROJECT.GetDouble("TolLineApp", 0.01));
		}
		break;
	case 5: // Comment
		SetComment(Single->Value);
		break;
	case 9: // Protection
		Protected = (Single->ST_Value == IDS_TRUE);
		break;
	}
	return 	Create(List);
}

bool NProgram::NReRead(const CString& FName, const NCUnit& ThisUnit)
{
	// Re read program from FName file
	CStdioFile f;
	if (!f.Open(FName, CFile::typeText | CFile::modeRead))
		return false;
	FILETIME Creation, Access, Write;
	::GetFileTime(f, &Creation, &Access, &Write);
	f.Close();
	if (CTime(Write) == GetFileTime())
		return true;
	for (int n = 0; n < CadrArray.GetSize(); n++)
		delete CadrArray[n];
	CadrArray.RemoveAll();
	NLoad(FName, ThisUnit);
	ManChanged = FALSE;// The program was not manualy changed
	return true;
}

