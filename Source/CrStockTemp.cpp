// CrStockTemp.cpp : implementation file
//

#include "stdafx.h"
#include "NCMDoc.h"
#include "CrStockTemp.h"
#include "afxdialogex.h"
#include "NStockBox.h"
#include "DimObjectStock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// CrStockTemp dialog

IMPLEMENT_DYNAMIC(CrStockTemp, CDialogEx)

CrStockTemp::CrStockTemp(class CNCMDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CDialogEx(CrStockTemp::IDD, pParent), _doc(pDoc)
	, TempStCen_X(0)
	, TempStCen_Y(0)
	, TempStCen_Z(0)
	, DepthTest(FALSE)
{
	// get TempStSize_X ... from xml
	BPoint Dim = NCM_PROJECT.Defaults.GetVector(_T("Defaults/Dimensions/TempStock/StockDim"), BPoint(11., 11., 11., 0.));
	double H = 0.;
	Dim.Get(TempStSize_X, TempStSize_Y, TempStSize_Z, H);

}

CrStockTemp::~CrStockTemp()
{
}

void CrStockTemp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	NFloatEdit::DDX_Text_my(pDX, IDC_TEMPSTCEN_X, TempStCen_X);
	NFloatEdit::DDX_Text_my(pDX, IDC_TEMPSTCEN_Y, TempStCen_Y);
	NFloatEdit::DDX_Text_my(pDX, IDC_TEMPSTCEN_Z, TempStCen_Z);
	NFloatEdit::DDX_Text_my(pDX, IDC_TEMPSTLEN_X, TempStSize_X);
	NFloatEdit::DDX_Text_my(pDX, IDC_TEMPSTLEN_Y, TempStSize_Y);
	NFloatEdit::DDX_Text_my(pDX, IDC_TEMPSTLEN_Z, TempStSize_Z);
	DDX_Check(pDX, IDC_TEMPSTVIS, DepthTest);
	DDX_Control(pDX, IDC_TEMPSTCEN_X, CTempStCen_X);
	DDX_Control(pDX, IDC_TEMPSTCEN_Y, CTempStCen_Y);
	DDX_Control(pDX, IDC_TEMPSTCEN_Z, CTempStCen_Z);
	DDX_Control(pDX, IDC_TEMPSTLEN_X, CTempStSize_X);
	DDX_Control(pDX, IDC_TEMPSTLEN_Y, CTempStSize_Y);
	DDX_Control(pDX, IDC_TEMPSTLEN_Z, CTempStSize_Z);
}


BEGIN_MESSAGE_MAP(CrStockTemp, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CALC, OnCalc)
	ON_BN_CLICKED(IDC_TEMPSTVIS, OnTempStVis)
END_MESSAGE_MAP()


void CrStockTemp::Init()
{
    _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
    _doc->GetSnaps().SetSnapType(Snaps::EmptySnapType);
    _doc->GetSnaps().SetSnapObjectType(Snaps::Model);
    _doc->Dimensions.ActivateSnaps();

	_doc->Dimensions.SetMouseClickAction(DDimensions::CreateStock);
}

void CrStockTemp::UpdateDialog()
{
	DimObjectStock *pDStock = _doc->Dimensions.GetDimStock();
	if(pDStock)
	{
		BBox Box = pDStock->GetGabar();
		BPoint C = Box.GetCenterPoint();
		TempStCen_X = C.GetX();
		TempStCen_Y = C.GetY();
		TempStCen_Z = C.GetZ();
		TempStSize_X = Box.dX();
		TempStSize_Y = Box.dY();
		TempStSize_Z = Box.dZ();
		DepthTest = BOOL(pDStock->GetDepthTest());
		GetDlgItem(IDC_TEMPSTVIS)->EnableWindow(TRUE);
		UpdateData(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMPSTVIS)->EnableWindow(FALSE);
	}
}


void CrStockTemp::OnOK()
{
	UpdateData();
	// store TempStSize_X ... to xml
	Close();
	NStockBox *pStock = new NStockBox();
	DimObjectStock *pDObj = _doc->Dimensions.GetDimStock();
	if(!pDObj)
		return;
	pStock->Create(pDObj->GetGabar());
	pStock->SetTemporary(true);
	_doc->RemoveTempStock();
	_doc->AddStock(pStock);
	_doc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR | N_STOCKSTATECHANGE,NULL);
}

void CrStockTemp::OnCancel()
{
	Close();
}

BOOL CrStockTemp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CrStockTemp::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;
}

void CrStockTemp::Close()
{
	ShowWindow(SW_HIDE);
	_doc->Dimensions.DeActivateSnaps();
	_doc->Dimensions.DeActivate();
    _doc->UpdateAllViews(NULL, N_OGL);
}

void CrStockTemp::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	DimObjectStock *pDObj = _doc->Dimensions.GetDimStock();
	if(pDObj)
		pDObj->Hide(bShow == FALSE);
	_doc->Dimensions.GetObjects().HideOrShowObjects(DimObjects::DimObjectSegmentEdgeType, bShow == FALSE);
}

void CrStockTemp::OnCalc()
{
	UpdateData();
	DimObjectStock *pDObj = _doc->Dimensions.GetDimStock();
	DimObjects & Objects = _doc->Dimensions.GetObjects();
	BPoint PMin(TempStCen_X - TempStSize_X / 2, TempStCen_Y - TempStSize_Y / 2, TempStCen_Z - TempStSize_Z / 2, 1.);
	BPoint PMax(TempStCen_X + TempStSize_X / 2, TempStCen_Y + TempStSize_Y / 2, TempStCen_Z + TempStSize_Z / 2, 1.);
	if(pDObj)
	{
		pDObj->SetPointMin(PMin);
		pDObj->SetPointMax(PMax);
		pDObj->SetDepthTest(DepthTest == TRUE);
		Objects.ChangeObjectStock(pDObj);
	}
	else
	{
		pDObj = new DimObjectStock(PMin, PMax, 2);
		Objects.AddObjectStock(pDObj);
	}
	// get Color from xml
	NColor Col = NCM_PROJECT.Defaults.GetColor(_T("Defaults/Dimensions/TempStock/Color"), NColor());
	const double Color[3] = { Col.R(), Col.G(), Col.B() };
	pDObj->SetColor(Color);
	_doc->UpdateAllViews(NULL, N_OGL);
}

void CrStockTemp::OnTempStVis()
{
	UpdateData();
	DimObjectStock *pDObj = _doc->Dimensions.GetDimStock();
	if(pDObj)
	{
		pDObj->SetDepthTest(DepthTest == TRUE);
		_doc->Dimensions.GetObjects().ChangeObjectStock(pDObj);
	}
	_doc->UpdateAllViews(NULL, N_OGL);
}
