// DPrimAutofix.cpp: файл реализации
//

#include "stdafx.h"
#include "DPrimAutofix.h"
#include "Snaps.h"
#include "afxdialogex.h"


// диалоговое окно DPrimAutofix

IMPLEMENT_DYNAMIC(DPrimAutofix, DPrimBaseCommon)

DPrimAutofix::DPrimAutofix(CNCMDoc* pDoc)
	: DPrimBaseCommon(DPrimAutofix::IDD), _doc(pDoc)
    , _size(5)
    , _width(1.0f)
    , _eps(10)
{
    _color.SetColor(0x00FF0000);
}

DPrimAutofix::~DPrimAutofix()
{
}

void DPrimAutofix::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_SNAP_MARKER, _size);
    DDX_Control(pDX, IDC_ROTCENTRCOLOR, _color);
    DDX_Text(pDX, IDC_SNAP_MARKER3, _width);
    DDX_Text(pDX, IDC_SELECTION_REGION, _eps);
}


BEGIN_MESSAGE_MAP(DPrimAutofix, DPrimBaseCommon)
    ON_EN_UPDATE(IDC_SNAP_MARKER, OnUpdateDialog)
    ON_EN_UPDATE(IDC_SNAP_MARKER3, OnUpdateDialog)
    ON_EN_UPDATE(IDC_SELECTION_REGION, OnUpdateDialog)
    ON_BN_CLICKED(IDC_ROTCENTRCOLOR, OnUpdateDialog)
END_MESSAGE_MAP()


// обработчики сообщений DPrimAutofix

void DPrimAutofix::FromDialog()
{
    Snaps* snaps = &_doc->GetSnaps();

    snaps->SetCursorSize(_size);
    snaps->SetCursorWidth(_width);

    COLORREF color = _color.GetColor();
    snaps->SetCursorColor(GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0);

    snaps->SetEps((double)_eps);
}

void DPrimAutofix::PostNcDestroy()
{
	CPropertyPage::PostNcDestroy();
	delete this;
}

BOOL DPrimAutofix::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
    FillDataFromXml();
    Snaps* snaps = &_doc->GetSnaps();

	_size = snaps->GetCursorSize();
	_width = snaps->GetCursorWidth();

	double r, g, b;
	snaps->GetCursorColor(r, g, b);
	_color.SetColor(RGB(r * 255.0, g * 255.0, b * 255.0));

	_eps = (int)snaps->GetEps();
	UpdateData(FALSE);
	return TRUE;
}

void DPrimAutofix::UpdateValues()
{
    FillXMLfromDialog();
    FillDataFromXml();
}

void DPrimAutofix::FillDataFromXml()
{
    Snaps* snaps = &_doc->GetSnaps();
    // from XML to data
    CString Key(_T("Defaults/Dimensions/Snaps/"));
    NColor Col = NCM_PROJECT.Defaults.GetColor((Key + CString(_T("Color"))).GetBuffer(), NColor());
    snaps->SetCursorColor(Col.R(), Col.G(), Col.B());
    Key += _T("Sizes@");
    snaps->SetCursorWidth(float(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Thickness"))).GetBuffer(), 2)));
    snaps->SetCursorSize(int(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Height"))).GetBuffer(), 10)));
    snaps->SetEps(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Attraction"))).GetBuffer(), 10));
}

void DPrimAutofix::FillXMLfromDialog()
{
    // from dialog to XML
    CString Key(_T("Defaults/Dimensions/Snaps/"));
    NCM_PROJECT.Defaults.SetColor((Key + CString(_T("Color"))).GetBuffer(), NColor().SetRGB(_color.GetColor()));
    Key += _T("Sizes@");
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Thickness"))).GetBuffer(), _width);
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Height"))).GetBuffer(), _size);
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Attraction"))).GetBuffer(), _eps);
}
