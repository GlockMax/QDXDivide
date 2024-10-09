// NCMFileInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NCMFileInfo.h"
#include "NCMFileInfoDlg.h"
#include "DlgProxy.h"
#include "NInfoFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


interface INCMGetData : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetProgramsNum  (long* pNum) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetUnitName (BSTR* ppbstrMessage) = 0;
	virtual HRESULT STDMETHODCALLTYPE LoadFile (BSTR* ppbstrMessage) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProgWLength(long ProgNum, double *Length) = 0;
	virtual HRESULT STDMETHODCALLTYPE Clear() = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProgName(long ProgNum, BSTR *Name) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProgComment(long ProgNum, BSTR* Text) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProgFLength(long ProgNum, double *Length) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProgTime(long ProgNum, double *Time) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProgLines(long ProgNum, long *Count) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProgSteps(long ProgNum, long *Count) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetToolsNum(long *pNum) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetUnitFile(BSTR *pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetUnitType(BSTR *pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetUnitMan(BSTR *pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetUnitOper(BSTR *pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetUnitAxes(BSTR *pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockStlName(BSTR *pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockType(BSTR * pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockWidth(double *pSize) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockHeight(double *pSize) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockLength(double *pSize) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockX(double *pCoord) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockY(double *pCoord) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockZ(double *pCoord) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockRotX(double *pCoord) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockRotY(double *pCoord) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockRotZ(double *pCoord) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStockBase(long *pNum) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetToolWLength(long ToolNum, double *pVal) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetToolFLength(long ToolNum, double *pVal) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetToolTime(long ToolNum, double *pVal) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetToolName(long ToolNum, BSTR *pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetToolType(long ToolNum, BSTR *pText) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetToolVolume(long ToolName, double *pVal) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetToolPos(long ToolNum, long *pNum) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNCMFileInfoDlg dialog

IMPLEMENT_DYNAMIC(CNCMFileInfoDlg, CDialog);

CNCMFileInfoDlg::CNCMFileInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNCMFileInfoDlg::IDD, pParent)
{  
	//{{AFX_DATA_INIT(CNCMFileInfoDlg)
	m_ProgNum = 0;
	m_ToolNum = 0;
	m_UnitDescr = _T("");
	m_StockDescr = _T("");
	m_FileName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CNCMFileInfoDlg::~CNCMFileInfoDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CNCMFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNCMFileInfoDlg)
	DDX_Control(pDX, IDC_TOOLS, m_ToolsCtrl);
	DDX_Control(pDX, IDC_PROGRAMS, m_ProgramsCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_ProgNum);
	DDX_Text(pDX, IDC_EDIT2, m_ToolNum);
	DDX_Text(pDX, IDC_EDIT3, m_UnitDescr);
	DDX_Text(pDX, IDC_EDIT4, m_StockDescr);
	DDX_Text(pDX, IDC_EDIT5, m_FileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNCMFileInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CNCMFileInfoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNCMFileInfoDlg message handlers

BOOL CNCMFileInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

// connect to interface
	CLSID clsid;
	IID   iid;   
	HRESULT hRes;
	hRes = AfxGetClassIDFromString(
                             "{8820E928-C1F1-11D5-B0AB-0050BF4A8273}", 
                             &clsid);
   
	if(!SUCCEEDED(hRes))
		return FALSE;
	hRes = AfxGetClassIDFromString("{8820E927-C1F1-11D5-B0AB-0050BF4A8273}", &iid);
	if(!SUCCEEDED(hRes))
		return FALSE;
	hRes = CoCreateInstance(clsid, NULL, CLSCTX_ALL, 
                                 iid, (void**)&m_pIServerData);
	if(!SUCCEEDED(hRes))
	{
		m_pIServerData = NULL;
		return FALSE;
	}
// Make tables	
	m_ProgramsCtrl.InsertColumn(0,"Имя",LVCFMT_LEFT,150,1);
	m_ProgramsCtrl.InsertColumn(1,"Комментарий",LVCFMT_LEFT,150,1);
	m_ProgramsCtrl.InsertColumn(2,"Cтрок",LVCFMT_LEFT,50,2);
	m_ProgramsCtrl.InsertColumn(3,"шагов",LVCFMT_LEFT,50,3);
	m_ProgramsCtrl.InsertColumn(4,"раб",LVCFMT_LEFT,50,4);
	m_ProgramsCtrl.InsertColumn(5,"хол",LVCFMT_LEFT,50,5);
	m_ProgramsCtrl.InsertColumn(6,"Время",LVCFMT_LEFT,50,6);

	m_ToolsCtrl.InsertColumn(0,"Имя",LVCFMT_LEFT,150,1);
	m_ToolsCtrl.InsertColumn(1,"Комментарий",LVCFMT_LEFT,150,1);
	m_ToolsCtrl.InsertColumn(2,"Cтрок",LVCFMT_LEFT,50,2);
	m_ToolsCtrl.InsertColumn(3,"шагов",LVCFMT_LEFT,50,3);
	m_ToolsCtrl.InsertColumn(4,"раб",LVCFMT_LEFT,50,4);
	m_ToolsCtrl.InsertColumn(5,"хол",LVCFMT_LEFT,50,5);
	m_ToolsCtrl.InsertColumn(6,"Время",LVCFMT_LEFT,50,6);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNCMFileInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNCMFileInfoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNCMFileInfoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CNCMFileInfoDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CNCMFileInfoDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CNCMFileInfoDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CNCMFileInfoDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void CNCMFileInfoDlg::Update()
{
	CString Message;
	BSTR bMessage;

//Update unit info
	if(m_pIServerData->GetUnitFile(&bMessage) != S_OK)
		return;
	Message = bMessage;
	::SysFreeString(bMessage);
	m_UnitDescr = Message;

	if(m_pIServerData->GetUnitName(&bMessage) != S_OK)
		return;
	Message = bMessage;
	::SysFreeString(bMessage);
	m_UnitDescr = m_UnitDescr+" | " + Message;

	if(m_pIServerData->GetUnitType(&bMessage) != S_OK)
		return;
	Message = bMessage;
	::SysFreeString(bMessage);
	m_UnitDescr = m_UnitDescr+" | " + Message;

	if(m_pIServerData->GetUnitMan(&bMessage) != S_OK)
		return;
	Message = bMessage;
	::SysFreeString(bMessage);
	m_UnitDescr = m_UnitDescr+" | " + Message;

	if(m_pIServerData->GetUnitOper(&bMessage) != S_OK)
		return;
	Message = bMessage;
	::SysFreeString(bMessage);
	m_UnitDescr = m_UnitDescr+" | " + Message;

	if(m_pIServerData->GetUnitAxes(&bMessage) != S_OK)
		return;
	Message = bMessage;
	::SysFreeString(bMessage);
	m_UnitDescr = m_UnitDescr+" | " + Message;

	long ltmp;
	char ctmp[64];
	double dnum;
	long num;
//Update programs info
	m_ProgramsCtrl.DeleteAllItems();
	if(m_pIServerData->GetProgramsNum(&ltmp) != S_OK)
		return;
	m_ProgNum = ltmp;
	for(int i=0; i<m_ProgNum; ++i)
	{
		if(m_pIServerData->GetProgName(i,&bMessage) != S_OK)
			return;
		Message = bMessage;
		::SysFreeString(bMessage);
		m_ProgramsCtrl.InsertItem(i,Message);
		
		if(m_pIServerData->GetProgComment(i,&bMessage) != S_OK)
			return;
		Message = bMessage;
		::SysFreeString(bMessage);
		m_ProgramsCtrl.SetItemText(i,1,Message);
		
		if(m_pIServerData->GetProgLines(i,&num) != S_OK)
			return;
		m_ProgramsCtrl.SetItemText(i,2,ltoa(num,ctmp,10));	

		if(m_pIServerData->GetProgSteps(i,&num) != S_OK)
			return;
		m_ProgramsCtrl.SetItemText(i,3,ltoa(num,ctmp,10));	

		if(m_pIServerData->GetProgWLength(i,&dnum) != S_OK)
			return;
		m_ProgramsCtrl.SetItemText(i,4,gcvt(dnum,6,ctmp));	

		if(m_pIServerData->GetProgFLength(i,&dnum) != S_OK)
			return;
		m_ProgramsCtrl.SetItemText(i,5,gcvt(dnum,6,ctmp));
		
		if(m_pIServerData->GetProgTime(i,&dnum) != S_OK)
			return;
		m_ProgramsCtrl.SetItemText(i,6,gcvt(dnum,6,ctmp));	
	}
//Update tools info
	m_ToolsCtrl.DeleteAllItems();
	if(m_pIServerData->GetToolsNum(&ltmp) != S_OK)
		return;
	m_ToolNum = ltmp;
	for(int j=0; j<m_ToolNum; ++j)
	{
		if(m_pIServerData->GetToolPos(j,&num) != S_OK)
			return;
		m_ToolsCtrl.InsertItem(j,ltoa(num,ctmp,10));	

		if(m_pIServerData->GetToolName(j,&bMessage) != S_OK)
			return;
		Message = bMessage;
		::SysFreeString(bMessage);
		m_ToolsCtrl.SetItemText(j,1,Message);

		if(m_pIServerData->GetToolType(j,&bMessage) != S_OK)
			return;
		Message = bMessage;
		::SysFreeString(bMessage);
		m_ToolsCtrl.SetItemText(j,2,Message);

		if(m_pIServerData->GetToolTime(j,&dnum) != S_OK)
			return;
		m_ToolsCtrl.SetItemText(j,3,gcvt(dnum,6,ctmp));	

		if(m_pIServerData->GetToolVolume(j,&dnum) != S_OK)
			return;
		m_ToolsCtrl.SetItemText(j,4,gcvt(dnum,6,ctmp));	

		if(m_pIServerData->GetToolWLength(j,&dnum) != S_OK)
			return;
		m_ToolsCtrl.SetItemText(j,5,gcvt(dnum,6,ctmp));	

		if(m_pIServerData->GetToolFLength(j,&dnum) != S_OK)
			return;
		m_ToolsCtrl.SetItemText(j,6,gcvt(dnum,6,ctmp));	

	}

//Update stock info
	if(m_pIServerData->GetStockType(&bMessage) != S_OK)
		return;
	Message = bMessage;
	::SysFreeString(bMessage);
	m_StockDescr = Message;
	
	bMessage = NULL;
	if(m_pIServerData->GetStockStlName(&bMessage) != S_OK)
		return;
	Message = bMessage;
	::SysFreeString(bMessage);
	m_StockDescr = m_StockDescr + " | " + Message;
	
	if(m_pIServerData->GetStockWidth(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockLength(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockHeight(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockX(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockY(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockZ(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockRotX(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockRotY(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockRotZ(&dnum) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(gcvt(dnum,6,ctmp));

	if(m_pIServerData->GetStockBase(&num) != S_OK)
		return;
	m_StockDescr =  m_StockDescr + " | " + CString(itoa(num,ctmp,10));

	UpdateData(FALSE);

}

BOOL CNCMFileInfoDlg::DestroyWindow() 
{
	
	m_pIServerData->Clear();
	m_pIServerData->Release();
	m_pIServerData = NULL;
	return CDialog::DestroyWindow();
}

void CNCMFileInfoDlg::Update(const CString &fname)
{
	BSTR tmp = NULL;
	tmp = fname.AllocSysString();
	BOOL ret = (m_pIServerData->LoadFile(&tmp) == S_OK);
	::SysFreeString(tmp);
	m_FileName = tmp;
	if(!ret)
		return;
	Update();

}


