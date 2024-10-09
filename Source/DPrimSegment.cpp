// C:\Work_Folder\NCM\trunk\Source\DPrimSegment.cpp: файл реализации
//

#include "stdafx.h"
#include "DPrimSegment.h"
#include "DimObjectPoint.h"
#include "DimObjectSegment.h"
#include "afxdialogex.h"

static CString strs[11];
//= {"",
//                             1    "”кажите ребро модели.",
//                             2    "”кажите первую точку.",
//                             3    "”кажите первую плоскость из двух.",
//                             4    "”кажите последнюю точку.",
//                             5    "”кажите вторую плоскость.",
//                             6    "”кажите две точки на модели, каркасе или траектории.",
//                             7    "”кажите две плоскости.",
//                             8    "—оздан! ƒлина = %.3f",
//                             9    "Ќевозможно построить.",
//                             10    "(%.3f, %.3f, %.3f)"};

// диалоговое окно DPrimSegment

IMPLEMENT_DYNAMIC(DPrimSegment, DPrimBase)

DPrimSegment::DPrimSegment(CNCMDoc* pDoc)
	: DPrimBase(pDoc, DPrimSegment::IDD), _doc(pDoc)
    , _length(100.0)
    , _width(2.0f)
    , _textCoord(_T(strs[0]))
    , _textStatus(_T(strs[0]))
    , _textHelp(_T(strs[1]))
    , _ind(0)
{
	strs[1].LoadString(IDS_DIM_LINE1);
	strs[2].LoadString(IDS_DIM_SIZE5);
	strs[3].LoadString(IDS_DIM_LINE3);
	strs[4].LoadString(IDS_DIM_SIZE11);
	strs[5].LoadString(IDS_DIM_SIZE17);
	strs[6].LoadString(IDS_DIM_LINE6);
	strs[7].LoadString(IDS_DIM_LINE7);
	strs[8].LoadString(IDS_DIM_LINE8);
	strs[9].LoadString(IDS_DIM_SIZE3);
	strs[10].LoadString(IDS_DIM_PT3);

    FillDataFromXml();
    if (_doc->Dimensions._dPrimSegm._set)
    {
        DDimensions::DialogDataSegm* data = &_doc->Dimensions._dPrimSegm;
        _width = data->_width;
        _length = data->_length;
        _color.SetColor(RGB(data->_color[0] * 255.0, data->_color[1] * 255.0, data->_color[2] * 255.0));
    }
    else
        _color.SetColor(0x00FF0000);

}

DPrimSegment::~DPrimSegment()
{
}

void DPrimSegment::DoDataExchange(CDataExchange* pDX)
{
    DPrimBase::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ROTCENRSIZE, _length);
    DDX_Text(pDX, IDC_ROTCENRSIZE2, _width);
    DDX_Control(pDX, IDC_ROTCENTRCOLOR, _color);
    DDX_Text(pDX, IDC_PRIMSEGMCOORD, _textCoord);
    DDX_Text(pDX, IDC_PRIMSEGMRESULT, _textStatus);
    DDX_Text(pDX, IDC_PRIMSEGMHELP, _textHelp);
 //   DDX_Radio(pDX, IDC_PRIMSEGMRADIO1, _ind);
// Modes
	DDX_Control(pDX, IDC_PRIMSEGMRADIO1, _MaskAutoLine);
	DDX_Control(pDX, IDC_PRIMSEGMRADIO2, _MaskTwoPtLine);
	DDX_Control(pDX, IDC_PRIMSEGMRADIO3, _MaskIntrsectLine);
}

BEGIN_MESSAGE_MAP(DPrimSegment, DPrimBase)
    ON_BN_CLICKED(IDC_CALC2, OnBnVisibilityClicked)
    ON_BN_CLICKED(IDC_CALC3, OnBnDeleteClicked)
    ON_BN_CLICKED(IDC_CALC4, OnBnAgainClicked)
    ON_BN_CLICKED(IDC_PRIMSEGMRADIO1, OnBnAutomatClicked)
    ON_BN_CLICKED(IDC_PRIMSEGMRADIO2, OnBnTwoPtsClicked)
    ON_BN_CLICKED(IDC_PRIMSEGMRADIO3, OnBnTwoPlnsClicked)
    ON_EN_UPDATE(IDC_ROTCENRSIZE, OnUpdateDialog)
    ON_EN_UPDATE(IDC_ROTCENRSIZE2, OnUpdateDialog)
    ON_BN_CLICKED(IDC_ROTCENTRCOLOR, OnUpdateDialog)
END_MESSAGE_MAP()


// обработчики сообщений DPrimSegment

void DPrimSegment::OnBnVisibilityClicked()
{
    _doc->Dimensions.GetObjects().HideOrShowObjects(DimObjects::DimObjectSegmentType);
    
    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimSegment::OnBnDeleteClicked()
{
    _doc->Dimensions.GetObjects().RemoveVisibleObjects(DimObjects::DimObjectSegmentType);

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimSegment::OnBnAgainClicked()
{
    UpdateData();
    _doc->Dimensions.GetObjects().ClearSelectedObjects();
    _doc->Dimensions._auxPts.clear();

	SetTexts();

   UpdateData(FALSE);
}

void DPrimSegment::OnBnAutomatClicked()
{
    UpdateData();
    FromDialog();
    _textStatus = strs[0];
    _textHelp = strs[1];
    BnAutomat(true);
    UpdateData(FALSE);
}

void DPrimSegment::OnBnTwoPtsClicked()
{
    UpdateData();
    FromDialog();
    _textStatus = strs[2];
    _textHelp = strs[6];
    BnTwoPts(true);
    UpdateData(FALSE);
}

void DPrimSegment::OnBnTwoPlnsClicked()
{
    UpdateData();
    FromDialog();
    _textStatus = strs[3];
    _textHelp = strs[7];
    BnTwoPlns(true);
    UpdateData(FALSE);
}

void DPrimSegment::UpdateValues()
{
    FillXMLfromDialog();
    FillDataFromXml();
    DDimensions::DialogDataSegm* data = &_doc->Dimensions._dPrimSegm;
    
    data->_set = true;
    data->_width = _width;
    data->_length = _length;

    COLORREF color = _color.GetColor();
    double colorArr[3] = {GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0};
    data->_color[0] = colorArr[0];
    data->_color[1] = colorArr[1];
    data->_color[2] = colorArr[2];
}

void DPrimSegment::FillDataFromXml()
{
    DDimensions::DialogDataSegm* data = &_doc->Dimensions._dPrimSegm;
    // from XML to data
    CString Key(_T("Defaults/Dimensions/Line/"));
    NColor Col = NCM_PROJECT.Defaults.GetColor((Key + CString(_T("Color"))).GetBuffer(), NColor());
    data->_color[0] = Col.R();
    data->_color[1] = Col.G();
    data->_color[2] = Col.B();
    Key += _T("Sizes@");
    data->_width = float(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Thickness"))).GetBuffer(), 2));
    data->_length = NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Length"))).GetBuffer(), 100);
    data->_set = true;
}

void DPrimSegment::FillXMLfromDialog()
{
    // from dialog to XML
    CString Key(_T("Defaults/Dimensions/Line/"));
    NCM_PROJECT.Defaults.SetColor((Key + CString(_T("Color"))).GetBuffer(), NColor().SetRGB(_color.GetColor()));
    Key += _T("Sizes@");
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Thickness"))).GetBuffer(), _width);
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Length"))).GetBuffer(), _length);
}

void DPrimSegment::FromDialog()
{
	DPrimBase::FromDialog();

	UpdateValues();
	Calc_ind();
    switch (_ind)
    {
    case 0: // автомат
        BnAutomat();
        break;
    case 1: // по двум точкам
        BnTwoPts();
        break;
    case 2: // по двум плоскост€м
        BnTwoPlns();
        break;
    };
}

void DPrimSegment::BnAutomat(bool iNoCheck)
{
    if (iNoCheck || (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreateSegmentWithTwoPoints))
    {
        _doc->Dimensions.SetMouseClickAction(DDimensions::CreateSegmentWithAuto);
        _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
    }
}

void DPrimSegment::BnTwoPts(bool iNoCheck)
{
    if (iNoCheck || (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreateSegmentWithTwoPoints))
    {
        _doc->Dimensions.GetObjects().ClearSelectedObjects();
        _doc->Dimensions._auxPts.clear();

        _doc->Dimensions.SetMouseClickAction(DDimensions::CreateSegmentWithTwoPoints);
        _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);

    }
}

void DPrimSegment::BnTwoPlns(bool iNoCheck)
{
    if (iNoCheck || (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreateSegmentWithTwoPlanes))
    {
        _doc->Dimensions.GetObjects().ClearSelectedObjects();
        _doc->Dimensions._auxPts.clear();
    
        _doc->Dimensions.SetMouseClickAction(DDimensions::CreateSegmentWithTwoPlanes);
        _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);

        _doc->Dimensions.DeActivateSnaps();
    }
}

void DPrimSegment::UpdateDialog()
{
	UpdateData();
    unsigned int count = (unsigned int)_doc->Dimensions._auxPts.size();
    if (count == 0)
        return;

    BPoint pt = _doc->Dimensions._auxPts[count - 1];
    _textCoord.Format(strs[10], pt.GetX(), pt.GetY(), pt.GetZ());

    switch (_doc->Dimensions.GetMouseClickAction())
    {
    case DDimensions::CreateSegmentWithAuto:
        {
            _textStatus.Format(strs[8], _length);

            _doc->Dimensions._auxPts.clear();

            break;
        }
    case DDimensions::CreateSegmentWithTwoPoints:
        {
            if (count == 1)
                _textStatus = strs[4];
            else
            {
                std::vector<DimObject*> segm;
                _doc->Dimensions.GetObjects().GetLastFewObjects(1, DimObjects::DimObjectSegmentType, segm);
                if (!segm.empty())
                {
                    BPoint pts[2];
                    segm[0]->GetEndPoints(pts[0], pts[1]);
                    _textStatus.Format(strs[8], sqrt((pts[0] - pts[1]).D2()));

                    _doc->Dimensions._auxPts.clear();
                }
            }

            break;
        }
    case DDimensions::CreateSegmentWithTwoPlanes:
        {
            if (count == 1)
                _textStatus = strs[5];
            else
            {
                if (_doc->Dimensions.GetMouseClickResult())
                {
                    _textStatus.Format(strs[8], _length);
                }
                else
                    _textStatus = strs[9];

                _doc->Dimensions.GetObjects().ClearSelectedObjects();
                _doc->Dimensions._auxPts.clear();
            }

            break;
        }
    };
    
    UpdateData(FALSE);
}

void DPrimSegment::PostNcDestroy()
{
	DPrimBase::PostNcDestroy();
	delete this;
}



BOOL DPrimSegment::OnInitDialog()
{
	DPrimBase::OnInitDialog();

// Modes
	CString ToolTip;
	_MaskAutoLine.SetImage(IDB_CREATEAUTOLINEOFF);
	_MaskAutoLine.SetCheckedImage(IDB_CREATEAUTOLINEON);
	_MaskAutoLine.SizeToContent(); 
	_MaskAutoLine.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskAutoLine.SetCheck(TRUE);
	ToolTip.LoadString(IDS_CREATEAUTOLINE);
	_MaskAutoLine.SetTooltip(ToolTip);

	_MaskTwoPtLine.SetImage(IDB_CREATETWOPTLINEOFF);
	_MaskTwoPtLine.SetCheckedImage(IDB_CREATETWOPTLINEON);
	_MaskTwoPtLine.SizeToContent(); 
	_MaskTwoPtLine.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskTwoPtLine.SetCheck(FALSE);
	ToolTip.LoadString(IDS_CREATETWOPTLINE);
	_MaskTwoPtLine.SetTooltip(ToolTip);

	_MaskIntrsectLine.SetImage(IDB_CREATEINTERSECTLINEOFF);
	_MaskIntrsectLine.SetCheckedImage(IDB_CREATEINTERSECTLINEON);
	_MaskIntrsectLine.SizeToContent(); 
	_MaskIntrsectLine.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskIntrsectLine.SetCheck(FALSE);
	ToolTip.LoadString(IDS_CREATEINTERSECTLINE);
	_MaskIntrsectLine.SetTooltip(ToolTip);

	UpdateData(FALSE);
	OnBnAutomatClicked();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DPrimSegment::Calc_ind(void)
{
	if(_MaskAutoLine.IsChecked())
		_ind = 0;
	else if(_MaskTwoPtLine.IsChecked())
		_ind = 1;
	else if(_MaskIntrsectLine.IsChecked())
		_ind = 2;
}

void DPrimSegment::SetTexts()
{
	_textCoord = strs[0];

	Calc_ind();
	switch (_ind)
	{
	case 0:
		_textStatus = strs[0];
		_textHelp = strs[1];
		break;
	case 1:
		_textStatus = strs[2];
		_textHelp = strs[6];
		break;
	case 2:
		_textStatus = strs[3];
		_textHelp = strs[7];
		break;
	}
}
