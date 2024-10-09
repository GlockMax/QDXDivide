
#include "stdafx.h"
#include "mainfrm.h"
#include "NCUnitHelpPane.h"
#include "Resource.h"
#include "NCMControl.h"
#include "gdiplus.h"
#include "NCMControl.h"

using namespace Gdiplus;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CNCUnitHelpPane

CNCUnitHelpPane::CNCUnitHelpPane()
{
	m_pBitmap = NULL;
}

CNCUnitHelpPane::~CNCUnitHelpPane()
{
	if(m_pBitmap != NULL)
		delete m_pBitmap;
}

BEGIN_MESSAGE_MAP(CNCUnitHelpPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void CNCUnitHelpPane::ShowHelp(CString text, CString PicName)
{
	class local
	{
	public:
		static CString BreakLines(CString str)
		{
			CString res;
			for(int i = 0; i < str.GetLength(); i ++)
			{
				if((str.GetLength() - i) >= 4)
				if(str[i] == _T('['))
				if(str[i+1] == _T('b'))
				if(str[i+2] == _T('r'))
				if(str[i+3] == _T(']'))
				{
					res = res + _T("\15\12");
					str.Delete(i+1,3);
					continue;
				}//if
				res = res + str[i];
			}//for
			return res;
		};//static CString BreakLines(CString str)
	};//class local
	/// Отображаем текст в панели помощи

	CString str = local::BreakLines(text);
	m_Edit.SetWindowText(str);
	//CString str = _T("dadsfadsf\15\12radsfas\15\12asdfasf\15\12asdfasdf\15\12asdfasfd");
	//m_Edit.SetWindowText(str);
	//str.GetLength();
	/// Метод Bitmap::FromFile может загружать картинку из файла,
	/// файл должен быть в формате wchar_t*
	/// Проект построен без использования юникода.

	/// 0 - получаем путь к каталогу файлов программы в wchar_t*
	char *cWorkPath = theApp.workPath.GetBuffer();
	wchar_t *cwWorkPath = new wchar_t[strlen(cWorkPath)+1];
	for(size_t i = 0; i < strlen(cWorkPath); i++)
	{
		cwWorkPath[i] = cWorkPath[i];
	}//for
	cwWorkPath[strlen(cWorkPath)] = L'\0';

	/// 1 - получаем имя файла рисунка в строку wchar_t*
	char *cPicName = PicName.GetBuffer();
	wchar_t *wcPicName = new wchar_t[strlen(cPicName)+1]; //(wchar_t*)malloc(sizeof(wchar_t)*strlen(str2)+1);
	for(size_t i = 0; i < strlen(cPicName); i++)
	{
		wcPicName[i] = cPicName[i];
	}//for
	wcPicName[strlen(cPicName)] = L'\0';

	/// Получаем путь до рисунка в строку wchar_t*
	wchar_t *wTemp4 = L"Settings\\pictures\\";
	wchar_t *wPath = new wchar_t[theApp.workPath.GetLength() + 1 + wcslen(wTemp4)+PicName.GetLength()+1];
	wcscpy(wPath, cwWorkPath);
	wPath = wcscat(wPath, wTemp4);
	wPath = wcscat(wPath, wcPicName);

	/// Загружаем картинку из файла
	if(m_pBitmap != NULL)
		delete m_pBitmap;
	m_pBitmap = Bitmap::FromFile(wPath);
	if(m_pBitmap == NULL)
	{
		CString str; str.LoadString(IDS_ERROR_PICTURE_LOADING);
		AfxMessageBox(str);
		delete [] wcPicName;
		delete [] wPath;
		delete [] cwWorkPath;
		return;
	}//if
	imageHeight = m_pBitmap->GetHeight();
	imageWidth = m_pBitmap->GetWidth();

	this->RedrawWindow();

	//Bitmap *pBitmap = Bitmap::FromFile(wPath);
	//if(pBitmap == NULL)
	//{
	//	CString str; str.LoadString(IDS_ERROR_PICTURE_LOADING);
	//	AfxMessageBox(str);
	//	delete [] wcPicName;
	//	delete [] wPath;
	//	delete [] cwWorkPath;
	//	return;
	//}//if
	//width = pBitmap->GetWidth();
	//height = pBitmap->GetHeight();
	//HBITMAP hBitmap = NULL;
	//pBitmap->GetHBITMAP(Color(0,0,0), &hBitmap);

	///** Инициализация gdi */
	//GdiplusStartupInput gdiplusStartupInput;
	//ULONG_PTR gdiplusToken;
	//::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	///**********************/

	//m_Static.SetBitmap(hBitmap);

	AdjustLayout();

	/// Освобождаем выделенную память.
	delete [] wcPicName;
	delete [] wPath;
	delete [] cwWorkPath;
}//void CNCUnitHelpPane::ShowHelp(CString text, CString PicName)

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CNCUnitHelpPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	// Load view images:
	//m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));

	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	//m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	//m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	if(m_pBitmap != NULL)
		delete m_pBitmap;
	m_pBitmap = Bitmap::FromFile(theApp.workPath + L"Settings\\pictures\\test.jpg");
	if(m_pBitmap == NULL)
	{
		CString str; str.LoadString(IDS_ERROR_PICTURE_LOADING);
		AfxMessageBox(str);
	}//if
	imageWidth = m_pBitmap->GetWidth();
	imageHeight = m_pBitmap->GetHeight();

	//Bitmap *pBitmap = Bitmap::FromFile(theApp.workPath + L"Settings\\pictures\\test.jpg");
	//if(pBitmap == NULL)
	//{
	//	CString str; str.LoadString(IDS_ERROR_PICTURE_LOADING);
	//	AfxMessageBox(str);
	//}//if
	//width = pBitmap->GetWidth();
	//height = pBitmap->GetHeight();
	//HBITMAP hBitmap = NULL;
	//pBitmap->GetHBITMAP(Color(0,0,0), &hBitmap);
	//CRect rect(0,0,100,100);
	//m_Static.Create(NULL , WS_CHILD | WS_VISIBLE| SS_BITMAP, rect, this);
	//m_Static.SetBitmap(hBitmap);

	CRect rect(0,0,100,100);
	m_Edit.Create(WS_CHILD | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | WS_BORDER | WS_TABSTOP, rect, this, 0);
	
	// Fill in some static tree view data (dummy code, nothing magic here)
	AdjustLayout();

	return 0;
}

void CNCUnitHelpPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CNCUnitHelpPane::OnContextMenu(CWnd* pWnd, CPoint point)
{
}

void CNCUnitHelpPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int windowHeight = rectClient.Height(); // - rectClient.top;

	int pictureWidth;
	if((imageWidth != 0) && (imageHeight != 0))
		pictureWidth = (imageWidth * windowHeight) / imageHeight;
	else
		pictureWidth = 0;

	//m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_Edit.SetWindowPos(NULL, rectClient.left+pictureWidth, rectClient.top, rectClient.right-pictureWidth, rectClient.bottom, SWP_NOACTIVATE | SWP_NOZORDER);
	//m_Static.SetWindowPos(NULL, rectClient.left+ rectClient.Width()-pictureWidth, rectClient.top, pictureWidth, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	//m_Static.RedrawWindow();

	this->RedrawWindow();
}

void CNCUnitHelpPane::OnProperties()
{
}

void CNCUnitHelpPane::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CNCUnitHelpPane::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CNCUnitHelpPane::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CNCUnitHelpPane::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CNCUnitHelpPane::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CNCUnitHelpPane::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CNCUnitHelpPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//************************************************************************************
	// Код из шаблона, использовался для отрисовки рамки вокруг контрола m_wndFileView

	//CRect rectTree;
	//m_wndFileView.GetWindowRect(rectTree);
	//ScreenToClient(rectTree);  /// Конвертирование координат из экранных в клиентские

	//rectTree.InflateRect(1, 1); /// Функция раздвигает стороны от центра
	//dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));

	//************************************************************************************

	//Bitmap *pBitmap = Bitmap::FromFile(theApp.workPath + L"Settings\\pictures\\test.jpg");
	//if(pBitmap == NULL)
	//{
	//	CString str; str.LoadString(IDS_ERROR_PICTURE_LOADING);
	//	AfxMessageBox(str);
	//}//if
	//width = pBitmap->GetWidth();
	//height = pBitmap->GetHeight();
	//HBITMAP hBitmap = NULL;
	//pBitmap->GetHBITMAP(Color(0,0,0), &hBitmap);

	//dc.DrawState(CPoint(0,0),CSize(100,200),hBitmap, DSS_NORMAL | DST_BITMAP);

	//CRgn *pRgn;
	//pRgn = new CRgn();
	//pRgn->CreateRectRgn(0,0,50,50);
	//CBrush cBr;
	//cBr.CreateSolidBrush(RGB(255,0,0));
	//dc.FillRgn(pRgn, &cBr);
	//dc.SelectObject(pRgn);

	/// Вычисление размеров изображения
	CRect rectClient;
	GetClientRect(rectClient);

	int pictureWidth;
	int pictureHeight;
	if(imageWidth != 0 && imageHeight != 0)
	{
		pictureWidth = (imageWidth * rectClient.Height()) / imageHeight;
		pictureHeight = rectClient.Height();
	}//if
	else
	{
		pictureWidth = 0;
		pictureHeight = 0;
	}//else

	if(m_pBitmap == NULL)
	{
		CString str; str.LoadString(IDS_ERROR_PICTURE_LOADING);
		AfxMessageBox(str);
	}//if

	Bitmap *tempBitmap = (Bitmap*)m_pBitmap->GetThumbnailImage(pictureWidth,pictureHeight);

	HBITMAP hBitmap = NULL;
	tempBitmap->GetHBITMAP(Color(0,0,0), &hBitmap);
	dc.DrawState(CPoint(0,0),CSize(pictureWidth,pictureHeight),hBitmap, DSS_NORMAL | DST_BITMAP);

	delete tempBitmap;
}

void CNCUnitHelpPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	//m_wndFileView.SetFocus();
}

void CNCUnitHelpPane::OnChangeVisualStyle()
{
	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	//m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	//m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	//m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	//m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


