#include "StdAfx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "NObjEnums.h"
#include "NxyzImage.h"
#include "BGeomArray.h"
#include "BDCEL.h"
#include "RDCEL.h"
#include "NColor.h"
#include "NTiParams.h"
#include "BaseRender.h"
#include "BEdgesAttrs.h"
#include "GrazingCurveElemContour.h"
#include "DXFOut.h"
#include "MBSPCell.h"
#include "RRopes.h"
#include "BStl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BStl::BStl(void) : BStlB()
{
	SetDefault();
	pRotSolid = nullptr;
}

BStl::~BStl(void)
{
	ClearRender();
	delete pRotSolid;
}

void BStl::SetDefault()
{
	EdgesWidth = 2.;
	SharpEdges = true;
	SmoothSurf = true;
	pDCELRender = nullptr;
	SharpInd = nullptr;
	RMode = RM_SHADE;
	EdgesColor = NColor(0., 0., 0., 1.);
}

void BStl::Clear()
{
	BStlB::Clear();
	ClearRender();
	DCEL.Clear();
	SetDefault();
}

void BStl::ClearRender()
{
	delete pDCELRender;
	pDCELRender = nullptr;
	delete[] SharpInd;
	SharpInd = nullptr;
}

bool BStl::Create()
{
	DCEL.Clear();
	if(!BStlB::Create())
		return false;
	return true;
}

void BStl::MakeCopy(BStl &object) const
{
	BStlB::MakeCopy(object);

	object.SharpEdges = this->SharpEdges;
	object.SmoothSurf = this->SmoothSurf;
	object.RMode = this->RMode;
	object.EdgesColor = this->EdgesColor;
	object.EdgesWidth = this->EdgesWidth;
}

bool BStl::MakeRDCEL()
{
	if (!DCEL.IsCreated())
	{
		DCEL.MakeFromStl(*this);
		delete pDCELRender;
		pDCELRender = nullptr;
	}
	if(pDCELRender == nullptr)
		pDCELRender = new RDCEL(DCEL);
	return true;
}

bool BStl::Is1Connected()
{
	if (!GetDCEL().IsCreated())
		GetDCEL().MakeFromStl(*this);
	bool Res = false;
	int ExtremeEdge = DCEL.FindUnmarkedEdge();
	if (ExtremeEdge == -1)
		Res = true;
	if (!Res)
	{
		DCEL.MarkConnected(ExtremeEdge);
		ExtremeEdge = DCEL.FindUnmarkedEdge();
		DCEL.UnmarkAll();
		if (ExtremeEdge == -1)
			Res = true;
	}
	return Res;
}

void BStl::DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool WFrame, bool DrawSharpEdges,  bool DrawSurf,  bool DrawSmoothSurf)
{
	if(Triangles == NULL)
		return;

	RenderMode ActRMode = (ExtRMode == RM_UNDEF) ? GetRMode() : ExtRMode;

	if(GlobRMode == RM_TRANSLUCENT && ActRMode != RM_TRANSLUCENT) 
			return;
	if(GlobRMode != RM_TRANSLUCENT && ActRMode == RM_TRANSLUCENT)
			return;

	if(WFrame)
		BaseRender::SetWFMode();
	else 
		BaseRender::SetShader(ActRMode);

	bool SmoothSurfLocal = (SmoothSurf && DrawSmoothSurf);
	bool SharpEdgesLocal = (SharpEdges && DrawSharpEdges);

	if(SmoothSurfLocal || SharpEdgesLocal)
	{
		if(!DCEL.IsCreated())
			DCEL.MakeFromStl(*this);
	}
	if(SmoothSurfLocal && DrawSurf)
	{
		if(pDCELRender == NULL)
		{
			pDCELRender = new RDCEL(DCEL);
		}
		pDCELRender->Render(SharpEdgesLocal);
	}
	if(!SmoothSurfLocal || SharpEdgesLocal)
	{
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		if(SharpEdgesLocal)
		{
			glPushAttrib(GL_ENABLE_BIT);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1., 1.);
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glInterleavedArrays(GL_N3F_V3F, 0, Triangles + 1);
		
		if(!SmoothSurfLocal && DrawSurf)
		{
			GLsizei Count = min(3 * long(Triangles[0]), 64000000);
			glDrawArrays(GL_TRIANGLES, 0, Count);
		}
		if (SharpEdgesLocal)
			glPopAttrib();
		// Draw sharp edges
		if(SharpEdgesLocal)
		{
			if(SharpInd == NULL)
				SharpInd = DCEL.FillEdges(true, false, false);
			if(SharpInd != NULL && EdgesWidth > 0.01)
			{
				if (!WFrame)
					BaseRender::SetWFMode();
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_LINE_BIT);
				glLineWidth(float(EdgesWidth));
				glColor3fv(EdgesColor.array);
                glPushName(OG_SKELETON);
				glDrawElements(GL_LINES, SharpInd[0] - 1, GL_UNSIGNED_INT, SharpInd + 1);
                glPopName();
				glPopAttrib();
				if (!WFrame)
					BaseRender::RemoveWFMode();
			}
		}

		glPopClientAttrib();
	}
	// Draw RotSolid
	//if (pRotSolid != nullptr)
	//{
	//	glColor3f(1., 0.5, 0.5);
	//	glLineWidth(3);
	//	glBegin(GL_LINES);
	//	for (int i = 0; i < pRotSolid->out.size(); i++)
	//	{
	//		glVertex3d(pRotSolid->out[i][0].GetX(), pRotSolid->out[i][0].GetY(), pRotSolid->out[i][0].GetZ());
	//		glVertex3d(pRotSolid->out[i][1].GetX(), pRotSolid->out[i][1].GetY(), pRotSolid->out[i][1].GetZ());
	//	}
	//	glEnd();
	//	glLineWidth(2);

	//	int sm = 300;
	//	int smm = 400;
	//	glColor3f(0, 0, 1.0f);
	//	int RotNum = 300;

	//	////отрисовка	
	//	glColor3f(0.0f, 0, 1.0f);
	//	for (int i = 0; i < pRotSolid->sled.size(); i++)
	//	{
	//		glBegin(GL_LINES);
	//		for (int j = 0; j < pRotSolid->sled[i].size() - 1;)
	//		{
	//			glVertex3d(pRotSolid->AllPoints.at(pRotSolid->sled[i][j]).GetX(), pRotSolid->AllPoints.at(pRotSolid->sled[i][j]).GetY() - sm, pRotSolid->AllPoints.at(pRotSolid->sled[i][j]).GetZ());
	//			++j;
	//			glVertex3d(pRotSolid->AllPoints.at(pRotSolid->sled[i][j]).GetX(), pRotSolid->AllPoints.at(pRotSolid->sled[i][j]).GetY() - sm, pRotSolid->AllPoints.at(pRotSolid->sled[i][j]).GetZ());
	//		}
	//		glEnd();
	//	}

	//	glColor3f(1.0f, 0, 0);
	//	glBegin(GL_LINES);
	//	for (int i = 0; i < pRotSolid->Lines.size(); i++)
	//	{
	//		glVertex3d(pRotSolid->AllPoints[pRotSolid->Lines[i].start].GetX(), pRotSolid->AllPoints[pRotSolid->Lines[i].start].GetY() + sm, pRotSolid->AllPoints[pRotSolid->Lines[i].start].GetZ());
	//		glVertex3d(pRotSolid->AllPoints[pRotSolid->Lines[i].end].GetX(), pRotSolid->AllPoints[pRotSolid->Lines[i].end].GetY() + sm, pRotSolid->AllPoints[pRotSolid->Lines[i].end].GetZ());
	//	}
	//	glEnd();
	//}

// End:Draw RotSolid
	if(WFrame)
		BaseRender::RemoveWFMode();
}

void BStl::Transform(const BMatr &M)
{
	BStlB::Transform(M);
	delete pDCELRender;
	pDCELRender = NULL;
}

void BStl::WriteSTL(CStdioFile & f)
{
	if(DCEL.IsCreated())
		DCEL.WriteSTL(f);
	else
		BStlB::WriteSTL(f);
}

int BStl::WriteSTLmulti(std::vector<CString>& FileNames)
{// FileNames[0] - start name
	//returns the number of files saved
	if (FileNames.empty())
		return 0;

	if (!DCEL.IsCreated())
		DCEL.MakeFromStl(*this);

	CString OldName = FileNames.front();
	OldName.Delete(OldName.GetLength() - 4, 4);
	constexpr int MaxInd = 1000;
	FileNames.clear();
	CStdioFile f;
	int LastFileIndex = 0;
	bool Error = false;
	for (int FileN = 0; FileN < MaxInd && !Error; ++FileN)
	{
		int ExtremeEdge = -1;
		if ((ExtremeEdge = DCEL.FindUnmarkedEdge()) < 0)
			break;
		// Make new name
		CString NewName;
		for (; LastFileIndex < MaxInd; ++LastFileIndex)
		{
			NewName.Format(_T("_%d"), LastFileIndex);
			NewName = OldName + NewName + _T(".STL");
			if (!f.Open(NewName, CFile::modeRead))
			{
				if (!f.Open(NewName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
					Error = true;
				break;
			}
			f.Close();
		}
		if (LastFileIndex == MaxInd)
			Error = true;
		++LastFileIndex;
		// Save 1 file
		if (!Error)
		{
			if(DCEL.WriteSTLmOneSolid(f, ExtremeEdge))
				FileNames.push_back(NewName);
			f.Close();
		}
	}
	DCEL.UnmarkAll();
	return Error ? -int(FileNames.size()) : int(FileNames.size());
}

void BStl::DrawSelected(void) const
{
	BaseRender::DrawBox(Gabar);
	glPushMatrix();
	BPoint Shift(GetOrigZero());
	glTranslated(Shift.GetX(), Shift.GetY(), Shift.GetZ());
	NxyzImage BaseXYZ;
	BaseXYZ.SetParam(TRUE, FALSE);
	BaseXYZ.SetSizes(5, 5, 5, 5, 0);
	BaseXYZ.Draw(false);
	glPopMatrix();
}

void BStl::SetSharpEdges(bool SEdges)
{
	SharpEdges = SEdges;
}

void BStl::SetSmoothSurf(bool SSurf)
{
	if(SmoothSurf == SSurf)
		return;
	SmoothSurf = SSurf;
	ClearRender();
}

void BStl::SetSmoothAngle(double Angle)
{
	DCEL.SetSmoothAngle(Angle);
	ClearRender();
}

void BStl::SetEdgesAttrs(const BEdgesAttrs & Attr)
{
	SetSmoothAngle(Attr.GetSmoothAngle());
	SetEdgesWidth(Attr.GetEdgesWidth());
	SetEdgesColor(Attr.GetEdgesColor());
}

void BStl::Serialize(CArchive &ar, double InFileVersion)
{
	BStlB::Serialize(ar, InFileVersion);
	if(!(ar.IsLoading() && InFileVersion < atof("3.9")))
	{
		SerializeElements(ar, &SharpEdges, 1);
		SerializeElements(ar, &SmoothSurf, 1);
		SerializeElements(ar, &RMode, 1);
		SerializeElements(ar, &EdgesWidth, 1);
		EdgesColor.Serialize(ar);
	}
}

bool BStl::FindSegments(SnapRopes& oSegments)
{
    if(Triangles == nullptr)
		return false;

	MBSPCell BSPCell(0, 0, Gabar.GetMinPoint().GetX(), Gabar.GetMaxPoint().GetX(), Gabar.GetMinPoint().GetY(), Gabar.GetMaxPoint().GetY());
	BSPCell.FillFromBDCEL(DCEL, true, false);
	DCEL.UnmarkAll();
	RRopes NewRopes;
	if (!NewRopes.CreateFromLines(BSPCell.LinesDCEL))
		return false;
	NewRopes.GetSegments(oSegments);

    return true;
}

int BStl::SplitIntoSolids(std::vector<BStl>& Solids)
{
	// TODO: Add your implementation code here.
	return 0;
}

bool BStl::WriteRotCont(bool IgnoreVoids, bool DoOrient)
{
	if (DoOrient)
	{
		if (!GetDCEL().IsCreated())
			GetDCEL().MakeFromStl(*this);
	}
	else
		GetDCEL().MakeFromSeqTri(GetTriangles(), false);

	delete pRotSolid;
	pRotSolid = new BRotSolid;
	bool Res = false;
	BClrContour Contour;
	if (pRotSolid->CrRotKontour(GetDCEL(), IgnoreVoids, Contour))
	{
		BGeomArray GeomArray;
		pRotSolid->Contour2GeomArray(Contour, GeomArray);
		Contour.clear();
		CString FileName(GetFileName());
		int Ind = FileName.ReverseFind('.');
		FileName.Delete(Ind, FileName.GetLength());
		DXFOut Export(&GeomArray, FileName);
		Res = true;
	}
	if (!DoOrient)
		GetDCEL().Clear();
	return Res;
}

bool BStl::MakeSpinContours(const BMatr& RelMatr, std::list<BClrContour>& Contours)
{
	if (Triangles == nullptr)
		return false;

	BBox bGabar = Gabar;
	size_t TriSize = 1 + size_t(Triangles[0]) * 3 * 6;
	float* bufTri = new float[TriSize];
	memcpy_s(bufTri, TriSize * sizeof(Triangles[0]), Triangles, TriSize * sizeof(Triangles[0]));
	Transform(RelMatr);

	BDCEL_STL& DCEL = GetDCEL();
	if (!DCEL.IsCreated())
		DCEL.MakeFromStl(*this);

	bool Res = BRotSolid::MakeEnvConts(DCEL, Contours, TGM_FULL);

	memcpy_s(Triangles, TriSize * sizeof(Triangles[0]), bufTri, TriSize * sizeof(Triangles[0]));
	delete[] bufTri;
	Gabar = bGabar;
	return true;
}
