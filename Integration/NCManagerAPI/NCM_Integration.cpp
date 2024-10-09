#include "StdAfx.h"
#include "Afxmt.h"
#include "math.h"
#include "MBSPForest.h"
#include "NStockBox.h"
#include "NStockStl.h"
#include "NCadrGeom.h"
#include "FProg2Bsp.h"
#include "NToolCombinedMill.h"
#include "GQuadRenderMill.h"
#include "NToolStd.h"
#include "NToolShaped.h"
#include "BProgram.h"
#include "BRangePos.h"
#include "NCMProject.h"
#include "BDebugState.h"
#include "NSurfSettings.h"
#include "NCM_Integration.h"
MBSPForest Forest;
NCMProject Project;
const int MAX_GEOM = 10000;
NCM_Integration::NCM_Integration(void)
{
	pStock = NULL;
	for(int i = 0; i < MAX_TOOLS; ++i)
		ComTools[i] = NULL;
	ToolsNum = 0;
	Geom = new NCadrGeom[MAX_GEOM];
	GeomSize = 0;
	LastNormal[0] = 0.; LastNormal[1] = 0.; LastNormal[2] = 0.; 
	LastPoint[0] = 0.; LastPoint[1] = 0.; LastPoint[2] = 0.; 
	LastTool = -1;
	Buffer = new NCadrGeom[MAX_GEOM];
	BufferSize = 0;
}

NCM_Integration::~NCM_Integration(void)
{
	delete pStock;
	ClearTools();
	delete[] Geom;
}

bool NCM_Integration::setStock(const double min[3],const double max[3],double resolution,const UCHAR rgb[3])
{
	delete pStock;
	pStock = NULL;
	FProg2BSP::SetRender(NULL);

	BDebugState::Get().CellSize = 32;
	NSurfSettings::SetTolerance(0.04);
	BBox Box;
	Box.Expand(BPoint(min[0], min[1], min[2], 1.));
	Box.Expand(BPoint(max[0], max[1], max[2], 1.));
	NStockBox *pStockBox = new NStockBox; 
	pStock = pStockBox;
	if(pStockBox->Create(Box))
	{
		pStock->Reset();
		pStock->WFrame = FALSE;
		MElemIDProc::SetAllowPCPalUse(true);
		pStock->CreateRender();
		return true;
	}

	delete pStock;
	pStock = NULL;
	return false;
}

bool  NCM_Integration::setStock(const char* stlFileName,double resolution,const UCHAR rgb[3])
{
	delete pStock;
	pStock = NULL;

	NStockStl *pStockStl = new NStockStl;
	CString Name(stlFileName);
	pStockStl->CreateEx(_T("tool"));
	pStock = pStockStl;
	if(pStockStl->IsCreated)
	{
		pStockStl->Reset();
		return true;
	}

	delete pStock;
	pStock = NULL;
	return false;
}

bool NCM_Integration::renderStock()
{
	if(!pStock)
		return false;

	Cut();

	if(FProg2BSP::GetRender())
		FProg2BSP::GetRender()->Render(0);
	else
		pStock->Render(0);
	return true;
}

void  NCM_Integration::resetStock()
{
	if(!pStock)
		return;

	ClearTools();
	pStock->Reset();
	return ;
}
bool NCM_Integration::removeToolTrace(int ToolId
		, const double Bp[3]
		, const double Bn[3]
		, const double Ep[3]
		, const double En[3]
		, const double Ca[3]
		, bool Arc
		, bool CCW
		, bool checkCollison)
{
	if(!pStock)
		return false;
	--ToolId;
	if(LastTool < 0)
		LastTool = ToolId;
	BPoint LastN(LastNormal[0], LastNormal[1], LastNormal[2], 0.);
	BPoint LastP(LastPoint[0], LastPoint[1], LastPoint[2], 0.);
	if(LastN.D2() < 1.e-6)
	{
		LastNormal[0] = Bn[0];
		LastNormal[1] = Bn[1];
		LastNormal[2] = Bn[2];
		LastPoint[0] = Bp[0];
		LastPoint[1] = Bp[1];
		LastPoint[2] = Bp[2];
	}
	BPoint N(Bn[0], Bn[1], Bn[2], 0.);
	BPoint P(Bp[0], Bp[1], Bp[2], 1.);
	if((LastTool != ToolId) || (fabs(LastN.Angle(N)) > 5.) || GeomSize >= MAX_GEOM - 2 || (LastP - P).D2() > 0.01)
	{
		MoveBuffer();	
		Cut();
		LastTool = ToolId;
		LastNormal[0] = Bn[0];
		LastNormal[1] = Bn[1];
		LastNormal[2] = Bn[2];
	}
	//if((LastP - P).D2() > 0.01)
	//{
	//	GeomSize = 0;
	//}
// Fill NCadrGeom
	if(Arc)
	{
		Buffer[BufferSize].Set(CCW ? ccwarc : cwarc
			, Bp[0], Bp[1], Bp[2]
			, Ep[0], Ep[1], Ep[2]
			, Ca[0] - Bp[0], Ca[1] - Bp[1], Ca[2] - Bp[2] 
			, XY
			);
	}
	else
	{ 
		Buffer[BufferSize].Set(line
			, Bp[0], Bp[1], Bp[2]
			, Ep[0], Ep[1], Ep[2]
			);
	}
	++BufferSize;
	LastPoint[0] = Ep[0];
	LastPoint[1] = Ep[1];
	LastPoint[2] = Ep[2];
	if(BufferSize > 0)
	{
		if(LastNormal[2] > 0.8)
		{ // Top surf
			if(BufferLength() > 0.2)
			{
				MoveBuffer();
			}
			if(GeomSize > 1)
				if(fabs(Geom[GeomSize - 2].GetB().GetZ() - Geom[GeomSize - 1].GetE().GetZ()) < 0.2)
				{
					if(fabs(Geom[GeomSize - 1].GetB().GetZ() - Geom[GeomSize - 1].GetE().GetZ()) > 1.e-5)
					{// Is not XY
						BPoint B = Geom[GeomSize - 1].GetB();
						B.SetZ(Geom[GeomSize - 2].GetB().GetZ());
						Geom[GeomSize - 2].SetE(B);
						Geom[GeomSize - 1].SetB(B); 
					}
				}
		}
		else
		{ 
			if(fabs(LastNormal[0]) > 0.1 && fabs(LastNormal[1]) > 0.1)
			{// углы
				if(LastNormal[2] > 0.1)
				{//Last
					if(BufferLength() > 0.9)
					{
						MoveBuffer();
						Cut();
					}
					if(BufferTol() > 0.006 )
					{
						MoveBuffer();
						Cut();
					}
				}
				else
				{
					if(BufferLength() > 0.9)
					{
						MoveBuffer();
						Cut();
					}
				}
			}
			else
			{
				if(BufferLength() > 0.2)
				{
					MoveBuffer();
				}
			}
		}
	}
	return true; 
}
bool NCM_Integration::Cut()
{
	if(!pStock)
		return false;

	if(GeomSize > 0)
	{
		BPoint N0(LastNormal[0], LastNormal[1], LastNormal[2], 0.);
		BMatr M;
		M.e0cong(BPoint(0., 0., 1., 0.), N0);
		FProg2BSP::SetRotMatr(M);
		FProg2BSP::SetMatr(M.invr());
		BProgram Prog;
		BRangePos Pos(Prog);
		//Geom[GeomSize].Set(line 
		//	, LastPoint[0], LastPoint[1], LastPoint[2]
		//	, LastPoint[0] + LastNormal[0] * 0.1, LastPoint[1] + LastNormal[1] * 0.1, LastPoint[2] + LastNormal[2] * 0.1
		//		);
		Geom[GeomSize] = Geom[GeomSize - 1];
		Geom[GeomSize].Reverse();


		FProg2BSP::EGAMill3x(Geom, GeomSize + 1, GeomSize, ComTools[LastTool], *((MQuadTree *)pStock->GetBodyMill()), Pos);

		GeomSize = 0;
	}
//	FProg2BSP::SetRender(NULL);
   
	return true; 
}

int NCM_Integration::newCustomMillingTool(const char* contour,const UCHAR rgb[3])
{
	NToolCombinedMill *pComTool = new NToolCombinedMill(_T("ComTool"));
	if(!pComTool->MakeGeneric(contour, true))
	{
		delete pComTool;
		return -1;
	}
	
	ComTools[ToolsNum] = pComTool;
	setToolColor(ToolsNum, rgb);
	++ToolsNum;
	
	return ToolsNum;
}

void NCM_Integration::ClearTools(void)
{
	for(int i = 0; i < MAX_TOOLS; ++i)
	{
		delete ComTools[i];
		ComTools[i] = NULL;
	}
	ToolsNum = 0;
}

bool  NCM_Integration::setToolColor(int toolId, const unsigned char rgb[3])
{
	if(rgb == NULL)
		return true;
	--toolId;
	if(toolId >= ToolsNum || toolId < 0)
		return true;
	ComTools[toolId]->Color.SetRGB(RGB(rgb[0], rgb[1], rgb[2]));
	return true;
}

double NCM_Integration::BufferLength(void)
{
	double Length = 0.;
	for(int i = 0; i < BufferSize; ++i)
		Length += Buffer[i].Length();
	return Length * Length;
}
double NCM_Integration::BufferHeight(void)
{
	if(BufferSize == 0)
		return 0.;
	return fabs((Buffer[0].GetB() - Buffer[BufferSize - 1].GetE()) * BPoint(LastNormal[0], LastNormal[1], LastNormal[2], 0));
}
void NCM_Integration::MoveBuffer(void)
{
	if(BufferSize <= 0)
		return;
	Geom[GeomSize] = Buffer[0];
	Geom[GeomSize].SetE(Buffer[BufferSize - 1].GetE());
	BufferSize = 0;
	++GeomSize;
}
double NCM_Integration::BufferTol(void)
{
	if(BufferSize == 0)
		return 0.;
	double Tol = 0.;
	BPoint S(Buffer[0].GetB());
	BPoint E(Buffer[BufferSize - 1].GetE());
	for(int i = 0; i < BufferSize - 1; ++i)
	{
		double T = Buffer[i].GetE().LineD2(S, E);
		Tol = max(Tol, T);
	}
	return Tol;
}