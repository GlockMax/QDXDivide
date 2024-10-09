#include "stdafx.h"
#include "NCMComData.h"
#include "NCMVersion.h"
#include "NSurfSettings.h"
#include "math.h"
#include "MBSPNode.h"
#include "SOrigSurf.h"
#include "ConstDef.h"
#include "MElemIDProc.h"
#include "NColor.h"
#include "NTiParams.h"
#include "BaseRender.h"
#include "gquadrenderrot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

double GQuadRenderRot::StartAngle = 0.;
double GQuadRenderRot::Angle = 270.;
double GQuadRenderRot::AngleStep = 2.;
int GQuadRenderRot::AngleStepsNum = 135;
double GQuadRenderRot::StartAngleB = 0.;
double GQuadRenderRot::AngleB = 270.;
double GQuadRenderRot::AngleStepB = 2.;
int GQuadRenderRot::AngleStepsNumB = 135;
int GQuadRenderRot::StoreCounter = 0;
NColor GQuadRenderRot::AxialSectionColorStock(0., 1., 1., 1.);
NColor GQuadRenderRot::AxialSectionColorPart(1., 1., 0., 1.);

GQuadRenderRot::GQuadRenderRot(const MQuadBody &Tree) :
	GQuadRender(Tree)
{
	StockRender = true;
}

GQuadRenderRot* GQuadRenderRot::CreateGQuadRenderRotList(const MQuadBody &Tree)
{
	MBody *pBody = Tree.GetNextMBody();
	GQuadRenderRot* pResult = new GQuadRenderRot(Tree);
	while (pBody)
	{
		GQuadRenderRot* pNext = new GQuadRenderRot( *((MQuadBody *)pBody) );
		pResult->AddRenderInList(pNext);
		pBody = pBody->GetNextMBody();
	}
	return pResult;
}

GQuadRenderRot::GQuadRenderRot(const GQuadRenderRot & inRender) :
	GQuadRender(inRender)
{
	StockRender = inRender.StockRender;

	typedef  GLvoid **p2;
	typedef  GLvoid *p1;

	Array = new p2[ColsNum];
	GLvoid **bp = new p1[RowsNum * ColsNum];
	for(int ix = 0; ix < ColsNum; ++ix)
	{
		Array[ix] = bp + ix * RowsNum;
		for(int iy = 0; iy < RowsNum; ++iy)
		{
			GLfloat *pC = (GLfloat *)inRender.Array[ix][iy];
			if(Empty(pC))
			{
				Array[ix][iy] = pC;
				continue;
			}
			int LinesNum = int(pC[0]);
			GLfloat *pRet = new GLfloat[1 + LINE_SIZE * LinesNum];
			memcpy(pRet, pC, (1 + LINE_SIZE * LinesNum) * sizeof(GLfloat));
			Array[ix][iy] = pRet;
		}
	}
}

GQuadRenderRot::~GQuadRenderRot(void)
{
	for(int ix = 0; ix < ColsNum ; ++ix)
	{
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			if(!Empty(Array[ix][iy]))
				delete[] Array[ix][iy];
		}
	}
	delete[] Array[0];
	delete[] Array;
}
void GQuadRenderRot::FillCell(MQuadBody & Body, int ix, int iy)
{
	//if(Body.GetModelMode() != NGlobalState::BSP)
	//	return;
	MQuadTree::ActLocStorage Act(static_cast<MQuadTree&>(Body), ix, iy);
	const MQuadTree &Tree = (MQuadTree &)Body;
	if(!Empty(Array[ix][iy]))
		delete[] Array[ix][iy];
	Array[ix][iy] = FillContArray(Tree.GetRootNodeV(ix, iy), SmoothRender);
}

int GQuadRenderRot::RenderCh(enum RenderMode GlobRMode, int Type, bool DrawSurf)
{
	GlobRMode; // Unused

	Type |= 3;
// Type : 1 - main, 2 - sections, 4 - edges, (8 - glitter : removed )
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	glPushMatrix();
	glRotated(90., 1., 0., 0.);
	glRotated(StartAngle, 0., 1., 0.);
	if((Type & 2) && Angle < 360.)
	{

		
		RenderAxialSection();

		glPushMatrix();
		glRotated(Angle, 0.,1.,0.);
		RenderAxialSection();
		glPopMatrix();

	}


	if((Type & 1) )
	{
		if(DrawSurf)
			RenderOrd();
	}
	glPopMatrix();
	singleLock.Unlock();
	return 0;
}

GLvoid * GQuadRenderRot::FillContArray(MBSPNode *pRoot, bool Smooth)
{// Returns GLfloat array. First element of the array contains lines number
	if(MBSPNode::IsTerminal(pRoot))
		return (GLvoid *)QR_EMPTY_DEF;

	MFOREST.SetLocMark();
	MBSPNode FacetRoot;
	pRoot->CrFacets(FacetRoot);
	bool fl = false;
	int LinesNum = 0;
	for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
	{
		for(MBSPNode *pN = pNode->GetRight() ; !MBSPNode::IsTerminal(pN) ; pN = pN->GetLeft())
		{
			if(pN->GetFace()->Area2() > MINST * MINST && pN->GetFace()->GetCount() == 4)
			{
				LinesNum++;
				fl = true;
			}
		}
	}
	GLfloat *pRet = (GLfloat *)QR_EMPTY_DEF;
	//double SmDist2 = 1.e-2;
	if(fl)
	{
		pRet = new GLfloat[1 + (LINE_SIZE) * LinesNum];
		pRet[0] = GLfloat(LinesNum);
		int LineInd = 0;
		for(MBSPNode *pNode = &FacetRoot ; !MBSPNode::IsTerminal(pNode); pNode = pNode->GetLeft())
		{
			ElemID ElID = pNode->GetBFaceN();
			int BFaceN = MFOREST.GetIDProc()->GetSurfID(ElID);
			PackedColor col = MFOREST.GetIDProc()->GetColor(ElID);
			
			bool SmoothLoc = Smooth && (BFaceN >= 0 && BFaceN != ID_EMPTY);
			for(MBSPNode *pN = pNode->GetRight(); !MBSPNode::IsTerminal(pN); pN = pN->GetLeft())
			{
				const MFace *pFace = pN->GetFace();
				if(pFace->Area2() > MINST * MINST && pFace->GetCount() == 4)
				{
					//find p0 and p1
                    const BPoint *p0, *p1;
					
					for(int ii = 0; ii < 4; ++ii)
					{
						p0 = pFace->GetPoint(ii);
						p1 = pFace->GetPoint((ii + 1) % 4);
						if(p0->GetZ() < 0. && p1->GetZ() < 0.)
							break;
					}
					BPoint n0b(pFace->GetPlane().GetNormal());
					BPoint n0(n0b);
					if( SmoothLoc )
					{
						SOrigSurf *pSurf = MFOREST.GetOrigSurf(BFaceN);
						//if((pSurf->GetPoint(*p0) - *p0).D2() < SmDist2)
						{
							n0 = pSurf ? pSurf->GetNormal(*p0) : n0b;
							if( n0.GetH() < -0.5)
								n0 = n0b;
							else if(n0 * n0b < 0.)
								n0 = n0 * -1.;
						}
					}
					BPoint n1(n0b);
					if( SmoothLoc )
					{
						SOrigSurf *pSurf = MFOREST.GetOrigSurf(BFaceN);
						//if((pSurf->GetPoint(*p1) - *p1).D2() < SmDist2)
						{
							n1 = pSurf ? pSurf->GetNormal(*p1) : n0b;
							if( n1.GetH() < -0.5)
								n1 = n0b;
							else if(n1 * n0b < 0.)
								n1 = n1 * -1.;
						}
					}
					double dirNk = -1.;// заменено после добавления знаков радиуса в GetNormal
					StoreLine(pRet + LINE_SIZE * LineInd + 1, col,
						*p0, n0 * dirNk, *p1, n1 * dirNk);
					++LineInd;
				}
			}
		}
	}
	MFOREST.RetToLocMark();
	FacetRoot.DeleteAll();
	return (GLfloat *)pRet;
}

void GQuadRenderRot::StoreLine(GLfloat *pRet, const PackedColor col, 
				const BPoint & p0, const BPoint & n0,
				const BPoint & p1, const BPoint & n1)
{
	pRet[ 0] = GLfloat(n0.GetX());
	pRet[ 1] = GLfloat(n0.GetY());
	pRet[ 2] = GLfloat(p0.GetX());
	pRet[ 3] = GLfloat(p0.GetY());
	pRet[ 4] = col.fval;
	pRet[ 5] = GLfloat(n1.GetX());
	pRet[ 6] = GLfloat(n1.GetY());
	pRet[ 7] = GLfloat(p1.GetX());
	pRet[ 8] = GLfloat(p1.GetY());
	pRet[ 9] = col.fval;
}

double GQuadRenderRot::GetMaxX(void) const
{
	double Xmax = 0.;
	for(int ix = ColsNum - 1; ix >= 0 ; --ix)
	{
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			for(int i = 0; i < int(pC[0]); ++i)
			{
				Xmax = max( Xmax
						, max(pC[LINE_SIZE * i + 1 + 2]
							, pC[LINE_SIZE * i + 1 + 2 + 5]));
			}
		}
		if(Xmax > 0.)
			break; // next column can't contain grater X
	}

	return Xmax;
}
void GQuadRenderRot::WriteSTL(CStdioFile & f) const
{
	double MaxX = GetMaxX();
	int SectorsNum = 4 * NSurfSettings::GetNumAproxLine(MaxX, 90.);
	BMatr RotM, Rot;
	Rot.RotX(0. ,0., 0., 90.);
	double AStep = 360. / SectorsNum;
	RotM.RotZ(BPoint(0.,0.,0.,1.), -AStep);

	int LinesNum = 0;
	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			LinesNum += int(pC[0]);
		}
	if(LinesNum <= 0)
		return;
	__int32 TriNum = LinesNum * SectorsNum * 2;
	f.SeekToBegin();
	char s[5*16+1] = "STL export                                                                      ";
	f.Write(s, 5*16);// title
	f.Write(&TriNum, 4);
	char buf[2];
	GLfloat fbuf[4*3];
	buf[0] = '\x0';
	buf[1] = '\x0';

	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			for(int i = 0; i < int(pC[0]); ++i)
			{
				BPoint P0(pC[LINE_SIZE * i + 1 + 2], pC[LINE_SIZE * i + 2 + 2], 0.,1.);
				BPoint P1(pC[LINE_SIZE * i + 1 + 2 + 5], pC[LINE_SIZE * i + 2 + 2 + 5], 0.,1.);
				BPoint N0(-pC[LINE_SIZE * i + 1], -pC[LINE_SIZE * i + 2], 0.,0.);
				P0 = P0 * Rot;
				P1 = P1 * Rot;
				N0 = N0 * Rot;
				BPoint N = (P0 - P1) % (P0 * RotM - P0);
				double d = sqrt(N.D2());
				if(d > DMIN)
				{
					d = (N * N0) > 0 ? 1. / d : -1. / d;
					N = N * d;
				}
				else
					N = N0;
				for(int k = 0; k < SectorsNum; ++k)
				{
					fbuf[0] = GLfloat(N.GetX());// Normal
					fbuf[1] = GLfloat(N.GetY());// Normal
					fbuf[2] = GLfloat(N.GetZ());// Normal
					BPoint P01 = P0 * RotM;
					BPoint P11 = P1 * RotM;

					fbuf[3] = GLfloat(P0.GetX());
					fbuf[4] = GLfloat(P0.GetY());
					fbuf[5] = GLfloat(P0.GetZ());

					fbuf[6] = GLfloat(P1.GetX());
					fbuf[7] = GLfloat(P1.GetY());
					fbuf[8] = GLfloat(P1.GetZ());

					fbuf[9] = GLfloat(P01.GetX());
					fbuf[10] = GLfloat(P01.GetY());
					fbuf[11] = GLfloat(P01.GetZ());

					f.Write(fbuf, 4*3*4);
					f.Write(buf, 2);

					fbuf[3] = GLfloat(P01.GetX());
					fbuf[4] = GLfloat(P01.GetY());
					fbuf[5] = GLfloat(P01.GetZ());

					fbuf[6] = GLfloat(P1.GetX());
					fbuf[7] = GLfloat(P1.GetY());
					fbuf[8] = GLfloat(P1.GetZ());

					fbuf[9] = GLfloat(P11.GetX());
					fbuf[10] = GLfloat(P11.GetY());
					fbuf[11] = GLfloat(P11.GetZ());

					f.Write(fbuf, 4*3*4);
					f.Write(buf, 2);

					P0 = P0 * RotM;
					P1 = P1 * RotM;
					N = N * RotM;
				}
			}
		}
}
float* GQuadRenderRot::MakeSTLArray(int& TriNum) const
{
	double MaxX = GetMaxX();
	int SectorsNum = 4 * NSurfSettings::GetNumAproxLine(MaxX, 90.);
	BMatr RotM, Rot;
	Rot.RotX(0., 0., 0., 90.);
	double AStep = 360. / SectorsNum;
	RotM.RotZ(BPoint(0., 0., 0., 1.), -AStep);

	int LinesNum = 0;
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			GLfloat* pC = (GLfloat*)Array[ix][iy];
			if (Empty(pC))
				continue;
			LinesNum += int(pC[0]);
		}
	if (LinesNum <= 0)
		return nullptr;
	TriNum = LinesNum * SectorsNum * 2;
	float* Triangles = new float[TriNum * 3 * 3/* + 1*/];
	auto *fbuf = Triangles;
	//fbuf[0] = float(TriNum);
	//++fbuf;
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			GLfloat* pC = (GLfloat*)Array[ix][iy];
			if (Empty(pC))
				continue;
			for (int i = 0; i < int(pC[0]); ++i)
			{
				BPoint P0(pC[LINE_SIZE * i + 1 + 2], pC[LINE_SIZE * i + 2 + 2], 0., 1.);
				BPoint P1(pC[LINE_SIZE * i + 1 + 2 + 5], pC[LINE_SIZE * i + 2 + 2 + 5], 0., 1.);
				P0 = P0 * Rot;
				P1 = P1 * Rot;
				for (int k = 0; k < SectorsNum; ++k)
				{
					BPoint P01 = P0 * RotM;
					BPoint P11 = P1 * RotM;

					fbuf[0] = GLfloat(P0.GetX());
					fbuf[1] = GLfloat(P0.GetY());
					fbuf[2] = GLfloat(P0.GetZ());

					fbuf[3] = GLfloat(P1.GetX());
					fbuf[4] = GLfloat(P1.GetY());
					fbuf[5] = GLfloat(P1.GetZ());

					fbuf[6] = GLfloat(P01.GetX());
					fbuf[7] = GLfloat(P01.GetY());
					fbuf[8] = GLfloat(P01.GetZ());

					fbuf[9] = GLfloat(P01.GetX());
					fbuf[10] = GLfloat(P01.GetY());
					fbuf[11] = GLfloat(P01.GetZ());

					fbuf[12] = GLfloat(P1.GetX());
					fbuf[13] = GLfloat(P1.GetY());
					fbuf[14] = GLfloat(P1.GetZ());

					fbuf[15] = GLfloat(P11.GetX());
					fbuf[16] = GLfloat(P11.GetY());
					fbuf[17] = GLfloat(P11.GetZ());

					P0 = P01;
					P1 = P11;
					fbuf += 18;
				}
			}
		}
	return Triangles;
}
int GQuadRenderRot::RenderAxialSection(void) const
{
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_POLYGON_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	GLfloat POffset = IsStockRender() ? 2.f : 1.f;
	const NColor &PColor = IsStockRender() ? AxialSectionColorStock : AxialSectionColorPart;
	glPolygonOffset(POffset, POffset);
	glColor3fv(PColor.array);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(1);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_NEVER, 0, 0);
	glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
	GenAxialQuads();
	glStencilFunc(GL_NOTEQUAL, 0, 1); 
	glStencilOp(GL_KEEP, GL_ZERO, GL_ZERO);
	glEnable(GL_DEPTH_TEST);
	GenAxialQuads();
	glPopAttrib();
	glPolygonOffset(1., 1.);
	return 0;
}

int GQuadRenderRot::GenAxialQuads(void) const
{
	glBegin(GL_QUADS);
	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			for(int i = 0; i < int(pC[0]); ++i)
			{
				BPoint P0(pC[LINE_SIZE * i + 1 + 2], pC[LINE_SIZE * i + 2 + 2], 0.,1.);
				BPoint P1(pC[LINE_SIZE * i + 1 + 2 + 5], pC[LINE_SIZE * i + 2 + 2 + 5], 0.,1.);
				glNormal3d(0., 0., (P0 - P1).GetY() > 0. ? 1. : -1.);
				glVertex2dv(P1.GetArray());
				glVertex2dv(P0.GetArray());
				glVertex2d(0., P0.GetY());
				glVertex2d(0., P1.GetY());
			}
		}
	glEnd();
	return 0;
}


void GQuadRenderRot::SetVisParam(double iStartAngle , double iAngle , double iAngleStep)
{
	StartAngle = iStartAngle;
	Angle = iAngle;
	AngleStepsNum = min( int(Angle / iAngleStep), 2);
	AngleStep = AngleStepsNum > 0 ? Angle / GQuadRenderRot::AngleStepsNum : 1;
}

void GQuadRenderRot::SetVisParam(double iStartAngle , double iAngle , int iAngleStepsNum)
{
	StartAngle = iStartAngle;
	Angle = iAngle;
	AngleStepsNum = iAngleStepsNum;
	AngleStep = AngleStepsNum > 0 ? Angle / AngleStepsNum : 1;
}

void GQuadRenderRot::GetVisParam(double &oStartAngle , double &oAngle, double &oAngleStep, int &oAngleStepsNum)
{
	oStartAngle = StartAngle;
	oAngle = Angle;
	oAngleStepsNum = AngleStepsNum;
	oAngleStep = AngleStep;
}

void GQuadRenderRot::SetFull()
{
	++StoreCounter;
	if(StoreCounter > 1)// Already stored
		return;
	// Store current params
	StartAngleB = StartAngle;
	AngleB = Angle;
	AngleStepsNumB = AngleStepsNum;
	AngleStepB = AngleStep;
	// Set full
	StartAngle = 0.;
	Angle = 360.;
	AngleStepsNum = int(Angle / AngleStep);
}
void GQuadRenderRot::ResetStored()
{
	--StoreCounter;
	if(StoreCounter > 0)// Already stored
		return;
	StartAngle = StartAngleB;
	Angle = AngleB;
	AngleStepsNum = AngleStepsNumB;
	AngleStep = AngleStepB;
}

void GQuadRenderRot::RenderOrd(void)
{
	if(AngleStepsNum <= 0)
		return;
	BMatr RotM;
	double AStep = Angle / AngleStepsNum;
	RotM.RotY(BPoint(0.,0.,0.,1.), AStep);

	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	int DispList = glGenLists(1);
	glNewList(DispList, GL_COMPILE);
	glBegin(GL_QUADS);
	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			for(int i = 0; i < int(pC[0]); ++i)
			{
				BPoint P0(pC[LINE_SIZE * i + 1 + 2], pC[LINE_SIZE * i + 2 + 2], 0.,1.);
				BPoint P1(pC[LINE_SIZE * i + 1 + 2 + 5], pC[LINE_SIZE * i + 2 + 2 + 5], 0.,1.);
				BPoint N0(-pC[LINE_SIZE * i + 1], -pC[LINE_SIZE * i + 2], 0.,0.);
				BPoint N1(-pC[LINE_SIZE * i + 1 + 5], -pC[LINE_SIZE * i + 2 + 5], 0.,0.);
				PackedColor c0; c0.fval = pC[LINE_SIZE * i + 1 + 4];
				PackedColor c1; c1.fval = pC[LINE_SIZE * i + 1 + 9];
				if (IsCollided())
				{
					const NColor& col = GetCollidedColor();
					c0.fr = col.r();
					c0.fg = col.g();
					c0.fb = col.b();
					c0.fa = col.a();
					c1 = c0;
				}
				glNormal3dv(N0.GetArray());
				glColor4ub(c0.fr, c0.fg, c0.fb, c0.fa);
				glVertex3dv(P0.GetArray());
				glNormal3dv(N1.GetArray());
				glColor4ub(c1.fr, c1.fg, c1.fb, c0.fa);
				glVertex3dv(P1.GetArray());
				P0 = P0 * RotM;
				P1 = P1 * RotM;
				N0 = N0 * RotM;
				N1 = N1 * RotM;
				glNormal3dv(N1.GetArray());
				glVertex3dv(P1.GetArray());
				glNormal3dv(N0.GetArray());
				glColor4ub(c0.fr, c0.fg, c0.fb, c0.fa);
				glVertex3dv(P0.GetArray());
			}
		}
	glEnd();
	glEndList();
	singleLock.Unlock();
	for(int ia = 0; ia < AngleStepsNum; ++ia)
	{
		glCallList(DispList);
		glRotated(AStep, 0., 1., 0.);
	}
	glDeleteLists(DispList,1);
}

const BPoint GQuadRenderRot::RayCasting(const BPoint & P, const BPoint & V, double /*MinW*/, BPoint &N) const
{
	// P and V are in stock CS i.e. take in consideration stock position but ignores MillTurn rotation and BodyRot
// Find the intersection point nearest to P in the V direction
	// N - external normal in P 
	double dv = sqrt(V.D2());
	if(dv > DMIN)
		dv = 1. / dv;
	else
		return 	BPoint(0., 0., 0., -1.);
	BPoint P0R(0., 0., 0., 1.), P1R(0., 0., 0., 1.);
	double MinT = 1.e12;
	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			for(int i = 0; i < int(pC[0]); ++i)
			{
				BPoint P0(pC[LINE_SIZE * i + 1 + 2], pC[LINE_SIZE * i + 2 + 2], 0.,1.);
				BPoint P1(pC[LINE_SIZE * i + 1 + 2 + 5], pC[LINE_SIZE * i + 2 + 2 + 5], 0.,1.);

				double X0, Y0, Z0, H0;
				P0.Get(X0, Z0, Y0, H0);
				double X1, Y1, Z1, H1;
				P1.Get(X1, Z1, Y1, H1);
				double dX = X1 - X0;
				double dZ = Z1 - Z0;
				double dZ2 = dZ * dZ;
				BPoint Vxy = V;
				Vxy.ProjXY();
				BPoint Pxy = P;
				Pxy.ProjXY();
				double F = X0 * Z1 + dX * P.GetZ() - X1 * Z0 ;
				double A = dZ2 * Vxy.D2() - dX * dX * V.GetZ() * V.GetZ();
				if(fabs(A) < MIND * MIND)
					continue;
				double Az = 0.5 / A;
				double B = 2. * ( dZ2 * (Vxy * Pxy) - F * dX * V.GetZ());
				double C = dZ2 * Pxy.D2() - F * F;
				double D = B * B - 4 * A * C;
				if(D < 0.)
					if(fabs(D) < 1.e-8 * (B * B + fabs(4 * A * C)))
						D = fabs(D);
					else
						continue;
				D = sqrt(D);
				double t1 = Az * (-B + D);
				double t2 = Az * (-B - D);
				t1 = min(t1, t2);
				BPoint R = P + V * t1;
				double q, q0, q1;
				if(fabs(dX) > fabs(dZ))
				{
					R.ProjXY();
					q = sqrt(R.D2());
					X0 = fabs(X0);
					X1 = fabs(X1);
					q0 = min(X0, X1);
					q1 = max(X0, X1);
				}
				else
				{
					q = R.GetZ();
					q0 = min(Z0, Z1);
					q1 = max(Z0, Z1);
				}
				if(q < q0 || q > q1)
					continue;
				if(t1 < MinT)
				{
					MinT = t1;
					P0R = P0;
					P1R = P1;
				}
			}
		}

	if(MinT >= 1.e6)
		return 	BPoint(0., 0., 0., -1.);
	BPoint Res = V * MinT + P;
	// Calculate normal. Hasn't been tested
	N = (P1R - P0R) * BMatr().RotY(0., 0., 0., 90.);
	double Dn = N.GetX() * N.GetX() + N.GetY() * N.GetY();
	BPoint Rv = Res;
	Rv.ProjXY();
	double Dt = Rv.D2();

	if(Dt > MIND * MIND)
	{
		double Alf = sqrt(Dn / Dt);
		double bz = N.GetZ();
		N.Set(Rv.GetX() * Alf, Rv.GetY() * Alf, bz, 0.);
	}
	if(V * N > 0.)
		N = N * (-1.);
	N = N * (1. / sqrt(N.D2()));
	return Res;

/*


	int SectorsNum = NSurfSettings::GetNumAproxLine(GetMaxX(), 360.);
//	SectorsNum = min(60, SectorsNum);
	BMatr RotM, Rot;
	Rot.RotX(0. ,0., 0., 90.);
	double AStep = 360. / SectorsNum;
	RotM.RotZ(BPoint(0.,0.,0.,1.), -AStep);

	double dv = sqrt(V.D2());
	if(dv > DMIN)
		dv = 1. / dv;
	else
		return 	BPoint(0., 0., 0., -1.);


	double MinT = 1.e12;
	for(int ix = 0; ix < ColsNum ; ++ix)
		for(int iy = 0; iy < RowsNum ; ++iy)
		{
			GLfloat *pC = (GLfloat *)Array[ix][iy];
			if(Empty(pC))
				continue;
			for(int i = 0; i < int(pC[0]); ++i)
			{
				BPoint P0(pC[LINE_SIZE * i + 1 + 2], pC[LINE_SIZE * i + 2 + 2], 0.,1.);
				BPoint P1(pC[LINE_SIZE * i + 1 + 2 + 5], pC[LINE_SIZE * i + 2 + 2 + 5], 0.,1.);
				BPoint n0(pC[LINE_SIZE * i + 1], pC[LINE_SIZE * i + 2], 0.,0.);
				P0 = P0 * Rot;
				P1 = P1 * Rot;
				n0 = n0 * Rot;
				BPoint n = (P0 - P1) % (P0 * RotM - P0) + (P0 - P1) % (P1 * RotM - P1);
				for(int k = 0; k < SectorsNum; ++k)
				{
					double z = n * V;
					if(fabs(z) > DMIN)
					{
						z = 1. / z;
					}
					else
						continue;
					BPoint P01 = P0 * RotM;
					BPoint P11 = P1 * RotM;


					BPoint p0 = P0 - P;
					double t = p0 * n * z;
					if(t < MinT)
					{
						BPoint IntP(V * t);
						BPoint p1 = P1 - P;
						BPoint p2 = P11 - P;
						BPoint p3 = P01 - P;
						if(!(((p1 - p0) % (p0 - IntP)) * n < 0 && IntP.LineD2(p1, p0) > MinW * MinW))
						{
							if(!(((p2 - p1) % (p1 - IntP)) * n < 0 && IntP.LineD2(p2, p1) > MinW * MinW))
							{
								if(!(((p0 - p2) % (p2 - IntP)) * n < 0 && IntP.LineD2(p0, p2) > MinW * MinW))
								{
									MinT = t;
									N = n;
								}
							}
						}
						if(!(((p2 - p0) % (p0 - IntP)) * n < 0 && IntP.LineD2(p2, p0) > MinW * MinW))
						{
							if(!(((p3 - p2) % (p2 - IntP)) * n < 0 && IntP.LineD2(p3, p2) > MinW * MinW))
							{
								if(!(((p0 - p3) % (p3 - IntP)) * n < 0 && IntP.LineD2(p0, p3) > MinW * MinW))
								{
									MinT = t;
									N = n;
								}
							}
						}
					}




					P0 = P01;
					P1 = P11;
					n = n * RotM;
				}
			}
		}
	if(MinT >= 1.e6)
		return 	BPoint(0., 0., 0., -1.);

	if(V * N > 0.)
		N = N * (-1.);
	return V * MinT + P;
	*/
}

BBox GQuadRenderRot::GetGabar() const
{
	BBox RotGabar;
	RotGabar.Expand(-Gabar.Xmax, -Gabar.Xmax, Gabar.Ymin);
	RotGabar.Expand(Gabar.Xmax, Gabar.Xmax, Gabar.Ymax);
	return RotGabar;
}

UINT64 GQuadRenderRot::GetHASH(const GBodyRender* pEnd) const
{
	auto pEndRot = dynamic_cast<const GQuadRenderRot*>(pEnd);
	UINT64 HASH = 0;
	for (int ix = 0; ix < ColsNum; ++ix)
		for (int iy = 0; iy < RowsNum; ++iy)
		{
			float* pCEnd = (pEndRot == nullptr) ? (float*)QR_EMPTY : pEndRot->GetArray()[ix][iy];
			float* pC = NotDefined(pCEnd) ? GetArray()[ix][iy] : pCEnd;
			HASH ^= UINT64(pC);
		}

	return HASH;
}

GBodyRender* GQuadRenderRot::MakeCopy() const
{
	GQuadRenderRot* pRet = new GQuadRenderRot(*this);
	return pRet;
}

enum NCMApplicationType GQuadRenderRot::GetType() const 
{
	return AT_TURN;
}

void GQuadRenderRot::SetDefaults(NTiParams &Par)
{
	CString Key = "Defaults/Render/Edges";
	EdgesWidth = Par.GetDouble((Key + "@Thick").GetBuffer(), 2.);
}
