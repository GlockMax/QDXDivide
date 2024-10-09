// QMeasureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "BConst.h"
#include "WM_APP_Messages.h"
#include "QMeasureDlg.h"




const COLORREF TranspColor = RGB(0,255,0);
// QMeasureDlg dialog

IMPLEMENT_DYNAMIC(QMeasureDlg, CDialog)
QMeasureDlg::QMeasureDlg(CWnd* pParent, UINT id)
	: CDialog(id, pParent)
	, pQMeasure(NULL)
{
}

QMeasureDlg::~QMeasureDlg()
{
}

void QMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULT, MeasureResult);
	DDX_Control(pDX, IDC_FIRST_X, XFirstC);
	DDX_Control(pDX, IDC_FIRST_Y, YFirstC);
	DDX_Control(pDX, IDC_FIRST_Z, ZFirstC);
	DDX_Control(pDX, IDC_THIRD_X, XThirdC);
	DDX_Control(pDX, IDC_THIRD_Y, YThirdC);
	DDX_Control(pDX, IDC_THIRD_Z, ZThirdC);
	DDX_Control(pDX, IDC_BIND, m_Bind);
	DDX_Control(pDX, IDC_BACK, m_Back);
	DDX_Control(pDX, IDC_END_POINT, m_EndPoint);
	DDX_Control(pDX, IDC_CALC, m_Calc);
	DDX_Control(pDX, IDC_GOTO_LINE, m_Goto);
	DDX_Control(pDX, IDC_LINE_NUMBER_STEP, m_Cadr);
	DDX_Control(pDX, IDC_PROGRAM_NAME, m_ProgName);

	DDX_Text(pDX, IDC_LINE_NUMBER_STEP, Cadr);
	DDX_Text(pDX, IDC_PROGRAM_NAME, ProgName);
}


BEGIN_MESSAGE_MAP(QMeasureDlg, CDialog)

	ON_BN_CLICKED(IDC_CALC, OnBnClickedCalc)
	ON_BN_CLICKED(IDC_BACK, OnBnClickedBack)
	ON_BN_CLICKED(IDC_GOTO_LINE, OnBnClickedGotoLine)
END_MESSAGE_MAP()


void QMeasureDlg::SetQMeasure(QMeasure * parent)
{
	pQMeasure=parent;
}
void QMeasureDlg::OnCancel()
{
	pQMeasure->DeActivate();
}

void QMeasureDlg::OnBnClickedCalc()
{
	pQMeasure->CalcManual();
	ReDraw();
}
void QMeasureDlg::SetPoint(BPoint point)
{
	str.Format((MM_INCH == M_MM) ? IDS_OUTPUT_FORMAT : IDS_OUTPUT_FORMAT_INCH,point.GetX());
	XFirstC.SetWindowText(str);
	str.Format((MM_INCH == M_MM) ? IDS_OUTPUT_FORMAT : IDS_OUTPUT_FORMAT_INCH,point.GetY());
	YFirstC.SetWindowText(str);
	str.Format((MM_INCH == M_MM) ? IDS_OUTPUT_FORMAT : IDS_OUTPUT_FORMAT_INCH,point.GetZ());
	ZFirstC.SetWindowText(str);

}
void QMeasureDlg::SetVector(BPoint vector)
{
	str.Format((MM_INCH == M_MM) ? IDS_OUTPUT_FORMAT : IDS_OUTPUT_FORMAT_INCH,vector.GetX());
	XThirdC.SetWindowText(str);
	str.Format((MM_INCH == M_MM) ? IDS_OUTPUT_FORMAT : IDS_OUTPUT_FORMAT_INCH,vector.GetY());
	YThirdC.SetWindowText(str);
	str.Format((MM_INCH == M_MM) ? IDS_OUTPUT_FORMAT : IDS_OUTPUT_FORMAT_INCH,vector.GetZ());
	ZThirdC.SetWindowText(str);
}
void QMeasureDlg::SetNotDefine(int Num)
{
	switch(Num) 
	{
	case 1:
		str.Format(IDS_BEGIN_POINT_NDEF);
		m_EndPoint.SetWindowText(str);	
		str.Format((MM_INCH == M_MM) ? IDS_1RESULT : IDS_1RESULT_INCH,0.0);
		MeasureResult.SetWindowText(str);	
		break;
	case 2:
		str.Format(IDS_END_POINT_NDEF);
		m_EndPoint.SetWindowText(str);
		str.Format((MM_INCH == M_MM) ? IDS_1RESULT : IDS_1RESULT_INCH,0.0);
		MeasureResult.SetWindowText(str);		
		break;
	}
}
BPoint QMeasureDlg::GetPoint(void)
{
	double x,y,z;
	XFirstC.GetWindowText(str);		x=atof(str.GetString());
	YFirstC.GetWindowText(str);		y=atof(str.GetString());
	ZFirstC.GetWindowText(str);		z=atof(str.GetString());
	BPoint p(x,y,z,1);
	return p;
}
BPoint QMeasureDlg::GetVector(void)
{
	double x,y,z;
	XThirdC.GetWindowText(str);		x=atof(str.GetString());
	YThirdC.GetWindowText(str);		y=atof(str.GetString());
	ZThirdC.GetWindowText(str);		z=atof(str.GetString());
	BPoint p(x,y,z,0);
	return p;
}


void QMeasureDlg::ReDraw(void)
{
	AfxGetMainWnd()->SendMessage(WM_APP_UPDATE_PANES, N_OGL);
	RedrawWindow();
}


void QMeasureDlg::OnBnClickedBack()
{

	bmp.DeleteObject();
	UINT res = m_Back.GetCheck() == BST_CHECKED ? IDB_SIZE_IN : IDB_SIZE_OUT;
	COLORMAP Map;
	Map.from = TranspColor;
	Map.to = GetSysColor(COLOR_3DFACE);
	bmp.LoadMappedBitmap(res, 0, &Map, 1);
	m_Back.SetBitmap(HBITMAP(bmp));	
}

void QMeasureDlg::OnBnClickedGotoLine()
{
	pQMeasure->GotoCurrentCadr();
}

BOOL QMeasureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	bmp.DeleteObject();
	COLORMAP Map;
	Map.from = TranspColor;
	Map.to = GetSysColor(COLOR_3DFACE);
	bmp.LoadMappedBitmap(IDB_SIZE_OUT, 0, &Map, 1);
	m_Back.SetBitmap(HBITMAP(bmp));
	m_Calc.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON1));
	CadrID = cadrID(-1);

	//Create the ToolTip control
	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{
		// Add tool tips to the controls, either by hard coded string 
		// or using the string table resource
		m_ToolTip.AddTool(&m_Bind, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&m_Back, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&m_Goto, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&m_Calc, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&MeasureResult, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&XFirstC, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&YFirstC, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&ZFirstC, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&XThirdC, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&YThirdC, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&ZThirdC, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&m_EndPoint, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&m_Cadr, IDS_END_POINT_NDEF);// change ID
		m_ToolTip.AddTool(&m_ProgName, IDS_END_POINT_NDEF);// change ID

		m_ToolTip.Activate(TRUE);
	}
	m_Goto.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void QMeasureDlg::OnOK()
{
	// To prevent closing when enter key pressed

//	CDialog::OnOK();
}


BOOL QMeasureDlg::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}
