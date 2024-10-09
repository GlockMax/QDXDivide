#pragma once

#include <list>
#include "src\DL_CreationAdapter.h"


class N_dl_import : public DL_CreationAdapter {
public:
	N_dl_import();

	void addLine(const DL_LineData& data) override;
	void addArc(const DL_ArcData& data) override;
	void addPolyline(const DL_PolylineData&) override;
	void addVertex(const DL_VertexData&) override;
	void endSequence() override;
	std::list<NCadrGeom>& GetContCadrs() { return ContCadrs; }
	bool GetPolylinePresent() const { return PolylinePresent; }
	int GetErrorCode() const { return ErrorCode; }
protected:
	std::list<NCadrGeom> ContCadrs;
	bool PolylineFinished;
	bool PolylinePresent;
	bool LineArcPresent;
	int ErrorCode; 
	BPoint PrevPoint;
};

