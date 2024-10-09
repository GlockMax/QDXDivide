// MTPartArray.cpp: implementation of the MTPartArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RPickID.h"
#include "BaseRender.h"
#include "MTMachine.h"
#include "MTPart.h"
#include "MTPartVisProp.h"
#include "mtpartarray.h"
#include <vector>
#include "MTConfig.h"
#include "mtpartarray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//IMPLEMENT_SERIAL(MTPartArray,CObject,0)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTPartArray::MTPartArray(const MTMachine& iMT) : Size(0), MT(iMT)
{
	SelectedPart = -1;
}

MTPartArray::~MTPartArray()
{
	RemoveAll();
}

void MTPartArray::AddPart(MTPart *NP)
{
	Size++;
	Part.Add(NP);
}

void MTPartArray::RemovePart(int n)
{
	if(n>=GetSize() || GetSize()==0)
		return;
	Part.RemoveAt(n);
	Size--;
}

int MTPartArray::GetSize() const
{
	return Size;
}

void MTPartArray::RemoveAll()
{
	Size = 0;
	for(int i = 0; i < Part.GetSize(); ++i)
	{
		delete Part[i];
	}
	Part.RemoveAll();
}

bool MTPartArray::HasTranslucent()
{
	for(int i = 0; i < GetSize(); ++i)
	{
		MTPartVisProp *pProp = Part[i]->GetPartProp();
		bool IsTranslucent = (pProp->RMode == RM_TRANSLUCENT);
		delete pProp;
		if(IsTranslucent)
			return true;
	}
	return false;
}

void MTPartArray::Draw(enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSharpEdges/* = true*/, bool DrawSurf/* = true*/,  bool DrawSmoothSurf/* = true*/)
{
	if(GetSize() == 0)
		return;
	for(int i = 0; i < GetSize(); ++i)
	{
		glPushName(MTConfig::GetPartID(i));

		Part[i]->Draw(MT.GetCurConfig(), GlobRMode, RMode, DrawSharpEdges, DrawSurf, DrawSmoothSurf);

		if( i == SelectedPart && GlobRMode != RM_TRANSLUCENT)
		{
			Part[i]->DrawGabar(MT.GetCurConfig());
		}

		glPopName();
	}
}
MTPart* MTPartArray::operator [] (int i) const
{
	return Part[i];
}

BBox MTPartArray::GetGabar()
{
	int i;
	BBox Gab,tg;
	Gab.Init();
	for (i = 0; i < GetSize(); i++)
	{
		Gab.Expand(Part[i]->GetGabar(MT.GetCurConfig()));
	}
	return Gab;
}

void MTPartArray::SelectPart(int PartIndex)
{
	SelectedPart = PartIndex;
}
void MTPartArray::SelectPart(const CString& PartName)
{
	SelectedPart = -1;
	for (int i = 0; i < GetSize() && SelectedPart == -1; ++i)
	{
		if (Part[i]->GetAxisName() == PartName)
			SelectedPart = i;
	}
}

int MTPartArray::GetSelected() const
{
	return SelectedPart;
}
//void MTPartArray::SelectPart(int PartIndex)

const BPoint MTPartArray::RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N, RPickID& PickID) const
{
	PickID.Set0();
	BPoint R(0., 0., 0., -1.);
	for(int i = 0; i < GetSize(); ++i)
	{
		BPoint Nc;
		BPoint Rc = Part[i]->RayCasting(MT.GetCurConfig(), P, V, 1.e-6, Nc);
		if(Rc.GetH() > 0.)// Find nearest
		{
			if(R.GetH() > 0.)
			{
				if((Rc - P) * V < (R - P) * V)
				{
					R = Rc;
					N = Nc;
					PickID.Set2(OG_MACHINE, i);
				}
			}
			else
			{
				R = Rc;
				N = Nc;
				PickID.Set2(OG_MACHINE, i);
			}
		}
	}
	return R;
}

bool MTPartArray::FindSegmentsWithStl(SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr)
{
    for(int i = 0; i < GetSize(); ++i)
        Part[i]->FindSegmentsWithStl(MT.GetCurConfig(), oSegments, oMatrsInvr);
    return true;
}
