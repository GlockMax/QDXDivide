#include "stdafx.h"
#include "MFace.h"
#include "MBSPNode.h"
#include "BDynBitArray.h"
#include "MLocStorage.h"

MLocStorage::MLocStorage()
{
	LocPASize = 0;
	LocFASize = 0;
	LastCompressPointsNum = 100;
	LastCompressFacesNum = 100;
}

void MLocStorage::ClearAll()
{
	m_PointArray.clear();
	m_FaceArray.clear();
	LastCompressPointsNum = 100;
	LastCompressFacesNum = 100;
}

void MLocStorage::MarkUsedForestPoints(BDynBitArray& Used) const
{
	for (size_type iFace = 0; iFace < m_FaceArray.size(); ++iFace)
	{
		const MFace& Face = m_FaceArray[iFace];
		Used.SetTrueAt(Face.GetPlane().GetNormalN());
		Used.SetTrueAt(Face.GetPlane().GetPointN());
		for (size_type i = 0; i < Face.GetCount(); ++i)
			Used.SetTrueAt(Face.GetIndPoint(i));
	}
}

size_type MLocStorage::CompressPoints(MLocStorage* pLocStor, bool ForceCompression)
{
	if (pLocStor == nullptr)
		return 0;
	if (!ForceCompression)
	{
		if (pLocStor->m_PointArray.size() < pLocStor->LastCompressPointsNum * 2)
			return size_type(pLocStor->m_PointArray.size());
	}

	BDynBitArray Used(size_type(pLocStor->m_PointArray.size()) + 1);// +1 is used to ensure that last block of used points will be processed
	Used.SetAllFalse();
	for (size_type iFace = 0; iFace < pLocStor->m_FaceArray.size(); ++iFace)
	{
		const MFace& Face = pLocStor->m_FaceArray[iFace];
		const int Ind = Face.GetPlane().GetNormalN();
		if(Ind < 0)
			Used.SetTrueAt(-Ind - 1);
		const int Ind1 = Face.GetPlane().GetPointN();
		if (Ind1 < 0)
			Used.SetTrueAt(-Ind1 - 1);
		for (size_type i = 0; i < Face.GetCount(); ++i)
		{
			const int Ind2 = Face.GetIndPoint(i);
			if (Ind2 < 0)
				Used.SetTrueAt(-Ind2 - 1);
		}
	}
	size_type d = 0;
	size_type* newind = new size_type[pLocStor->m_PointArray.size()];
	bool PrevUsed = false;
	for (size_type k = 0, dest = 0, source = 0; k < pLocStor->m_PointArray.size() + 1; ++k)
	{
		if (Used[k])
			newind[k] = d++;

		if (PrevUsed != Used[k])
		{
			if (PrevUsed)
			{//Block ended
				size_type length = k - source;
				for (int i = 0; i < length; ++i)
					pLocStor->m_PointArray.at(dest + i) = pLocStor->m_PointArray.at(source + i);
				dest += length;
			}
			else
			{// Block started
				source = k;
			}
		}
		PrevUsed = Used[k];
	}

	for (size_type iFace = 0; iFace < pLocStor->m_FaceArray.size(); ++iFace)
	{
		MFace& Face = pLocStor->m_FaceArray[iFace];
		int Ind = Face.GetPlane().GetNormalN();
		Ind = Ind < 0 ? -newind[-Ind - 1] - 1 : Ind;
		int Ind1 = Face.GetPlane().GetPointN();
		Ind1 = Ind1 < 0 ? -newind[-Ind1 - 1] - 1 : Ind1;
		Face.SetPlane(MPlane(Ind, Ind1));
		for (size_type i = 0; i < Face.GetCount(); ++i)
		{
			const int Ind2 = Face.GetIndPoint(i);
			if (Ind2 < 0)
				Face.SetPointAt(i, -newind[-Ind2 - 1] - 1);
		}
	}
	delete[] newind;
	pLocStor->m_PointArray.resize(d);

	pLocStor->LastCompressPointsNum = size_type(pLocStor->m_PointArray.size());
	return size_type(pLocStor->m_PointArray.size());
}

size_type MLocStorage::CompressFaces(MLocStorage* pLocStor, MBSPNode* pRoot, bool ForceCompression)
{
	if (pLocStor == nullptr)
		return 0;
	if (MBSPNode::IsTerminal(pRoot))
		return size_type(pLocStor->m_FaceArray.size());

	if (!ForceCompression)
	{
		if (pLocStor->m_FaceArray.size() < pLocStor->LastCompressFacesNum * 2)
			return size_type(pLocStor->m_FaceArray.size());
	}

	BDynBitArray Used(size_type(pLocStor->m_FaceArray.size()) + 1);// +1 is used to ensure that last block of used points will be processed
	Used.SetAllFalse();

	std::vector<MBSPNode*> NodeStack;
	NodeStack.push_back(pRoot);
	while (!NodeStack.empty())
	{
		const MBSPNode* pNode = NodeStack.back();
		NodeStack.pop_back();
		int Ind = pNode->GetFaceN();
		if (Ind < 0)
			Used.SetTrueAt(-Ind - 1);
		if (!MBSPNode::IsTerminal(pNode->GetLeft()))
			NodeStack.push_back(pNode->GetLeft());
		if (!MBSPNode::IsTerminal(pNode->GetRight()))
			NodeStack.push_back(pNode->GetRight());
	}

	size_type d = 0;
	size_type* newind = new size_type[pLocStor->m_FaceArray.size()];
	for (size_type k = 0; k < pLocStor->m_FaceArray.size() + 1; ++k)
	{
		if (Used[k])
		{
			pLocStor->m_FaceArray.at(d).MoveFrom(pLocStor->m_FaceArray.at(k));
			newind[k] = d++;
		}
	}

	NodeStack.push_back(pRoot);
	while (!NodeStack.empty())
	{
		MBSPNode* pNode = NodeStack.back();
		NodeStack.pop_back();
		int Ind = pNode->GetFaceN();
		if (Ind < 0)
			pNode->SetFace(-newind[-Ind - 1] - 1);
		if (!MBSPNode::IsTerminal(pNode->GetLeft()))
			NodeStack.push_back(pNode->GetLeft());
		if (!MBSPNode::IsTerminal(pNode->GetRight()))
			NodeStack.push_back(pNode->GetRight());
	}
	delete[] newind;
	pLocStor->m_FaceArray.resize(d);

	pLocStor->LastCompressFacesNum = size_type(pLocStor->m_FaceArray.size());
	return size_type(pLocStor->m_FaceArray.size());
}

void MLocStorage::TransformPts(const BMatr& M)
{
	for(auto it = m_PointArray.begin(); it != m_PointArray.end(); ++it)
		*it = (*it) * M;
}

void MLocStorage::operator=(const MLocStorage& In)
{
	m_FaceArray = In.m_FaceArray;
	m_PointArray = In.m_PointArray;
	LocPASize = 0;
	LocFASize = 0;
	LastCompressPointsNum = In.LastCompressPointsNum;
	LastCompressFacesNum = In.LastCompressFacesNum;
}

void MLocStorage::SetMark()
{
	LocPASize = static_cast<size_type>(m_PointArray.size());
	LocFASize = static_cast<size_type>(m_FaceArray.size());
}

void MLocStorage::RetToMark()
{
	m_PointArray.resize(LocPASize);
	m_FaceArray.resize(LocFASize);
}

void MLocStorage::SetNewIndexes(int* newind)
{
	for (size_type iFace = 0; iFace < m_FaceArray.size(); ++iFace)
		m_FaceArray[iFace].SetNewIndexes(newind);
}

MBSPForest::ForestSizes MLocStorage::GetSizes() const
{
	MBSPForest::ForestSizes Sizes;
	Sizes.Points = m_PointArray.size() * sizeof(m_PointArray[0]);
	Sizes.Faces = m_FaceArray.size() * sizeof(m_FaceArray[0]);
	Sizes.PointsMem = m_PointArray.capacity() * sizeof(m_PointArray[0]);
	Sizes.FacesMem = m_FaceArray.capacity() * sizeof(m_FaceArray[0]);
	return Sizes;
}

void MLocStorage::Shrink()
{
	m_PointArray.shrink_to_fit();
	m_FaceArray.shrink_to_fit();
}

