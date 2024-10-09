// DPrimBase.cpp: файл реализации
//

#include "stdafx.h"
#include "DPrimBase.h"
#include "Snaps.h"
#include "afxdialogex.h"


// диалоговое окно DPrimBase

IMPLEMENT_DYNAMIC(DPrimBaseCommon, CPropertyPage)
IMPLEMENT_DYNAMIC(DPrimBase, DPrimBaseCommon)

BEGIN_MESSAGE_MAP(DPrimBaseCommon, CPropertyPage)
	ON_BN_CLICKED(IDC_SAVE_THIS_PARAMETERS, OnSaveParameters)
END_MESSAGE_MAP()

DPrimBaseCommon::DPrimBaseCommon(UINT nIDTemplate)
	: CPropertyPage(nIDTemplate)
{
}

DPrimBase::DPrimBase(CNCMDoc* pDoc, UINT nIDTemplate)
	: DPrimBaseCommon(nIDTemplate), _doc(pDoc)
	, _skeleton(FALSE)
	, _dimObj(FALSE)
	, _trajectory(FALSE)
	, _model(FALSE)
	, _endPt(FALSE)
	, _midPt(FALSE)
	, _center(FALSE)
	, _ptOnElem(FALSE)
	, _wholeObj(FALSE)
{
}

DPrimBase::~DPrimBase()
{
}

void DPrimBase::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK1, _skeleton);
    DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK2, _dimObj);
    DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK3, _trajectory);
    DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK4, _model);
	DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK5, _endPt);
    DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK6, _midPt);
    DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK7, _center);
    DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK8, _ptOnElem);
    DDX_Check(pDX, IDC_PRIMAUTOFIXCHECK9, _wholeObj);
    DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK5, _bns[0]);
    DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK6, _bns[1]);
    DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK7, _bns[2]);
    DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK8, _bns[3]);
//	Pictures
	DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK1, _MaskSkeleton);
	DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK2, _MaskPrimObj);
    DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK3, _MaskTrajectory);
    DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK4, _MaskModel);
    DDX_Control(pDX, IDC_PRIMAUTOFIXCHECK9, _MaskWholeObj);
}


BEGIN_MESSAGE_MAP(DPrimBase, DPrimBaseCommon)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK1, OnBnClicked)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK2, OnBnClicked)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK3, OnBnClicked)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK4, OnBnClicked)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK5, OnBnClicked)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK6, OnBnClicked)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK7, OnBnClicked)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK8, OnBnClicked)
    ON_BN_CLICKED(IDC_PRIMAUTOFIXCHECK9, OnBnClicked)
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// обработчики сообщений DPrimBase

void DPrimBase::OnBnClicked()
{
    UpdateData();
    UpdateBNSButtons();
	FromDialog();
	::SetFocus(NULL);
}

void DPrimBase::UpdateBNSButtons()
{
	for(int i = 0; i < 4; ++i)
		if(::IsWindow(_bns[i].GetSafeHwnd()))
			if(_bns[i].IsWindowEnabled() != (_wholeObj == 0))
				_bns[i].EnableWindow(_wholeObj != 0 ? FALSE : TRUE);
}

void DPrimBase::FromDialog()
{
	UpdateData();
	// Make snaps to be correspondent to dialog settings
	Snaps* snaps = &_doc->GetSnaps();

    int snapObjType = 0;
    if (_skeleton)
        snapObjType |= Snaps::Skeleton;
    if (_dimObj)
        snapObjType |= Snaps::DimObjects;
    if (_trajectory)
        snapObjType |= Snaps::Trajectory;
    if (_model)
        snapObjType |= Snaps::Model;

	snaps->SetSnapObjectType(snapObjType | Snaps::UCS);

	if (_wholeObj)
        snaps->DeActivate();
    else
		snaps->Activate();

	int snapType = 0;
	if (_endPt)
		snapType |= Snaps::End;
	if (_center)
		snapType |= Snaps::Center;
	if (_midPt)
		snapType |= Snaps::Mid;
	if (_ptOnElem)
		snapType |= Snaps::EmptySnapType;
	snaps->SetSnapType(snapType);

}

BOOL DPrimBase::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

// Filters
	_MaskSkeleton.SetImage(IDB_FiltrSceletOff);
	_MaskSkeleton.SetCheckedImage(IDB_FILTRSCELETON);
	_MaskSkeleton.SizeToContent(); 
	_MaskSkeleton.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskSkeleton.SetCheck(TRUE);
	CString ToolTip;
	ToolTip.LoadString(IDS_FILTRSCELETON);
	_MaskSkeleton.SetTooltip(ToolTip);

	_MaskPrimObj.SetImage(IDB_FILTROBJOFF);
	_MaskPrimObj.SetCheckedImage(IDB_FILTROBJON);
	_MaskPrimObj.SizeToContent(); 
	_MaskPrimObj.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskPrimObj.SetCheck(TRUE);
	ToolTip.LoadString(IDS_FILTROBJ);
	_MaskPrimObj.SetTooltip(ToolTip);

	_MaskTrajectory.SetImage(IDB_FILTRTRAJECTOFF);
	_MaskTrajectory.SetCheckedImage(IDB_FILTRTRAJECTON);
	_MaskTrajectory.SizeToContent(); 
	_MaskTrajectory.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskTrajectory.SetCheck(TRUE);
	ToolTip.LoadString(IDS_FILTRTRAJECTORY);
	_MaskTrajectory.SetTooltip(ToolTip);

	_MaskModel.SetImage(IDB_FILTRMODELOFF);
	_MaskModel.SetCheckedImage(IDB_FILTRMODELON);
	_MaskModel.SizeToContent(); 
	_MaskModel.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskModel.SetCheck(TRUE);
	ToolTip.LoadString(IDS_FILTRMODEL);
	_MaskModel.SetTooltip(ToolTip);

// Autofix
	_bns[0].SetImage(IDB_AUTOFIXENDOFF);
	_bns[0].SetCheckedImage(IDB_AUTOFIXENDON);
	_bns[0].SizeToContent(); 
	_bns[0].m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_bns[0].SetCheck(TRUE);
	ToolTip.LoadString(IDS_AUTOFIXEND);
	_bns[0].SetTooltip(ToolTip);

	_bns[1].SetImage(IDB_AUTOFIXMIDOFF);
	_bns[1].SetCheckedImage(IDB_AUTOFIXMIDON);
	_bns[1].SizeToContent(); 
	_bns[1].m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_bns[1].SetCheck(TRUE);
	ToolTip.LoadString(IDS_AUTOFIXMID);
	_bns[1].SetTooltip(ToolTip);

	_bns[2].SetImage(IDB_AUTOFIXCENTROFF);
	_bns[2].SetCheckedImage(IDB_AUTOFIXCENTRON);
	_bns[2].SizeToContent(); 
	_bns[2].m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_bns[2].SetCheck(TRUE);
	ToolTip.LoadString(IDS_AUTOFIXCENTER);
	_bns[2].SetTooltip(ToolTip);

	_bns[3].SetImage(IDB_AUTOFIXONCURVOFF);
	_bns[3].SetCheckedImage(IDB_AUTOFIXONCURVON);
	_bns[3].SizeToContent(); 
	_bns[3].m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_bns[3].SetCheck(TRUE);
	ToolTip.LoadString(IDS_AUTOFIXONCURVE);
	_bns[3].SetTooltip(ToolTip);

	_MaskWholeObj.SetImage(IDB_CHECK32);
	_MaskWholeObj.SetCheckedImage(IDB_CHECKNO32);
	_MaskWholeObj.SizeToContent(); 
	_MaskWholeObj.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskWholeObj.SetCheck(FALSE);
	ToolTip.LoadString(IDS_AUTOFIXONOFF);
	_MaskWholeObj.SetTooltip(ToolTip);

	UpdateData();
	SetTexts();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DPrimBase::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		FromDialog();
	}
}

void DPrimBaseCommon::OnSaveParameters()
{
	CString FileName;
	if (FileName.LoadString(IDS_DEFIN_FILENAME))
	{
		CString Path = NCM_PROJECT.GetSettingsPath() + FileName;
		const bool res = NCM_PROJECT.Defaults.SaveFile(Path.GetBuffer());
		if (!res)
			AfxMessageBox(IDS_BSP_SAVE_ERR);
	}
}
