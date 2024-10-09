
// HostNCMView.cpp : implementation of the CHostNCMView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "HostNCM.h"
#endif

#include "HostNCMDoc.h"
#include "..\..\ncmext\NCMExt.h"
#include "HostNCMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHostNCMView

IMPLEMENT_DYNCREATE(CHostNCMView, CView)

BEGIN_MESSAGE_MAP(CHostNCMView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHostNCMView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_NCMANAGER_SENDDATA, &CHostNCMView::OnNcmanagerSenddata)
	ON_COMMAND(ID_NCMANAGER_DEFINESTOCK, &CHostNCMView::OnNcmanagerSenddata)
	ON_COMMAND(ID_NCMANAGER_NCMSUBSCRIBETOPROGRAMCHANGES, &CHostNCMView::OnNcmanagerNcmsubscribetoprogramchanges)
	ON_COMMAND(ID_NCMANAGER_NCMUNSUBSCRIBEFROMPROGRAMCHANGES, &CHostNCMView::OnNcmanagerNcmunsubscribefromprogramchanges)
	ON_COMMAND(ID_NCMANAGER_NCMBLANKALLPROGRAMS, &CHostNCMView::OnNcmanagerNcmblankallprograms)
	ON_COMMAND(ID_NCMANAGER_NCMUNBLANKALLPROGRAMS, &CHostNCMView::OnNcmanagerNcmunblankallprograms)
	ON_COMMAND(ID_NCMANAGER_NCMSHOWPROGRAMTEXT, &CHostNCMView::OnNcmanagerNcmshowprogramtext)
	ON_COMMAND(ID_NCMANAGER_NCMSAVEPROGRAM, &CHostNCMView::OnNcmanagerNcmsaveprogram)
	ON_COMMAND(ID_NCMANAGER_NCMRESET, &CHostNCMView::OnNcmanagerNcmreset)
	ON_COMMAND(ID_NCMANAGER_NCMSTARTEXECUTION, &CHostNCMView::OnNcmanagerNcmstartexecution)
	ON_COMMAND(ID_NCMANAGER_NCMSTOPEXECUTION, &CHostNCMView::OnNcmanagerNcmstopexecution)
	ON_COMMAND(ID_NCMANAGER_NCMSETMODECUT, &CHostNCMView::OnNcmanagerNcmsetmodecut)
	ON_COMMAND(ID_NCMANAGER_NCMSETMODEDONTCUT, &CHostNCMView::OnNcmanagerNcmsetmodedontcut)
	ON_COMMAND(ID_NCMANAGER_NCMSETMODEFASTCUT, &CHostNCMView::OnNcmanagerNcmsetmodefastcut)
	ON_COMMAND(ID_NCMANAGER_NCMSETTMSMOOTHLY, &CHostNCMView::OnNcmanagerNcmsettmsmoothly)
	ON_COMMAND(ID_NCMANAGER_NCMSETTMFEEDRATE, &CHostNCMView::OnNcmanagerNcmsettmfeedrate)
	ON_COMMAND(ID_NCMANAGER_NCMSETTMRESULT, &CHostNCMView::OnNcmanagerNcmsettmresult)
	ON_COMMAND(ID_NCMANAGER_NCMVIEWZOOMALL, &CHostNCMView::OnNcmanagerNcmviewzoomall)
	ON_COMMAND(ID_NCMANAGER_NCMVIEWROTATEX, &CHostNCMView::OnNcmanagerNcmviewrotatex)
	ON_COMMAND(ID_NCMANAGER_NCMVIEWROTATEY, &CHostNCMView::OnNcmanagerNcmviewrotatey)
	ON_COMMAND(ID_NCMANAGER_NCMVIEWROTATEZ, &CHostNCMView::OnNcmanagerNcmviewrotatez)
	ON_COMMAND(ID_NCMANAGER_NCMVIEWPAN, &CHostNCMView::OnNcmanagerNcmviewpan)
	ON_COMMAND(ID_NCMANAGER_NCMVIEWSCALE, &CHostNCMView::OnNcmanagerNcmviewscale)
	ON_COMMAND(ID_NCMANAGER_NCMVIEWZOOMW, &CHostNCMView::OnNcmanagerNcmviewzoomw)
	ON_COMMAND(ID_NCMVIEWSETVIEW_TOP, &CHostNCMView::OnNcmanagerNcmviewsetviewtop)
	ON_COMMAND(ID_NCMVIEW_NCMSETSTOCKVISIBILITY, &CHostNCMView::OnNcmanagerNcmsetstockvisibility)
	ON_COMMAND(ID_NCMVIEW_NCMSETSTOCKCOLOUR, &CHostNCMView::OnNcmanagerNcmsetstockcolour)
	ON_COMMAND(ID_NCMVIEW_NCMSETSTOCKEDGES, &CHostNCMView::OnNcmanagerNcmsetstockedges)
	ON_COMMAND(ID_NCMVIEW_NCMSETSTOCKEDGESTHICKNESS, &CHostNCMView::OnNcmanagerNcmsetstockedgesthickness)
	ON_COMMAND(ID_NCMVIEW_NCMSETPROGRAMSSHOW, &CHostNCMView::OnNcmanagerNcmsetprogramsshow)
	ON_COMMAND(ID_NCMVIEW_NCMSETPROGRAMTHICKNESS, &CHostNCMView::OnNcmanagerNcmsetprogramthickness)
	ON_COMMAND(ID_NCMVIEW_NCMSETTOOLSVISIBILITY, &CHostNCMView::OnNcmanagerNcmsettoolsvisibility)
	ON_COMMAND(ID_NCMVIEW_NCMSETSTOCKQUALITY, &CHostNCMView::OnNcmanagerNcmsetstockquality)
	ON_COMMAND(ID_NCMVIEW_NCMSETTOOLTOOLPATH, &CHostNCMView::OnNcmanagerNcmsettooltoolpath)
	ON_COMMAND(ID_NCMVIEW_NCMREMOVEALLSUBPROGRAMS, &CHostNCMView::OnNcmanagerNcmremoveallsubprograms)
	ON_COMMAND(ID_NCMVIEW_NCMREMOVESUBPROGRAM, &CHostNCMView::OnNcmanagerNcmremovesubprogram)
	ON_COMMAND(ID_NCMVIEW_NCMSAVEPROGRAM, &CHostNCMView::OnNcmanagerNcmsaveprogram)
END_MESSAGE_MAP()

// CHostNCMView construction/destruction

CHostNCMView::CHostNCMView()
{
	// TODO: add construction code here

}

CHostNCMView::~CHostNCMView()
{
}

BOOL CHostNCMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CHostNCMView drawing

void CHostNCMView::OnDraw(CDC* /*pDC*/)
{
	CHostNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CHostNCMView printing


void CHostNCMView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHostNCMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHostNCMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHostNCMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CHostNCMView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHostNCMView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHostNCMView diagnostics

#ifdef _DEBUG
void CHostNCMView::AssertValid() const
{
	CView::AssertValid();
}

void CHostNCMView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHostNCMDoc* CHostNCMView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHostNCMDoc)));
	return (CHostNCMDoc*)m_pDocument;
}
#endif //_DEBUG


// CHostNCMView message handlers


void CHostNCMView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// NCManager hosting
	if(NCMStartHost(m_hWnd) != NCME_OK)
		AfxMessageBox(_T("Can not start NCManager"));
	
}


void CHostNCMView::OnDestroy()
{
	CView::OnDestroy();

	NCMStopHost();
}


void CHostNCMView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CRect ClientRect;
	GetClientRect(ClientRect);
	NCMSizeHost(ClientRect);
}


void CHostNCMView::OnNcmanagerSenddata()
{
	NCMClear();
//	NCM_ShowMainMenu(0);
//  Установка типа проекта
	NCMSetMillProject(); 
//  Установка стойки ЧПУ и станка
	NCMSetNCControl("ISO.sm3");
	NCMSetNCMachine("3X_DEFAULT");
//	Загрузка программы
	NCMAddProgram("C:\\Test\\Test.nc");
//	Загрузка подпрограммы
	NCMAddSubProgram("C:\\Test\\demo.nc");
//	Определение заготовки
	NCMSetMillStockCyl(100., 100., 0., 50., 200., 2);
//	Определение инструмента (режущая часть, оправка и их цвет )
	NCMAddMillCutCont("G0X0.000000Y0.000000G1X4.000000Y0.000000G3X5.000000Y1.000000I0.000000J1.000000G1X5.000000Y50.000000G1X0.000000Y50.000000", 1, 0, 0);
	NCMAddMillHolderCont("G0X0.000000Y50.000000G1X20.000000Y50.00000G1X20.000000Y100.00000G1X0.000000Y100.00000", 1, 0, 0);
	NCMSetToolColors(RGB(255, 128, 255), RGB(255, 0, 0), 1, 0);
	NCMAddMillCutCont("G0X0.000000Y0.000000G3X4.000000Y4.000000I0.000000J4.000000G1X4.000000Y50.000000G1X0.000000Y50.000000", 2, 0, 0);
	NCMAddMillCutCont("G0X0.000000Y0.000000G3X2.500000Y2.500000I0.000000J2.500000G1X2.500000Y50.000000G1X0.000000Y50.000000", 3, 0, 0);
	NCMAddMillCutCont("G0X0.000000Y0.000000G3X1.000000Y1.000000I0.000000J1.000000G1X1.000000Y12.000000G1X0.000000Y12.000000", 4, 0, 0);
	NCMAddMillCutCont("G0X0.000000Y0.000000G1X3.000000Y3.000000G1X3.000000Y20.000000G1X0.000000Y20.000000", 5, 0, 0);
	NCMAddMillHolderCont("G0X0.000000Y50.000000G1X20.000000Y50.00000G1X20.000000Y100.00000G1X0.000000Y100.00000", 2, 0, 0);
	NCMSetToolColors(RGB(0, 255, 0), RGB(255, 0, 0), 2, 0);
	NCMAddMillHolderCont("G0X0.000000Y50.000000G1X20.000000Y50.00000G1X20.000000Y100.00000G1X0.000000Y100.00000", 3, 0, 0);
	NCMSetToolColors(RGB(255, 128, 64), RGB(255, 0, 0), 3, 0);
	NCMAddMillHolderCont("G0X0.000000Y12.000000G1X20.000000Y12.00000G1X20.000000Y100.00000G1X0.000000Y100.00000", 4, 0, 0);
	NCMSetToolColors(RGB(192, 192, 192), RGB(255, 0, 0), 4, 0);
	NCMAddMillHolderCont("G0X0.000000Y20.000000G1X20.000000Y20.00000G1X20.000000Y100.00000G1X0.000000Y100.00000", 5, 0, 0);
	NCMSetToolColors(RGB(255, 255, 0), RGB(255, 0, 0), 5, 0);
// Активный инструмент в начале и его положение 
	NCMStartToolNumber(1, 0);
	NCMStartToolPosition(0., 0., 100., 0., 0., 0.);
// Смещение СК G54
	NCMAddFixOffset(0, 50., 0., 40., 0., 0., 0.);



	NCMSendHost();
}


void CHostNCMView::OnNcmanagerNcmsubscribetoprogramchanges()
{
	// Подписаться на оповещение
	NCMSubscribeToProgramChanges(AfxGetMainWnd()->operator HWND(), 25);
}


void CHostNCMView::OnNcmanagerNcmunsubscribefromprogramchanges()
{
	NCMUnsubscribeFromProgramChanges();
}


void CHostNCMView::OnNcmanagerNcmblankallprograms()
{
	NCMBlankAllPrograms();
}


void CHostNCMView::OnNcmanagerNcmunblankallprograms()
{
	NCMUnblankAllPrograms();
}


void CHostNCMView::OnNcmanagerNcmshowprogramtext()
{
	NCMShowProgramText("C:\\Test\\Test.nc");
}


//void CHostNCMView::OnNcmanagerNcmsaveprogram()
//{
//	NCMSaveProgram("C:\\Test\\Test.nc", "C:\\Test\\Test.nc");
//}


void CHostNCMView::OnNcmanagerNcmreset()
{
	NCMReset();
}


void CHostNCMView::OnNcmanagerNcmstartexecution()
{
	NCMStartExecution();
}


void CHostNCMView::OnNcmanagerNcmstopexecution()
{
	NCMStopExecution();
}


void CHostNCMView::OnNcmanagerNcmsetmodecut()
{
	NCMSetModeCut();
}


void CHostNCMView::OnNcmanagerNcmsetmodedontcut()
{
	NCMSetModeDontCut();
}


void CHostNCMView::OnNcmanagerNcmsetmodefastcut()
{
	NCMSetModeFastCut();
}


void CHostNCMView::OnNcmanagerNcmsettmsmoothly()
{
	NCMSetTMSmoothly();
}


void CHostNCMView::OnNcmanagerNcmsettmfeedrate()
{
	NCMSetTMFeedrate();
}


void CHostNCMView::OnNcmanagerNcmsettmresult()
{
	NCMSetTMResult();
}

//Zoom all-
void CHostNCMView::OnNcmanagerNcmviewzoomall()
{
	NCMViewZoomAll();
}

//Rotate around coordinate axis-
// Axis = 0 - X axis
// Axis = 1 - Y axis
// Axis = 2 - Z axis
void CHostNCMView::OnNcmanagerNcmviewrotatex()
{
	NCMViewRotateA(0);
}
void CHostNCMView::OnNcmanagerNcmviewrotatey()
{
	NCMViewRotateA(1);
}
void CHostNCMView::OnNcmanagerNcmviewrotatez()
{
	NCMViewRotateA(2);
}
//Scale-
void CHostNCMView::OnNcmanagerNcmviewscale()
{
	NCMViewScale();
}
//Zoom window-
void CHostNCMView::OnNcmanagerNcmviewzoomw()
{
	NCMViewZoomW();
}

//Pan-
void CHostNCMView::OnNcmanagerNcmviewpan()
{
	NCMViewPan();
}
//Set view-
//eyex, eyey, eyez - The position of the eye point.
//upx, upy, upz - The direction of the up vector.
//See gluLookAt documentation for details
void CHostNCMView::OnNcmanagerNcmviewsetviewtop()
{
	NCMViewSetView(0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
}

void CHostNCMView::OnNcmanagerNcmsetstockcolour()
{
	NCMSetStockColour(RGB(255, 0, 0));
}

void CHostNCMView::OnNcmanagerNcmsetstockvisibility()
{
	NCMSetStockVisibility(FALSE);
}

void CHostNCMView::OnNcmanagerNcmsetstockedges()
{
	NCMSetStockEdges(FALSE);
}

void CHostNCMView::OnNcmanagerNcmsetstockedgesthickness()
{
	NCMSetStockEdgesThickness(2.5);
}

void CHostNCMView::OnNcmanagerNcmsetprogramsshow()
{
	NCMSetProgramsShow(TP_WORM);
}

void CHostNCMView::OnNcmanagerNcmsetprogramthickness()
{
	NCMSetProgramThickness("C:\\ncm-prt\\demo\\demo.nc", 5.0);
}

void CHostNCMView::OnNcmanagerNcmsettoolsvisibility()
{
	NCMSetToolsVisibility(FALSE);
}

void CHostNCMView::OnNcmanagerNcmsetstockquality()
{
	int max = 500000;
	NCMGetMaxStockQuality(max);
	NCMSetStockQuality(max);
}

void CHostNCMView::OnNcmanagerNcmsettooltoolpath()
{
	NCMSetToolToolpath(1, 0, TRUE);
}

void CHostNCMView::OnNcmanagerNcmremoveallsubprograms()
{
	NCMRemoveAllSubprograms();
	NCMSendHost();
}

void CHostNCMView::OnNcmanagerNcmremovesubprogram()
{
	NCMRemoveSubprogram("C:\\test\\demo.NC");
	NCMSendHost();

}

void CHostNCMView::OnNcmanagerNcmsaveprogram()
{
	NCMSaveProgram("C:\\Test\\demo.nc", "C:\\Test\\demo2.nc");
}