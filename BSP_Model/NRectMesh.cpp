#include "stdafx.h"
#include "BPoint.h"
#include "ConstDef.h"
#include "CopyMap.h"
#include "MFace.h"
#include "MBSPForest.h"
#include "MBody.h"
#include "SOrigSurf.h"
#include "Gl\GLu.h"
#include "math.h"
#include "BBox.h"
#include "DXSubModel5X.h"
#include "NMeshArray.h"
#include "NTriMesh.h"
#include "NRectMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NRectMesh::NRectMesh(void) : NParamMesh()
{
	m_IsDeg = 0;
	m_arr = nullptr;
	m_r = 0;
	m_Closed = false;
}

NRectMesh::~NRectMesh(void)
{
	NParamMesh::~NParamMesh();
	delete[] m_arr;
	m_arr = nullptr;
}

void NRectMesh::SetRowsNum(int r)
{
	if(m_arr)
		delete[] m_arr;
	m_arr = new CUIntArray[r];
	m_r = r;
}

void NRectMesh::SetSize(int r, int c)
{
	if(m_arr)
		delete[] m_arr;
	m_arr = new CUIntArray[r];
	for (int i=0; i<r; ++i)
		m_arr[i].SetSize(c);
	m_r = r;
}
CUIntArray* NRectMesh::operator [](int r)
{
#ifdef _DEBUG
	if (0 >= m_r || r < 0 || r >= m_r)
		return 0;
#endif
	return &m_arr[r];
}
void NRectMesh::Draw(bool is_point, bool view_num, bool view_norm)
{
#ifdef _DEBUG
	if (0 >= m_r)
		return;
#endif

	const BPoint *p;
	
	if (view_num && !view_norm)
	{
		//char s[10];
		//Text glText;
		//glColor3d(0.23f, 0.35f, 0.75f); 
		//glText.Init(wglGetCurrentDC(), USEFONTBITMAPS, "arial.ttf");
		//for (int i=0, j; i<2/*m_r*/; i++)
		//	for (j=0; j<m_arr[i].GetSize(); j++)
		//	{
		//		p = MFOREST.GetPoint(m_arr[i][j]);
		//		sprintf(s, "%d", m_arr[i][j]);
		//		glText.Draw3D(p->GetX(), p->GetY(), p->GetZ(), s);
		//	}
	}
	if (view_norm)
	{
		glColor3d(0.8f, 0.2f, 0.2f);
		BPoint b(0., 0., 0., 1.), c(0., 0., 0., 1.);
		BPoint a = *MFOREST.GetPoint(m_arr[0][0]);
		if (m_r > 1)
		{
			b = *MFOREST.GetPoint(m_arr[0][1]);
			c = *MFOREST.GetPoint(m_arr[1][1]);
		}
		else
		{
			b = *MFOREST.GetPoint(m_arr[0][1]);
			c = *MFOREST.GetPoint(m_arr[0][m_arr[0].GetSize()-1]);
		}
		b = (b-a)%(c-a);
		b = b*(1./sqrt(b.D2()));
		b = a+b*(m_sgn*1.3);
		glBegin(GL_LINES);
			glVertex3d(a.GetX(), a.GetY(), a.GetZ());
			glVertex3d(b.GetX(), b.GetY(), b.GetZ());
		glEnd();
		glBegin(GL_POINTS);
			glVertex3d(a.GetX(), a.GetY(), a.GetZ());
		glEnd();
		if (view_num)
		{
			//char s[10];
			//Text glText;
			//int tp = MFOREST.GetOrigSurf(m_iSurf)->GetType();
			//glColor3d(0.23f, 0.35f, 0.75f); 
			//glText.Init(wglGetCurrentDC(), USEFONTBITMAPS, "arial.ttf");
			//
			//sprintf(s, "%d:%d%s%s", m_CadrNum, m_arr[0][0], 
			//	(m_IsDeg == 1 ? "-": (m_IsDeg == 0 ? "+": "+-")),
			//	(tp == 0 ? "p": (tp == 1 ? "c" :"s")));
			//glText.Draw3D(b.GetX(), b.GetY(), b.GetZ(), s);
		}
	}
	if (is_point)
	{
		glColor3d(0.23f, 0.35f, 0.75f); 
		glBegin(GL_POINTS);
		for (int i=0; i<2; i++)
			for (int j=0; j<m_arr[i].GetSize(); j++)
			{
				p = MFOREST.GetPoint(m_arr[i][j]);
				glVertex3d(p->GetX(), p->GetY(), p->GetZ());
			}
		glEnd();
	}
	else
	{
		glColor3d(0.23f, 0.35f, 0.75f); 
		//for (int i=0, j; i<m_arr[0].GetSize(); i++)
		for (int i=0; i<m_arr[1].GetSize(); i++)
		{
			glBegin(GL_LINE_STRIP);
			for (int j=0; j<m_r; j++)
			{
				p = MFOREST.GetPoint(m_arr[j].GetAt(i));
				if (p)
					glVertex3d(p->GetX(), p->GetY(), p->GetZ());
			}
			glEnd();
		}
		for (int i=0; i<m_r; i++)
		{
			glBegin(GL_LINE_STRIP);
			for (int j=0; j<m_arr[i].GetSize(); j++)
			{
				p = MFOREST.GetPoint(m_arr[i].GetAt(j));
				if (p)
					glVertex3d(p->GetX(), p->GetY(), p->GetZ());
			}
			glEnd();
		}
	}
	glColor3d(0.f, 0.2f, 0.5f);
}
void NRectMesh::JoinEdge(const NRectMesh * pMesh)
{
	if(!pMesh)
		return;
// Add first row of the pMesh as a last row of "this"
// m_r == 1 сетка получена дла NearSmall элемента
	if(pMesh->m_r == 1)
	{
		for(int i = 0; i < m_r; ++i)
			m_arr[i].Add(pMesh->m_arr[0][0]);
	}
	else if(m_r == 1)
	{
		for(int i = 0; i < pMesh->m_r; ++i)
			pMesh->m_arr[i][0] = m_arr[0][m_arr[0].GetSize() - 1];
	}
	else
	{ // Ordinary
		for(int i = 0; i < m_r; ++i)
			m_arr[i].Add(pMesh->m_arr[i][0]);
	}
}
void NRectMesh::JoinEdgeRev(const NRectMesh * pMesh)
{
	if(!pMesh)
		return;
	for(int i = 0; i < pMesh->m_r; ++i)
		pMesh->m_arr[i][0] = m_arr[i][m_arr[i].GetSize() - 1];
}

float * NRectMesh::MakeTriangles(const BMatr * pMatr)
{
	if(m_r < 1)// Mesh is cleared
		return nullptr;

	float *Triangles;
	int TriNum = 0;

	int iMax = m_r;
	int jMax = int(m_arr[0].GetSize());
	int TriInd = 4;
	BPoint BP0(0., 0., 0., 1.), BP1(0., 0., 0., 1.), BP2(0., 0., 0., 1.);
	if( iMax == 1 )// Flat contour
	{
		if(jMax < 3)
			return nullptr; // Empty contour
		TriNum = jMax - 2;
		Triangles = new float[TriNum * 6 * 3 + 1];
		Triangles[0] = float(TriNum);
		const BPoint *pP0 = MFOREST.GetPoint(m_arr[0][0]);
		const BPoint *pP1 = MFOREST.GetPoint(m_arr[0][1]);
		if(pMatr)
		{
			BP0 = (*pP0) * (*pMatr); pP0 = &BP0; 
			BP1 = (*pP1) * (*pMatr); pP1 = &BP1; 
		}
		for(int j = 2; j < jMax; ++j)
		{
			const BPoint *pP2 = MFOREST.GetPoint(m_arr[0][j]);
			if(pMatr)
			{
				BP2 = (*pP2) * (*pMatr); pP2 = &BP2; 
			}

			Triangles[TriInd++] = float(pP0->GetX());
			Triangles[TriInd++] = float(pP0->GetY());
			Triangles[TriInd++] = float(pP0->GetZ());
			TriInd += 3;
			Triangles[TriInd++] = float(pP1->GetX());
			Triangles[TriInd++] = float(pP1->GetY());
			Triangles[TriInd++] = float(pP1->GetZ());
			TriInd += 3;
			Triangles[TriInd++] = float(pP2->GetX());
			Triangles[TriInd++] = float(pP2->GetY());
			Triangles[TriInd++] = float(pP2->GetZ());
			TriInd += 3;
			pP1 = pP2;
		}
	}
	else
	{
		TriNum = (iMax - 1) * (jMax - 1) * 2;
		if(m_IsDeg > 0)
			TriInd -= iMax - 1;
		if(m_IsDeg > 1)
			TriInd -= iMax - 1;

		Triangles = new float[TriNum * 6 * 3 + 1];
		Triangles[0] = float(TriNum);

		int jStart = 0;
		int jEnd = jMax-1;
		TriNum = (iMax - 1);
		if(m_IsDeg > 0)
		{
			int TriIndT = 4;
			jStart++;
			for(int i = 0; i < iMax-1; ++i)
			{
				const BPoint *pP0 = MFOREST.GetPoint(m_arr[0  ][0]);
				const BPoint *pP1 = MFOREST.GetPoint(m_arr[i  ][1]);
				const BPoint *pP2 = MFOREST.GetPoint(m_arr[i+1][1]);
				if(pMatr)
				{
					BP0 = (*pP0) * (*pMatr); pP0 = &BP0; 
					BP1 = (*pP1) * (*pMatr); pP1 = &BP1; 
					BP2 = (*pP2) * (*pMatr); pP2 = &BP2; 
				}
				Triangles[TriIndT++] = float(pP0->GetX());
				Triangles[TriIndT++] = float(pP0->GetY());
				Triangles[TriIndT++] = float(pP0->GetZ());
				TriIndT += 3;
				Triangles[TriIndT++] = float(pP1->GetX());
				Triangles[TriIndT++] = float(pP1->GetY());
				Triangles[TriIndT++] = float(pP1->GetZ());
				TriIndT += 3;
				Triangles[TriIndT++] = float(pP2->GetX());
				Triangles[TriIndT++] = float(pP2->GetY());
				Triangles[TriIndT++] = float(pP2->GetZ());
				TriIndT += 3;
			}
		}
		if(m_IsDeg > 1)
		{
			for(int i = 0; i < iMax-1; ++i) 
			{
				const BPoint *pP0 = MFOREST.GetPoint(m_arr[i  ][jEnd - 1]);
				const BPoint *pP1 = MFOREST.GetPoint(m_arr[i+1][jEnd - 1]);
				const BPoint *pP2 = MFOREST.GetPoint(m_arr[0][jEnd]);
				if(pMatr)
				{
					BP0 = (*pP0) * (*pMatr); pP0 = &BP0; 
					BP1 = (*pP1) * (*pMatr); pP1 = &BP1; 
					BP2 = (*pP2) * (*pMatr); pP2 = &BP2; 
				}
				Triangles[TriInd++] = float(pP0->GetX());
				Triangles[TriInd++] = float(pP0->GetY());
				Triangles[TriInd++] = float(pP0->GetZ());
				TriInd += 3;
				Triangles[TriInd++] = float(pP1->GetX());
				Triangles[TriInd++] = float(pP1->GetY());
				Triangles[TriInd++] = float(pP1->GetZ());
				TriInd += 3;
				Triangles[TriInd++] = float(pP2->GetX());
				Triangles[TriInd++] = float(pP2->GetY());
				Triangles[TriInd++] = float(pP2->GetZ());
				TriInd += 3;
			}
			jEnd--;
		}
		for(int i = 0; i < iMax-1; ++i)
		{
			for(int j = jStart; j < jEnd; ++j)
			{
				{
					const BPoint *pP0 = MFOREST.GetPoint(m_arr[i  ][j  ]);
					const BPoint *pP1 = MFOREST.GetPoint(m_arr[i  ][j+1]);
					const BPoint *pP2 = MFOREST.GetPoint(m_arr[i+1][j+1]);
					if(pMatr)
					{
						BP0 = (*pP0) * (*pMatr); pP0 = &BP0; 
						BP1 = (*pP1) * (*pMatr); pP1 = &BP1; 
						BP2 = (*pP2) * (*pMatr); pP2 = &BP2; 
					}
					Triangles[TriInd++] = float(pP0->GetX());
					Triangles[TriInd++] = float(pP0->GetY());
					Triangles[TriInd++] = float(pP0->GetZ());
					TriInd += 3;
					Triangles[TriInd++] = float(pP1->GetX());
					Triangles[TriInd++] = float(pP1->GetY());
					Triangles[TriInd++] = float(pP1->GetZ());
					TriInd += 3;
					Triangles[TriInd++] = float(pP2->GetX());
					Triangles[TriInd++] = float(pP2->GetY());
					Triangles[TriInd++] = float(pP2->GetZ());
					TriInd += 3;
				}
				{
					const BPoint *pP0 = MFOREST.GetPoint(m_arr[i  ][j  ]);
					const BPoint *pP1 = MFOREST.GetPoint(m_arr[i+1][j+1]);
					const BPoint *pP2 = MFOREST.GetPoint(m_arr[i+1][j]);
					if(pMatr)
					{
						BP0 = (*pP0) * (*pMatr); pP0 = &BP0; 
						BP1 = (*pP1) * (*pMatr); pP1 = &BP1; 
						BP2 = (*pP2) * (*pMatr); pP2 = &BP2; 
					}
					Triangles[TriInd++] = float(pP0->GetX());
					Triangles[TriInd++] = float(pP0->GetY());
					Triangles[TriInd++] = float(pP0->GetZ());
					TriInd += 3;
					Triangles[TriInd++] = float(pP1->GetX());
					Triangles[TriInd++] = float(pP1->GetY());
					Triangles[TriInd++] = float(pP1->GetZ());
					TriInd += 3;
					Triangles[TriInd++] = float(pP2->GetX());
					Triangles[TriInd++] = float(pP2->GetY());
					Triangles[TriInd++] = float(pP2->GetZ());
					TriInd += 3;
				}
			}
		}
	}
	return Triangles;
}

NParamMesh * NRectMesh::CopyTransform(CopyMap &Map) const
{
	NRectMesh *pNewMesh = new NRectMesh(*this);
	FillCopy(*pNewMesh, Map);
	pNewMesh->m_arr = nullptr;
	int iMax = m_r;
	int jMax = int(m_arr[0].GetSize());
	pNewMesh->SetSize(iMax, jMax);
	for (int i = 0; i < iMax; ++i)
	{
		for (int j = 0; j < jMax; ++j)
		{
			pNewMesh->m_arr[i][j] = Map.AddOldIndPt(m_arr[i][j]);
		}
	}
	return pNewMesh;
}

void NRectMesh::MakeEquid(double Dist)
{
	if (m_r < 2 || m_arr[0].GetSize() < 2)
		return;
	BPoint* N = new BPoint[m_r * m_arr[0].GetSize()];
	const SOrigSurf* pSurf = MFOREST.GetOrigSurf(m_iSurf);
	for (int i = 0; i < m_r; ++i)
		for (int j = 0; j < m_arr[0].GetSize(); ++j)
			//			N[j * m_r + i] = CalcNormal(i, j);
			N[j * m_r + i] = pSurf->GetNormal(*MFOREST.GetPoint(m_arr[i][j]));
	for (int i = 0; i < m_r; ++i)
		for (int j = 0; j < m_arr[0].GetSize(); ++j)
			m_arr[i][j] = MFOREST.AddPoints((*MFOREST.GetPoint(m_arr[i][j])) + N[j * m_r + i] * Dist);
	delete[] N;
}

BPoint NRectMesh::CalcNormal(int i, int j) const
{
	std::vector<int> Inds3;
	Inds3.reserve(5);
	Inds3.push_back(m_arr[i][j]);
	int Count = 0;
	if (i == 0)
	{
		if (j == 0)
		{
			Inds3.push_back(m_arr[i][j + 1]);
			Inds3.push_back(m_arr[i + 1][j]);
		}
		else if (j == m_arr[0].GetSize() - 1)
		{
			Inds3.push_back(m_arr[i + 1][j]);
			Inds3.push_back(m_arr[i][j - 1]);
		}
		else
		{
			Inds3.push_back(m_arr[i][j + 1]);
			Inds3.push_back(m_arr[i + 1][j]);
			Inds3.push_back(m_arr[i][j - 1]);
		}
	}
	else if (i == m_r - 1)
	{
		if (j == 0)
		{
			Inds3.push_back(m_arr[i - 1][j]);
			Inds3.push_back(m_arr[i][j + 1]);
		}
		else if (j == m_arr[0].GetSize() - 1)
		{
			Inds3.push_back(m_arr[i][j - 1]);
			Inds3.push_back(m_arr[i - 1][j]);
		}
		else
		{
			Inds3.push_back(m_arr[i][j - 1]);
			Inds3.push_back(m_arr[i - 1][j]);
			Inds3.push_back(m_arr[i][j + 1]);
		}
	}
	else if (j == 0)
	{
		Inds3.push_back(m_arr[i - 1][j]);
		Inds3.push_back(m_arr[i][j + 1]);
		Inds3.push_back(m_arr[i + 1][j]);
	}
	else if (j == m_arr[0].GetSize() - 1)
	{
		Inds3.push_back(m_arr[i + 1][j]);
		Inds3.push_back(m_arr[i][j - 1]);
		Inds3.push_back(m_arr[i - 1][j]);
	}
	else
	{
		Inds3.push_back(m_arr[i][j + 1]);
		Inds3.push_back(m_arr[i + 1][j]);
		Inds3.push_back(m_arr[i][j - 1]);
		Inds3.push_back(m_arr[i - 1][j]);
		Count = 5;
	}
	if (Count != 5)
		Count = int(Inds3.size()) - 1;
	// Calculate normalized normal vector to the Pi1 Pi2 Pi3 plane
	// and store it in the forest
	// Pi1 Pi2 Pi3 is oriented CCW
	// it is assumed that i1 != i2 != i3
	// Returns the number of the calculated vector in the forest
	BPoint N(0., 0., 0., 0.);
	BPoint P(*MFOREST.GetPoint(Inds3[0]));
	for (int k = 1; k < Count; ++k)
	{
		BPoint V2(*MFOREST.GetPoint(Inds3[k]) - P);
		double d2 = V2.D2();
		BPoint V3(*MFOREST.GetPoint(Inds3[(k + 1) % Inds3.size()]) - P);// % - for Count == 5 only
		double d3 = V3.D2();
		if (d2 < MIND * MIND || d3 < MIND * MIND || (V2 - V3).D2() < MIND * MIND)
			continue;
		BPoint PP = V2 % V3;
		double d = sqrt(PP.D2());
		N += PP * (m_sgn / d);
	}
	double d = sqrt(N.D2());
	N *= 1. / d;
	return N;
}

NParamMesh* NRectMesh::Convert2TriMesh()
{
	NTriMesh* pTriMesh = new NTriMesh;
	*static_cast<NParamMesh*>(pTriMesh) = *static_cast<NParamMesh*>(this);
	for (int i = 0; i < m_r - 1; ++i)
		for (int j = 0; j < m_arr[0].GetSize() - 1; ++j)
		{
			pTriMesh->AddTri(m_arr[i][j], m_arr[i + 1][j + 1], m_arr[i + 1][j]);
			pTriMesh->AddTri(m_arr[i][j], m_arr[i][j + 1], m_arr[i + 1][j + 1]);
		}
	return pTriMesh;
}

void NRectMesh::StoreMesh(MBody & Body, ElemID ElID, cadrID CaID, bool ToAux) const
{
	// Process single body

	// Сеть может быть вырождена
	// IsDeg == 1, то совпадают точки [*][0]
	// IsDeg == 2, то дополнительно совпадают точки [*][j-end]
	// массивы [*][j] при j=0 или j = j-end могут содержать повторяющиеся номера, но не одновременно.
	// если Mesh треугольник, то элементы [0][1] и [0][2] определяют
	// отрезок, который соответствует дуге Mesh[0] на смежной сети.
	if(m_r < 1)// Mesh is cleared
		return;

	if (MeshID != 0)
		MElemIDProc::SetPCn(&ElID, MElemIDProc::GetPCn(MeshID));

	MElemIDProc::SetSurfID(&ElID,m_iSurf);

	int iMax = m_r;
	int jMax = int(m_arr[0].GetSize());
	double sign = m_sgn;
	if( iMax == 1 )// Flat contour
	{
	// Remove equal numbers
		int i = 0;
		for(int j=0; j<jMax; ++j)
		{
			if(m_arr[0][(j+1)%jMax] != m_arr[0][j])
				m_arr[0][i++] = m_arr[0][j];
		}
		if(i == 0)
			return; // Empty contour
		jMax = i;
	//
		int n = MFOREST.CalcNormal(
			m_arr[0][0], m_arr[0][1], m_arr[0][jMax-1], sign); 
		MFace BFace(n, m_arr[0][0]);
		BFace.SetArraySize(jMax);
		for(int j = 0; j < jMax; ++j)
			BFace.SetPointAt(j,m_arr[0][j]);
		Body.StoreFace(&BFace, ElID, CaID, ToAux);
	}
	else
	{
		int jStart = 0;
		int jEnd = jMax-1;
		if(m_IsDeg > 0)
		{
			jStart++;
			for(int i = 0; i < iMax-1; ++i)
			{
				MFace *pFace = MFOREST.CalcFace(
								m_arr[0  ][0],
								m_arr[0  ][0],
								m_arr[i  ][1], 
								m_arr[i+1][1], sign);
				Body.StoreFace(pFace, ElID, CaID, ToAux);
				delete pFace;
			}
		}
		if(m_IsDeg > 1)
		{
//			jEnd--;
			for(int i = 0; i < iMax-1; ++i) 
			{
				MFace *pFace = MFOREST.CalcFace(
								m_arr[i  ][jEnd - 1],
								m_arr[i+1][jEnd - 1], 
								m_arr[0][jEnd],
								m_arr[0][jEnd], sign);
				Body.StoreFace(pFace, ElID, CaID, ToAux);
				delete pFace;
			}
			jEnd--;
		}
		for(int i = 0; i < iMax-1; ++i)
		{
			for(int j = jStart; j < jEnd; ++j)
			{
				MFace *pFace = MFOREST.CalcFace(
								m_arr[i  ][j  ],
								m_arr[i  ][j+1], 
								m_arr[i+1][j+1],
								m_arr[i+1][j  ], sign);
//				if(DEBUGMODE)
//				{
				if(pFace == nullptr)
				{
					pFace = MFOREST.CalcFace(
							m_arr[i  ][j  ],
							m_arr[i  ][j+1], 
							m_arr[i+1][j+1],
							m_arr[i+1][j+1], sign);
					Body.StoreFace(pFace, ElID, CaID, ToAux);
					delete pFace;
					pFace = MFOREST.CalcFace(
							m_arr[i  ][j  ],
							m_arr[i+1][j+1], 
							m_arr[i+1][j+1],
							m_arr[i+1][j], sign);
				}
//				}
				Body.StoreFace(pFace, ElID, CaID, ToAux);
				delete pFace;
			}
		}
	}
}

void NRectMesh::StoreMesh2Aux(class DXSubModel5X &Mod5, ElemID ElID, cadrID CaID) const
{
	// Сеть может быть вырождена
	// IsDeg == 1, то совпадают точки [*][0]
	// IsDeg == 2, то дополнительно совпадают точки [*][j-end]
	// массивы [*][j] при j=0 или j = j-end могут содержать повторяющиеся номера, но не одновременно.
	// если Mesh треугольник, то элементы [0][1] и [0][2] определяют
	// отрезок, который соответствует дуге Mesh[0] на смежной сети.
	if(m_r < 1)// Mesh is cleared
		return;

	if(MeshID != 0)
		ElID = MeshID;

	MElemIDProc::SetSurfID(&ElID,m_iSurf);

	for (int d = 0; d < 3; ++d)
	{
		DXDirection3X &Dir = Mod5.GetAuxDir(d);
		int iMax = m_r;
		int jMax = int(m_arr[0].GetSize());
		double sign = m_sgn;
		if (iMax == 1)// Flat contour
		{
			// Remove equal numbers
			int i = 0;
			for (int j = 0; j < jMax; ++j)
			{
				if (m_arr[0][(j + 1) % jMax] != m_arr[0][j])
					m_arr[0][i++] = m_arr[0][j];
			}
			if (i == 0)
				return; // Empty contour
			jMax = i;
			//
			int n = MFOREST.CalcNormal(
				m_arr[0][0], m_arr[0][1], m_arr[0][jMax - 1], sign);
			MFace BFace(n, m_arr[0][0]);
			BFace.SetArraySize(jMax);
			for (int j = 0; j < jMax; ++j)
				BFace.SetPointAt(j, m_arr[0][j]);
			Dir.StoreFace(&BFace, DX_ID(ElID, CaID));
		}
		else
		{
			int jStart = 0;
			int jEnd = jMax - 1;
			if (m_IsDeg > 0)
			{
				jStart++;
				for (int i = 0; i < iMax - 1; ++i)
				{
					MFace *pFace = MFOREST.CalcFace(
						m_arr[0][0],
						m_arr[0][0],
						m_arr[i][1],
						m_arr[i + 1][1], sign);
					Dir.StoreFace(pFace, DX_ID(ElID, CaID));
					delete pFace;
				}
			}
			if (m_IsDeg > 1)
			{
				//			jEnd--;
				for (int i = 0; i < iMax - 1; ++i)
				{
					MFace *pFace = MFOREST.CalcFace(
						m_arr[i][jEnd - 1],
						m_arr[i + 1][jEnd - 1],
						m_arr[0][jEnd],
						m_arr[0][jEnd], sign);
					Dir.StoreFace(pFace, DX_ID(ElID, CaID));
					delete pFace;
				}
				jEnd--;
			}
			for (int i = 0; i < iMax - 1; ++i)
			{
				for (int j = jStart; j < jEnd; ++j)
				{
					MFace *pFace = MFOREST.CalcFace(
						m_arr[i][j],
						m_arr[i][j + 1],
						m_arr[i + 1][j + 1],
						m_arr[i + 1][j], sign);
					//				if(DEBUGMODE)
					//				{
					if (pFace == nullptr)
					{
						pFace = MFOREST.CalcFace(
							m_arr[i][j],
							m_arr[i][j + 1],
							m_arr[i + 1][j + 1],
							m_arr[i + 1][j + 1], sign);
						Dir.StoreFace(pFace, DX_ID(ElID, CaID));
						delete pFace;
						pFace = MFOREST.CalcFace(
							m_arr[i][j],
							m_arr[i + 1][j + 1],
							m_arr[i + 1][j + 1],
							m_arr[i + 1][j], sign);
					}
					//				}
					Dir.StoreFace(pFace, DX_ID(ElID, CaID));
					delete pFace;
				}
			}
		}
	}
}

const BBox & NRectMesh::Expand(BBox & Gab) const
{
	for(int i = 0 ; i < m_r; ++i)
		for(int j = 0 ; j < m_arr[0].GetSize(); ++j)
			Gab.Expand(*MFOREST.GetPoint(m_arr[i][j]));
	return Gab;
}
