#include "stdafx.h"
#include "Octree.h"
#include "MRay.h"
#include "NCMDoc.h"
#include <limits>

using namespace std;

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

using namespace std;

template <class IDType>
Octree<IDType>::OctreeNode::OctreeNode(const BPoint& center, double halfWidth[3]) : isLeaf(true)
{
	this->center = center;
	for (int i = 0; i < 3; i++)
	{
		this->halfWidth[i] = halfWidth[i];
	}
	for (int i = 0; i < 8; i++)
	{
		childNodes[i] = nullptr;
	}
}

template <class IDType>
Octree<IDType>::OctreeNode::~OctreeNode()
{
	for (int i = 0; i < 8; i++)
		delete childNodes[i];
}

template <class IDType>
bool Octree<IDType>::OctreeNode::IsEmpty() const
{
	return objects.empty();
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


template <class IDType>
Octree<IDType>::Octree() : root(nullptr), pDoc(nullptr)
{
}

template <class IDType>
Octree<IDType>::~Octree()
{
	delete root;
}

template <class IDType>
void Octree<IDType>::Initialize(BBox& AABB, class CNCMDoc *pDoc)
{
	this->pDoc = pDoc;
	delete root; // recursively clear tree
	AABB.ApplyLimits();
	this->sceneAABB = AABB;

	double halfWidth[3];
	halfWidth[0] = sceneAABB.dX() / 2.0;
	halfWidth[1] = sceneAABB.dY() / 2.0;
	halfWidth[2] = sceneAABB.dZ() / 2.0;

	this->root = new OctreeNode(sceneAABB.GetCenterPoint(), halfWidth);
}

template <class IDType>
void Octree<IDType>::InsertObject(IDType& objectsCadrID, const BBox &AABB)
{
	if (AABB.IsDefined())
		this->InsertObjectInNode(root, objectsCadrID, AABB);
}

template <class IDType>
void Octree<IDType>::InsertObjectInNode(OctreeNode* node, IDType& objectsCadrID, const BBox& AABB)
{	
	bool canInsertInChild = true;
	double childHalfWidth[3];
	for (int i = 0; i < 3; i++)
	{
		// check for limit side size
		if ((childHalfWidth[i] = node->halfWidth[i] / 2.0) < MinWidth)
			canInsertInChild = false;
	}
	if (canInsertInChild)
	{
		int index = 0;
		if (this->IsFullInChild(node, AABB, index))
		{
			if (node->childNodes[index] == nullptr)
			{
				BPoint childOffcet((index & 1) ? childHalfWidth[0] : -childHalfWidth[0],
					(index & 2) ? childHalfWidth[1] : -childHalfWidth[1],
					(index & 4) ? childHalfWidth[2] : -childHalfWidth[2],
					0);
				node->childNodes[index] = new OctreeNode(node->center + childOffcet, childHalfWidth);
				node->isLeaf = false;
			}
			this->InsertObjectInNode(node->childNodes[index], objectsCadrID, AABB);
			return;
		}
	}	
	node->objects.push_front(objectsCadrID);
}

template <class IDType>
bool Octree<IDType>::IsFullInChild(OctreeNode* node, const BBox& AABB, int& index) const
{
	double AABBWidths[3];
	AABBWidths[0] = AABB.dX();
	AABBWidths[1] = AABB.dY();
	AABBWidths[2] = AABB.dZ();
	BPoint CenterPoint = AABB.GetCenterPoint();

	for (int i = 0; i < 3; i++)
	{
		double delta = CenterPoint[i] - node->center[i];
		if (2. * fabs(delta) < AABBWidths[i])
			return false;
		if (delta > 0.0)
			index |= (1 << i);
	}
	return true;
}

template <class IDType>
IDType Octree<IDType>::RayCast(const BPoint& P, const BPoint& V, double Epsilon, BPoint& res, double MaxL2, const bool &StopCast, int StPos)
{
	BPoint normal; // may be useful?
	BPoint sceneIntersectionPoint = sceneAABB.RayCasting(P, V, Epsilon, normal);
	IDType ID;
	ID.SetEmpty();
	if (sceneIntersectionPoint.GetH() < 0.0) // no intersection with scene AABB
		return ID;
	double currentMinT = MaxL2; // squared max distanse
	ID = RayCastInNode(root, sceneIntersectionPoint, currentMinT, P, V, Epsilon, StopCast, StPos);

	if (!ID.IsEmpty())
		res = P + V * sqrt(currentMinT);
	return ID;
}

template <class IDType>
IDType Octree<IDType>::RayCastInNode(OctreeNode* node, const BPoint &nodeIntersection, double& currentMinT, const BPoint& P, const BPoint& V, double Epsilon, const bool &StopCast, int StPos)
{
	IDType ID;
	ID.SetEmpty();
	if (StopCast)
		return ID;
	if (node == nullptr)
		return ID;
	for (auto i = node->objects.begin(); i != node->objects.end(); i++)
	{
		const NCadrGeom *pCadr = GetCadrGeom(*i, StPos);
		if (pCadr != nullptr) // Cadr is visible
		{	
			NCadrGeom cCadr(*pCadr);
			cCadr.ApplyMatr();
			BPoint intersectionPoint = cCadr.RayCast(P, V, Epsilon);
			if (intersectionPoint.GetH() > 0.0)
			{
				double dist = BPoint::Dist2(intersectionPoint, P); // squared dist!
				if (dist < currentMinT)
				{
					currentMinT = dist;
					ID = *i;
				}
			}	
		}
	}
	if (node->isLeaf)
		return ID;
	int index = GetChildIndexByPoint(node, nodeIntersection);
	BPoint Pn(nodeIntersection);
	do
	{
		IDType newID;
		if (!(newID = this->RayCastInNode(node->childNodes[index], Pn, currentMinT, P, V, Epsilon, StopCast, StPos)).IsEmpty())
		{
			ID = newID;
			return ID;
		}
		index = GetNextChildIndexByVector(node, index, Pn, V); // FIND NEW INDEX
	} while (index >= 0 && BPoint::Dist2(P, Pn) < currentMinT);
	return ID;
}

template <class IDType>
int Octree<IDType>::GetChildIndexByPoint(OctreeNode* node, const BPoint& point)
{
	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		if (point.GetArray()[i] - node->center[i] > 0.0)
			index |= (1 << i);
	}
	return index;
}

template <class IDType>
int Octree<IDType>::GetNextChildIndexByVector(OctreeNode* node, int currentIndex, BPoint& P, const BPoint& V)
{
	int currentDir = 0;
	double minCoord = 0.0;
	double M = 0.0;
	double dist = (numeric_limits<double>::max)();
	int currentCoordPos = 0;
	unsigned short neededCoord = 0;
	double VD = sqrt(V.D2());
	for (int i = 0; i < 3; i++)
	{
		int iDir = fabs(V.GetCoord(i)) < numeric_limits<double>::epsilon() ? 0 : (V.GetCoord(i) > 0 ? 1 : -1);
		if (iDir == 0)
			continue;
		int coordPos = currentIndex & (1 << i);
		minCoord = node->center[i] - node->halfWidth[i] * (coordPos ? 0 : 1);
		M = iDir > 0 ? (minCoord + node->halfWidth[i]) - P.GetCoord(i) : P.GetCoord(i) - minCoord;
		double res = fabs(M * VD / V.GetCoord(i));
		if (res < dist)
		{
			dist = res;
			currentDir = iDir;
			currentCoordPos = coordPos;
			neededCoord = i;
		}
	}
	P = P + V * dist;
	if (currentDir > 0)
	{
		if (currentCoordPos)
			currentIndex = -1;
		else
			currentIndex += 1 << neededCoord;
	}
	else
	{
		if (currentCoordPos)
			currentIndex -= 1 << neededCoord;
		else
			currentIndex = -1;
	}
	return currentIndex;
}

template <class IDType>
BBox Octree<IDType>::GetSceneOverallDimensions(CMapStringToNProgram *pPList)
{
	BBox Res;
	for (int j = 0; j < pPList->GetSize(); ++j)
	{
		CString Key;
		NProgram *pProg = nullptr;
		pPList->GetAt(j, Key, (CObject *&)pProg);
		if (pProg)
			Res.Expand(pProg->GetGabar());
	}
	return Res;
}

