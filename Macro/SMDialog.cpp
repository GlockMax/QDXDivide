// ..\Macro\SMDialog.cpp : implementation file
//

//#include "stdafx.h"
#include "pch.h"
#include "resinv.h"
#include "afxdialogex.h"
#include "ResLocal.h"
#include "NCMProject.h"
#include "SMGroup.h"
#include "SGenMacro.h"
#include "SMDefiniton.h"
#include "SMProperty.h"
#include "SMDialog.h"


// SMDialog dialog

IMPLEMENT_DYNAMIC(SMDialog, CDialogEx)

BEGIN_MESSAGE_MAP(SMDialog, CDialogEx)
	ON_BN_CLICKED(IDC_MACRO_APPLY, OnApply)
	ON_BN_CLICKED(IDC_MACRO_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_MACRO_PDF, OnPdf)
	ON_BN_CLICKED(IDC_MACRO_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_MACRO_FAVORITES, OnFavourites)
	ON_CBN_CLOSEUP(IDC_MACRO_COMBO, OnComboSelect)
	ON_CBN_CLOSEUP(IDC_MACRO_GROUPS, OnGroupSelect)
	ON_LBN_SELCHANGE(IDC_MACRO_LIST, OnTcnSelChangeList)
END_MESSAGE_MAP()


// SMDialog message handlers

SMDialog::SMDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MACRO, pParent)
{
	genMacro_ = nullptr;
	listInd_ = 0;
	xRightGap_ = 14;
	yBotGap_ = 14;
	oldVersion_ = FALSE;
	groupInd_ = 1;
	showFavourits_ = false;
}

SMDialog::~SMDialog()
{
}

void SMDialog::FillFromGenMacro()
{
	UpdateData();
	SetRedraw(FALSE);
	if (genMacro_ == nullptr)
		return;
	// init groupCtrl_
	CString str;
	str.LoadString(IDS_MACRO_FAVORITES);
	groupCtrl_.ResetContent();
	groupCtrl_.AddString(str);
	if (genMacro_->groups_.empty())
	{
		str.LoadString(IDS_MACRO_ALL);
		groupCtrl_.AddString(str);
	}
	else
	{
		for each (auto gr in genMacro_->groups_)
			groupCtrl_.AddString(gr.GetName());
	}
	groupCtrl_.SetCurSel(groupInd_);
	//END: init groupCtrl_
	showFavourits_ = (groupInd_ == 0);
	listCtrl_.ResetContent();
	for(int i = 0, k = 0; i < genMacro_->allMacros_.size(); ++i)
	{

		const auto& el = genMacro_->allMacros_.at(i);
		if ((showFavourits_ && el.favourite_) || (!showFavourits_ && groupInd_ - 1 == el.groupID_))
		{
			listCtrl_.AddString(el.name_);
			listCtrl_.SetItemData(k, i);
			++k;
		}
	}
	listCtrl_.SetCurSel(listInd_);
	AdjustListWidth();
	FillFromMacro();
	SetRedraw(TRUE);
	RedrawWindow();
}

SMDefiniton* SMDialog::GetSelDef() const
{
	if (genMacro_ == nullptr)
		return nullptr;
	if (listInd_ < 0 || listInd_ >= listCtrl_.GetCount())
		return nullptr;
	auto macroInd = listCtrl_.GetItemData(listInd_);
	return &genMacro_->allMacros_.at(macroInd);

}

void SMDialog::FillFromMacro()
{
	auto pDef = GetSelDef();
	if (pDef == nullptr)
		return;
	auto& def = *pDef;
	def.SetStartValues();
	propCtrl_.SetDef(&def);
	propCtrl_.RemoveAll();
	for (int i = 0; i < def.params_.size(); ++i)
	{
		auto& el = def.params_.at(i);
		if(!def.hideFormulas_ || !el.isFunc_ )// don't show func parameter if hideFormulas_
			propCtrl_.AddProperty(new SMProperty(el, this));
	}
	propCtrl_.RedrawWindow();
	LoadPicture(def.picture_);
	pdfReadCtrl_.ShowWindow(def.attachment_.IsEmpty() ? SW_HIDE : SW_SHOW);
	butFavourite_.SetCheck(def.favourite_);
}

void SMDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MACRO_LIST, listCtrl_);
	DDX_Control(pDX, IDC_MACRO_PROPERTY, propCtrl_);
	DDX_Control(pDX, IDC_MACRO_PICTURE, staticCtrl_);
	DDX_Control(pDX, IDC_MACRO_PDF, pdfReadCtrl_);
	DDX_Control(pDX, IDC_MACRO_COMBO, comboCtrl_);
	DDX_Control(pDX, IDC_MACRO_GROUPS, groupCtrl_);
	DDX_Control(pDX, IDC_MACRO_FAVORITES, butFavourite_);
	DDX_Control(pDX, IDC_MACRO_DEFAULT, butDefault_);
	DDX_Control(pDX, IDC_MACRO_APPLY, butApply_);
	DDX_Control(pDX, IDC_MACRO_IMPORT, butImport_);
	DDX_Control(pDX, IDC_CIMCOV8, checkOldV_);
	DDX_Check(pDX, IDC_CIMCOV8, oldVersion_);
}

void SMDialog::OnApply()
{
	Apply();
}

void SMDialog::OnPdf()
{
	auto pDef = GetSelDef();
	if (pDef == nullptr)
		return;
	const auto& def = *pDef;
	LoadPdf(def.attachment_);
}

void SMDialog::OnDefault()
{
	auto pDef = GetSelDef();
	if (pDef == nullptr)
		return;
	auto& def = *pDef;
	def.SetDefaultValues();
	UpdateParams();
}

void SMDialog::OnImport()
{
	UpdateData();
	if (genMacro_ != nullptr)
	{
		genMacro_->ImportMacroDefFile(oldVersion_);
	}
	InitComboCtrl();
	groupInd_ = 1;
	listInd_ = 0;
	FillFromGenMacro();
	UpdateData(FALSE);
}

void SMDialog::OnTcnSelChangeList()
{
	listInd_ = listCtrl_.GetCurSel();
	if (genMacro_ == nullptr)
		return;
	FillFromMacro();
	UpdateData(FALSE);
}

void SMDialog::OnComboSelect()
{
	if (genMacro_ == nullptr)
		return;
	genMacro_->SaveXML();
	int sel = comboCtrl_.GetCurSel();
	if (sel == CB_ERR)
		return;
	comboCtrl_.GetLBText(sel, genMacro_->fileName_);
	if (genMacro_->fileName_.IsEmpty())
		return;
	int dotInd = genMacro_->fileName_.ReverseFind('.');
	if (dotInd > 0)
		genMacro_->fileName_ = genMacro_->fileName_.Left(dotInd);
	genMacro_->LoadXML();
	groupInd_ = 1;
	listInd_ = 0;
	FillFromGenMacro();
	UpdateData(FALSE);
}

void SMDialog::OnGroupSelect()
{
	UpdateData();
	groupInd_ = groupCtrl_.GetCurSel();
	listInd_ = 0;
	FillFromGenMacro();
	UpdateData(FALSE);
}

void SMDialog::OnFavourites()
{
	UpdateData();
	auto pDef = GetSelDef();
	if (pDef == nullptr)
	{
		butFavourite_.SetCheck(0);
		return;
	}
	auto& def = *pDef;
	def.favourite_ = butFavourite_.IsChecked();
	FillFromGenMacro();
}

bool SMDialog::CheckAllDefined()
{
	UpdateData();
	auto pDef = GetSelDef();
	if (pDef == nullptr)
		return false;
	auto& def = *pDef;
	propCtrl_.GetValuesFromCtrl();
	bool ok = def.AllParametersDefined();
	if (!ok)
		AfxMessageBox(IDS_MES_MACRO_PARAMDEF);
	return ok;
}

bool SMDialog::CheckMinMax()
{
	auto pDef = GetSelDef();
	if (pDef == nullptr)
		return false;
	auto& def = *pDef;
	propCtrl_.GetValuesFromCtrl();
	bool errPres = false;
	for (int i = 0, k = 0; i < def.params_.size(); ++i)
	{
		errPres |= !def.params_.at(i).IsMinMaxValid();
	}
	if (errPres)
		AfxMessageBox(IDS_MES_MACRO_PARAMMINMAXLIM);
	return !errPres;
}

bool SMDialog::Apply()
{
	if (!CheckAllDefined())
		return false;
	if (!CheckMinMax())
		return false;
	auto pDef = GetSelDef();
	if (pDef == nullptr)
		return false;
	auto& def = *pDef;
	def.Apply(genMacro_->result_);
	genMacro_->ApplyMacro();
	return true;
}

void SMDialog::UpdateParams()
{
	for (int i = 0; i < propCtrl_.GetPropertyCount(); ++i)
		static_cast<SMProperty*>(propCtrl_.GetProperty(i))->Update();
 }

void SMDialog::InitComboCtrl()
{
	comboCtrl_.ResetContent();
	comboCtrl_.Dir(0, NCM_PROJECT.GetMacroPath() + genMacro_->currentPath_ + CString(_T("*")) + genMacro_->fileExt_);
	if (genMacro_ != nullptr)
		comboCtrl_.SelectString(0, genMacro_->GetFileName());
	if (comboCtrl_.GetCurSel() < 0)
	{
		CString ti;
		ti.LoadString(IDS_MACRO_SELECT);
		ti = ti + '\0';
		int size = MultiByteToWideChar(CP_THREAD_ACP, 0, ti, ti.GetLength(), NULL, 0);
		wchar_t* wti = new wchar_t[size];
		MultiByteToWideChar(CP_THREAD_ACP, 0, ti, ti.GetLength(), wti, size);
		ComboBox_SetCueBannerText(comboCtrl_.GetSafeHwnd(), wti);
		delete[] wti;
	}
}

BOOL SMDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//width first column
    HDITEM hdItem{};
    hdItem.mask = HDI_WIDTH;
    hdItem.cxy = 160;
    propCtrl_.GetHeaderCtrl().SetItem(0, &hdItem);

	staticCtrl_.GetWindowRect(origPictRectW_);
	staticCtrl_.GetClientRect(origPictRectC_);
	staticCtrl_.ClientToScreen(origPictRectC_);
	ScreenToClient(origPictRectW_);
	ScreenToClient(origPictRectC_);

	InitComboCtrl();
	groupInd_ = 1;
	listInd_ = 0;
	FillFromGenMacro();


	butFavourite_.SetImage(IDB_FAVORITES_OFF);
	butFavourite_.SetCheckedImage(IDB_FAVORITES_ON);
	//	m_MaskFixCtrl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;

	//Create the ToolTip control
	if (!toolTip_.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{
		// Add tool tips to the controls
		toolTip_.AddTool(&butDefault_, IDS_TOOLTIP_MACRO_RESET);
		toolTip_.AddTool(&butApply_, IDS_TOOLTIP_MACRO_APPLY);
		toolTip_.AddTool(&pdfReadCtrl_, IDS_TOOLTIP_MACRO_PDF);
		toolTip_.AddTool(&butImport_, IDS_TOOLTIP_MACRO_IMPORTCIMCO);
		toolTip_.AddTool(&checkOldV_, IDS_TOOLTIP_MACRO_IMPORTCIMCOV8);
		toolTip_.AddTool(&butFavourite_, IDS_TOOLTIP_MACRO_FAVORITES);

		toolTip_.Activate(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void SMDialog::OnOK()
{
	if (!Apply())
		return;

	CDialogEx::OnOK();
}

void SMDialog::AdjustToPicture()
{
	CRect rect;
	staticCtrl_.GetWindowRect(rect);
	ScreenToClient(rect);// in dialog client coord
	CRect bigRect;
	GetWindowRect(bigRect);
	ScreenToClient(bigRect);// in dialog client coord
	bigRect.right = rect.right + xRightGap_;
	bigRect.bottom = max(rect.bottom, origPictRectW_.bottom) + yBotGap_;
	ClientToScreen(bigRect);
	CWnd* par = GetParent();
	MoveWindow(bigRect);
}

bool SMDialog::LoadPicture(const CString& pictFile)
{
	image_.Destroy();
	if (!image_.IsNull())
		return false;
	HBITMAP hOldBitmap = staticCtrl_.SetBitmap(NULL);
	if (hOldBitmap)
		DeleteObject(hOldBitmap);
	if (pictFile.IsEmpty())
	{
		staticCtrl_.MoveWindow(origPictRectW_);
		AdjustToPicture();
		return true;
	}
	CString pictFilePath = NCM_PROJECT.GetMacroPath() + genMacro_->picturesPath_ + pictFile;
	if (image_.Load(pictFilePath) != S_OK)
		return false;
	CRect imageRect(0, 0, image_.GetWidth(), image_.GetHeight());
	CRect newRect = imageRect
		+ CRect(0, 0, origPictRectW_.Width() - origPictRectC_.Width(), origPictRectW_.Height() - origPictRectC_.Height())
		+ origPictRectW_.TopLeft();// in dialog coordinates

	staticCtrl_.ShowWindow(SW_HIDE);
	staticCtrl_.MoveWindow(newRect);
	staticCtrl_.SetBitmap(HBITMAP(image_));
	staticCtrl_.ShowWindow(SW_SHOWNORMAL);
	AdjustToPicture();
	return true;
}
bool SMDialog::LoadParamPicture(const CString& pictFile)
{
	if (pictFile.IsEmpty())
	{
		auto pDef = GetSelDef();
		if (pDef == nullptr)
			return false;
		const auto& def = *pDef;
		LoadPicture(def.picture_);
	}
	else
	{
		// Set this parameter picture
		LoadPicture(pictFile);
	}
	return false;
}

bool SMDialog::LoadPdf(const CString& pdfFile)
{
	CString pdfFilePath = NCM_PROJECT.GetMacroPath() + genMacro_->descriptionsPath_ + pdfFile;
	CString command = _T("powershell ") + pdfFilePath;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		command.GetBuffer(),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return false;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return false;
}

void SMDialog::RecalcFunctions()
{
	UpdateData();
	propCtrl_.UpdateFunctionsVal();
	UpdateData(FALSE);
}

BOOL SMDialog::PreTranslateMessage(MSG* pMsg)
{
	toolTip_.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}

void SMDialog::AdjustListWidth()
{
	// Find the longest string in the list box.
	CString str;
	CSize sz;
	int dx = 0;
	TEXTMETRIC tm;
	CDC* pDC = listCtrl_.GetDC();
	CFont* pFont = listCtrl_.GetFont();

	// Select the listbox font, save the old font
	CFont* pOldFont = pDC->SelectObject(pFont);
	// Get the text metrics for avg char width
	pDC->GetTextMetrics(&tm);

	for (int i = 0; i < listCtrl_.GetCount(); i++)
	{
		listCtrl_.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		// Add the avg width to prevent clipping
		sz.cx += tm.tmAveCharWidth;

		if (sz.cx > dx)
			dx = sz.cx;
	}
	// Select the old font back into the DC
	pDC->SelectObject(pOldFont);
	listCtrl_.ReleaseDC(pDC);

	// Set the horizontal extent so every character of all strings
	// can be scrolled to.
	listCtrl_.SetHorizontalExtent(dx);
}
