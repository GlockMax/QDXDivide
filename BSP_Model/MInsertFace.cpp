#include "stdafx.h"
#include "NCMComData.h"
#include ".\minsertface.h"
#include "MBSPNode.h"
#include "SOrigSurf.h"
#include "BPoint.h"
#include "ConstDef.h"
#include "math.h"
#include "NDSizedAr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Initialize the static members
thread_local MStack MInsertFace::Stack;

MInsertFace::~MInsertFace(void)
{
}

int MInsertFace::Insert(void)
{
	for(int i = 0;; ++i)
	{
		if(MBSPNode::IsTerminal(*pRoot))
		{// Put pFace to the root node
			(*pRoot) = new MBSPNode();
			(*pRoot)->SetLeft((MBSPNode *)M_IN_PTR);
			(*pRoot)->SetRight((MBSPNode *)M_OUT_PTR);
			(*pRoot)->SetFace(nFace);
			(*pRoot)->SetBFace(ElementID);
			(*pRoot)->SetCadrID(HistID);
			return 1; // One face was inserted
		}
		// pRoot is not terminal
		MFace BPosFace, BNegFace;
		MFace *pPosFace = &BPosFace, *pNegFace = &BNegFace;
		M_POSITION res = DetPosition(pPosFace, pNegFace);
		switch(res)
		{
		case M_POS:
			pRoot = (*pRoot)->GetLeftPtr();
			break;
		case M_NEG:
			pRoot = (*pRoot)->GetRightPtr();
			break;
		default:
		case M_ERR:
			if(DEBUGMODE)
			{
				LOGFILE.WriteString("InsertFace::Insert:M_ERR\n");
			}
			PlacePriElemID(*pRoot);// Surf priority
			return 0;// Ignore face
		case M_EMP:
			if(DEBUGMODE)
			{
				LOGFILE.WriteString("M_EMP\n");
			}
			PlacePriElemID(*pRoot);// Surf priority
			return 0;// Ignore face
		case M_IDN:
			PlacePriElemID(*pRoot);// Surf priority
			return 0;// Ignore face
		case M_INT:
			{
				Stack.Push(
					(void *)new MInsertFace(
						(*pRoot)->GetRightPtr(),
						MFOREST.AddFace(*pNegFace),
						ElementID, HistID)
						);
				if (nFace < 0)
				{
					MFOREST.GetFacePtr(nFace)->ReplacePoints(*pPosFace);
				}
				else
				{
					nFace = MFOREST.AddFace(*pPosFace);
					pFace = MFOREST.GetFace(nFace);
				}
				pRoot = (*pRoot)->GetLeftPtr();
			}
			break;
		}
	}
//	return -2;
}

M_POSITION MInsertFace::DetPosition(MFace* pP1, MFace* pP2)
{
	double Eps = MINDP;
	if(!pFace)
		return M_EMP;
	if(pFace->IsEmpty())
		return M_EMP;
	BPoint N((*pRoot)->GetFace()->GetPlane().GetNormal());
	BPoint P((*pRoot)->GetFace()->GetPlane().GetPoint());
	double Sin2 = (N % pFace->GetPlane().GetNormal()).D2();
	if( Sin2 <= MINAR*MINAR)
	{// Planes are parallel
		double s = (pFace->GetPlane().GetPoint() - P) * N;
		if(fabs(s) <= Eps)// Planes are identical
			return M_IDN;
		else
			return s > 0 ? M_POS : M_NEG;
	}
	// Find all signes for pFace face points
	int Npts = pFace->GetCount();
	NDSizedAr Signes(Npts);
	double P_N = P * N;
	for(int kp = 0; kp < Npts; ++kp)
	{
		double buf = (*pFace->GetPoint(kp)) * N - P_N;
		if(fabs(buf) < MIND)
			buf = 0.;
		Signes[kp] = buf;
	}
	// Find all signum changes points
	int sgn = 0;
	//Find first not zero value
	int isb = 0;
	for( ;  isb < Npts ; ++isb)
		if(Signes[isb] != 0.)
			break;
	if(isb == Npts)
	{
		if(DEBUGMODE)
		{
			LOGFILE.WriteString("InsertFace::DetPosition:All signums are zero\n");
		}
		 return M_ERR;
		/* ISSUE 39 
		// find the value with maximum module
		double Val = 0;
		for (int kp = 0; kp < Npts; ++kp)
		{
			double buf = (*pFace->GetPoint(kp)) * N - P_N;
			if (fabs(buf) > fabs(Val))
				Val = buf;
		}
		return Val > 0. ? M_POS : (Val < 0. ? M_NEG : M_ERR);*/
	}
	int TPk[2];
	for( int ks = isb; ks < Npts;)
	{
		// Find next not zero value
		int is = 1;
		for( ; Signes[(ks+is)%Npts] == 0. && (ks+is)%Npts != isb ; ++is);
		if( Signes[(ks+is)%Npts] * Signes[ks] < 0.)
		{
			TPk[sgn%2] = ks;
			++sgn; // Signum change found
		}
		ks = ks+is;
	}
	double FirstSign = Signes[isb];
	if( sgn != 2) // The face don't intersect the plane or error
	{
		if( sgn != 0) // Error
		{
			if(DEBUGMODE)
			{
				LOGFILE.WriteString("InsertFace::DetPosition:sgn != 2 && sgn != 0\n");
			}
			return M_ERR;
		}
		return FirstSign > 0. ? M_POS : M_NEG;
	}
	// Faces intersects
	// Find and store intersection points
	int k0,k1;
	double s0,s1;
	k0 = TPk[0];
	k1 = (k0+1)%Npts;
	s0 = (*pFace->GetPoint(k0) - P) * N;
	s1 = (*pFace->GetPoint(k1) - P) * N;
	BPoint TP0 = ((*pFace->GetPoint(k0)) * fabs(s1) + (*pFace->GetPoint(k1)) * fabs(s0))*(1./(fabs(s0) + fabs(s1)));

	k0 = TPk[1];
	k1 = (k0+1)%Npts;
	s0 = (*pFace->GetPoint(k0) - P) * N;
	s1 = (*pFace->GetPoint(k1) - P) * N;
	BPoint TP1 = ((*pFace->GetPoint(k0)) * fabs(s1) + (*pFace->GetPoint(k1)) * fabs(s0))*(1./(fabs(s0) + fabs(s1)));

	// All intersection points found (in TP)
	int temp = MFOREST.AddPoints(TP0);
	int temp1 = MFOREST.AddPoints(TP1);
	if(TPk[1]<TPk[0])
	{// TPk[0] <-> TPk[0]
		int b=TPk[0];TPk[0]=TPk[1];TPk[1]=b;
	 // temp <-> temp1
		b=temp;temp=temp1;temp1=b;
	}
	MFace *pPpos = pP1, *pPneg = pP2;

	if(FirstSign < 0.)
	{
		pPpos = pP2;
		pPneg = pP1;
	}
	pPpos->SetArraySize(TPk[0]+1+2+pFace->GetCount()-TPk[1]-1);
	int kr=0;
	int i;
	for(i=0; i <= TPk[0]; ++i)
		pPpos->SetPointAt(kr++,pFace->GetIndPoint(i));
	pPpos->SetPointAt(kr++,temp);
	pPpos->SetPointAt(kr++,temp1);
	for(i=TPk[1]+1; i < pFace->GetCount(); ++i)
		pPpos->SetPointAt(kr++,pFace->GetIndPoint(i));
	kr=0;
	pPneg->SetArraySize(TPk[1]-TPk[0]+2);
	pPneg->SetPointAt(kr++,temp);
	for(i=TPk[0]+1; i <= TPk[1]; ++i)
		pPneg->SetPointAt(kr++,pFace->GetIndPoint(i));
	pPneg->SetPointAt(kr++,temp1);
	pP1->SetPlane(pFace->GetPlane());
	pP2->SetPlane(pFace->GetPlane());
	return M_INT;
}
MInsertFace::MInsertFace(MBSPNode ** pR, int iF, int ID, cadrID cID) : ElementID(ID), HistID(cID)
{
	pRoot = pR;
	nFace = iF;
	pFace = MFOREST.GetFace(nFace);
}

int MInsertFace::DoInsert(void)
{
	Stack.Push(this);
	MInsertFace *Cur;
	int num = 0;
	while((Cur = (MInsertFace *)Stack.Pop()) != NULL)
	{
		int b = Cur->Insert();
		if(Cur != this)
			delete Cur;
		if(b < 0)
		{
			Stack.Clear();
			return b;
		}
		num += b;
	}
	return num;
}

MInsertFace::MInsertFace(void) : ElementID(0), HistID(-1)
{
}

void MInsertFace::Init(void)
{
	Stack.Clear();
}

void MInsertFace::PlacePriElemID(MBSPNode * pNode)
{
	int NodeSurfID = MFOREST.GetIDProc()->GetSurfID(pNode->GetBFaceN());
	if(NodeSurfID < 0 || NodeSurfID == ID_EMPTY)
	{
		pNode->SetBFace(ElementID);
		pNode->SetCadrID(HistID);
		return;
	}
	int ThisSurfID = MFOREST.GetIDProc()->GetSurfID(ElementID);
	if (NodeSurfID == ThisSurfID)
		return;
	if(ThisSurfID < 0 || ThisSurfID == ID_EMPTY)
		return;
	// Prefer plane to other
	auto pNodeSurf = MFOREST.GetOrigSurf(NodeSurfID);
	if (pNodeSurf == nullptr)
		return;
	if(pNodeSurf->GetType() == PLANE)
		return;
	auto pThisSurf = MFOREST.GetOrigSurf(ThisSurfID);
	if (pThisSurf == nullptr)
		return;
	if (pThisSurf->GetType() == PLANE)
	{
		pNode->SetBFace(ElementID);
		pNode->SetCadrID(HistID);
		return;
	}
	if (pNodeSurf->GetType() == CYLINDER)
		return;
	if (pThisSurf->GetType() == CYLINDER)
	{
		pNode->SetBFace(ElementID);
		pNode->SetCadrID(HistID);
		return;
	}
}
