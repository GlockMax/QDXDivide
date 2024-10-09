// ..\NCTool\NToolsCrBorePage.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "NTiParams.h"
#include "BConst.h"
#include "NCMProject.h"
#include "StContOut.h"
#include "NToolVert.h"
#include "NCadrGeom.h"
#include "NToolsCrBorePage.h"


// NToolsCrBorePage dialog

IMPLEMENT_DYNAMIC(NToolsCrBorePage, NToolsCrCommonPage)

NToolsCrBorePage::NToolsCrBorePage()
	: NToolsCrCommonPage(IDD_TOOLS_CREATE_BORE)
{
	m_displacement = 0.0;
	m_h1 = 0.0;
	IsNameAuto = false;
	m_name = "Bore";
	m_check_file = FALSE;
	m_check2 = TRUE;
	m_FlipXY = TRUE;
	CString Key = "Defaults/Tools/Mill/Bore";
	NTiParams &Par = NCM_PROJECT.Defaults;

	m_a = int(Par.GetDouble((Key + "@A").GetBuffer(), 0));
	if (MM_INCH == M_MM)
	{
		Key += "/mm";
		m_D = Par.GetDouble((Key + "@D").GetBuffer(), 35.);
		m_H = Par.GetDouble((Key + "@H").GetBuffer(), 36.);
		m_h = Par.GetDouble((Key + "@H2").GetBuffer(), 1.);
		m_L = Par.GetDouble((Key + "@L").GetBuffer(), 35.);
		m_R = Par.GetDouble((Key + "@R").GetBuffer(), 0.5);
		m_W = Par.GetDouble((Key + "@W").GetBuffer(), 4.6);
	}
	else
	{
		Key += "/inch";
		m_D = Par.GetDouble((Key + "@D").GetBuffer(), 1.378);
		m_H = Par.GetDouble((Key + "@H").GetBuffer(), 1.4173);
		m_h = Par.GetDouble((Key + "@H2").GetBuffer(), 0.0394);
		m_L = Par.GetDouble((Key + "@L").GetBuffer(), 1.378);
		m_R = Par.GetDouble((Key + "@R").GetBuffer(), 0.0197);
		m_W = Par.GetDouble((Key + "@W").GetBuffer(), 0.1811);
	}
}

NToolsCrBorePage::~NToolsCrBorePage()
{
}

void NToolsCrBorePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CORONCA_DRAW, m_Draw);
	DDX_Control(pDX, IDC_CORONCA_DRAW2, m_Draw2);
	DDX_Control(pDX, IDC_DISK_NAME, m_name_ctrl);
	DDX_Control(pDX, IDC_BROWSE, m_browse_ctrl);
	DDX_Control(pDX, IDC_BORE_ALF, m_a_ctrl);
	DDX_Control(pDX, IDC_BORE_W, m_W_ctrl);
	DDX_Control(pDX, IDC_BORE_L, m_L_ctrl);
	DDX_Text(pDX, IDC_DISK_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
	DDX_Text(pDX, IDC_DISK_NAME, m_name);
	DDX_Text(pDX, IDC_FILE_NAME, m_FileName);
	DDX_Check(pDX, IDC__BORE_CHECK__FILE, m_check_file);
	DDX_Check(pDX, IDC__BORE_CHECK_BALLEND, m_check2);
	DDX_Text(pDX, IDC_BORE_DIAM, m_D);
	DDV_MinMaxDouble(pDX, m_D, 0., 500.);
	DDX_Text(pDX, IDC_BORE_H, m_H);
	DDV_MinMaxDouble(pDX, m_H, 0., 500.);
	DDX_Text(pDX, IDC_BORE_H2, m_h);
	DDV_MinMaxDouble(pDX, m_h, 0., 500.);
	DDX_Text(pDX, IDC_BORE_L, m_L);
	DDV_MinMaxDouble(pDX, m_L, 0., 500.);
	DDX_Text(pDX, IDC_BORE_W, m_W);
	DDV_MinMaxDouble(pDX, m_W, 0., 500.);
	DDX_Text(pDX, IDC_BORE_ALF, m_a);
	DDV_MinMaxDouble(pDX, m_a, 0., 90.);
	DDX_Text(pDX, IDC_BORE_R, m_R);
	DDV_MinMaxDouble(pDX, m_R, 0., 500.);
	DDX_Control(pDX, IDC_BORE_R, m_R_ctrl);
}


BEGIN_MESSAGE_MAP(NToolsCrBorePage, CPropertyPage)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
	ON_BN_CLICKED(IDC__BORE_CHECK__FILE, OnBnClickedCheckFile)
	ON_BN_CLICKED(IDC__BORE_CHECK_BALLEND, OnDiskCheck2)
	ON_EN_CHANGE(IDC_BORE_ALF, OnChangeAlf)
	ON_EN_CHANGE(IDC_BORE_W, OnChangeW)
	ON_EN_CHANGE(IDC_BORE_L, OnChangeL)
END_MESSAGE_MAP()

void NToolsCrBorePage::OnBnClickedCheckFile()
{
	UpdateData();
	m_browse_ctrl.EnableWindow(m_check_file);
	UpdateData(FALSE);
}

void NToolsCrBorePage::OnBnClickedBrowse()
{
	if (!UpdateData())
		return;
	CString FileName;
	StContOut Import;// ADD ROT AXIS
	NCadrGeom *GeomArr;
	int Num = Import.Load(GeomArr, FileName);
	if (Num < 0)
		return; // User refused
	if (Num == 0)
	{
		AfxMessageBox(IDS_STOCK_ERREAD);
		return;
	}
	if (m_FlipXY)
		NCadrGeom::FlipXY(GeomArr, Num);
	NContour ToolCont;
	ToolCont.MakeFromCadrGeom(GeomArr, Num);
	delete[] GeomArr;
	ToolCont.ExchangeYZ();
	pToolVert = NToolVert::MakeTool(m_name, FileName, ToolCont, true);
	if (pToolVert == nullptr)
	{
		AfxMessageBox(IDS_STOCK_WRONG_CONTOUR);// wrong contour
		return;
	}
	m_FileName = FileName;
	UpdateData(FALSE);
}

// NToolsCrBorePage message handlers
bool NToolsCrBorePage::IsOkInput()
{
	if (!UpdateData())
		return false;
	// проверки на введённие всех небходимых данных
	if (m_name == "")
	{
		AfxMessageBox(IDS_MES_T_NAME);
		return false;
	}
	//if (m_diam < 0. || m_h < 0. || m_alf < 0. || m_beta < 0.
	//	|| m_e < 0. || m_f < 0. || m_r < 0. || m_hm < 0)
	//{
	//	AfxMessageBox(IDS_MES_POSV);
	//	return false;
	//}
	return true;
}

void NToolsCrBorePage::IsError(int IDErr) const
{
	CString str;
	str.LoadString(IDS_MES_PAR);
	//CString tmp, tmps;
	//CString tmp1, tmp2;
	//switch (IDErr)
	//{
	//case 1:
	//	tmps.LoadString(IDS_MES_RAD);
	//	str += tmps;
	//	m_r_ctrl.SetFocus();
	//	m_r_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 2:
	//	tmps.LoadString(IDS_MES_ANG);
	//	str += tmps;
	//	if (PI - m_alf*PI / 180. < MINAR)
	//	{
	//		m_alf_ctrl.SetFocus();		m_alf_ctrl.SetSel(0, -1, FALSE);
	//	}
	//	else
	//	{
	//		m_beta_ctrl.SetFocus();		m_beta_ctrl.SetSel(0, -1, FALSE);
	//	}

	//	break;
	//case 3: // невыпуклый инструмент
	//	tmps.LoadString(IDS_MES_ANG);
	//	str += tmps;
	//	m_alf_ctrl.SetFocus();
	//	m_alf_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 4: // невыпуклый инструмент
	//	tmps.LoadString(IDS_MES_ANG);
	//	str += tmps;
	//	m_beta_ctrl.SetFocus();
	//	m_beta_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//case 6: // активная часть инструмента
	//	double a;
	//	CString tmp;
	//	int  decimal1, decimal2, sign;

	//	a = m_r*tan((PI / 2. - m_alf*PI / 180.) / 2);
	//	a = m_diam / 2.*tan(m_beta*PI / 180.) + a*cos(m_alf*PI / 180.);
	//	tmp1 = _fcvt(a, 3, &decimal1, &sign);
	//	tmp2 = _fcvt(m_h, 3, &decimal2, &sign);

	//	str.Format(IDS_MES_INT,
	//		tmp1.Mid(0, decimal1), tmp1.Mid(decimal1),
	//		tmp2.Mid(0, decimal2), tmp2.Mid(decimal2));
	//	m_hm_ctrl.SetFocus();
	//	m_hm_ctrl.SetSel(0, -1, FALSE);
	//	break;
	//}
	AfxMessageBox(str);
}

const CString& NToolsCrBorePage::GetName() const
{
	return m_name;
}

double NToolsCrBorePage::CalcTorusRad() const
{
	double Bot = m_W - 2. * m_L * tan(m_a * PI / 180.);
	return Bot / (2. * tan(PI / 4. - m_a * PI / 360.));
}

bool NToolsCrBorePage::IsTorusBottom() const
{
	double R = CalcTorusRad();

	return (fabs(R - m_R) < MIND);
}

BOOL NToolsCrBorePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	pToolVert = nullptr;
	m_browse_ctrl.EnableWindow(m_check_file);
	m_check2 = FALSE;
	if (IsTorusBottom())
	{
		m_check2 = TRUE;
		m_R_ctrl.EnableWindow(FALSE);
	}
	m_Draw.ShowWindow(!m_check2 ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow(m_check2 ? SW_SHOW : SW_HIDE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void NToolsCrBorePage::OnDiskCheck2()
{
	UpdateData();

	m_R_ctrl.EnableWindow(!m_check2);

	if (m_check2)
	{
		m_R = CalcTorusRad();
		m_R = floor(m_R * 100000.) / 100000.;
		UpdateData(FALSE);
	}

	m_Draw.ShowWindow(!m_check2 ? SW_SHOW : SW_HIDE);
	m_Draw2.ShowWindow(m_check2 ? SW_SHOW : SW_HIDE);

}

void NToolsCrBorePage::OnChangeAlf()
{
	if(m_a_ctrl.GetWindowTextLength() != 0)
		OnDiskCheck2();
}

void NToolsCrBorePage::OnChangeW()
{
	UpdateData();
	if (m_W_ctrl.GetWindowTextLength() != 0)
	{
		if (m_check2)
		{
			m_R = CalcTorusRad();
			m_R = floor(m_R * 100000.) / 100000.;
			CString str;
			str.Format("%f", m_R);
			m_R_ctrl.SetWindowText(str);
		}
	}
//		OnDiskCheck2();
//	m_W_ctrl.SetWindowText(str);
}

void NToolsCrBorePage::OnChangeL()
{
	if (m_L_ctrl.GetWindowTextLength() != 0)
		OnDiskCheck2();
}

