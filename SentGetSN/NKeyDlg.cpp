// NKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NKey.h"
#include "sent.h"
#include "NKeyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNKeyDlg dialog




CNKeyDlg::CNKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNKeyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_hSN);
}

BEGIN_MESSAGE_MAP(CNKeyDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CNKeyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CNKeyDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CNKeyDlg message handlers

BOOL CNKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//  when the application's main window is not a dialog
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNKeyDlg::OnPaint()
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
HCURSOR CNKeyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNKeyDlg::OnBnClickedOk() 
{
}

void CNKeyDlg::OnBnClickedButton1()
{
	m_hSN.SetWindowText("Dongle not found!");
	RB_SPRO_APIPACKET ApiPacket;
	mng = new HK_MNG;
	memset(&ApiPacket, 0, sizeof(ApiPacket));
	if(RNBOsproFormatPacket(&ApiPacket, sizeof(ApiPacket))==SP_SUCCESS)
		if(RNBOsproInitialize(&ApiPacket)==SP_SUCCESS)
			if(RNBOsproFindFirstUnit(&ApiPacket, DEV_ID)/*:TST:*/==SP_SUCCESS)
			{
				if(mng->ReadSN() == SP_SUCCESS)
				{
					CString str;
					str.Format("%d", mng->GetKEY_SN());
					m_hSN.SetWindowText(str);
				}
			}
	UpdateData(FALSE);
}
