#include "stdafx.h"
#include "ConstDef.h"
#include "MBSPNode.h"
#include "math.h"
#include "NDSizedAr.h"
#include "MLocStorage.h"
#include "mdividetree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MDivideTree::~MDivideTree(void)
{
}
MDivideTree::Start::Start()
{
}
MDivideTree::Start::~Start()
{
}

double MDivideTree::CalcSign(BPoint & Pi)
{
	const double *Mi = Pi.GetArray();
	const double *Mp = P.GetArray();
	const double *Mn = N.GetArray();
	return (Mi[0]-Mp[0])*Mn[0] + (Mi[1]-Mp[1])*Mn[1] + (Mi[2]-Mp[2])*Mn[2];
}

MDivideTree::MDivideTree(const MFace * pF, MBSPNode * pR, MBSPNode *&p1, MBSPNode *&p2) : DFace(*pF)
{
	pRoot = pR;
	pV1 = &p1;
	pV2 = &p2;

	BUF = -1;
}

void MDivideTree::Init(const BPoint & Pi, const BPoint & Ni)
{
	P = Pi;
	N = Ni;
}
void MDivideTree::Init(const MPlane & Plane)
{
	Init(Plane.GetPoint(), Plane.GetNormal());
}

M_POSITION MDivideTree::DoDivide(void)
{
	Start1.Init(pV1); 
	Start2.Init(pV2); 
	for(int i=0 ;; ++i)
	{
		if(MBSPNode::IsTerminal(pRoot))
		{
			*pV1 = pRoot;
			Start1.CheckEnd(pRoot);
			*pV2 = pRoot;
			Start2.CheckEnd(pRoot);
			return M_INT;
		}
		M_POSITION pos21,pos12;
		int temp, temp1;
		if(!DetPosition(*pRoot->GetFace(),pos12,pos21, temp1, temp))
		{
			if(DEBUGMODE)
			{
				CString str;
				str.Format("Îøèáêà: DoDivide:DetPosition. pos12=%d, pos21=%d, i=%d.\n",pos12,pos21,i);
				LOGFILE.WriteString(str);
			}
			pos12 = M_IDN;
		}
		// If DFace and pRoot face does not intersect each other
		if ( pos12 == M_POS )
		{
			if(pos21 == M_POS)
			{
				*pV1 = pRoot;
				pV1 = pRoot->GetLeftPtr();
				Start1.Check(pV1, pRoot->GetRight());
			}
			else
			{
				*pV2 = pRoot;
				pV2 = pRoot->GetLeftPtr();
				Start2.Check(pV2, pRoot->GetRight());
			}
			pRoot = pRoot->GetLeft();
			continue;
		}
		if ( pos12 == M_NEG )
		{
			if(pos21 == M_POS)
			{
				*pV1 = pRoot;
				pV1 = pRoot->GetRightPtr();
				Start1.Check(pV1, pRoot->GetLeft());
			}
			else
			{
				*pV2 = pRoot;
				pV2 = pRoot->GetRightPtr();
				Start2.Check(pV2, pRoot->GetLeft());
			}
			pRoot = pRoot->GetRight();
			continue;
		}
		if ( pos12 == M_IDN )
		{// Planes are identical
			if(N * pRoot->GetFace()->GetPlane().GetNormal() > 0.)
			{
				*pV1 = pRoot->GetLeft();
				*pV2 = pRoot->GetRight();
			}
			else
			{
				*pV2 = pRoot->GetLeft();
				*pV1 = pRoot->GetRight();
			}
			BUF = pRoot->GetBFaceN();// Surf priority
			delete pRoot;// Added 21.07.05
			Start1.CheckEnd(*pV1);
			Start2.CheckEnd(*pV2);


			return M_IDN;
		}
		if( pos12 == M_INT )
		{
			int Var = (MBSPNode::IsTerminal(pRoot->GetLeft()) ? 1 : 0)
				+ (MBSPNode::IsTerminal(pRoot->GetRight()) ? 2 : 0);
			// Var 3 - both terminal, 2 | 1 - one terminal, 0 - both not terminal
			switch( Var )
			{
			case 3: // Both pRoot childs are terminal
				*pV2 = pRoot;
				(*pV2)->SetFace(F2);
				*pV1 = new MBSPNode;
				**pV1 = *pRoot;
				(*pV1)->SetFace(F1);
				return M_INT;
			case 0: // Both pRoot childs are not terminal
				{
					MFace BF1, BF2;
					MFace *pF1 = &BF1, *pF2 = &BF2;
					if(!CrParts(pRoot->GetFace()->GetPlane(), pF1, pF2, temp1, temp))
					{
						if(DEBUGMODE)
						{
							LOGFILE.WriteString("Îøèáêà: DoDivide:CrParts\n");
						}
						return M_ERR;
					}
					MBSPNode *bV1 = (MBSPNode *)M_NDEF_PTR,*bV2 = (MBSPNode *)M_NDEF_PTR;
					pF1->SetPlane(DFace.GetPlane());// pF1->Plane is empty after CrParts execution
					MDivideTree Div(pF1,pRoot->GetLeft(), bV1, bV2);
					Div.Init(P, N);
					M_POSITION res1 = Div.DoDivide();
					MBSPNode *cV1 = (MBSPNode *)M_NDEF_PTR,*cV2 = (MBSPNode *)M_NDEF_PTR;
					Div.Reset(pF2,pRoot->GetRight(), cV1, cV2);
					M_POSITION res2 = Div.DoDivide();
					if(res1 == M_ERR || res2 == M_ERR)
					{
						if(DEBUGMODE)
						{
							LOGFILE.WriteString("Îøèáêà: DoDivide:DoDivide\n");
						}
						return M_ERR;
					}
					*pV2 = pRoot;
					(*pV2)->SetLeft(bV2);
					(*pV2)->SetRight(cV2);
					MBSPNode::CheckNode(*pV2);
					if(*pV2 == pRoot) // May be changed by CheckNode
						*pV1 = new MBSPNode;//memory leaks (see CheckNode)
					else
						*pV1 = pRoot;
					(*pV1)->SetLeft(bV1);
					(*pV1)->SetRight(cV1);
					MBSPNode::CheckNode1(*pV1);// Method CheckNode1 have been added 21.12.07 instead of CheckNode to fix memory leaks mentioned above
					// This way may be incorrrect
					if(MBSPNode::IsTerminal(*pV1))
					{
						Start1.CheckEnd(*pV1);
					}
					else
					{
						(*pV1)->SetFace(F1);
						(*pV1)->SetBFace(pRoot->GetBFaceN());
						(*pV1)->SetCadrID(pRoot->GetCadrID());
					}
					if(MBSPNode::IsTerminal(*pV2))
					{
						Start2.CheckEnd(*pV2);
					}
					else
					{
						(*pV2)->SetFace(F2);
						(*pV2)->SetBFace(pRoot->GetBFaceN());
						(*pV2)->SetCadrID(pRoot->GetCadrID());
					}
					return M_INT;
				}
			case 2: // Right son is terminal and left son is not
				{
					*pV1 = new MBSPNode;
					(*pV1)->SetRight(pRoot->GetRight());
					(*pV1)->SetFace(F1);
					(*pV1)->SetBFace(pRoot->GetBFaceN());
					(*pV1)->SetCadrID(pRoot->GetCadrID());
					pV1 = (*pV1)->GetLeftPtr();
					Start1.Check(pV1, pRoot->GetRight());
					*pV2 = pRoot;
					(*pV2)->SetFace(F2);
					pV2 = (*pV2)->GetLeftPtr();
					Start2.Check(pV2, pRoot->GetRight());
					MFace BF1, BF2;
					MFace* pF = &BF1, * pFr = &BF2;
					if(!CrParts(pRoot->GetFace()->GetPlane(), pFr, pF, temp1, temp))
					{
						if(DEBUGMODE)
						{
							LOGFILE.WriteString("Îøèáêà: DoDivide:CrParts2\n");
						}
						return M_ERR;
					}
					DFace.ReplacePoints(*pFr);
					pRoot = pRoot->GetLeft();
					break;
				}
			case 1: // Left son is terminal and right son is not
				{
					*pV1 = new MBSPNode;
					(*pV1)->SetLeft(pRoot->GetLeft());
					(*pV1)->SetFace(F1);
					(*pV1)->SetBFace(pRoot->GetBFaceN());
					(*pV1)->SetCadrID(pRoot->GetCadrID());
					pV1 = (*pV1)->GetRightPtr();
					Start1.Check(pV1, pRoot->GetLeft());
					*pV2 = pRoot;
					(*pV2)->SetFace(F2);
					pV2 = (*pV2)->GetRightPtr();
					Start2.Check(pV2, pRoot->GetLeft());
					MFace F, Fr;
					MFace* pF = &F;
					MFace* pFr = &Fr;
					if(!CrParts(pRoot->GetFace()->GetPlane(), pF, pFr, temp1, temp))
					{
						if(DEBUGMODE)
						{
							LOGFILE.WriteString("Îøèáêà: DoDivide:CrParts3\n");
						}
						return M_ERR;
					}
					DFace.ReplacePoints(*pFr);
					//delete pFr;
					//delete pF;
					pRoot = pRoot->GetRight();
					break;
				}
			}
			continue;
		}
	}
//	return M_INT;
}
void MDivideTree::Start::Check(MBSPNode ** GrPoint, MBSPNode * pIn)
{
	if ( !MBSPNode::IsTerminal(pIn))
	{// Set new start
		ppS = GrPoint;
		Type = (MBSPNode *)M_NDEF_PTR;
		return;
	}
//	pIn is terminal
	if( Type == (MBSPNode *)M_NDEF_PTR )// Type is undefined yet
	{// Define type
		Type = pIn;
		return;
	}
	if( Type != pIn)
	{// Set new start
		ppS = GrPoint;
		Type = pIn;
	}
}

void MDivideTree::Start::CheckEnd(MBSPNode * pIn)
{
	if( pIn == Type)
	{// Chain to be deleted found
		MBSPNode *pCur = *ppS;
		*ppS = pIn;
		while(!MBSPNode::IsTerminal(pCur))
		{
			MBSPNode *pNext = MBSPNode::IsTerminal(pCur->GetLeft()) ?
				pCur->GetRight() : pCur->GetLeft();
			delete pCur;
			pCur = pNext;
		}
	}
	Type = (MBSPNode *)M_NDEF_PTR;
}

void MDivideTree::Start::Init(MBSPNode ** InitPtr)
{
	ppS = InitPtr;
	Type = (MBSPNode *)M_NDEF_PTR;
}

bool MDivideTree::DetPosition(const MFace & F, M_POSITION & Pos12, M_POSITION & Pos21, int& temp1, int& temp)
{
	if(DFace.IsEmpty() || F.IsEmpty())
	{
		if(DEBUGMODE)
		{
			CString str;
			str.Format("EmtyFace\n");
			LOGFILE.WriteString(str);
		}
		return false;
	}
	double Sin2 = (N % F.GetPlane().GetNormal()).D2();
	if( Sin2 <= MINAR*MINAR)
	{// Planes are parallel
		double s = (F.GetPlane().GetPoint() - P) * N;
		if(fabs(s) <= MIND)// Planes are identical
		{
			Pos12 = Pos21 = M_IDN;
			return true;
		}
		else
		{
			Pos21 = s > 0 ? M_POS : M_NEG;
			Pos12 = N * F.GetPlane().GetNormal() < 0 ? Pos21 : 
								Pos21 == M_POS ? M_NEG : M_POS;
			return true;
		}
	}
	//Find sign change points
	int Npts = F.GetCount();
	double P_N = P * N;
	int FirstSign = 0;
	int kp1 = 0;
	for(; kp1 < Npts && FirstSign == 0; ++kp1)
	{
		double buf = (*F.GetPoint(kp1)) * N - P_N;
		FirstSign = (buf > EQP) ? 1 : (buf < -EQP) ? -1 : 0;
	}
	int TPk[2];
	int sgn = 0;
	int PrevSign = FirstSign;
	--kp1;
	for(int kp = Npts - 1; kp >= kp1; --kp) 
	{
		double buf = (*F.GetPoint(kp)) * N - P_N;
		int CurSign = (buf > EQP) ? 1 : (buf < -EQP) ? -1 : 0;
		if( PrevSign * CurSign < 0.)
		{
			TPk[sgn % 2] = kp;
			++sgn; // Signum change found
			PrevSign = CurSign;
		}
	}
	if(FirstSign == 0)// All signums are zero
	{
		if(Sin2 < MINSR)//???????
		{
			Pos12 = Pos21 = M_IDN;
			return true;
		}
		if(DEBUGMODE)
		{
			CString str;
			str.Format("MDivideTree.DetPosition:Can't find non zero value\n");
			LOGFILE.WriteString(str);
		}
		return false;
	}
	// Find all signes for F face points
	//int Npts = F.GetCount();
	//NDSizedAr Signes(Npts);
	//double P_N = P * N;
	//int isb = -1;// First non zero value
	//for(int kp = Npts - 1; kp >= 0; --kp)// Backward order for the isb value founding
	//{
	//	double buf = (*F.GetPoint(kp)) * N - P_N;
	//	bool MakeZero = (fabs(buf) < EQP);
	//	Signes[kp] = MakeZero ? 0. : buf;
	//	if(!MakeZero)
	//		isb = kp;
	//}
	//// Find all signum changes points
	//////Find first not zero value
	////for(int isb = 0 ;  isb < Npts ; ++isb)
	////	if(Signes[isb] != 0.)
	////		break;
	//if(isb == -1)
	//{
	//	if(Sin2 < MINSR)//???????
	//	{
	//		Pos12 = Pos21 = M_IDN;
	//		return true;
	//	}
	//	if(DEBUGMODE)
	//	{
	//		CString str;
	//		str.Format("MDivideTree.DetPosition:Can't find non zero value\n");
	//		LOGFILE.WriteString(str);
	//	}
	//	return false;
	//}
	//int TPk[2];
	//int sgn = 0;
	//for( int ks = isb; ks < Npts;)
	//{
	//	// Find next not zero value
	//	int is = 1, bs = (ks + 1) % Npts;
	//	for(; Signes[bs] == 0. && bs != isb ; ++is, bs = (bs + 1) % Npts);
	//	if( Signes[bs] * Signes[ks] < 0.)
	//	{
	//		TPk[sgn % 2] = ks;
	//		++sgn; // Signum change found
	//	}
	//	ks += is;
	//}
	//double FirstSign = Signes[isb];
	if( sgn != 2) // The face don't intersect the plane or error
	{
		if( sgn != 0) // Error
		{
			if(DEBUGMODE)
			{
				//CString str;
				//str.Format("Npts:%d,%e,%e,%e,%e,%e,%e,%e\n",Npts,Signes[0],Signes[1],Signes[2],Signes[3],Signes[4],Signes[5],Signes[6]);
				//LOGFILE.WriteString(str);
			}
			return false;
		}
		Pos21 = FirstSign > 0 ? M_POS : M_NEG;
		//Find first not zero value in PDFace contour
		int NptsD = DFace.GetCount();
		double fs = 0.,fsb = 0.;
		int isb = 0;
		for(; isb < NptsD ; ++isb)
		{
			fs = (*DFace.GetPoint(isb) - F.GetPlane().GetPoint()) *
						F.GetPlane().GetNormal();
			if(fabs(fs) > DMIN)
				break;
			else
				if(fabs(fsb) < fabs(fs))
					fsb = fs;
		}
		if(isb == NptsD) 
			if(fsb != 0.)
				fs = fsb;
			else// Error
			{
				if(DEBUGMODE)
				{
					//CString str;
					//str.Format("NptsD:%d,%e,%e,%e,%e,%e,%e,%e\n",NptsD,Signes[0],Signes[1],Signes[2],Signes[3],Signes[4],Signes[5],Signes[6]);
					//LOGFILE.WriteString(str);
				}
				return false;
			}
		Pos12 = fs > 0. ? M_POS : M_NEG;
		return true;
	}
	// Faces intersects
	// Find and store intersection points
	int k0, k1;
	double s0, s1, z;
	k0 = TPk[0];
	k1 = (k0 + 1) % Npts;
	const BPoint &P0 = *F.GetPoint(k0); 
	s0 = fabs(P0 * N - P_N);
	const BPoint &P1 = *F.GetPoint(k1); 
	s1 = fabs(P1 * N - P_N);
	z = 1. / (s0 + s1);
	BPoint TP0(P0, s1 * z, P1, s0 * z);

	k0 = TPk[1];
	k1 = (k0 + 1) % Npts;
	const BPoint &P2 = *F.GetPoint(k0); 
	s0 = fabs(P2 * N - P_N);
	const BPoint &P3 = *F.GetPoint(k1); 
	s1 = fabs(P3 * N - P_N);
	z = 1. / (s0 + s1);
	BPoint TP1(P2, s1 * z, P3, s0 * z);

	// All intersection points found (in TP)
	temp = MFOREST.AddPoints(TP0);
	temp1 = MFOREST.AddPoints(TP1);
	if(TPk[1]<TPk[0])
	{// TPk[0] <-> TPk[0]
		int b=TPk[0];TPk[0]=TPk[1];TPk[1]=b;
	 // temp <-> temp1
		b=temp;temp=temp1;temp1=b;
	}
	MFace *pPpos = &F1, *pPneg = &F2;

	if(FirstSign < 0)
	{
		pPpos = &F2;
		pPneg = &F1;
	}
	pPpos->SetArraySize(TPk[0]+1+2+F.GetCount()-TPk[1]-1);
	int kr=0;
	int i;
	for(i=0; i <= TPk[0]; ++i)
		pPpos->SetPointAt(kr++,F.GetIndPoint(i));
	pPpos->SetPointAt(kr++,temp);
	pPpos->SetPointAt(kr++,temp1);
	for(i=TPk[1]+1; i < F.GetCount(); ++i)
		pPpos->SetPointAt(kr++,F.GetIndPoint(i));
	kr=0;
	pPneg->SetArraySize(TPk[1]-TPk[0]+2);
	pPneg->SetPointAt(kr++,temp);
	for(i=TPk[0]+1; i <= TPk[1]; ++i)
		pPneg->SetPointAt(kr++,F.GetIndPoint(i));
	pPneg->SetPointAt(kr++,temp1);
	F1.SetPlane(F.GetPlane());
	F2.SetPlane(F.GetPlane());
	Pos21 = Pos12 = M_INT;
	return true;
}

bool MDivideTree::CrParts(const MPlane &Pl,MFace *& pA1, MFace *& pA2, int temp1, int temp)
{// Intersection points must be the last points in the forest 

	//int temp1 = MFOREST.GetPointsNum() - 1;
	//if (MBSPForest::GetLocStorage() != nullptr)
	//	temp1 = -MBSPForest::GetLocStorage()->GetPSize() + 1;
	//int temp = temp1 - 1;
	//
	BPoint tP(Pl.GetPoint());
	BPoint tN(Pl.GetNormal());
	BPoint Pr(*MFOREST.GetPoint(temp)); 
	BPoint VrN((*MFOREST.GetPoint(temp1) - Pr) % N);
	int Npts = DFace.GetCount();
	NDSizedAr Signes(Npts);
	for(int i = 0; i < Npts ; ++i)
	{
		Signes[i] = VrN * (*DFace.GetPoint(i) - Pr);

		if(fabs(Signes[i]) < EQP)
			Signes[i] = 0.;
	}
	int k = 0;
	for( ; k < Npts ; ++k)
		if(Signes[k] != 0.)
			break;
	if(k == Npts) // First nonzero value
	{
		pA1->SetArraySize(3);
		pA1->SetPointAt(0,temp);
		pA1->SetPointAt(1,temp1);
		pA1->SetPointAt(2,temp);
		pA2->SetArraySize(3);
		pA2->SetPointAt(0,temp);
		pA2->SetPointAt(1,temp1);
		pA2->SetPointAt(2,temp);
//		CString f;
//		f.Format("CrParts:Npts:%d,%e,%e,%e,%e,%e,%e,%e",Npts,Signes[0],Signes[1],Signes[2],Signes[3],Signes[4],Signes[5],Signes[6]);
//		AfxMessageBox(f);
		return true;
	}
	double FirstPointSign = ((k == 0) ? Signes[k] : -Signes[k]);
	int l = 1;
	for( ; l < Npts ; ++l)
		if(Signes[l] * FirstPointSign < 0.)
			break;
	if(l == Npts) // First point with another sign
	{// Error recover
		pA1->ReplacePoints(DFace);
		pA2->SetArraySize(4);
		pA2->SetPointAt(0,temp);
		pA2->SetPointAt(1,temp1);
		pA2->SetPointAt(2,temp1);
		pA2->SetPointAt(3,temp);
		if ( FirstPointSign < 0.)
		{
			MFace *b = pA1; pA1 = pA2; pA2 = b;
		}
		return true;
	}
	int m = l + 1;
	for( ; m < Npts ; ++m)
		if(Signes[m] * Signes[l] < 0.)
			break;
	if ( FirstPointSign < 0.)
	{
		MFace *b = pA1; pA1 = pA2; pA2 = b;
	}
	if( (FirstPointSign < 0.) == DFace.IsCCW())
	{
		int ib = temp; temp = temp1; temp1 = ib;
	}
	pA2->SetArraySize(2 + m - l);
	pA2->SetPointAt(0,temp);
	pA2->SetPointAt(1,temp1);
	for( int r = l; r < m ; ++r)
		pA2->SetPointAt(r - l + 2, DFace.GetIndPoint(r));
	pA1->SetArraySize(Npts - m + l + 2);
	pA1->SetPointAt(0,temp1);
	pA1->SetPointAt(1,temp);
	int r = 2;
	for( ; m < Npts ; ++m, ++r)
		pA1->SetPointAt(r, DFace.GetIndPoint(m));
	for( int v = 0; v < l ; ++v, ++r)
		pA1->SetPointAt(r, DFace.GetIndPoint(v));
	if( (VrN * tN ) * FirstPointSign < 0.)
	{
		MFace *b = pA1; pA1 = pA2; pA2 = b;
	}
	return true;
}

void MDivideTree::Reset(const MFace * pF, MBSPNode * pR, MBSPNode *&p1, MBSPNode *&p2)
{
	DFace.ReplacePoints(*pF);
	pRoot = pR;
	pV1 = &p1;
	pV2 = &p2;
}

void MDivideTree::OutputDebug() const
{
	CString s;
	s.Format("%lf, %lf, %lf,  %lf, %lf, %lf\n", P.GetX(), P.GetY(), P.GetZ(), N.GetX(), N.GetY(), N.GetZ());
	OutputDebugString(s);
}
