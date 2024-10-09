
// RemoteControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "NFloatEdit.h"
#include "BPoint.h"
#include "BMatr.h"
#include "RemoteCommands.h"
#include "WM_APP_Messages.h"
#include "RemoteControl.h"
#include "RemoteControlDlg.h"
const double PI = 3.14159265358979;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const COLORREF TranspColor = RGB(0,255,0);
const int ADD_DIST = 5;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX_RC };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRemoteControlDlg dialog




CRemoteControlDlg::CRemoteControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteControlDlg::IDD, pParent)
	, CadrText(_T(""))
	, m_X(0)
	, m_Y(0)
	, m_Z(0)
	, m_I(0)
	, m_J(0)
	, m_K(1)
	, m_Axes(0)
	, m_Variant(0)
	, m_SecondDecision(0)
	, m_FromPick(0)
	, m_XZ(0)
	, m_YZ(0)
	, m_AbsA(0)
	, m_AbsC(0)
	, m_RotPart(0)
	, m_IncrA(0)
	, m_IncrC(0)
	, m_ShortDist(1)
	, m_Delta(0)
	, m_ActiveCS(1)
	, m_FullSize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CADRTEXT, CadrText);
	DDV_MaxChars(pDX, CadrText, 1024);
	NFloatEdit::DDX_Text_my(pDX, IDC_X, m_X);
	NFloatEdit::DDX_Text_my(pDX, IDC_Y, m_Y);
	NFloatEdit::DDX_Text_my(pDX, IDC_Z, m_Z);
	NFloatEdit::DDX_Text_my(pDX, IDC_I, m_I);
	NFloatEdit::DDX_Text_my(pDX, IDC_J, m_J);
	NFloatEdit::DDX_Text_my(pDX, IDC_K, m_K);
	DDX_Text(pDX, IDC_XZ, m_XZ);
	DDV_MinMaxDouble(pDX, m_XZ, -89.9, 89.9);
	DDX_Text(pDX, IDC_YZ, m_YZ);
	DDV_MinMaxDouble(pDX, m_YZ, -89.9, 89.9);
	DDX_Text(pDX, IDC_ABS_A, m_AbsA);
	DDV_MinMaxDouble(pDX, m_AbsA, 0., 180.);
	DDX_Text(pDX, IDC_ABS_C, m_AbsC);
	DDV_MinMaxDouble(pDX, m_AbsC, -180., 180.);
	DDX_Radio(pDX, IDC_ROTAB, m_Axes);
	DDX_Radio(pDX, IDC_VECTOR, m_Variant);
	DDX_Check(pDX, IDC_WCS, m_ActiveCS);
	DDX_Check(pDX, IDC_SOLUTION, m_SecondDecision);
	DDX_Check(pDX, IDC_FROM_PICK, m_FromPick);
	DDX_Check(pDX, IDC_ROTPART, m_RotPart);
	DDX_Check(pDX, IDC_SHORTDIST, m_ShortDist);
	DDX_Text(pDX, IDC_INCR_A, m_IncrA);
	DDV_MinMaxDouble(pDX, m_IncrA, 0., 90.);
	DDX_Text(pDX, IDC_INCR_C, m_IncrC);
	DDV_MinMaxDouble(pDX, m_IncrC, -180., 180.);
	DDX_Text(pDX, IDC_DELTA, m_Delta);
	DDX_Control(pDX, IDC_KADRLIST, m_KadrList_Ctrl);
	DDX_Control(pDX, IDC_CADRTEXT, m_CadrText);
	DDX_Control(pDX, IDC_SELECT, m_PickButton);
	DDX_Check(pDX, IDC_FULL_SIZE, m_FullSize);
}

BEGIN_MESSAGE_MAP(CRemoteControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GO, &CRemoteControlDlg::OnBnClickedGo)
	ON_BN_CLICKED(IDC_STOP, &CRemoteControlDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_RESET, &CRemoteControlDlg::OnBnClickedReset)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_GO_ABS, &CRemoteControlDlg::OnBnClickedGoAbs)
	ON_BN_CLICKED(IDC_VECTOR, &CRemoteControlDlg::OnBnClickedVector)
	ON_BN_CLICKED(IDC_PROJECT, &CRemoteControlDlg::OnBnClickedProject)
	ON_BN_CLICKED(IDC_TWOANGL, &CRemoteControlDlg::OnBnClickedTwoangl)
	ON_BN_CLICKED(IDC_GO_INCR, &CRemoteControlDlg::OnBnClickedGoIncr)
	ON_BN_CLICKED(IDC_GO_DELTA, &CRemoteControlDlg::OnBnClickedGoDelta)
	ON_BN_CLICKED(IDC_GO_PR, &CRemoteControlDlg::OnBnClickedGoPr)
	ON_BN_CLICKED(IDC_CLEAR_PR, &CRemoteControlDlg::OnBnClickedClearPr)
	ON_BN_CLICKED(IDC_SELECT, &CRemoteControlDlg::OnBnClickedSelect)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_FROM_PICK, &CRemoteControlDlg::OnBnClickedFromPick)
	ON_BN_CLICKED(IDC_FULL_SIZE, &CRemoteControlDlg::OnBnClickedFullSize)
END_MESSAGE_MAP()


// CRemoteControlDlg message handlers

BOOL CRemoteControlDlg::OnInitDialog()
{
	GetWindowRect(FullRect);

	m_FullSize = theApp.GetProfileInt(theApp.Version, _T("RCFullSize"), 0);
	CRect Rect;
	GetWindowRect(Rect);
	ClientToScreen(Rect);
	if(!m_FullSize)
	{
		CRect TopRect;
		GetDlgItem(IDC_TOP_PART)->GetWindowRect(TopRect);
		ClientToScreen(TopRect);
		Rect.bottom = TopRect.bottom + ADD_DIST;
	}
	BOOL Result = ::SetWindowPos(m_hWnd, HWND_TOPMOST, Rect.left, Rect.top, Rect.Width(), Rect.Height(), SWP_SHOWWINDOW);

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	OnBnClickedVector();

	// Init PickButton
	bmp.DeleteObject();
	COLORMAP Map;
	Map.from = TranspColor;
	Map.to = GetSysColor(COLOR_3DFACE);
	bmp.LoadMappedBitmap(IDB_RDISP_OFF, 0, &Map, 1);
	BITMAP BitMap;
	bmp.GetBitmap(&BitMap);
	CRect rect;
	m_PickButton.GetWindowRect(rect);
	rect.right = rect.left + BitMap.bmWidth;
	rect.bottom = rect.top + BitMap.bmWidth;
	ScreenToClient(&rect);
	m_PickButton.MoveWindow(rect);
	m_PickButton.SetBitmap(HBITMAP(bmp));

	//
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	int Res = SendCommand(RC_Connect);

	if(Res == 0)
		AfxMessageBox(IDS_MSG_CONNECT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRemoteControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRemoteControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRemoteControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CRemoteControlDlg::Connect()
{
	return SendCommand(RC_Connect);
}
int CRemoteControlDlg::SendCommand(enum RemoteCommands Command, CString &Text/* = CString(_T(""))*/)
{
	COPYDATASTRUCT Data2Send;
	Data2Send.dwData = Command;
	Data2Send.cbData = Text.GetLength();
	Data2Send.lpData = ((Data2Send.cbData == 0) ? NULL : Text.GetBuffer());
	CString ClassName = theApp.GetProfileString(theApp.Version, _T("WindowClassName"));
	HWND DestHandle = ::FindWindow(ClassName, NULL);
	auto Res = ::SendMessage(DestHandle, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&Data2Send);
	return int(Res);
}

void CRemoteControlDlg::OnBnClickedGo()
{
	UpdateData(TRUE);

	LRESULT Res = SendCommand(RC_Empty, CadrText);
	if(Res != 0)
	{
		CString Text;
		m_KadrList_Ctrl.GetWindowText(Text);
		m_KadrList_Ctrl.SetWindowText(Text + CadrText + _T("\x0D\x0A"));
		m_KadrList_Ctrl.LineScroll(m_KadrList_Ctrl.GetLineCount());
	}
	UpdateData(FALSE);
}


INT_PTR CRemoteControlDlg::DoModal()
{
	return CDialog::DoModal();
}


void CRemoteControlDlg::OnBnClickedStop()
{
	SendCommand(RC_Stop);
}

void CRemoteControlDlg::OnBnClickedReset()
{
	SendCommand(RC_Reset);
}

void CRemoteControlDlg::MoveCommand(bool Plus, const CString &Name)
{
	CString Text(_T("F1000G1G91"));
	Text = Text + "\n" + (Plus ? "+" : "-") + Name + _T("100") + "\n" + (Plus ? "+" : "-") + Name + _T("100") + "\n" + (Plus ? "+" : "-") + Name + _T("100") + "\n" + (Plus ? "+" : "-") + Name + _T("100");
	SendCommand(RC_Stop, Text);
}
void CRemoteControlDlg::OnClose()
{
	int Res = SendCommand(RC_Disconnect);

	theApp.WriteProfileInt(theApp.Version, _T("RCFullSize"), m_FullSize);

	CDialog::OnClose();
}

void CRemoteControlDlg::OnOK()
{
	OnBnClickedGo();
}

void CRemoteControlDlg::OnBnClickedGoAbs()
{
	UpdateData(TRUE);

	LRESULT Res = SendCommand(RC_MakePlaceString, MakeGoAbsStr());
	if(Res == 0)
	{
		AfxMessageBox(IDS_MSG_CONNECT);
		return;
	}
	CadrText = theApp.GetProfileString(theApp.Version, _T("PlaceString"));
	UpdateData(FALSE);
	OnBnClickedGo();

}



void CRemoteControlDlg::OnBnClickedVector()
{
	GetDlgItem(IDC_NOTE_I)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NOTE_J)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NOTE_K)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_I)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_J)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_K)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NOTE_XZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_YZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_XZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_YZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_A)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_C)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ABS_A)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ABS_C)->ShowWindow(SW_HIDE);
	
}

void CRemoteControlDlg::OnBnClickedProject()
{
	GetDlgItem(IDC_NOTE_I)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_J)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_K)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_I)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_J)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_K)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_XZ)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NOTE_YZ)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_XZ)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_YZ)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NOTE_A)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_C)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ABS_A)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ABS_C)->ShowWindow(SW_HIDE);
}

void CRemoteControlDlg::OnBnClickedTwoangl()
{
	GetDlgItem(IDC_NOTE_I)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_J)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_K)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_I)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_J)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_K)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_XZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_YZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_XZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_YZ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NOTE_A)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NOTE_C)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ABS_A)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ABS_C)->ShowWindow(SW_SHOW);
}

void CRemoteControlDlg::OnBnClickedGoIncr()
{
	UpdateData(TRUE);

	CString Val, Text2Send(_T("X0Y0Z0"));
	Val.Format(_T("%lf"), m_IncrA);
	Text2Send += _T("I") + Val;
	Text2Send += _T("J0");
	Val.Format(_T("%lf"), m_IncrC);
	Text2Send += _T("K") + Val;
	switch(m_Axes)
	{
	case 0:
		Val = _T("L1");
		break;
	case 1:
		Val = _T("L3");
		break;
	case 2:
	default:
		Val = _T("L2");
		break;
	}
	Text2Send += Val;

	Text2Send += (m_ShortDist != 0) ? _T("W0") : _T("W4");
	Text2Send += (m_RotPart != 0) ? _T("U1") : _T("U0");
	Text2Send += _T("V1");

	LRESULT Res = SendCommand(RC_MakePlaceString, Text2Send);
	if(Res == 0)
	{
		AfxMessageBox(IDS_MSG_CONNECT);
		return;
	}
	CadrText = theApp.GetProfileString(theApp.Version, _T("PlaceString"));
	UpdateData(FALSE);
	OnBnClickedGo();

}

void CRemoteControlDlg::OnBnClickedGoDelta()
{
	UpdateData(TRUE);

	CadrText = MakeGoDeltaStr(m_Delta);
	if(CadrText.IsEmpty())
	{
		AfxMessageBox(IDS_MSG_CONNECT);
		return;
	}
	UpdateData(FALSE);
	OnBnClickedGo();
}


void CRemoteControlDlg::OnBnClickedGoPr()
{
	UpdateData(TRUE);
	int CharInd = m_KadrList_Ctrl.LineIndex(-1);
	if(CharInd < 0)
		return;
	int LineInd = 0;// This cycle needed because LineFromChar doesn't work
	for(; LineInd < m_KadrList_Ctrl.GetLineCount(); ++LineInd)
	{
		if(m_KadrList_Ctrl.LineIndex(LineInd) > CharInd)
			break;
	}
	--LineInd;
	if(LineInd < 0)
		return;
	char Cadr[256];
	memset(Cadr, 0x0, 256);
	m_KadrList_Ctrl.GetLine(LineInd, Cadr, 256);
	CadrText = Cadr;
	if(CadrText.IsEmpty())
		return;
	UpdateData(FALSE);
	OnBnClickedGo();
}

void CRemoteControlDlg::OnBnClickedClearPr()
{
	UpdateData(TRUE);
	m_KadrList_Ctrl.SetWindowText(_T(""));
	UpdateData(FALSE);
}

void CRemoteControlDlg::OnBnClickedSelect()
{
	bmp.DeleteObject();
	UINT res = (m_PickButton.GetCheck() == BST_CHECKED) ? IDB_RDISP_ON : IDB_RDISP_OFF;
	COLORMAP Map;
	Map.from = TranspColor;
	Map.to = GetSysColor(COLOR_3DFACE);
	bmp.LoadMappedBitmap(res, 0, &Map, 1);
	m_PickButton.SetBitmap(HBITMAP(bmp));

	BOOL Enable = (m_PickButton.GetCheck() != BST_CHECKED);

	GetDlgItem(IDC_X)->EnableWindow(Enable);
	GetDlgItem(IDC_Y)->EnableWindow(Enable);
	GetDlgItem(IDC_Z)->EnableWindow(Enable);
	//GetDlgItem(IDC_FROM_PICK)->EnableWindow(!Enable);
	GetDlgItem(IDC_WCS)->EnableWindow(Enable);
	//GetDlgItem(IDC_I)->EnableWindow(Enable);
	//GetDlgItem(IDC_J)->EnableWindow(Enable);
	//GetDlgItem(IDC_K)->EnableWindow(Enable);
	//GetDlgItem(IDC_VECTOR)->EnableWindow(Enable);
	//GetDlgItem(IDC_PROJECT)->EnableWindow(Enable);
	//GetDlgItem(IDC_TWOANGL)->EnableWindow(Enable);
	//GetDlgItem(IDC_SOLUTION)->EnableWindow(Enable);
	//GetDlgItem(IDC_XZ)->EnableWindow(Enable);
	//GetDlgItem(IDC_YZ)->EnableWindow(Enable);
	//GetDlgItem(IDC_ABS_A)->EnableWindow(Enable);
	//GetDlgItem(IDC_ABS_C)->EnableWindow(Enable);

	UpdateData(TRUE);
	CString Text2Send(MakeGoAbsStr());

	LRESULT Res = SendCommand(RC_WaitPoint, Text2Send);
}

CString CRemoteControlDlg::MakeGoAbsStr()
{
	BPoint Dir(0., 0., 1., 0.);
	switch(m_Variant)
	{
	case 0:
		Dir.Set(m_I, m_J, m_K, 0.);
		break;
	case 1:
		Dir = Dir * BMatr().RotY(0., 0., 0., m_XZ);
		Dir.SetY(-tan(m_YZ * PI / 180.) * Dir.GetZ());
		break;
	case 2:
		Dir = Dir
			* BMatr().RotY(0., 0., 0., m_AbsA)
			* BMatr().RotZ(0., 0., 0., m_AbsC);
		break;

	}
	CString Val, Text2Send;
	Val.Format(_T("%lf"), m_X);
	Text2Send += _T("X") + Val;
	Val.Format(_T("%lf"), m_Y);
	Text2Send += _T("Y") + Val;
	Val.Format(_T("%lf"), m_Z);
	Text2Send += _T("Z") + Val;
	Val.Format(_T("%lf"), Dir.GetX());
	Text2Send += _T("I") + Val;
	Val.Format(_T("%lf"), Dir.GetY());
	Text2Send += _T("J") + Val;
	Val.Format(_T("%lf"), Dir.GetZ());
	Text2Send += _T("K") + Val;
	switch(m_Axes)
	{
	case 0:
		Val = _T("1");
		break;
	case 1:
		Val = _T("3");
		break;
	case 2:
	default:
		Val = _T("2");
		break;
	}
	Text2Send += _T("L") + Val;

	Text2Send += (m_SecondDecision != 0) ? _T("W2") : _T("W1");
	Text2Send += (m_RotPart != 0) ? _T("U1") : _T("U0");
	Text2Send += (m_ActiveCS != 0) ? _T("V3") : _T("V0");
	return Text2Send;
}

BOOL CRemoteControlDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	if(!pCopyDataStruct)
		return FALSE;
	double Nx, Ny, Nz;
	if(pCopyDataStruct->dwData == RC_Pick)
	{
		UpdateData(TRUE);
		PickStr = theApp.GetProfileString(theApp.Version, _T("PlaceString"));
		if(sscanf_s(PickStr, _T("X%lfY%lfZ%lfI%lfJ%lfK%lf"), &m_X, &m_Y, &m_Z, &Nx, &Ny, &Nz)
			!= 6)
			return FALSE;
		if(m_FromPick)
		{
			BPoint Dir(0., 0., 1., 0.);
			switch(m_Variant)
			{
			case 0:
				m_I = Nx;
				m_J = Ny;
				m_K = Nz;
				break;
			case 1:
				if(Nz == 0.)
				{
					m_XZ = 0.;
					m_YZ = 0.;
				}
				else
				{
					m_XZ = atan(Nx / Nz) * 180. / PI;
					m_YZ = -atan(Ny / Nz) * 180. / PI;
				}
				break;
			case 2:
				m_AbsA = acos(Nz / sqrt(Nx * Nx + Ny * Ny + Nz * Nz)) * 180. / PI;
				if(Nx == 0.)
					m_AbsC = Ny > 0. ? 90. : -90.;
				else
					m_AbsC = atan(Ny / Nx) * 180. / PI;
				break;

			}
		}
		UpdateData(FALSE);
		return TRUE;
	}
	return FALSE;
}

CString CRemoteControlDlg::MakeGoDeltaStr(double Delta)
{

	CString Val, Text2Send(_T("X0Y0Z0I0J0"));
	Val.Format(_T("%lf"), Delta);
	Text2Send += _T("K") + Val;

	Text2Send += _T("L1W0U0V2");

	LRESULT Res = SendCommand(RC_MakePlaceString, Text2Send);
	if(Res == 0)
		return _T("");

	return theApp.GetProfileString(theApp.Version, _T("PlaceString"));
}

void CRemoteControlDlg::OnBnClickedFromPick()
{
	UpdateData(TRUE);
	BOOL Enable = (m_FromPick == 0) ? TRUE : FALSE;
	GetDlgItem(IDC_NOTE_I)->EnableWindow(Enable);
	GetDlgItem(IDC_NOTE_J)->EnableWindow(Enable);
	GetDlgItem(IDC_NOTE_K)->EnableWindow(Enable);
	GetDlgItem(IDC_I)->EnableWindow(Enable);
	GetDlgItem(IDC_J)->EnableWindow(Enable);
	GetDlgItem(IDC_K)->EnableWindow(Enable);
	GetDlgItem(IDC_NOTE_XZ)->EnableWindow(Enable);
	GetDlgItem(IDC_NOTE_YZ)->EnableWindow(Enable);
	GetDlgItem(IDC_XZ)->EnableWindow(Enable);
	GetDlgItem(IDC_YZ)->EnableWindow(Enable);
	GetDlgItem(IDC_NOTE_A)->EnableWindow(Enable);
	GetDlgItem(IDC_NOTE_C)->EnableWindow(Enable);
	GetDlgItem(IDC_ABS_A)->EnableWindow(Enable);
	GetDlgItem(IDC_ABS_C)->EnableWindow(Enable);
	UpdateData(FALSE);
}
void CRemoteControlDlg::SetDlgSize(void)
{
	CRect Rect;
	GetWindowRect(Rect);
	if(m_FullSize)
	{
		Rect.bottom = Rect.top + FullRect.Height();
	}
	else
	{
		CRect TopRect;
		GetDlgItem(IDC_TOP_PART)->GetWindowRect(TopRect);
		Rect.bottom = TopRect.bottom + ADD_DIST;
	}
	MoveWindow(Rect);
}

void CRemoteControlDlg::OnBnClickedFullSize()
{
	UpdateData(TRUE);
	SetDlgSize();
}
