
// NetConfigEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetConfigEdit.h"
#include "NetConfigEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetConfigEditDlg dialog




CNetConfigEditDlg::CNetConfigEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetConfigEditDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME_NET);
}

void CNetConfigEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONFIG_LIST, m_ConfigList);
	DDX_Control(pDX, IDC_MOD, m_Modules);
}

BEGIN_MESSAGE_MAP(CNetConfigEditDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CNetConfigEditDlg message handlers

BOOL CNetConfigEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	m_ConfigList.Enabled.LoadString(IDS_MODULES_ENABLE);
	m_ConfigList.Disabled.LoadString(IDS_MODULES_DISABLE);

	CString Delim(_T("\n"));
	CString Modules;
	Modules.LoadString(IDS_MODULES);

	for(int i = 0; Modules.GetLength() > 1; ++i)
	{
		CString Line = Modules.SpanExcluding(Delim);
		m_Modules.AddString(Line);
		if((0x1 << i) & m_ConfigList.Config)
			m_ConfigList.AddString(m_ConfigList.Enabled);
		else
			m_ConfigList.AddString(m_ConfigList.Disabled);

		Modules.Delete(0, Line.GetLength() + 1);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetConfigEditDlg::OnPaint()
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
HCURSOR CNetConfigEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


INT_PTR CNetConfigEditDlg::DoModal(NCMConfig Conf)
{
	m_ConfigList.Config = Conf;
	return CDialog::DoModal();
}

void CNetConfigEditDlg::OnOK()
{
	m_ConfigList.Config = 0x0;
	for(int i = 0; i < m_ConfigList.GetCount(); ++i)
	{
		CString Text;
		m_ConfigList.GetText(i, Text);
		if(Text == m_ConfigList.Enabled)
			m_ConfigList.Config |= 0x1 << i;
	}

	CDialog::OnOK();
}
