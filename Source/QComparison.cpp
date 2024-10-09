#include "stdafx.h"
#include "BBox.h"
#include "BStl.h"
#include "RDCEL.h"
#include "NStockStl.h"
#include "CMapStringToNStock.h"
#include "qcomparisondlg.h"
#include "math.h"
#include "GQuadRenderMill.h"
#include "GQuadRenderDX5X.h"
#include "WM_APP_messages.h"
#include "SimpleCloud.h"
#include "SimpleMesh.h"
#include "DistanceComputationTools.h"
#include "QCallback.h"
#include "GQuadRenderDX5X.h"
#include "DXSubModel5x.h"
#include "NStock.h"
#include "NCMDoc.h"
#include "QComparison.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace CCLib;

const double EPS = 1.0e-5;
double EPSD;

QComparison::QComparison(CNCMDoc& iDoc) : Doc(iDoc)
{
	pPointCloud = nullptr;
	pActPartList = nullptr;
	QRender = nullptr;
	DRender = nullptr;
	Active = false;//объект не активен
    Terminate = false;//поток вычислений не следует останавливать
    ThreadActive = false;//поток вычислений не активен
	/*перед тем как передали диалогу самого себя установим цвет по умолчанию*/
	def_Color = new COLORREF [Dlg.k_Color];//по умолчанию цветов фиксированное количество
	Color_tmp = new COLORREF [Dlg.k_Color];

	Color_tmp[0] = def_Color[0]	=	RGB(127,0,0);
	Color_tmp[1] = def_Color[1]	=	RGB(255,0,0);
	Color_tmp[2] = def_Color[2]	=	RGB(255,127,0);
	Color_tmp[3] = def_Color[3]	=	RGB(191,191,191);
	Color_tmp[4] = def_Color[4]	=	RGB(0,127,0);
	Color_tmp[5] = def_Color[5]	=	RGB(0,255,255);
	Color_tmp[6] = def_Color[6]	=	RGB(0,0,255);
	Color_tmp[7] = def_Color[7] =	def_Color[0];
	
	Dlg.SetParComp(this);
	pParent = NULL;

	Lim_tmp = new double [Dlg.k_Color - 1];
	for(unsigned int ix = 0; ix < Dlg.k_Color - 1; ++ix)
		Lim_tmp[ix] = Dlg.m_Lim[ix];

	kolc = 0; 
	kolr = 0;
	pMesh = nullptr;
}
QComparison::~QComparison(void)
{
	delete static_cast<SimpleCloud *>(pPointCloud);
	pPointCloud = nullptr;
	delete pMesh;
	pMesh = nullptr;
	delete def_Color;
	delete Color_tmp;
	delete Lim_tmp;
}
void QComparison::Activate(CWnd *pWnd, const CMapStringToNStock *pPartList, NStock* ipStock)
{
	if(pPartList->empty())
	{
		AfxMessageBox(IDS_COMP_NOPART);//Деталь не загружена
		return;
	}
	pStock = ipStock;
	if (pStock == nullptr)
		return;
	GBodyRender* pQuadRender = pStock->GetRender();
	if (pQuadRender == nullptr)
		return;

	Active = true;
	PickedPoint.Set(0., 0., 0., -1.);
	pActPartList = pPartList;

	QRender = nullptr;
	DRender = nullptr;

	DRender = dynamic_cast<GQuadRenderDX5X *>(pQuadRender);
	QRender = dynamic_cast<GQuadRenderMill *>(pQuadRender);

	if (pParent == NULL)
		Dlg.Create(IDD_QCOMPARISON, pWnd);
	pParent = pWnd;
	pParent->Invalidate();
	Dlg.ShowWindow(SW_SHOW);


	PolychromeBuf = MElemIDProc::GetAllowPCPalUse();
	MElemIDProc::SetAllowPCPalUse(true);

	Dlg.StartProgressBar();
	Verification(true, true);
	return;
}

void QComparison::RepaintCC()
{
	if (pPointCloud == nullptr)
		return;
	SimpleCloud &pointCloud = *static_cast<SimpleCloud *>(pPointCloud);

	int SumInd = 0;
	for (GQuadRenderDX5X *pCurRender = DRender; pCurRender != nullptr
		; pCurRender = static_cast<GQuadRenderDX5X *>(pCurRender->GetNextRender()))
	{
		for (int i = 0; i < pCurRender->GetAreasNum(); ++i)
		{
			RTriRandMesh *pTrianglArea = pCurRender->GetArea(i);
			if (!pTrianglArea)
				continue;
			int Num = pTrianglArea->GetActSizeP();
			for (int k = 0; k < Num; ++k)
			{
				auto buf = pTrianglArea->GetAtNP(k);
				auto Dist = pointCloud.getPointScalarValue(SumInd++);
				buf[6] = Dist2Color(Dist);
			}
			pTrianglArea->DeleteVBO();
		}
	}
}

void QComparison::RepaintBSP()
{
	if (pPointCloud == nullptr)
		return;
	SimpleCloud &pointCloud = *static_cast<SimpleCloud *>(pPointCloud);

	int SumInd = 0;
	for (GQuadRenderMill *pCurRender = QRender; pCurRender != nullptr
		; pCurRender = static_cast<GQuadRenderMill *>(pCurRender->GetNextRender()))
	{
		for (unsigned int ix = 0; ix < pCurRender->GetColsNum(); ++ix)
		{
			for (unsigned int iy = 0; iy < pCurRender->GetRowsNum(); ++iy)
			{
				auto pRDCEL = pCurRender->GetRend(ix, iy);
				if (pRDCEL == nullptr)
					continue;
				int Num = pRDCEL->GetPointsCount();
				for (int k = 0; k < Num; ++k)
				{
					auto buf = pRDCEL->GetPoint(k);
					buf[6] = Dist2Color(pointCloud.getPointScalarValue(SumInd++));
				}
			}
		}
	}
}

float QComparison::Dist2Color(double Dist)
{
	auto Diap = Diapason(Dist);
	PackedColor val;
	val.fa = 255;
	val.fr = unsigned int(GetRValue(DWORD(Dlg.m_ColorCtrl[Diap].GetColor())));
	val.fg = unsigned int(GetGValue(DWORD(Dlg.m_ColorCtrl[Diap].GetColor())));
	val.fb = unsigned int(GetBValue(DWORD(Dlg.m_ColorCtrl[Diap].GetColor())));
	return val.fval;
}

void QComparison::Verification(bool Recalc, bool Recreate)
{
    progress = 0;
    Terminate = false;
	if (Recreate && QRender != nullptr)
	{
		for (GQuadRenderMill *pCurRender = QRender; pCurRender != nullptr
			; pCurRender = static_cast<GQuadRenderMill *>(pCurRender->GetNextRender()))
		{
			BBox &Gabar = pCurRender->GetGabar();
			double S = Gabar.dX() * Gabar.dY() + Gabar.dX() * Gabar.dZ() + Gabar.dZ() * Gabar.dY();
			double MaxEdgeLength = 5. * sqrt(S / (Dlg.MaxNumber * 100000));
			pCurRender->ActivateRendArray(MaxEdgeLength);
		}
	}
	if (Recalc || Recreate)
	{
		if (VerTh = AfxBeginThread(VeriThread, this, THREAD_PRIORITY_BELOW_NORMAL, 2000000, NULL))
			ThreadActive = true;//если поток успешно создан
	}
	else
	{
		GQuadRenderDX5X *pDX = dynamic_cast<GQuadRenderDX5X *>(DRender);
		if (pDX != nullptr)
		{
			RepaintCC();
		}
		GQuadRenderMill *pBSP = dynamic_cast<GQuadRenderMill *>(QRender);
		if (pBSP != nullptr)
		{
			RepaintBSP();
		}
		Dlg.SendMessage(WM_APP_COMPARERDY);
	}
	return;
}

void QComparison::PreparePointCloudCC()
{
	// Prepare pointCloud
	int SumNum = 0;
	for (const GQuadRenderDX5X *pCurRender = DRender; pCurRender != nullptr
		; pCurRender = static_cast<GQuadRenderDX5X *>(pCurRender->GetNextRender()))
	{
		for (int i = 0; i < pCurRender->GetAreasNum(); ++i)
		{
			const RTriRandMesh *pTrianglArea = pCurRender->GetArea(i);
			if (!pTrianglArea)
				continue;
			SumNum += pTrianglArea->GetActSizeP();
		}
	}
	delete static_cast<SimpleCloud *>(pPointCloud);
	pPointCloud = new SimpleCloud();
	SimpleCloud &pointCloud = *static_cast<SimpleCloud *>(pPointCloud);
	pointCloud.reserve(SumNum);
	for (const GQuadRenderDX5X *pCurRender = DRender; pCurRender != nullptr
		; pCurRender = static_cast<GQuadRenderDX5X *>(pCurRender->GetNextRender()))
	{
		for (int i = 0; i < pCurRender->GetAreasNum(); ++i)
		{
			const RTriRandMesh *pTrianglArea = pCurRender->GetArea(i);
			if (!pTrianglArea)
				continue;
			int Num = pTrianglArea->GetActSizeP();
			for (int k = 0; k < Num; ++k)
			{
				auto buf = pTrianglArea->GetAtNP(k);
				pointCloud.addPoint(CCVector3(buf[3], buf[4], buf[5]));
			}
		}
	}
}

void QComparison::PreparePointCloudBSP()
{
	int SumNum = 0;
	for (const GQuadRenderMill *pCurRender = QRender; pCurRender != nullptr
		; pCurRender = static_cast<GQuadRenderMill *>(pCurRender->GetNextRender()))
	{
		for (unsigned int ix = 0; ix < pCurRender->GetColsNum(); ++ix)
		{
			for (unsigned int iy = 0; iy < pCurRender->GetRowsNum(); ++iy)
			{
				auto pRDCEL = pCurRender->GetRend(ix, iy);
				if (pRDCEL == nullptr)
					continue;
				SumNum += pRDCEL->GetPointsCount();
			}
		}
	}
	delete static_cast<SimpleCloud *>(pPointCloud);
	pPointCloud = new SimpleCloud();
	SimpleCloud &pointCloud = *static_cast<SimpleCloud *>(pPointCloud);
	pointCloud.reserve(SumNum);
	for (const GQuadRenderMill *pCurRender = QRender; pCurRender != nullptr
		; pCurRender = static_cast<GQuadRenderMill *>(pCurRender->GetNextRender()))
	{
		for (unsigned int ix = 0; ix < pCurRender->GetColsNum(); ++ix)
		{
			for (unsigned int iy = 0; iy < pCurRender->GetRowsNum(); ++iy)
			{
				auto pRDCEL = pCurRender->GetRend(ix, iy);
				if (pRDCEL == nullptr)
					continue;
				int Num = pRDCEL->GetPointsCount();
				for (int k = 0; k < Num; ++k)
				{
					auto buf = pRDCEL->GetPoint(k);
					pointCloud.addPoint(CCVector3(buf[3], buf[4], buf[5]));
				}
			}
		}
	}
}

void QComparison::VeriCalcCC()
{
	PreparePointCloudCC();
	int Res = CalcDistances();
	if (0 == Res)
		RepaintCC();
	else
	{
		delete static_cast<SimpleCloud *>(pPointCloud);
		pPointCloud = nullptr;
	}
}

void QComparison::VeriCalcBSP()
{
	PreparePointCloudBSP();
	int Res = CalcDistances();
	if (0 == Res)
		RepaintBSP();
	else
	{
		delete static_cast<SimpleCloud *>(pPointCloud);
		pPointCloud = nullptr;
	}
}

bool QComparison::PrepareMeshPointCloud()
{
	SimpleCloud &pointCloud = *static_cast<SimpleCloud *>(pPointCloud);

	int PointsNum = 0;
	int FacesCount = 0;
	for (int i = 0; i < pActPartList->GetSize(); ++i)
	{
		NStockStl *pPart = nullptr;
		CString Key;
		pActPartList->GetAt(i, Key, (CObject *&)pPart);
		if (pPart == nullptr)
			continue;
		if (!pPart->IsKindOf(RUNTIME_CLASS(NStockStl)))
			continue;
		const GLfloat *Triangles = pPart->GetTriangles();
		if (Triangles == nullptr)
			continue;
		BStl &Stl = pPart->GetSTL();
		const RDCEL *pDCELRender = Stl.GetRDCEL();
		if (pDCELRender == nullptr)
		{
			Stl.MakeRDCEL();
			pDCELRender = Stl.GetRDCEL();
		}
		if (pDCELRender == nullptr)
			return false;
		PointsNum += pDCELRender->GetVertexesCount();
		FacesCount += pDCELRender->GetFacesCount();
	}
	if (PointsNum == 0)
		return false;
	meshPointCloud.clear();
	meshPointCloud.reserve(PointsNum);
	delete pMesh;
	pMesh = new SimpleMesh(&meshPointCloud);
	SimpleMesh& mesh = *pMesh;;
	if (!mesh.reserve(FacesCount))
		return false;
	int PointsShift = 0;
	for (int i = 0; i < pActPartList->GetSize(); ++i)
	{
		NStockStl *pPart = nullptr;
		CString Key;
		pActPartList->GetAt(i, Key, (CObject *&)pPart);
		if (pPart == nullptr)
			continue;
		if (!pPart->IsKindOf(RUNTIME_CLASS(NStockStl)))
			continue;
		const GLfloat *Triangles = pPart->GetTriangles();
		if (Triangles == nullptr)
			continue;
		BStl &Stl = pPart->GetSTL();
		const RDCEL *pDCELRender = Stl.GetRDCEL();
		for (int i = 0; i < pDCELRender->GetVertexesCount(); ++i)
		{
			auto buf = pDCELRender->GetVertex(i);
			meshPointCloud.addPoint(CCVector3(buf[0], buf[1], buf[2]));
		}
		for (int i = 0; i < pDCELRender->GetFacesCount(); ++i)
		{
			int i0, i1, i2;
			pDCELRender->GetFaceVertexes(i, i0, i1, i2);
			mesh.addTriangle(i0 + PointsShift, i1 + PointsShift, i2 + PointsShift);
		}
		PointsShift += pDCELRender->GetVertexesCount();
	}
	return true;
}

int QComparison::CalcDistances()
{
	if (!PrepareMeshPointCloud())
		return -1;
	SimpleCloud& pointCloud = *static_cast<SimpleCloud*>(pPointCloud);
	DistanceComputationTools::Cloud2MeshDistanceComputationParams params;
	params.octreeLevel = CalcOctreeLevel(pointCloud, meshPointCloud);
	params.multiThread = false;
	params.signedDistances = true;
	params.flipNormals = false;
	params.maxSearchDist = ScalarType(GetMaxDist());
	params.useDistanceMap = true;
	//	, maxThreadCount(0)
	//	, CPSet(0)
	DgmOctree octree(&pointCloud);
	QCallback Callback(*this);
	return DistanceComputationTools::computeCloud2MeshDistance(&pointCloud, pMesh, params, &Callback, &octree);
}

bool QComparison::GetInfoTipText(const BPoint& P, CString& Str)
{
	double D = Calc1Distace(P);
	bool Ok = (D != 1.e12);
	if (Ok)
		Str.Format("%.3f", int(D * 1000. + 0.5) / 1000.);
	return Ok;
}

double QComparison::Calc1Distace(const BPoint& P)
{
	SetPickedPoint(BPoint(0., 0., 0., -1.));
	SimpleCloud pointCloud;
	pointCloud.reserve(1);
	pointCloud.addPoint(CCVector3(float(P.GetX()), float(P.GetY()), float(P.GetZ())));

	DistanceComputationTools::Cloud2MeshDistanceComputationParams params;
	params.octreeLevel = CalcOctreeLevel(pointCloud, meshPointCloud);
	params.multiThread = false;
	params.signedDistances = true;
	params.flipNormals = false;
	params.maxSearchDist = 0;
	params.useDistanceMap = true;
	DgmOctree octree(&pointCloud);
	QCallback Callback(*this);
	if (DistanceComputationTools::computeCloud2MeshDistance(&pointCloud, pMesh, params, &Callback, &octree) != 0)
		return 1.e12;
	auto Dist = pointCloud.getPointScalarValue(0);
	return Dist;
}

void QComparison::Proc1Distace(const BPoint& P, const BPoint& V)
{
	double Dist = Calc1Distace(P);
	if (Dist != 1.e12)
	{
		SetPickedPoint(P);
		Dlg.SetDistance(P, Dist);

		// set current line info to the dialog
		BPoint FoundP, FoundN;
		ElemID FoundID;
		cadrID FoundCadrID;
		RayShootingStatic(pStock, P, V, true, FoundP, FoundN, FoundID, FoundCadrID);
		CString Chan, Prog, Line, Step;
		Doc.GetCadrIDStrings(FoundCadrID, Chan, Prog, Line, Step);
		Dlg.ProgName = "Name= " + Prog;
		Dlg.Cadr = "N= " + Line + " St= " + Step;
		cadrID ID0, ID1;
		FoundCadrID.SplitMix(ID0, ID1);
		Dlg.CadrID = ID0;
		Dlg.GetDlgItem(IDC_GOTO_LINE)->EnableWindow(Dlg.CadrID.IsEmpty() ? FALSE : TRUE);
		Dlg.UpdateData(FALSE);
	}
}

int QComparison::CalcOctreeLevel(SimpleCloud &pointCloud, SimpleCloud &meshPointCloud)
{
	//we'll use the mesh vertices to estimate the right octree level
	DgmOctree dataOctree(&pointCloud);
	DgmOctree modelOctree(&meshPointCloud);
	if (dataOctree.build() < static_cast<int>(pointCloud.size()) || modelOctree.build() < static_cast<int>(meshPointCloud.size()))
	{
		//an error occurred during the octree computation: probably there's not enough memory
		return -1;
	}

	return dataOctree.findBestLevelForComparisonWithOctree(&modelOctree);
}

void QComparison::VeriCalc()//функция потока вычислений
{
	progress = 0;
	Terminate = false;
	if (DRender != nullptr)
	{
		Dlg.StartProgressBar();
		VeriCalcCC();
		Terminate = false;
		return;
	}
	if (QRender != nullptr)
	{
		Dlg.StartProgressBar();
		VeriCalcBSP();
		Terminate = false;
		return;
	}
}

void QComparison::VeriFinish()//вызываем по окончанию вычислений
{
	for (GQuadRenderMill *pCurRender = QRender; pCurRender != nullptr
		; pCurRender = static_cast<GQuadRenderMill *>(pCurRender->GetNextRender()))
	{
		pCurRender->SetRendArrayReady();
	}
}

UINT QComparison::VeriThread(LPVOID cmp)//функция для отдельного потока вычислений
{
    ((QComparison *)cmp)->VeriCalc();//запускаем функцию вычислений в этом потоке
    ((QComparison *)cmp)->ThreadActive = false;
	((QComparison *)cmp)->Dlg.SendMessage(WM_APP_COMPARERDY);
	return NULL;
}

void QComparison::DeActivate(void)
{
    if(!Active)
		return;

    //завершаем поток, если он работает
    while(IsActiveThread())
        TerminateThread();

	Active = false;

	delete static_cast<SimpleCloud *>(pPointCloud);
	pPointCloud = nullptr;
	delete pMesh;
	pMesh = nullptr;
	MElemIDProc::SetAllowPCPalUse(PolychromeBuf);
	if (QRender != nullptr)
	{
		GQuadRender::SetSmooth(Smooth);
		for (GQuadRenderMill *pCurRender = QRender; pCurRender != nullptr
			; pCurRender = static_cast<GQuadRenderMill *>(pCurRender->GetNextRender()))
		{
			pCurRender->ClearRendArray();
		}
	}
	else if (DRender != nullptr)
	{
		CWnd *pMainWnd = AfxGetMainWnd();
		if(pMainWnd != nullptr)
			pMainWnd->SendMessage(WM_APP_REGENERATE, N_OGL, OG_STOCKS); // To restore original colors
	}
	if (Dlg.GetSafeHwnd())
		Dlg.ShowWindow(SW_HIDE);
}

bool QComparison::IsNewColor(void)
{
	bool f = false;
	for(int i = 0; i < int(Dlg.k_Color); i++)
		if(Dlg.m_ColorCtrl[i].GetColor() != Color_tmp[i])
		{
			PackedColor val;
			val.fa = 255;
			val.fr = unsigned int(GetRValue(DWORD(Dlg.m_ColorCtrl[i].GetColor())));
			val.fg = unsigned int(GetGValue(DWORD(Dlg.m_ColorCtrl[i].GetColor())));
			val.fb = unsigned int(GetBValue(DWORD(Dlg.m_ColorCtrl[i].GetColor())));

			for(unsigned int ix = 0; ix < Dlg.k_Color; ix++)
			{
				if(ix != i)
				{
					if(val.fr == unsigned int(GetRValue(DWORD(Dlg.m_ColorCtrl[ix].GetColor()))) &&
					val.fg == unsigned int(GetGValue(DWORD(Dlg.m_ColorCtrl[ix].GetColor()))) &&
					val.fb == unsigned int(GetBValue(DWORD(Dlg.m_ColorCtrl[ix].GetColor()))))
					{
						//Присваивать будем тот же цвет
						val.fr = unsigned int(GetRValue(DWORD(Color_tmp[i])));
						val.fg = unsigned int(GetGValue(DWORD(Color_tmp[i])));
						val.fb = unsigned int(GetBValue(DWORD(Color_tmp[i])));
						//цвет кнопки изменим обратно на старый
						Dlg.m_ColorCtrl[i].SetColor(Color_tmp[i]);
						if(i > 0)
							i=-1;
						break;
					}
				}
			}
			f = true;
		}
	return f;
}
bool QComparison::IsNewLim(void)
{
	/*Если границы диапазонов изменились и идут в правильной последовательности*/
	for(unsigned int i = 0; i < Dlg.k_Color - 1; i++)
		if((Dlg.m_Lim[i] - Lim_tmp[i])*(Dlg.m_Lim[i] - Lim_tmp[i]) > 1.e-20)
			return true;
	return false;
}
void QComparison::SetNewColor(void)
{
	for(unsigned int i = 0; i < Dlg.k_Color; i++)
		Color_tmp[i] = Dlg.m_ColorCtrl[i].GetColor();
}
void QComparison::SetNewLim(void)
{
	for(unsigned int i = 0; i < Dlg.k_Color - 1; i++)
		Lim_tmp[i] = Dlg.m_Lim[i];
}
void QComparison::SetNewColorRender(float ***A)
{
	unsigned int num;
	for(unsigned int ix = 0; ix < kolc; ix++)
	{
		for(unsigned int iy = 0; iy < kolr; iy++)
		{
			GLfloat *pC = (GLfloat *)A[ix][iy];
			if(QRender->Empty(pC))
				continue;
			for(unsigned int in = 7,im = 0; in < unsigned int(pC[0]*21+1) ; in+=7, im++)//Идем по массиву вершин
			{
				PackedColor *pPC;
				pPC = (PackedColor *)(pC + in); 
				num = NumColor(RGB(pPC->fr, pPC->fg, pPC->fb));
				COLORREF tmp = Dlg.m_ColorCtrl[num].GetColor();
				pPC->fr = GetRValue(tmp);
				pPC->fg = GetGValue(tmp);
				pPC->fb = GetBValue(tmp);
			}
		}
	}
}
unsigned int QComparison::NumColor(COLORREF c)
{
	for(unsigned int i = 0; i < Dlg.k_Color; i++)
		if(c == Color_tmp[i])
			return i;
	return 0;
}
void QComparison::Clear(float ***&A)
{
	for(unsigned int ix = 0; ix < kolc; ix++)
		delete [] A[ix];
	delete [] A;
	A = NULL;
}
bool QComparison::IsActive(void) const
{
	return Active;
}
void QComparison::Draw(void) const
{
	if(!Active)
		return;
	glPushAttrib(GL_CURRENT_BIT);
	glColor3d(0., 1., 0.);
	BaseRender::DrawBall(PickedPoint, 5., true);
	glPopAttrib();
}
unsigned int QComparison::Diapason(double dist)
{
	unsigned int k;
	bool d = true; //расстояние больше верхней границы большего диапазона
	for (k = 0; k < Dlg.k_Color - 1; ++k)
	{
		if (dist < Dlg.m_Lim[k])
		{
			d = false;//расстояние меньше верхней границы большего диапазона
			break;
		}
	}
	if (d == true)
		k = Dlg.k_Color - 1;
	return k;
}

double QComparison::GetMaxDist()
{
	return max(fabs(Dlg.m_Lim[0]), fabs(Dlg.m_Lim[Dlg.k_Color - 2]));
}

bool QComparison::IsActiveThread() const
{
	Sleep(10);
    if(ThreadActive)
        return true;
    WaitForSingleObject(VerTh, 1000000);//вообще говоря, так делать не хорошо
    return false;//следовало бы выяснить, что вернула WaitForSingleObject
}

void QComparison::Serialize(CArchive &ar)
{
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.8")))
		SerializeElements(ar, &Dlg.MaxNumber, 1);
	SerializeElements(ar, def_Color, Dlg.k_Color);
	SerializeElements(ar, Dlg.m_Lim, Dlg.k_Color - 1);
	if(ar.IsLoading())	
	{		
		def_Color[7] = def_Color[0];
		for(unsigned int ix = 0; ix < Dlg.k_Color - 1; ++ix)
			Lim_tmp[ix] = Dlg.m_Lim[ix];
		for(unsigned int ix = 0; ix < Dlg.k_Color; ++ix)
			Color_tmp[ix] = def_Color[ix];	
	}
}

void QComparison::GotoCurrentCadr(const cadrID& CadrID) const
{
	Doc.Jump2AbsCadr(CadrID, false);
}

bool QComparison::RayShootingStatic(NStock* pStock, const BPoint& p, const BPoint& v, bool INOUT, BPoint& FoundP, BPoint& FoundN, ElemID& FoundID, cadrID& FoundCadrID)
{
	FoundID = MElemIDProc::ID_NULL;
	switch (NCM_PROJECT.GetGlobalState().GetModelMode())
	{
	case NGlobalState::BSP:
	{
		auto pTree = static_cast<MQuadTree*>(pStock->GetBodyMill());
		if (pTree == nullptr)
			return false;
		if (!pTree->RayShooting(p, v, INOUT, FoundP, FoundN, FoundID, FoundCadrID))
			return false;
		return true;
	}
	break;
	case NGlobalState::DX:
	{
		auto pBody = pStock->GetBodyMill();
		auto pBodyRender = pStock->GetRender();
		if (pBodyRender == nullptr)
			return false;
		const GQuadRenderDX5X* pRender5X = dynamic_cast<const GQuadRenderDX5X*>(pBodyRender);
		if (pRender5X == nullptr)
			return false;
		BPoint q[3];// vertexes of the triangle containing nearest point
		FoundP = pRender5X->RayCasting(p, v, 1.e-6, FoundN, q[0], q[1], q[2]);
		if (!FoundP.IsPoint())
			return false;
		DXSubModel5X* pBodyDX = dynamic_cast<DXSubModel5X*>(pBody);
		if (pBodyDX != nullptr)
		{
			BMatrPair M;
			const BMatrPair& BodyRot = pStock->GetBodyMill()->GetBodyRot();
			if (NCMComData::IsTurnEnabled())
				M = BodyRot.Or() * BMatr().RotX(0., 0., 0., 90.);
			else
				M = BodyRot;
			const double IdentDist = 0.0001 * (pBodyDX->GetXstep() * pBodyDX->GetXstep() + pBodyDX->GetYstep() * pBodyDX->GetYstep() + pBodyDX->GetZstep() * pBodyDX->GetZstep());
			MBodyPointInf Inf[3];
			double D2[3];
			for (int i = 0; i < 3; ++i)
			{
				D2[i] = pBodyDX->GetNearestPoint(q[i], Inf[i]);// distance from dexel vertex
				if (D2[i] < IdentDist)
				{
					FoundID = Inf[i].ElemID_;
					FoundN = Inf[i].Nr_ * M.Or();
					FoundCadrID = Inf[i].HistCadrID_;
				}
			}
		}
		return true;
	}
	break;
	default:
		return false;
	}
}
