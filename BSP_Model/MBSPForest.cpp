// BForest.cpp: implementation of the BForest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <set>
#include "ppltasks.h"
#include "math.h"
#include "ConstDef.h"
#include "BMatr.h"
#include "MFace.h"
#include "MBSPTree.h"
#include "MBody.h"
#include "DXSubModel5X.h"
#include "MBSPNode.h"
#include "SOTorus.h"
#include "SOCyl.h"
#include "SOPlane.h"
#include "SOSphere.h"
#include "MElemIDProc.h"
#include "MInsertFace.h"
#include "NRectMesh.h"
#include "BDynBitArray.h"
#include "MLocStorage.h"
#include "MBSPForest.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Static data initialisation
static thread_local MLocStorage* pLocStorage = nullptr;

MBSPForest GlobalForest;
MBSPForest &MBSPForest::activForest = GlobalForest;

const size_type POINT_STEP = 1000000;
const size_type FACE_STEP = 200000;
const size_type ORIG_STEP = 200000;
MBSPForest::MBSPForest()
{
	Init();
}

MBSPForest::~MBSPForest()
{
	ClearAll();
}

void MBSPForest::ClearAll()
{
	m_FaceArray.clear();
	for (size_type i=0; i<m_OrigArray.size(); i++)
	{
		delete m_OrigArray[i];
	}
	m_OrigArray.clear();

	for (size_type i=0; i<m_TreeArray.size(); i++)
	{
		delete m_TreeArray[i];
	}
	m_TreeArray.clear();
	m_PointArray.clear();
	if(pIDProc)
	{
		delete pIDProc;
		pIDProc = NULL;
	}
}

// ДОБАВЛЕНИЕ НОВЫХ ТОЧЕК В МАССИВ


// добавление новой точки в массив
size_type MBSPForest::AddPoints(const BPoint & pt)
{
	if (pLocStorage != nullptr)
	{
		return -static_cast<size_type>(pLocStorage->m_PointArray.push_back(pt) - pLocStorage->m_PointArray.cbegin()) - 1;
	}
	#ifdef _DEBUG
		//ASSERT(pt.GetH() > -1);
	#endif
////////////////////////////////TMP
//if(DEBUGMODE)
//{
//if(!(pt.GetX() >= 0))
//	if(!(pt.GetX() < 0))
//{
//	Beep(500,500);
//}
//}
	const auto it = m_PointArray.push_back(pt);
	auto d = it - m_PointArray.cbegin();
	return static_cast<size_type>(d);
}

// добавление новой грани в массив
size_type MBSPForest::AddFaces(const MFace *f)
{
	if (pLocStorage != nullptr)
	{
		const auto it = pLocStorage->m_FaceArray.push_back(*f);
		return -static_cast<size_type>(it - pLocStorage->m_FaceArray.cbegin()) - 1;
	}
	const auto it = m_FaceArray.push_back(*f);
	return static_cast<size_type>(it - m_FaceArray.cbegin());
	//++LastAddedFaceNum;
	//for(size_type j = LastAddedFaceNum; j < m_FaceArray.size(); ++j)
	//{
	//	if(m_FaceArray[j] == NULL)
	//	{
	//		LastAddedFaceNum = j;
	//		m_FaceArray[LastAddedFaceNum] = f;
	//		return LastAddedFaceNum;
	//	}
	//}
	//LastAddedFaceNum = min(LastAddedFaceNum, size_type(m_FaceArray.size()));// LAFN may be grater than FAGS after RetToMark
	//for(size_type i = 0; i < LastAddedFaceNum; ++i)
	//{
	//	if(m_FaceArray[i] == NULL)
	//	{
	//		LastAddedFaceNum = i;
	//		m_FaceArray[LastAddedFaceNum] = f;
	//		return LastAddedFaceNum;
	//	}
	//}
	//// Array is full
	//LastAddedFaceNum = size_type(m_FaceArray.size());
	//m_FaceArray.resize(LastAddedFaceNum + FACE_STEP);
	//m_FaceArray[LastAddedFaceNum] = f;
	//for(size_type k = LastAddedFaceNum+1; k < m_FaceArray.size(); ++k)
	//	m_FaceArray[k] = NULL;
	//return LastAddedFaceNum;
}

size_type MBSPForest::AddFace(const MFace& F)
{
	if (pLocStorage != nullptr)
	{
		return -static_cast<size_type>(pLocStorage->m_FaceArray.push_back(F) - pLocStorage->m_FaceArray.cbegin()) - 1;
	}
	return static_cast<size_type>(m_FaceArray.push_back(F) - m_FaceArray.cbegin());
}

const BPoint * MBSPForest::GetPoint(size_type index) const
{
	if (index < 0)
	{
		ASSERT(pLocStorage != nullptr);
		ASSERT(-index - 1 < pLocStorage->m_PointArray.size());
		return pLocStorage == nullptr ? nullptr : &pLocStorage->m_PointArray.at(-index - 1);
	}
	ASSERT(index < MFOREST.m_PointArray.size());
	
	return &m_PointArray.at(index);
}

const MFace * MBSPForest::GetFace(size_type index) const
{
	return GetFacePtr(index);
}

MFace* MBSPForest::GetFacePtr(size_type index) const
{
	if (index < 0)
	{
		ASSERT(pLocStorage != nullptr);
		ASSERT(-index - 1 < pLocStorage->m_FaceArray.size());
		return pLocStorage == nullptr ? nullptr : &pLocStorage->m_FaceArray.at(-index - 1);
	}
#ifdef _DEBUG
	if (index < 0 ||
		index >= MFOREST.m_FaceArray.size() ||
		MFOREST.m_FaceArray.size() <= 0)
		return nullptr;
#endif

	return &MFOREST.m_FaceArray[index];
}

MBody * MBSPForest::GetTree(size_type index)
{
	if (index < 0 || 
		index >= MFOREST.m_TreeArray.size() || 
		MFOREST.m_TreeArray.size() <= 0)
		return NULL;
	
	return MFOREST.m_TreeArray[index];
}

bool MBSPForest::SetPoint(size_type i, const BPoint & P)
{
	m_PointArray.at(i) = P;
	return true;
}

size_type MBSPForest::AddTree(MBody *pNewTree)
{
	m_TreeArray.push_back(pNewTree);
	return size_type(m_TreeArray.size() - 1);
}
bool MBSPForest::SetTree(size_type i, MBody *pNewTree)
{
	m_TreeArray[i] =  pNewTree;
	return true;
}

void MBSPForest::Transform(const BMatr & T)
{
	for( size_type i = 0; i < m_PointArray.size(); ++i)
		m_PointArray.at(i) = m_PointArray.at(i) * T;

	for( size_type k = 0; k < m_OrigArray.size(); ++k)
		m_OrigArray[k]->Transform(T); 
}

void MBSPForest::TransformFMark(const BMatr & T)
{
	for( size_type i = Marks[LastMarkNum].PASize; i < m_PointArray.size(); ++i)
		m_PointArray.at(i) = m_PointArray.at(i) * T;

	for( size_type k = Marks[LastMarkNum].SASize; k < m_OrigArray.size(); ++k)
		m_OrigArray[k]->Transform(T); 
}

inline MBSPForest& MBSPForest::GetActive()
{
	return activForest;
};

MLocStorage*& MBSPForest::GetLocStorage()
{
	return pLocStorage; 
}

size_type MBSPForest::SetMark(void)
{// Returns new LastMarkNum value
	if(LastMarkNum < MAX_MARKS - 1)
		++LastMarkNum;
	else // Shift Marks left (loose first mark)
	{
		for(size_type k = 0; k < MAX_MARKS-1; ++k)
			Marks[k] = Marks[k+1];
	}
	Marks[LastMarkNum].PASize = size_type(m_PointArray.size());
	Marks[LastMarkNum].FASize = size_type(m_FaceArray.size());
	Marks[LastMarkNum].SASize = size_type(m_OrigArray.size());
	Marks[LastMarkNum].TASize = size_type(m_TreeArray.size());
	return LastMarkNum;
}

size_type MBSPForest::RetToMark(void)
{
	if(LastMarkNum < 0)
		return LastMarkNum;
	m_PointArray.resize(Marks[LastMarkNum].PASize);
	//for(size_type i = Marks[LastMarkNum].FASize; i < m_FaceArray.size(); ++i)
	//	delete m_FaceArray[i];
	m_FaceArray.resize(Marks[LastMarkNum].FASize);
	for(size_type i = Marks[LastMarkNum].SASize; i < m_OrigArray.size(); ++i)
		delete m_OrigArray[i];
	m_OrigArray.resize(Marks[LastMarkNum].SASize);
	for(size_type i = Marks[LastMarkNum].TASize; i < m_TreeArray.size(); ++i)
		delete m_TreeArray[i];
	m_TreeArray.resize(Marks[LastMarkNum].TASize);
	--LastMarkNum;
	return LastMarkNum+1;
}

void MBSPForest::SetLocMark()
{
	if (pLocStorage != nullptr)
		pLocStorage->SetMark();
}

void MBSPForest::RetToLocMark()
{
	if (pLocStorage != nullptr)
		pLocStorage->RetToMark();
}

inline size_type MBSPForest::GetPointsNum(void)
{
	return size_type(m_PointArray.size());
}
inline size_type MBSPForest::GetFacesNum(void)
{
	return size_type(m_FaceArray.size());
}

size_type MBSPForest::GetTreeCount()
{
	return size_type(MFOREST.m_TreeArray.size());
}

size_type MBSPForest::CompressPoints(bool ForceCompression)
{
	if(!ForceCompression)
	{
		if(m_PointArray.size() < LastCompressPointsNum + POINT_STEP)
			return size_type(m_PointArray.size());
	}

	BDynBitArray Used(size_type(m_PointArray.size()) + 1);// +1 is used to ensure that last block of used points will be processed
	Used.SetAllFalse();

	Used.SetTrueAt(TempPlane.GetPointN());
	Used.SetTrueAt(TempPlane.GetNormalN());
	for(size_type iFace = 0; iFace < m_FaceArray.size(); ++iFace)
	{
		MFace *pFace = &m_FaceArray[iFace];
		if(!pFace)
			continue;
		Used.SetTrueAt(pFace->GetPlane().GetNormalN());
		Used.SetTrueAt(pFace->GetPlane().GetPointN());
		for(size_type i = 0; i < pFace->GetCount(); ++i)
		{
			Used.SetTrueAt(pFace->GetIndPoint(i));
		}
	}
	for (size_type i = 0; i < m_TreeArray.size(); ++i)
	{
		MBody* pBody = m_TreeArray[i];
		if (pBody)
			pBody->MarkUsedPoints(Used);
	}
	size_type d = 0;
	size_type *newind = new size_type[m_PointArray.size()];
	bool PrevUsed = false;
	for(size_type k = 0, dest = 0, source = 0; k < m_PointArray.size() + 1; ++k)
	{
		if(Used[k])
			newind[k] = d++;

		if(PrevUsed != Used[k])
		{
			if(PrevUsed)
			{//Block ended
				size_type length = k - source;
				for (int i = 0; i < length; ++i)
					m_PointArray.at(dest + i) = m_PointArray.at(source + i);
				dest += length;
			}
			else
			{// Block started
				source = k;
			}
		}
		PrevUsed = Used[k];
	}

	for(size_type iFace = 0; iFace < m_FaceArray.size(); ++iFace)
		m_FaceArray[iFace].SetNewIndexes(newind);
	for (size_type i = 0; i < m_TreeArray.size(); ++i)
	{
		MBody* pBody = m_TreeArray[i];
		if (pBody)
			pBody->SetNewIndexes(newind);
	}
	TempPlane = MPlane(newind[TempPlane.GetNormalN()], newind[TempPlane.GetPointN()]);
	delete[] newind;
	m_PointArray.resize(d);
	LastCompressPointsNum = size_type(fmax(POINT_STEP, m_PointArray.size()));
	return size_type(m_PointArray.size());
}

size_type MBSPForest::CompressFacesOrig(bool ForceCompression)
{
	if (ForceCompression || m_FaceArray.size() > LastCompressFacesNum + FACE_STEP)
	{
		BDynBitArray UsedF(size_type(m_FaceArray.size()));
		UsedF.SetAllFalse();
		BDynBitArray UsedO(size_type(m_OrigArray.size()));
		UsedO.SetAllFalse();
		for (size_type i = 0; i < m_TreeArray.size(); ++i)
		{
			MBody* pBody = m_TreeArray[i];
			if (pBody)
				pBody->MarkUsedFacesOrig(UsedF, UsedO);
		}
		if (UsedF.IsAllFalse())
			m_FaceArray.resize(0);
		else
		{
			size_type d = 0;
			size_type* newind = new size_type[m_FaceArray.size()];
			for (size_type k = 0; k < m_FaceArray.size() + 1; ++k)
			{
				if (UsedF[k])
				{
					m_FaceArray.at(d).MoveFrom(m_FaceArray.at(k));
					newind[k] = d++;
				}
			}
			m_FaceArray.resize(d);
			for (size_type i = 0; i < m_TreeArray.size(); ++i)
			{
				MQuadTree* pTree = dynamic_cast<MQuadTree*>(m_TreeArray[i]);
				if (pTree)
					pTree->RenumUsedFaces(newind);
			}
		}
		LastCompressFacesNum = size_type(fmax(FACE_STEP, m_FaceArray.size()));
		size_type l = 0;
		for (size_type k = 0; k < m_OrigArray.size(); ++k)
		{
			if (!UsedO[k])
			{
				delete m_OrigArray[k];
				m_OrigArray[k] = NULL;
			}
			else
				++l;
		}
		if (l == 0)
			m_OrigArray.resize(0);
		LastCompressOrigNum = l;
		return l;
	}
	else
	{
		return CompressOrig(ForceCompression);
	}
}

size_type MBSPForest::AddOrigSurf(SOrigSurf* pSurf)
{
	m_OrigArray.push_back(pSurf);
	++LastCompressOrigNum;
	return (LastAddedOrigNum = size_type(m_OrigArray.size() - 1));
}

size_type MBSPForest::CompressOrig(bool ForceCompression)
{
	if(m_OrigArray.empty())
		return 0;
	if (!ForceCompression && m_OrigArray.size() < LastCompressOrigNum + ORIG_STEP)
		return static_cast<size_type>(m_OrigArray.size());
	BDynBitArray Used(size_type(m_OrigArray.size()));
	Used.SetAllFalse();

	for(size_type i = 0; i < m_TreeArray.size(); ++i)
	{
		MBody *pBody = m_TreeArray[i];
		if(pBody)
			pBody->MarkUsedOrig(Used);
	}
	size_type l = 0;
	for(size_type k = 0; k < m_OrigArray.size(); ++k)
	{
		if(!Used[k])
		{
			delete m_OrigArray[k];
			m_OrigArray[k] = NULL;
		}
		else
			++l;
	}
	if(l == 0)
		m_OrigArray.resize(0);
	LastCompressOrigNum = l;
	return l;
}

size_type MBSPForest::AddOrigSurfInside(SOrigSurf * pSurf)
{
	for(size_type i = LastAddedOrigNum + 1; i < m_OrigArray.size(); ++i)
	{
		if(m_OrigArray[i] == NULL)
		{
			LastAddedOrigNum = i;
			++LastCompressOrigNum;
			m_OrigArray[i] = pSurf;
			return LastAddedOrigNum;
		}
	}
	if(LastCompressOrigNum < 0.5 * m_OrigArray.size())
	{
		for(size_type i = 0; i < MFOREST.m_OrigArray.size(); ++i)
		{
			if(m_OrigArray[i] == NULL)
			{
				LastAddedOrigNum = i;
				++LastCompressOrigNum;
				m_OrigArray[i] = pSurf;
				return LastAddedOrigNum;
			}
		}
	}
	++LastCompressOrigNum;
	m_OrigArray.push_back(pSurf);
	return (LastAddedOrigNum = size_type(m_OrigArray.size() - 1));
}

SOrigSurf * MBSPForest::GetOrigSurf(size_type index) const
{
	if (MElemIDProc::IsSurfIndEmpty(index))
		return nullptr;
#ifdef _DEBUG
	if (index < 0 || index >= m_OrigArray.size())
		return nullptr;
#endif
	
	return MFOREST.m_OrigArray[index];
}

size_type MBSPForest::RemoveMark(void)
{
	if(LastMarkNum < 0)
		return LastMarkNum;
	--LastMarkNum;
	return LastMarkNum+1;
}

void MBSPForest::Init(void)
{
	m_PointArray.resize(100);
	m_FaceArray.resize(0);
	m_OrigArray.resize(0);
	LastMarkNum = -1;
	LastCompressPointsNum = POINT_STEP;
	LastCompressFacesNum = FACE_STEP;
	LastAddedFaceNum = -1;
	m_PointArray.push_back(BPoint(0., 0., 0., 1.));
	auto i1 = m_PointArray.size() - 1;
	m_PointArray.push_back(BPoint(0., 0., 0., 1.));
	TempPlane = MPlane(size_type(i1), size_type(m_PointArray.size() - 1));

	pIDProc = nullptr;
}

void MBSPForest::DeleteTree(size_type i)
{
	using namespace concurrency;
	MBody *pBody = GetTree(i);
	SetTree(i, nullptr);
	create_task([pBody] { delete pBody; });
	//if(pBody)
	//	delete pBody;
	m_TreeArray[i] = nullptr;
}

size_type MBSPForest::CalcNormalSafe(size_type i1, size_type i2, size_type i3, double sign)
{
	// Calculate normalized normal vector to the Pi1 Pi2 Pi3 plane
	// and store it in the forest
	// Pi1 Pi2 Pi3 is oriented CCW
	// it is assumed that i1 != i2 != i3
	// Returns the number of the calculated vector in the forest
	BPoint P(*GetPoint(i1));
	BPoint V2(*GetPoint(i2) - P);
	double d2 = V2.D2();
	BPoint V3(*GetPoint(i3) - P);
	double d3 = V3.D2();
	if (d2 < MIND * MIND || d3 < MIND * MIND || (V2 - V3).D2() < MIND * MIND)
		return -1;
	P = V2 % V3;
	double d = sqrt(P.D2());
	if (DEBUGMODE)
	{
		if (d < DMIN)
		{
			CString frm("Zero normal:i1: %d,i2: %d,i3: %d, d2: %lf \n");
			CString str;
			str.Format(frm, i1, i2, i3, d);
			LOGFILE.WriteString(str);
		}
	}
	return AddPoints(P * (sign / d));
}

size_type MBSPForest::CalcNormal(size_type i1, size_type i2, size_type i3, double sign)
{
	// Calculate normalized normal vector to the Pi1 Pi2 Pi3 plane
	// and store it in the forest
	// Pi1 Pi2 Pi3 is oriented CCW
	// it is assumed that i1 != i2 != i3
	// Returns the number of the calculated vector in the forest
	BPoint P(*GetPoint(i1));
	BPoint V2(*GetPoint(i2) - P);
	BPoint V3(*GetPoint(i3) - P);
	P = V2 % V3;
	double d = sqrt(P.D2());
	if (DEBUGMODE)
	{
		if (d < DMIN)
		{
			CString frm("Zero normal:i1: %d,i2: %d,i3: %d, d2: %lf \n");
			CString str;
			str.Format(frm, i1, i2, i3, d);
			LOGFILE.WriteString(str);
		}
	}
	return AddPoints(P * (sign / d));
}

size_type MBSPForest::CalcNormal(size_type i1, size_type i2, size_type i3, size_type i4, double sign)
{
// Calculate normalized normal vector to the Pi1 Pi2 Pi3 Pi4 plane
// and store it in the forest
// Pi1 Pi2 Pi3 Pi4 is oriented CCW
// it is assumed that at least 3 point numbers are different
// Returns number of the calculated vector in the forest
	if(i1 == i2 || i2 == i3)
		return CalcNormal(i1,i3,i4,sign);

	return CalcNormal(i1,i2,i3,sign);
}

MFace * MBSPForest::CalcFace(size_type i1, size_type i2, size_type i3, size_type i4, double sign)
{
// Returned pointer must be deleted by the caller
	size_type n[4], ind = 0;
//	if(DEBUGMODE)
	{
		if((*GetPoint(i1) - *GetPoint(i2)).D2() < EQP*EQP)
			i1 = i2;
		if((*GetPoint(i2) - *GetPoint(i3)).D2() < EQP*EQP)
			i2 = i3;
		if((*GetPoint(i3) - *GetPoint(i4)).D2() < EQP*EQP)
			i3 = i4;
		if((*GetPoint(i4) - *GetPoint(i1)).D2() < EQP*EQP)
			i4 = i1;
	}
//
	if(i1 != i2) n[ind++] = i1;
	if(i2 != i3) n[ind++] = i2;
	if(i3 != i4) n[ind++] = i3;
	if(i4 != i1) n[ind++] = i4;

	if(ind < 3)
		return NULL;
	if(DEBUGMODE)
	{
		if(ind > 3)
		{
			size_type n1 = CalcNormal(i1,i2,i3,sign);
			size_type n2 = CalcNormal(i1,i3,i4,sign);
			double d = (*GetPoint(n1))*(*GetPoint(n2));
			if(DEBUGMODE && d < 0.9998)
			{
				CString str;
				str.Format("NonPlaneFace:d:%lf\n",d);
				LOGFILE.WriteString(str);
				return NULL;
			}
		}
	}
	MFace *pFace = new MFace(CalcNormal(n[0], n[1], n[2], sign),n[0]);
	pFace->SetArraySize(ind);
	for(size_type i = 0; i < ind; ++i)
		pFace->SetPointAt(i,n[i]);

	return pFace;
}

void MBSPForest::SetIDProc(MElemIDProc * pIDP)
{
	delete pIDProc;
	pIDProc = pIDP;
}

MElemIDProc * MBSPForest::GetIDProc(void)
{
	if (pIDProc == nullptr)
		pIDProc = new MElemIDProc;// for NCMHandlers
	return pIDProc;
}

CString MBSPForest::GetInfoStr(void)
{
	size_t MaxTreeDepth = 0;
	size_t DXSize = 0;
	size_t DXUsedSize = 0;
	for(size_type i = 0; i < m_TreeArray.size(); ++i)
	{
		MQuadTree *pQuadTree = dynamic_cast<MQuadTree *>(m_TreeArray[i]);
		if (pQuadTree)
			MaxTreeDepth = max(MaxTreeDepth, pQuadTree->GetMaxDepth());
		else
		{
			DXSubModel5X *pDXSubModel = dynamic_cast<DXSubModel5X *>(m_TreeArray[i]);
			if (pDXSubModel)
			{
				DXSize += pDXSubModel->GetGlobSize();
				DXUsedSize += pDXSubModel->GetUsedSize();
			}
		}
	}
	CString str;
	str.Format("\nTrees:%d; Faces:%d; Points:%d; Surf:%d\nCFace:%d; CPoint:%d; AFace:%d; TreeDepth:%d; DXSize:%d; DXUsedSize:%d",
		m_TreeArray.size(), m_FaceArray.size(), m_PointArray.size(),m_OrigArray.size(),
		LastCompressFacesNum, LastCompressPointsNum, LastAddedFaceNum, MaxTreeDepth, DXSize, DXUsedSize);
	return str;
}

void MBSPForest::Compress(bool ForceCompression)
{
	MFOREST.CompressFacesOrig(ForceCompression);
	MFOREST.CompressPoints(ForceCompression);
}

void MBSPForest::Transform1Tree(int TreeNum, const BMatr & M)
{
	MBody *pTree = GetTree(TreeNum);
	if (pTree == nullptr)
		return;
	pTree->Transform(M);
}

void MBSPForest::TransformSelected(NTransfData &TransfData, const BMatr & M)
{
	for each(auto Ind in TransfData.PointsIndexes)
		m_PointArray.at(Ind) = m_PointArray.at(Ind) * M;
	for each(auto Ind in TransfData.SurfIndexes)
		if(m_OrigArray[Ind] != nullptr)
			m_OrigArray[Ind]->Transform(M);
}

void MBSPForest::DebugPrintSubtree(const MBSPNode* pV) const
{
	if (MBSPNode::IsTerminal(pV))
		return;

	DebugPrint(pV);

	DebugPrintSubtree(pV->GetLeft());
	DebugPrintSubtree(pV->GetRight());
}

void MBSPForest::DebugPrint(const MBSPNode* pV) const
{
	CString s;
	s.Format("%d %d %d %d\n", pV->GetFaceN(), pV->GetBFaceN(), pV->GetLeft(), pV->GetRight());
	OutputDebugString(s);
}

MBSPForest::ForestSizes MBSPForest::GetSizes() const
{
	ForestSizes Sizes;
	Sizes.Points = m_PointArray.size() * sizeof(m_PointArray[0]);
	Sizes.Faces = m_FaceArray.size() * sizeof(m_FaceArray[0]);
	Sizes.Origs = m_OrigArray.size() * sizeof(m_OrigArray[0]);
	Sizes.PointsMem = m_PointArray.capacity() * sizeof(m_PointArray[0]);
	Sizes.FacesMem = m_FaceArray.capacity() * sizeof(m_FaceArray[0]);
	Sizes.OrigsMem = m_OrigArray.capacity() * sizeof(m_OrigArray[0]);

	for (size_type i = 0; i < m_TreeArray.size(); ++i)
	{
		MQuadTree* pQuadTree = dynamic_cast<MQuadTree*>(m_TreeArray[i]);
		if (pQuadTree != nullptr)
			Sizes += pQuadTree->GetSizes();
		else
		{
			//DXSubModel5X* pDXSubModel = dynamic_cast<DXSubModel5X*>(m_TreeArray[i]);
			//if (pDXSubModel)
			//{
			//	DXSize += pDXSubModel->GetGlobSize();
			//	DXUsedSize += pDXSubModel->GetUsedSize();
			//}
		}
	}
	return Sizes;
}

MBSPForest::ForestSizes::ForestSizes()
{
	Points = 0;
	Faces = 0;
	Origs = 0;
	Trees = 0;
	MaxTreeDepth = 0;
	PointsMem = 0;
	FacesMem = 0;
	OrigsMem = 0;
}

const MBSPForest::ForestSizes& MBSPForest::ForestSizes::operator+=(const ForestSizes& In)
{
	Points += In.Points;
	Faces += In.Faces;
	Origs += In.Origs;
	Trees += In.Trees;
	PointsMem += In.PointsMem;
	FacesMem += In.FacesMem;
	OrigsMem += In.OrigsMem;
	MaxTreeDepth = max(MaxTreeDepth, In.MaxTreeDepth);
	return *this;
}
