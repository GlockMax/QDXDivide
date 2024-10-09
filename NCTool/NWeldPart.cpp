#include "stdafx.h"
#include "NColor.h"
#include "BDCEL.h"
#include "NWeldPart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NWeldPart, NStockStl, 0)


NWeldPart::NWeldPart(void) :
	NStockStl()
{
	VisibleSect = true;
}

void NWeldPart::CreateCopyTo(class NStock* &stock) const
{
	if(!stock)
		stock = new NWeldPart();
	NStockStl::CreateCopyTo(stock);

	((NWeldPart*)stock)->VisibleSect = this->VisibleSect;
}

NStock::StockCrType NWeldPart::GetType() const
{
	return SCT_WELD;
}

NWeldPart::~NWeldPart(void)
{
}
void NWeldPart::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool DrawSurf)
{
	NStockStl::DrawOriginal(GlobRMode, ExtRMode, DrawSurf);
// Draw sections
	if(!VisibleSect || GlobRMode == RM_TRANSLUCENT)
		return;
	glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);
	glDisable(GL_DEPTH_TEST);
	SectionSet.DrawSections();
	glPopAttrib();
}
void NWeldPart::Create()
{
	NStockStl::Create();
	MakeStockSections();
}

void NWeldPart::SetColor(const NColor & Col)
{
	NStockStl::SetColor(Col);
	if(SectArrParams.ColorByPart)
		SectionSet.SetAllColors(Color);
}

void NWeldPart::Serialize(CArchive &ar)
{
	NStockStl::Serialize(ar);
	SerializeElements(ar, &VisibleSect, 1);
}

void NWeldPart::MakeStockSections()
{
	SectionSet.Clear();
	switch(SectArrParams.Type)
	{
	case SAT_LINE:
		SectionSet.SetLineArray(SectArrParams.P + GetOrigZero() - BPoint(0., 0., 0., 1.), SectArrParams.V, SectArrParams.IntervalsNum, SectArrParams.StartVal, SectArrParams.EndVal);
		break;
	case SAT_CIRCLE:
		SectionSet.SetArcArray(SectArrParams.P + GetOrigZero() - BPoint(0., 0., 0., 1.), SectArrParams.V, SectArrParams.StartDir, SectArrParams.IntervalsNum, SectArrParams.StartVal, SectArrParams.EndVal);
		break;
	}
	SectionSet.MakeStockSections(this);
	SectionSet.SetThickness(GLfloat(SectArrParams.LineThickness));
	SectionSet.SetAllColors(SectArrParams.Color);
}
void NWeldPart::Transformm(const BMatr & M)
{
	NStockStl::Transformm(M);
	MakeStockSections();
}

NSectArrParams &NWeldPart::GetSectParams()
{
	SectArrParams.PartColor = GetColor();
	if(SectArrParams.ColorByPart)
		SectArrParams.Color = SectArrParams.PartColor;
	return SectArrParams;
}
