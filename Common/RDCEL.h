#pragma once
#include "OPENGLRENDER_API.h"
#include "BDCEL.h"
#include <vector>
#include "BPoint.h"

class OPENGLRENDER_API RDCELBase
{// base class needed for Render only
public:
	RDCELBase();
	RDCELBase(const RDCELBase& In);

	void Render(bool SharpEdges, bool MultiColor = false);
protected:
	std::vector<GLfloat> Points;
	int* Indexes;
	int FacesCount;
	int Stride;
};

class OPENGLRENDER_API RDCEL : public RDCELBase
{
	friend class StlRend;
public:
	RDCEL(class BDCEL &DCEL);
	~RDCEL(void);
	virtual bool Close(void);
	int GetVertexesCount() const { return int(Vertexes.size()); }
	int GetPointsCount() const { return PNIndex; }
	const GLfloat *GetVertex(int i) const;
	GLfloat *GetPoint(int i);
	int GetFacesCount() const { return FacesCount; }
	int GetIndex(int i) const { return Indexes[i]; }
	bool GetFaceVertexes(int FaceNum, int &i0, int &i1, int &i2) const;
private:
	void Creating(void);
	BPoint GetNormByEdges(int EdgeOne, int EdgeTwo) const;
	int GetExpPointsCount(void);
	void FillSegment(int EdgeStart, int EdgeEnd, int VertexIndex, BPoint Normal);
	int AddVertexAndNormal(int VertexIndex, BPoint Normal);
	int FindIndexInFace(int Vertex, int Face) const;
	void Repair(int BadIndex);

	const BDCEL *DCEL;
	int PNIndex;
	int PointsCount;

	const std::vector<Edge> &Edges;
	const std::vector<Vertex> &Vertexes;
	const std::vector<Face> &Faces;
};

