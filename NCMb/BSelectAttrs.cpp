#include "stdafx.h"
#include "NTiParams.h"
#include "BSelectAttrs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BSelectAttrs::BSelectAttrs()
{
	Main = NColor(1, 1, 0, 1); 
	Box = NColor(0, 0, 0, 0);
	Edge = NColor(0, 1, 0, 1);
	EdgeWidth = 3.;
	BoxWidth = 3.;
}

BSelectAttrs::BSelectAttrs(NTiParams& Par)
{
	CString Key(_T("Defaults/View/Select/"));
	BoxWidth = fmax(1., Par.GetDouble(Key + _T("Box@Thick"), 3.));
	EdgeWidth = Par.GetDouble(Key + _T("Edges@Thick"), 3.);
	Main.R() = fmaxf(0., float(Par.GetDouble((Key + "Color@R").GetBuffer(), 1.)));
	Main.G() = fmaxf(0., float(Par.GetDouble((Key + "Color@G").GetBuffer(), 1.)));
	Main.B() = fmaxf(0., float(Par.GetDouble((Key + "Color@B").GetBuffer(), 0.)));
	Main.A() = fmaxf(0., float(Par.GetDouble((Key + "Color@A").GetBuffer(), 1.)));
	Box.R() = fmaxf(0., float(Par.GetDouble((Key + "Box/Color@R").GetBuffer(), 0.)));
	Box.G() = fmaxf(0., float(Par.GetDouble((Key + "Box/Color@G").GetBuffer(), 0.)));
	Box.B() = fmaxf(0., float(Par.GetDouble((Key + "Box/Color@B").GetBuffer(), 0.)));
	Box.A() = fmaxf(0., float(Par.GetDouble((Key + "Box/Color@A").GetBuffer(), 0.)));
	Edge.R() = fmaxf(0., float(Par.GetDouble((Key + "Edges/Color@R").GetBuffer(), 0.)));
	Edge.G() = fmaxf(0., float(Par.GetDouble((Key + "Edges/Color@G").GetBuffer(), 1.)));
	Edge.B() = fmaxf(0., float(Par.GetDouble((Key + "Edges/Color@B").GetBuffer(), 0.)));
	Edge.A() = fmaxf(0., float(Par.GetDouble((Key + "Edges/Color@A").GetBuffer(), 1.)));
}

BSelectAttrs::~BSelectAttrs()
{
}

