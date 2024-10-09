// MFace.cpp: implementation of the MFace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MFace.h"
#include "ConstDef.h"
#include "BMatr.h"
#include "math.h"
#include "Gl\GLu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "MRay.h"
#include "MFace.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MFace::MFace()
{
	Size = 0;
	MaxSize = INT_SIZE;
	m_indPoints = m_ordPoints;
	m_extraPoints = nullptr;
}

MFace::MFace(const BPoint& n, const BPoint& b)
{
	Size = 0;
	MaxSize = INT_SIZE;
	m_indPoints = m_ordPoints;
	m_extraPoints = nullptr;
	m_plane.Set(n, b);
}
MFace::MFace(int n, int b)
{
	Size = 0;
	MaxSize = INT_SIZE;
	m_indPoints = m_ordPoints;
	m_extraPoints = nullptr;
	m_plane = MPlane(n,b);
}

MFace::~MFace()
{
	delete[] m_extraPoints;
}

int MFace::GetCount()  const
{
	return Size;
}


const BPoint* MFace::GetPoint(int index) const
{
#ifdef _DEBUG
	if (index <0 || index >= Size)
		return NULL;
#endif
	return MFOREST.GetPoint(m_indPoints[index]);
}

int MFace::GetPointN(int index) const
{
	return m_indPoints[index];
}

void MFace::RemoveAll()
{
	Size = 0;
}


M_POSITION MFace::DivideByPlane(const MPlane &V, MFace &P1, MFace &P2) const
// Divide this area by the "V" plane
// P1 lies in positive subspace; 
// P2 lies in negative subspace; 
{
	int TPk[2];   // Intersection points indexes
	MSLine PlInt;
	M_POSITION res = DetPosition( V.GetPoint(),V.GetNormal(), TPk);
	
	if(res != M_INT)
		return res;
	BPoint P(V.GetPoint());
	BPoint N(V.GetNormal());
	if( CrParts(P, N, TPk, P1, P2) < 0 )
	{
		//AfxMessageBox(IDS_DIVEDEBYPLANE_ERR);
		//return M_ERR;
	}
	//DEBUG
	//if (P1.IsSmall() || P2.IsSmall())
	//{
	//	Beep(1000, 1000);
	//}
	//END:DEBUG
	return res;
}

bool MFace::IsEmpty()  const
{
	return Size < 1;
}

const MPlane & MFace::GetPlane() const
{
	return m_plane;
}

MPlane & MFace::GetPlane()
{
	return m_plane;
}

void MFace::SetPlane(const MPlane &plane)
{
	m_plane = plane;
}

MFace::MFace(const MFace &F)
{
	Size = 0;
	MaxSize = INT_SIZE;
	m_indPoints = m_ordPoints;
	m_extraPoints = nullptr;
	SetArraySize(F.Size);
	m_plane = F.m_plane;
	memcpy_s(m_indPoints, Size * sizeof(m_indPoints[0]), F.m_indPoints, Size * sizeof(m_indPoints[0]));
}

void MFace::CrPlane(void)
{
	SetArraySize(4);
	BMatr M,T;
	M.e0cong(BPoint(0.,0.,1.,0),GetPlane().GetNormal());
	M = M * T.Trans(BPoint(0.,0.,0.,1.),GetPlane().GetPoint());
	m_indPoints[0] = MFOREST.AddPoints(BPoint(-MAXC,-MAXC,0.,1.) * M);
	m_indPoints[1] = MFOREST.AddPoints(BPoint( MAXC,-MAXC,0.,1.) * M);
	m_indPoints[2] = MFOREST.AddPoints(BPoint( MAXC, MAXC,0.,1.) * M);
	m_indPoints[3] = MFOREST.AddPoints(BPoint(-MAXC, MAXC,0.,1.) * M);
}

void MFace::ReplacePoints(const MFace & InFace)
{
	SetArraySize(InFace.Size);
	memcpy(m_indPoints, InFace.m_indPoints, InFace.Size * sizeof(int));
}

bool MFace::Contains(const BPoint & P) const
{
	for(int i=0; i < GetCount()-1; ++i)
	{
		if(((P - *GetPoint(i))%(*GetPoint(i+1) - *GetPoint(i)))*GetPlane().GetNormal() > 0.)
			return false;
	}
	if(((P - *GetPoint(GetCount()))%(*GetPoint(0) - *GetPoint(GetCount())))*GetPlane().GetNormal() > 0.)
		return false;
	return true;
}

void MFace::Draw() const
{// Convex face only
	if (GetCount() < 3)
		return;
	
	if(GetCount() == 3)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_TRIANGLE_FAN);
	for (int i=0; i<GetCount(); i++)
	{
		const BPoint *p = GetPoint(i);
		glVertex3d(p->GetX(), p->GetY(), p->GetZ());
	}
	glEnd();
}

//void MFace::Draw(Text *glText, CString str, bool bDrawText)
//{
//	// Convex face only
//	BPoint n, p;
//	if (GetCount() < 3)
//	{
//		glColor3d(1.f, 0.2f, 0.5f);
//		n = m_plane.GetNormal();
//		p = m_plane.GetPoint();
//		char s[20];
//		sprintf(s, "%s", str);
//		glText->Draw3D(p.GetX()+n.GetX(), p.GetY()+n.GetY()+TextPOS, p.GetZ(), s);
//		glColor3d(1.f, 1.f, 0.f);
//		glBegin(GL_LINES);
//			// сама нормаль
//			glVertex3d(p.GetX(), p.GetY(), p.GetZ());
//			n = p+n*LengthNORM;
//			glVertex3d(n.GetX(), n.GetY(), n.GetZ());
//		glEnd();
//		glBegin(GL_POINTS);
//		glVertex3d(p.GetX(), p.GetY(), p.GetZ());
//		glEnd();		
//		return;
//	}
//	
//	if(GetCount() == 3)
//		glBegin(GL_TRIANGLES);
//	else
//		glBegin(GL_POLYGON/*GL_TRIANGLE_FAN*/);
//
//	n = m_plane.GetNormal();
//	glNormal3d(n.GetX(), n.GetY(), n.GetZ());
//	for (int i=0; i<GetCount(); i++)
//	{
//		glVertex3d(GetPoint(i)->GetX(), GetPoint(i)->GetY(), GetPoint(i)->GetZ());
//	}
//	glEnd();
//
//	// временно
//	if (!bDrawText)
//		return;
//
////	glDisable(GL_LIGHTING);
//	glColor3d(1.f, 0.2f, 0.5f); 
//	n = (*GetPoint(0) + *GetPoint(2))*0.5;
//	char s[20];
//	sprintf(s, "%s", str);
//	if (str.Find("B") != -1)
//		glText->Draw3D(n.GetX()+TextPOS, n.GetY()-TextPOS, n.GetZ(), s);
//	else
//		glText->Draw3D(n.GetX()+TextPOS, n.GetY()+TextPOS, n.GetZ(), s);
//
//	p = m_plane.GetPoint();
//	glColor3d(1.f, 1.f, 0.f); 
//	glBegin(GL_LINES);
//		n = (*GetPoint(0) + *GetPoint(2))*0.5;
//		// указатель на базовую точку нормали
//	//	glVertex3d(n.GetX(), n.GetY(), n.GetZ());
//	//	glVertex3d(p.GetX(), p.GetY(), p.GetZ());
//		p = n;
//		// сама нормаль
//		glVertex3d(n.GetX(), n.GetY(), n.GetZ());
//		n = n+m_plane.GetNormal()*LengthNORM;
//		glVertex3d(n.GetX(), n.GetY(), n.GetZ());
//	glEnd();
//
//	glBegin(GL_POINTS);
//		glVertex3d(p.GetX(), p.GetY(), p.GetZ());
//	glEnd();
////	glEnable(GL_LIGHTING);
//}

void MFace::SetPointAt(int i, int v)
{
	m_indPoints[i] = v;
}
void MFace::SetPointAt(int i, const BPoint &P)
{
	m_indPoints[i] = MFOREST.AddPoints(P);
}

void MFace::SetArraySize(int iSize)
{
	if (MaxSize < iSize)
	{
		if (iSize > INT_SIZE)
		{
			delete[] m_extraPoints;
			m_extraPoints = new int[iSize];
			m_indPoints = m_extraPoints;
		}
		MaxSize = iSize;
	}
	Size = iSize;
}

M_POSITION MFace::DetPosition(const BPoint &P, const BPoint &N, int  TPk[2]) const
{// Determine this face position according to P,N plane

	TPk[0] = TPk[1] = -1;
	if(IsEmpty())
		return M_EMP;
	if( (N % m_plane.GetNormal()).D2() <= MINAR*MINAR)
	{// Planes are parallel
		double s = (m_plane.GetPoint() - P) * N;
		if(fabs(s) <= MIND)// Planes are identical
			return M_IDN;
		else
			return s > 0 ? M_POS : M_NEG;
	}
	//Find sign change points
	int Npts = GetCount();
	double P_N = P * N;
	int FirstSign = 0;
	int kp1 = 0;
	for(; kp1 < Npts && FirstSign == 0; ++kp1)
	{
		double buf = (*GetPoint(kp1)) * N - P_N;
		FirstSign = (buf > EPS_DP) ? 1 : (buf < -EPS_DP) ? -1 : 0;
	}
	int sgn = 0;
	int PrevSign = FirstSign;
	--kp1;
	for(int kp = Npts - 1; kp >= kp1; --kp) 
	{
		double buf = (*GetPoint(kp)) * N - P_N;
		int CurSign = (buf > EPS_DP) ? 1 : (buf < -EPS_DP) ? -1 : 0;
		if( PrevSign * CurSign < 0.)
		{
			TPk[sgn % 2] = kp;
			++sgn; // Signum change found
			PrevSign = CurSign;
		}
	}
	if( sgn != 2) // The face don't intersect the plane or error
	{
		if( sgn != 0) // Error
		{
			//CString f;
			//f.Format("NptsF:%d,%e,%e,%e,%e,%e,%e,%e",Npts,Signes[0],Signes[1],Signes[2],Signes[3],Signes[4],Signes[5],Signes[6]);
			//AfxMessageBox(f);
			return M_ERR;
		}
		return FirstSign > 0 ? M_POS : M_NEG;
	}
	return M_INT;
}
int MFace::CrParts(const BPoint &P, const BPoint &N,
						  int  TPk[2],
						  MFace & P1, MFace & P2) const
{
	if( TPk[0] < 0 || TPk[1] < 0)
		return -1;
	// Calculate intersection points
	int k0, k1;
	double s0, s1, z;
	double P_N = P * N;
	k0 = TPk[0];
	k1 = (k0 + 1) % GetCount();
	const BPoint &Q0 = *GetPoint(k0); 
	s0 = fabs(Q0 * N - P_N);
	const BPoint &Q1 = *GetPoint(k1); 
	s1 = fabs(Q1 * N - P_N);
	z = 1. / (s0 + s1);
	BPoint TP0(Q0, s1 * z, Q1, s0 * z);

	k0 = TPk[1];
	k1 = (k0 + 1) % GetCount();
	const BPoint &Q2 = *GetPoint(k0); 
	s0 = fabs(Q2 * N - P_N);
	const BPoint &Q3 = *GetPoint(k1); 
	s1 = fabs(Q3 * N - P_N);
	z = 1. / (s0 + s1);
	BPoint TP1(Q2, s1 * z, Q3, s0 * z);

	// All intersection points found (in TP)
	// Two intersection points
	int temp = MFOREST.AddPoints(TP0);
	int temp1 = MFOREST.AddPoints(TP1);
	if(TPk[1]<TPk[0])
	{// TPk[0] <-> TPk[0]
		int b=TPk[0];TPk[0]=TPk[1];TPk[1]=b;
	 // temp <-> temp1
		b=temp;temp=temp1;temp1=b;
	}
	MFace *pPpos = &P1, *pPneg = &P2;
	double FirstSign = 0;
	for(int j=TPk[0]+1; fabs(FirstSign) < EPS_DP && j <= TPk[1]; ++j)
		FirstSign = (*GetPoint(j) - P) * N;

	if(FirstSign > 0.)
	{
		pPpos = &P2;
		pPneg = &P1;
	}
	pPpos->SetArraySize(TPk[0]+1+2+Size-TPk[1]-1);
	int kp=0;
	int i;
	for(i=0; i <= TPk[0]; ++i)
		pPpos->SetPointAt(kp++,GetIndPoint(i));
	pPpos->SetPointAt(kp++,temp);
	pPpos->SetPointAt(kp++,temp1);
	for(i=TPk[1]+1; i < Size; ++i)
		pPpos->SetPointAt(kp++,GetIndPoint(i));
	kp=0;
	pPneg->SetArraySize(TPk[1]-TPk[0]+2);
	pPneg->SetPointAt(kp++,temp);
	for(i=TPk[0]+1; i <= TPk[1]; ++i)
		pPneg->SetPointAt(kp++,GetIndPoint(i));
	pPneg->SetPointAt(kp++,temp1);
	P1.SetPlane(m_plane);
	P2.SetPlane(m_plane);
	return temp;
}

double MFace::Area2(void) const
{
// Calculate a square of the polygon area 
	if( GetCount() < 3)
		return 0.;
	BPoint b(*GetPoint(0));
	BPoint cr0(*GetPoint(1));
	cr0 -= b;
	BPoint v(0.,0.,0.,0.);
	for(int i = 2; i < GetCount() ; ++i)
	{
		BPoint cr1(*GetPoint(i));
		cr1 -= b;
		v += cr0 % cr1;
		cr0 = cr1;
	}
	return v.D2()*0.25;
}

bool MFace::IsSmall() const
{
	if ((*GetPoint(0) - *GetPoint(1)).D2() < MIND * MIND)
		return true;
	if ((*GetPoint(0) - *GetPoint(2)).D2() < MIND * MIND)
		return true;
	if ((*GetPoint(2) - *GetPoint(1)).D2() < MIND * MIND)
		return true;
	return false;
}

// уточнить грань
//void MFace::RefinedFace(MBSPTree *pTree, MBSPNode *pNode)
//{
//	return;
//}
//
//void MFace::RefinedFace1(MBSPTree *pTree, MBSPNode *pNode, bool bNeedTest, int &count)
//{
//	return;
//}
//
//MBSPNode * MFace::RefinedFace2(MBSPTree *pTree, MBSPNode *pRoot, MBSPNode *pNode, bool bLeft)
//{
//	return pNode;
//}
//
//bool MFace::LocRefinedFace(MBSPTree *pTree, MBSPNode *pNode)
//{
//	return true;
//}
//
// Определить состояние
// уточнена ли грань
bool MFace::GetState(void)
{
	return false;
}
void MFace::GetGabar(BPoint & Min, BPoint & Max) const
{
	int n = GetCount();
	if(n <= 0)
	{
		Min.Set(1.e6,1.e6,1.e6,1.);
		Max.Set(-1.e6,-1.e6,-1.e6,1.);
		return;
	}
	const BPoint *pP = GetPoint(0);
	double x0 = pP->GetX();
	double y0 = pP->GetY();
	double z0 = pP->GetZ();
	double x1 = x0;
	double y1 = y0;
	double z1 = z0;
	for(int i=1; i < GetCount(); ++i)
	{
		pP = GetPoint(i);
		double x = pP->GetX();
		if(x > x1) x1 = x;
		if(x < x0) x0 = x;
		double y = pP->GetY();
		if(y > y1) y1 = y;
		if(y < y0) y0 = y;
		double z = pP->GetZ();
		if(z > z1) z1 = z;
		if(z < z0) z0 = z;
	}
	Min.Set(x0,y0,z0,1.);
	Max.Set(x1,y1,z1,1.);
}

//void MFace::Intersect(const MFace & InFace)
//{
//// Calculate intersection between this face and InFace
//// Store result in this face
//	if(IsEmpty())
//		return;
//	if(InFace.IsEmpty())
//	{
//		SetArraySize(0);
//		return;
//	}
////	Orient();
//	MFace *pFace = this;
//	MFace P1,P2;
//	MFace *pP1 = &P1;
//	MPlane CurPlane;
//	CurPlane.SetNormal(BPoint(1,0,0,0));// To reserve the place for the normal
//	CurPlane.SetPointN(InFace.GetIndPoint(InFace.GetCount()-1));
//	BPoint n(InFace.GetPlane().GetNormal() % (*InFace.GetPoint(0) - *InFace.GetPoint(InFace.GetCount()-1)));
//	for(int i=0 ; ; ++i)
//	{
//		n = n * (1./sqrt(n.D2()));
//		MFOREST.SetPoint(CurPlane.GetNormalN(),n);
//		M_POSITION res = pFace->DivideByPlane(CurPlane,*pP1,P2);
//		switch(res)
//		{
//		case M_INT:
//		{
//			MFace* b = pFace;
//			pFace = pP1;
//			pP1 = b;
//		}
//			break;
//		case M_NEG:
//			SetArraySize(0);
//			return;
//			break;
//		}
//		CurPlane.SetPointN(InFace.GetIndPoint(i));
//		if(i >= InFace.GetCount()-1)
//			break;
//		n = InFace.GetPlane().GetNormal() % (*InFace.GetPoint(i+1) - *InFace.GetPoint(i));
//	}
//	ReplacePoints(*pFace);
//}


void MFace::Orient(void)
{
// Establish right orientation for the face
// For the convex face only
	int n;
	if(2 > (n = GetCount()))
		return;
	double v = GetPlane().GetNormal() * 
		((*GetPoint(1) - *GetPoint(0)) % (*GetPoint(2) - *GetPoint(1)));
	if(v > 0)
	{
		for(int i = 0; i < n/2; ++i)
		{
			int b = m_indPoints[i];
			m_indPoints[i] = m_indPoints[n-1-i];
			m_indPoints[n-1-i] = b;
		}
	}

}
bool MFace::IsConvex(void) const
{
	int n;
	if(2 > (n = GetCount()))
		return false;
	double v = GetPlane().GetNormal() * 
		((*GetPoint(0) - *GetPoint(n-1)) % (*GetPoint(1) - *GetPoint(0)));
	for(int i = 1; i < n; ++i)
	{
		if( v * (GetPlane().GetNormal() * 
			((*GetPoint(i) - *GetPoint(i-1)) % (*GetPoint((i+1)%n) - *GetPoint(i)))) < 0.)
			return false;
	}
	return true;
}
bool MFace::IsRightOriented(void) const
{
	int n;
	if(2 > (n = GetCount()))
		return false;
	double v = GetPlane().GetNormal() * 
		((*GetPoint(0) - *GetPoint(n-1)) % (*GetPoint(1) - *GetPoint(0)));
	for(int i = 0; i < n; ++i)
	{
		double vv = GetPlane().GetNormal() * 
			((*GetPoint((i+1)%n) - *GetPoint(i)) % (*GetPoint((i+2)%n) - *GetPoint((i+1)%n)));
		if(fabs(vv) > DMIN)
			return vv > 0.;
	}
	return v > 0.;
}

bool MFace::IsCCW(void) const
{// It is assumed that contour have more than 2 vertexes and is convex
	int n;
	if(3 > (n = GetCount()))
		return false;
	const BPoint *pP0 = GetPoint(0);
	const BPoint *pP1 = GetPoint(1);
	const BPoint *pP2 = GetPoint(2);
	for(int i = 0; i < n; ++i)
	{
		double v = GetPlane().GetNormal() * ((*pP1 - *pP0) % (*pP2 - *pP1));
		if(fabs(v) > DMIN)
			return v > 0.;
		pP0 = pP1;
		pP1 = pP2;
		pP2 = GetPoint((i+2)%n);
	}
	return false;
}

bool MFace::SetOrder(void)
{
// Специальная перестановка вершин для построения деревьев в секторах
// This face must have 3 or 4 vertexes. This vertexes must have Y = 0
// (bottom vertexes) or Y > 0 (top vertexes)
// After rearrangement vertexes ## 0 and 1 are bottom vertexes and others is top vertexes
// If GetCount() == 4 then vertex #0 corresponds to vertex #3 and #1 to #2
// If GetCount() == 3 then one of the #0 and #1 vertexes has X = 0 and it hasn't corresponding vertex
// Another bottom vertex corresponds to vertex #2.

	int N = GetCount();
	if((N != 4) && (N != 3))
		return false;
	// Find ANY top vertex
	int it = 0;
	for(;it < N; ++it)
		if(GetPoint(it)->GetZ() > DMIN)
			break;
	// Find FIRST bottom vertex
	int ib = (it + 1) % N;
	for(int i = 0; i < N; ++i)
	{
		if(GetPoint(ib)->GetZ() < DMIN)
			break;
		ib = (ib + 1) % N;
	}
	int ib1 = (ib + 1) % N;
	it = (ib1 + 1) % N;
	int np0 = GetIndPoint(ib);
	int np1 = GetIndPoint(ib1);
	int np2 = GetIndPoint(it);
	int np3 = N > 3 ? GetIndPoint((it + 1) % N) : 0;
	SetPointAt(0, np0);
	SetPointAt(1, np1);
	SetPointAt(2, np2);
	if(N > 3)
		SetPointAt(3, np3);
	return true;
}

void MFace::SpecRotCopy(const MFace & /*OrigFace*/, const BMatr & M)
{
// See comments in SetOrder
// OrigFace may be this face

	SetPlane(MPlane(MFOREST.AddPoints(GetPlane().GetNormal() * M),
					MFOREST.AddPoints(GetPlane().GetPoint() * M)));
	if(GetCount() == 4)
	{
		SetPointAt(0, GetIndPoint(3));
		SetPointAt(1, GetIndPoint(2));
		SetPointAt(2, (*GetPoint(2)) * M);
		SetPointAt(3, (*GetPoint(3)) * M);
	}
	if(GetCount() == 3)
	{
		// Find bottom vertex with X = 0
		int i0 = 0;
		for(; i0 < 2; ++i0)
			if(fabs(GetPoint(i0)->GetX()) < DMIN)
				if(fabs(GetPoint(i0)->GetZ()) < DMIN)
					break;
		switch(i0)
		{
		case 0:
			SetPointAt(1, GetIndPoint(2));
			SetPointAt(2, (*GetPoint(2)) * M);
			break;
		case 1:
			SetPointAt(0, GetIndPoint(2));
			SetPointAt(2, (*GetPoint(2)) * M);
			break;
		default:
			//ERROR
			break;
		}
	}
}

void MFace::MoveFrom(MFace& In)
{
	if (this == &In)
		return;
	delete[] m_extraPoints;
	memcpy_s(this, sizeof(*this), &In, sizeof(In));
	if (MaxSize <= INT_SIZE)
		m_indPoints = m_ordPoints;
	else
	{
		In.m_extraPoints = nullptr;
		In.m_indPoints = In.m_ordPoints;
		In.MaxSize = INT_SIZE;
		In.Size = 0;
	}
}

void MFace::SetNewIndexes(int* newind)
{
	auto n = GetPlane().GetNormalN();
	if (n >= 0)
		n = newind[n];
	auto b = GetPlane().GetPointN();
	if (b >= 0)
		b = newind[b];
	SetPlane(MPlane(n, b));
	for (size_type i = 0; i < GetCount(); ++i)
	{
		auto Ind = GetIndPoint(i);
		if(Ind >= 0)
			SetPointAt(i, newind[Ind]);
	}
}
