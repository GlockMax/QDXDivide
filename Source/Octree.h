#pragma once
#include "BPoint.h"
#include "BBox.h"
#include "NProgram.h"
#include "CadrID.h"
#include "NCadrGeom.h"
#include <forward_list>

template <class IDType> class Octree
{
private:
	class OctreeNode
	{
		friend class Octree;
	protected:
		BPoint center;
		double halfWidth[3];
		OctreeNode *childNodes[8];	
		std::forward_list<IDType> objects;
		bool isLeaf;
	public:
		OctreeNode(const BPoint& center, double halfWidth[3]);
		~OctreeNode();
		bool IsEmpty() const;
	};
protected:
	OctreeNode* root;
	BBox sceneAABB;
	class CNCMDoc *pDoc;
public:
	Octree();
	~Octree();

	void Initialize(BBox& AABB, class CNCMDoc *pDoc);

	void InsertObject(IDType& objectsCadrID, const BBox &AABB);
	IDType RayCast(const BPoint& P, const BPoint& V, double Epsilon, BPoint& res, double MaxL2, const bool &StopCast, int StPos);
private:
	const double MinWidth = 1.;
	void InsertObjectInNode(OctreeNode* node, IDType& objectsCadrID, const BBox& AABB);

	bool IsFullInChild(OctreeNode* node, const BBox& AABB, int& index) const;
	int GetChildIndexByPoint(OctreeNode* node, const BPoint& point);
	int GetNextChildIndexByVector(OctreeNode* node, int currentIndex, BPoint& P, const BPoint& V);
	IDType RayCastInNode(OctreeNode* node, const BPoint &p, double& currentMinT, const BPoint& P, const BPoint& V, double Epsilon, const bool &StopCast, int StPos);
protected:
	virtual const NCadrGeom *GetCadrGeom(IDType ID, int StPos) = 0;

	static BBox GetSceneOverallDimensions(class CMapStringToNProgram *pPList);
};
#include "Octree.cpp"

