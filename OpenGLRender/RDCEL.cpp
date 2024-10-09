#pragma once
#include "stdafx.h"
#include "GLee.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "NColor.h"
#include "Math.h"
#include "RDCEL.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif


RDCEL::RDCEL(BDCEL &DCEL) : Vertexes(DCEL.GetVertexes()), Edges(DCEL.GetEdges()), Faces(DCEL.GetFaces())
{
	// &DCEL is not const reference because BDCEL::DrawEdges is not const, and i can't invoke it
	this->DCEL = &DCEL;
//	Points = NULL;
	Indexes = NULL;
	PNIndex = 0;
	PointsCount = 0;

	FacesCount = this->DCEL->GetFacesCount();
	Stride = this->DCEL->GetSVS();
	
	Creating();
//	this->DCEL = nullptr;// Don't use DCEL after constructor finished
}

RDCEL::~RDCEL(void)
{
//	delete[] Points;
	delete[] Indexes;
}

void RDCEL::Repair(int BadIndex)
{
	// repair pair point:normal in BadIndex
	int FaceIndex = (int)(BadIndex / 3);
	int EdgeShift = BadIndex % 3;
	int Edge = Faces[FaceIndex][0];
	for (int i = 0; i < EdgeShift; ++i)
		Edge = Edges[Edge][1];
	BPoint Normal = GetNormByEdges(Edge, Edges[Edge][1]);
	int VN = AddVertexAndNormal(Edges[Edge][0], Normal);
	Indexes[BadIndex] = VN;
}

bool RDCEL::GetFaceVertexes(int FaceNum, int & i0, int & i1, int & i2) const
{
	int Edge = Faces[FaceNum][0];
	i0 = Edges[Edge][0];
	Edge = Edges[Edge][1];
	i1 = Edges[Edge][0];
	Edge = Edges[Edge][1];
	i2 = Edges[Edge][0];
	return true;
}

void RDCEL::Creating(void)
{
	PointsCount = GetExpPointsCount();
	if(PointsCount <= 0)
		return;
	Points.reserve(PointsCount * Stride);
	Indexes = new int[FacesCount * 3];
	for (int i = 0; i < FacesCount * 3; ++i)
		Indexes[i] = -1;

	for (int i = 0; i < GetVertexesCount(); ++i)
	{
		int StartEdge = Vertexes[i][0];
		if(StartEdge < 0)
			continue;

		//************
		// if segment is not closed, StartEdge will be initialized by boundary halfedge of segment
		bool Closed = true;
		int NextEdge = StartEdge;
		int CurrentEdge;
		do
		{
			CurrentEdge = NextEdge;
			NextEdge = DCEL->NextHalfEdge(CurrentEdge);
		} 
		while ((NextEdge != StartEdge)&&(NextEdge != -1));
		if (NextEdge == -1)
		{
			Closed = false;
			NextEdge = CurrentEdge;
			do
			{
				CurrentEdge = NextEdge;
				NextEdge = DCEL->NextHalfEdgeCCW(CurrentEdge);
			} 
			while (NextEdge != -1);
			StartEdge = CurrentEdge;
		}
		//************

		// calculate average normal and write pair point:normal
		// we have 4 situations
		// edges in segment may be sharp or not
		// segment may be closed or not

		CurrentEdge = StartEdge;
		bool Sharpness = false;
		NextEdge = DCEL->NextHalfEdge(CurrentEdge);

		BPoint Normal(0., 0., 0., 0.);
		Normal = Normal + GetNormByEdges(CurrentEdge, Edges[CurrentEdge][1]);
		while ((NextEdge != StartEdge) && (NextEdge != -1))
		{
			int Shift = Edges[CurrentEdge][3] == 1 ? 0 : -1;
			if (DCEL->IsSharpEdge(CurrentEdge + Shift))
			{
				Sharpness = true;
				break;
			}
			Normal = Normal + GetNormByEdges(NextEdge, Edges[NextEdge][1]);
			CurrentEdge = NextEdge;
			NextEdge = DCEL->NextHalfEdge(CurrentEdge);
		}

		if (!Sharpness)
		{
			if (!Closed)
				// segment has not sharp edges, and is not closed
				FillSegment (StartEdge, -1, i, Normal);
			else
				// segment is closed without sharp edges
				FillSegment(StartEdge, StartEdge, i, Normal);
		}
		else
		{
			if (!Closed)
			{
				// segment is not closed, fill between StartEdge and first sharp edge
				FillSegment(StartEdge, NextEdge, i, Normal);
			}
			Normal.SetX(0.);
			Normal.SetY(0.);
			Normal.SetZ(0.);
			Normal.SetH(0.);
			// StartEdge is initialize by first sharp edge
			int ShEdge = StartEdge = NextEdge;
			CurrentEdge = StartEdge;
			NextEdge = DCEL->NextHalfEdge(CurrentEdge);

			Normal = Normal + GetNormByEdges(CurrentEdge, Edges[CurrentEdge][1]);

			while ((NextEdge != StartEdge) && (NextEdge != -1))
			{
				int Shift = Edges[CurrentEdge][3] == 1 ? 0 : -1;
				if (DCEL->IsSharpEdge(CurrentEdge + Shift))
				{
					// segment between two sharp edges
					FillSegment(ShEdge, NextEdge, i, Normal);
					ShEdge = NextEdge;
					Normal.SetX(0.);
					Normal.SetY(0.);
					Normal.SetZ(0.);
					Normal.SetH(0.);
				}
				Normal = Normal + GetNormByEdges(NextEdge, Edges[NextEdge][1]);
				CurrentEdge = NextEdge;
				NextEdge = DCEL->NextHalfEdge(CurrentEdge);
			}
			if (NextEdge == -1)
				// segment is not closed, fill between last sharp edge and end of segment
				FillSegment(ShEdge, -1, i, Normal);
			else
				// segment is closed, fill between last sharp edge and first sharp edge
				FillSegment(ShEdge, StartEdge, i, Normal);
		}
	}
	// find bad index and repair pair point:normal in this index
	for (int i = 0; i < FacesCount * 3; ++i)
		if (Indexes[i] == -1) Repair(i);
}

bool RDCEL::Close(void)
{
	return true;
}

BPoint RDCEL::GetNormByEdges(int EdgeOne, int EdgeTwo) const
{
	BPoint Normal(0., 0., 0., 0.);
	if (EdgeOne == -1 || EdgeTwo == -1)
		return Normal;
	BPoint VectEdgeOne = DCEL->GetEdgeVec(EdgeOne);
	BPoint VectEdgeTwo = DCEL->GetEdgeVec(EdgeTwo);
	Normal = VectEdgeOne % VectEdgeTwo;

	if (Normal.D2() < 1e-8)
		return Normal;
	Normal = Normal * (1 / sqrt(Normal.D2()));
	return Normal;
}

int RDCEL::GetExpPointsCount(void)
{
	// return expected number of points
	int result = DCEL->GetFacesCount() * 3;
	int StartEdge, CurrentEdge, NextEdge, count;
	bool Sharpness = true;
	int Shift;
	for (int i = 0; i < GetVertexesCount(); ++i)
	{
		StartEdge = Vertexes[i][0];
		if(StartEdge < 0)
			continue;
		//************
		// if segment is not closed, StartEdge will be initialized by boundary halfedge of segment
		NextEdge = StartEdge;
		do
		{
			CurrentEdge = NextEdge;
			NextEdge = DCEL->NextHalfEdge(CurrentEdge);
		} while ((NextEdge != StartEdge)&&(NextEdge != -1));
		if (NextEdge == -1)
		{
			NextEdge = CurrentEdge;
			do
			{
				CurrentEdge = NextEdge;
				NextEdge = DCEL->NextHalfEdgeCCW(CurrentEdge);
			} while (NextEdge != -1);
			StartEdge = CurrentEdge;
		}
		CurrentEdge = StartEdge;
		count = 0;
		bool HaveSharp = false;
		do
		{
			if (!DCEL->IsBorderEdge(CurrentEdge))
			{
				Edges[CurrentEdge][3] == 1 ? Shift = 0 : Shift = -1;
				if (!DCEL->IsSharpEdge(CurrentEdge + Shift))
					++count;
				else
					HaveSharp = true;
			}
			else
				HaveSharp = true;
			CurrentEdge = DCEL->NextHalfEdge(CurrentEdge);
		} while (CurrentEdge != StartEdge && CurrentEdge != -1);
		result -= count - (HaveSharp ? 0 : 1);
	}
	return result;

}

int RDCEL::FindIndexInFace(int Vertex, int Face) const
{
	// return Vertex index in Face
	int Edge = Faces[Face][0];
	int Current = Vertexes[Vertex][2];
	if(Current == Vertexes[ Edges[Edge][0] ][2])
		return 0;
	Edge = Edges[Edge][1];
	if(Current == Vertexes[ Edges[Edge][0] ][2])
		return 1;
	return 2;
}

void RDCEL::FillSegment(int EdgeStart, int EdgeEnd,int VertexIndex, BPoint Normal)
{
	// write pair point:normal in Faces which is found between EdgeStart and EdgeEnd
	if (Normal.D2() > 1e-8)
		Normal = Normal * (1 / sqrt(Normal.D2()));
	int IndexVN = AddVertexAndNormal(VertexIndex, Normal);
	int Edge = EdgeStart;
	do 
	{
		int FaceIndex = Edges[Edge][2];
		int Ind = 3 * FaceIndex + FindIndexInFace(VertexIndex, FaceIndex);
		Indexes[Ind] = IndexVN;
		Edge = DCEL->NextHalfEdge(Edge);
	} while (Edge != EdgeEnd);
}

int RDCEL::AddVertexAndNormal(int VertexIndex, BPoint Normal)
{
	// write pair point:normal and return index
	int PointNumber = Vertexes[VertexIndex][2];
	auto Base = DCEL->GetBasePtrByNum(PointNumber);
	Points.resize((PNIndex + 1) * Stride);
	
	Points[Stride * PNIndex + 0] = GLfloat(Normal.GetX());
	Points[Stride * PNIndex + 1] = GLfloat(Normal.GetY());
	Points[Stride * PNIndex + 2] = GLfloat(Normal.GetZ());
	for (int k = 3; k < Stride; ++k)
		Points[Stride * PNIndex + k] = Base[k];
	return PNIndex++;
}

const GLfloat *RDCEL::GetVertex(int i) const
{
	return DCEL->GetPointPtrByStorNum(Vertexes[i][2]);
}

GLfloat *RDCEL::GetPoint(int i)
{
	return Points.data() + i * Stride;
}

void RDCELBase::Render(bool SharpEdges, bool MultiColor /* = false*/)
{
	if (Indexes == NULL)
		return;
	if (SharpEdges)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1., 1.);
	}
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (MultiColor)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, Stride * sizeof(Points[0]), Points.data() + 3 + 3);
	}
	else
		glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, Stride * sizeof(Points[0]), Points.data() + 3);
	glNormalPointer(GL_FLOAT, Stride * sizeof(Points[0]), Points.data());
	glDrawElements(GL_TRIANGLES, 3 * FacesCount, GL_UNSIGNED_INT, Indexes);
	glPopClientAttrib();
	if (SharpEdges)
	{
		glPopAttrib();
	}
}

RDCELBase::RDCELBase()
{
}

RDCELBase::RDCELBase(const RDCELBase& In)
{
	Points = In.Points;
	FacesCount = In.FacesCount;
	Stride = In.Stride;
	Indexes = new int[FacesCount * 3];
	memcpy_s(Indexes, FacesCount * 3 * sizeof(int), In.Indexes, FacesCount * 3 * sizeof(int));
}

