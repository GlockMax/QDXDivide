#pragma once
#include "COMPARE_API.h"
#include "DimObject.h"
#include "NCadrGeom.h"

class AFX_EXT_CLASS DimObjectStock: public DimObject
{
public:
	DECLARE_SERIAL(DimObjectStock);
    
	DimObjectStock(const BPoint &Min, const BPoint &Max, float Width);
	DimObjectStock() {};
    
    virtual ~DimObjectStock() {}
    
    void Change(const BPoint& iPoint, const BPoint& iViewDir) override;
	bool IsEqualTo(class DimObject* iDimObject) const override { return dynamic_cast<DimObjectStock*>(iDimObject) ? true : false; }
	void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const override { oPoint0 = PointMin; oPoint1 = PointMax; };
	DimObjects::DimObjectsType GetType() const override { return DimObjects::DimObjectStockType; }
	BBox GetGabar() const override;
	void Serialize(CArchive& ar) override;
	void SetEdgeID(int i, unsigned int EdgeID) { Edges[i] = EdgeID;}
	void GenEdges(class DimObjectSegmentEdge *Edges[12]);
	void ChangeByEdge(const BPoint& iPoint, const BPoint& iViewDir, const class DimObjectSegmentEdge *pDOSEObj);
	const BPoint &GetPointMin() const {return PointMin;}
	const BPoint &GetPointMax() const {return PointMax;}
	void SetPointMin(const BPoint &PMin) { PointMin = PMin;}
	void SetPointMax(const BPoint &PMax) { PointMax = PMax;}
protected:
	BPoint PointMin;
	BPoint PointMax;
	unsigned int Edges[12];
};