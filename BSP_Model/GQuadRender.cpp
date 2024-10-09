#include "stdafx.h"
#include "ppl.h"
#include "BDCEL.h"
#include "gbodyrender.h"
#include "MBSPNode.h"
#include "SOrigSurf.h"
#include "NLine.h"
#include "MElemIDProc.h"
#include "MQuadMask.h"
#include "math.h"
#include "NColor.h"
#include <vector>
#include "MQuadBody.h"
#include "MBSPCell.h"
#include <ppltasks.h>
#include "RRopes.h"
#include "gquadrender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

double GQuadRender::SmoothAngle = 44.;

GQuadRender::GQuadRender(const GQuadRender &inRender) :
	GBodyRender(inRender)
{
	ColsNum = inRender.ColsNum;
	RowsNum = inRender.RowsNum;
	if(inRender.ExtArr != nullptr)
	{
		ActivateExtArr();
		for(int ix = 0; ix < ColsNum; ++ix)
		{
			for(int iy = 0; iy < RowsNum; ++iy)
			{
				MBSPCell* pI = inRender.ExtArr[ix][iy];
				if(Empty(pI))
				{
					ExtArr[ix][iy] = pI;
					continue;
				}
				MBSPCell* pRet = new MBSPCell;
				*pRet = *pI;
				ExtArr[ix][iy] = pRet;
			}
		}
	}
	else
		ExtArr = nullptr;
}

unsigned int GQuadRender::GetRowsNum() const { return unsigned int(RowsNum); }
unsigned int GQuadRender::GetColsNum() const { return unsigned int(ColsNum); }

GQuadRender::GQuadRender(const MQuadBody &Tree) :
	GBodyRender(Tree)
{

	ColsNum = Tree.ColsNum;
	RowsNum = Tree.RowsNum;

	typedef  GLvoid** p2;
	typedef  GLvoid* p1;
	Array = new p2[ColsNum];
	GLvoid** bp = new p1[RowsNum * ColsNum];
	for (int ix = 0; ix < ColsNum; ++ix)
	{
		Array[ix] = bp + ix * RowsNum;
		for (int iy = 0; iy < RowsNum; ++iy)
			Array[ix][iy] = (GLvoid*)QR_EMPTY;
	}

	ExtArr = nullptr;
}

GQuadRender::~GQuadRender(void)
{
	ClearExtArr();
}

void GQuadRender::ClearExtArr(void)
{
	if(ExtArr == nullptr)
		return;

	for(int ix = 0; ix < ColsNum ; ++ix)
	{
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			if(!Empty(ExtArr[ix][iy]))
				delete ExtArr[ix][iy];
		}
	}
	delete[] ExtArr[0];
	delete[] ExtArr;
	ExtArr = nullptr;

}

int GQuadRender::FillAllArraysCh(MQuadBody & Body)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if(!Proper(Body))
		return -1;
	using namespace concurrency;
	if (NCM_PROJECT.GetUseQuadTreeParallel())
	{
		std::vector<task<void>> tasks;
		tasks.reserve(ColsNum * RowsNum);
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
				tasks.push_back(create_task([&, ix, iy] { FillCell(Body, ix, iy); }));
		auto joinTask = when_all(begin(tasks), end(tasks));
		// Wait for the tasks to finish.
		joinTask.wait();
	}
	else
	{
		for (int ix = 0; ix < ColsNum; ++ix)
			for (int iy = 0; iy < RowsNum; ++iy)
				FillCell(Body, ix, iy);
	}
	return 0;
}

int GQuadRender::FillModifArraysCh(MQuadBody & Tree)
{
	using namespace concurrency;
	std::vector<task<void>> tasks;
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if (!Proper(Tree))
		return -1;
	int count = 0;
	static_cast<MQuadMask *>(Tree.pModifiedCells)->StartIterate();
	int ix, iy;
	while (static_cast<MQuadMask *>(Tree.pModifiedCells)->NextPair(&ix, &iy))
	{
		tasks.push_back(create_task([&, ix, iy] { FillCell(Tree, ix, iy); }));
		++count;
	}
	pRopes->Clear();
	auto joinTask = when_all(begin(tasks), end(tasks));
	// Wait for the tasks to finish.
	joinTask.wait();
	return count;
}

bool GQuadRender::Proper(const MQuadBody & Tree)
{
	return (ColsNum == Tree.ColsNum && RowsNum == Tree.RowsNum);
}



void GQuadRender::StoreID(GLfloat *pRet, ElemID ElID)
{
	*((ElemID *)pRet) = ElID;
}

void GQuadRender::ActivateExtArr(void)
{
	typedef  MBSPCell** p2;
	typedef  MBSPCell* p1;
	ExtArr = new p2[ColsNum];
	MBSPCell** bp = new p1[RowsNum * ColsNum];
	for(int ix = 0; ix < ColsNum; ++ix)
	{
		ExtArr[ix] = bp + ix * RowsNum;
		for(int iy = 0; iy < RowsNum; ++iy)
			ExtArr[ix][iy] = (MBSPCell*)QR_EMPTY;
	}
}

double GQuadRender::GetSmoothAngle(void)
{
	return SmoothAngle; 
}

void GQuadRender::SetSmoothAngle(double Angle)
{
	SmoothAngle = Angle; 
}

void GQuadRender::GetCellIndexes(double x, double y, int & ix, int & iy) const
{
	double Xstep = Gabar.dX() / ColsNum;
	ix = min(ColsNum - 1, max(0, int((x - Gabar.GetMinPoint().GetX()) / Xstep)));
	double Ystep = Gabar.dY() / RowsNum;
	iy = min(RowsNum - 1, max(0, int((y - Gabar.GetMinPoint().GetY()) / Ystep)));
}

MQuadBody::Window GQuadRender::CalcWindow(const BBox& Bounds) const
{
	if (Bounds.Zmin > Gabar.Zmax ||
		Bounds.Zmax < Gabar.Zmin ||
		Bounds.Ymin > Gabar.Ymax ||
		Bounds.Ymax < Gabar.Ymin ||
		Bounds.Xmin > Gabar.Xmax ||
		Bounds.Xmax < Gabar.Xmin)
	{
		return MQuadBody::Window();
	}

	int xS, xE, yS, yE;
	GetCellIndexes(Bounds.Xmin, Bounds.Ymin, xS, yS);
	GetCellIndexes(Bounds.Xmax, Bounds.Ymax, xE, yE);
	MQuadBody::Window Win;
	Win.Set(xS, xE + 1, yS, yE + 1);
	return Win;
}

void GQuadRender::TransModifArraysCh(GBodyRender& QinRender)
{
	GQuadRender* pinRender = dynamic_cast<GQuadRender*>(&QinRender);
	if (pinRender == nullptr)
		return;
	GQuadRender& inRender = *pinRender;
	// Transfer non empty elements from inRender to this.
	// transfer substituted elements from this to inRender
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			GLvoid* pC = inRender.Array[ix][iy];
			if (NotDefined(pC))
				continue;
			inRender.Array[ix][iy] = Array[ix][iy];
			Array[ix][iy] = pC;
			if (inRender.ExtArr == NULL || ExtArr == NULL)
				continue;
			MBSPCell* pI = inRender.ExtArr[ix][iy];
			inRender.ExtArr[ix][iy] = ExtArr[ix][iy];
			ExtArr[ix][iy] = pI;
		}
	QinRender.pRopes->Clear();
	pRopes->Clear();
}
