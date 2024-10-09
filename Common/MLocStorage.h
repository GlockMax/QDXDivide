#pragma once
#include "BSP_MODEL_API.h"
#include "MBSPForest.h"

class BSP_MODEL_API MLocStorage
{
	friend class MBSPForest;
	friend class MQuadTree;

protected:
	MFaceArray	m_FaceArray;	// Faces
	MPointArray m_PointArray;	// Points
	size_type LocPASize;
	size_type LocFASize;
	size_type LastCompressPointsNum;
	size_type LastCompressFacesNum;
public:
	MLocStorage();
	size_type GetPSize() const { return static_cast<size_type>(m_PointArray.size()); }
	void ClearAll();
	void MarkUsedForestPoints(BDynBitArray& Used) const;
	static size_type CompressPoints(MLocStorage* pLocStor, bool ForceCompression = false);
	static size_type CompressFaces(MLocStorage* pLocStor, MBSPNode *pRoot, bool ForceCompression = false);
	void SetNewIndexes(int* newind);
	MBSPForest::ForestSizes GetSizes() const;
	void Shrink();
	const BPoint* GetPoint(int i) const {
		return &m_PointArray.at(i);
	}
	void TransformPts(const BMatr& M);
	void operator=(const MLocStorage& In);
protected:
	void SetMark();
	void RetToMark();
};

