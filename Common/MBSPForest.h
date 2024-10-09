// MBSPForest.h: interface for the MBSPForest class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BSP_MODEL_API.h"
#include "MPlane.h"
#include "MFace.h"
#include "BDebugState.h"
#include "concurrent_vector.h"
#include <set>

using namespace concurrency;

class SOrigSurf;
class MBSPNode;
class MBSPTree;
class MBody;
class GQuadRender;
class BMatr;
class MElemIDProc;
class MeshArr;
class MLocStorage;

typedef int size_type;
typedef concurrent_vector<BPoint> MPointArray; 
typedef	concurrent_vector<MFace> MFaceArray;
typedef	concurrent_vector<SOrigSurf*> MOrigArray;
typedef	concurrent_vector<MBody*> MTreeArray;


#define MFOREST MBSPForest::GetActive()
#define DEBUGMODE (BDebugState::Get().GetDebugMode())
#define LOGFILE (BDebugState::Get().GetLogFile())
const size_type MAX_MARKS = 128;

class BSP_MODEL_API MBSPForest
{
	struct Mark
	{
		size_type FASize;
		size_type SASize;
		size_type PASize;
		size_type TASize;
	};
public:
	struct NTransfData
	{
		std::set<int> PointsIndexes;
		std::set<int> SurfIndexes;
	};
	class ForestSizes
	{
	public:
		ForestSizes();
		const ForestSizes& operator +=(const ForestSizes& In);
		size_t Points;
		size_t Faces;
		size_t Origs;
		size_t Trees;
		size_t MaxTreeDepth;
		size_t PointsMem;
		size_t FacesMem;
		size_t OrigsMem;
	};
protected:
	static MBSPForest &activForest;
	MElemIDProc *pIDProc;
	size_type LastMarkNum;
	Mark Marks[MAX_MARKS];
	MFaceArray	m_FaceArray;	// Faces
	MOrigArray	m_OrigArray;	// Original surfaces
	MPointArray m_PointArray;	// Points
	MTreeArray	m_TreeArray;	// Bodies
	size_type LastCompressPointsNum;
	size_type LastCompressFacesNum;
	size_type LastCompressOrigNum;
	size_type LastAddedFaceNum;
	size_type LastAddedOrigNum;
public:
	MPlane TempPlane;

public:
	void ClearAll();
	MBSPForest();
	virtual ~MBSPForest();

	size_type AddPoints(const BPoint & pt);
	size_type AddFaces(const MFace* f);
	size_type AddFace(const MFace& F);

	size_type AddTree(MBody *pNewTree);

	const BPoint * GetPoint(size_type index) const;
	const MFace* GetFace(size_type index) const;
	MFace* GetFacePtr(size_type index) const;
	MBody * GetTree(size_type index);
	size_type GetTreeCount();

	static MBSPForest& GetActive();
	static MLocStorage*& GetLocStorage();


public:
	bool SetPoint(size_type i,const BPoint & P);
	bool SetTree(size_type i, MBody *pNewTree);
	void Transform(const BMatr & T);
	void TransformFMark(const BMatr & T);
	size_type SetMark(void);
	size_type RetToMark(void);
	void SetLocMark();
	void RetToLocMark();
	size_type GetPointsNum(void);
	size_type GetFacesNum(void);
	size_type CompressPoints(bool ForceCompression = false);
	size_type CompressOrig(bool ForceCompression);
	size_type CompressFacesOrig(bool ForceCompression = false);
	size_type AddOrigSurf(SOrigSurf * pSurf);
	size_type AddOrigSurfInside(SOrigSurf * pSurf);
	SOrigSurf * GetOrigSurf(size_type index) const;
	size_type RemoveMark(void);
	void Init(void);
	void DeleteTree(size_type i);
	size_type CalcNormal(size_type i1, size_type i2, size_type i3, double sign);
	size_type CalcNormalSafe(size_type i1, size_type i2, size_type i3, double sign);
	size_type CalcNormal(size_type i1, size_type i2, size_type i3, size_type i4, double sign);
	MFace * CalcFace(size_type i1, size_type i2, size_type i3, size_type i4, double sign);
	void SetIDProc(MElemIDProc * pIDP);
	MElemIDProc * GetIDProc(void);
	CString GetInfoStr(void);
	void Compress(bool ForceCompression = false);
	void Transform1Tree(int TreeNum, const BMatr &M);
	void TransformSelected(NTransfData &TransfData, const BMatr &M);
	void DebugPrintSubtree(const MBSPNode* pV) const;
	void DebugPrint(const MBSPNode* pV) const;
	ForestSizes GetSizes() const;
};

