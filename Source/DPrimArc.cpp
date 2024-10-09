// DPrimArc.cpp: файл реализации
//

#include "stdafx.h"
#include "DPrimArc.h"
#include "DimObjectArc.h"
#include "afxdialogex.h"

static CString strs[9];
//= {"",
//                        1        "Укажите ребро модели вблизи вершины.",
//                        2        "Укажите первую из трёх точек.",
//                        3        "Укажите вторую из трёх точек.",
//                        4        "Укажите последнюю точку.",
//                        5        "Создана! Радиус %.2f.",
//                        6        "Укажите три точки на модели",
//                        7        "Невозможно построить.",
//                        8        "%.3f, %.3f, %.3f)"};

// диалоговое окно DPrimArc

IMPLEMENT_DYNAMIC(DPrimArc, DPrimBase)

DPrimArc::DPrimArc(CNCMDoc* pDoc)
	: DPrimBase(pDoc, DPrimArc::IDD), _doc(pDoc)
    , _length(90.0)
    , _width(2.0f)
    , _textCoord(_T(strs[0]))
    , _textStatus(_T(strs[0]))
    , _textHelp(_T(strs[1]))
    , _ind(0)
    , _isCircle(0)
{
	strs[1].LoadString(IDS_DIM_ARC1);
	strs[2].LoadString(IDS_DIM_ARC2);
	strs[3].LoadString(IDS_DIM_SIZE10);
	strs[4].LoadString(IDS_DIM_SIZE11);
	strs[5].LoadString(IDS_DIM_SIZE8);
	strs[6].LoadString(IDS_DIM_ARC6);
	strs[7].LoadString(IDS_DIM_SIZE3);
	strs[8].LoadString(IDS_DIM_PT3);

    FillDataFromXml();
    // from data to dialog
    if (_doc->Dimensions._dPrimArc._set)
    {
        DDimensions::DialogDataArc* data = &_doc->Dimensions._dPrimArc;
        _length = data->_length;
        _width = data->_width;
        _color.SetColor(RGB(data->_color[0] * 255.0, data->_color[1] * 255.0, data->_color[2] * 255.0));
    }
    else
        _color.SetColor(0x00FF0000);
}

DPrimArc::~DPrimArc()
{
}

void DPrimArc::DoDataExchange(CDataExchange* pDX)
{
    DPrimBase::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ROTCENRSIZE, _length);
    DDX_Text(pDX, IDC_ROTCENRSIZE2, _width);
    DDX_Control(pDX, IDC_ROTCENTRCOLOR, _color);
    DDX_Text(pDX, IDC_PRIMARCCOORD, _textCoord);
    DDX_Text(pDX, IDC_PRIMARCRESULT, _textStatus);
    DDX_Text(pDX, IDC_PRIMARCHELP, _textHelp);
//    DDX_Radio(pDX, IDC_PRIMARCAUTO, _ind);
    DDX_Check(pDX, IDC_CHECK1, _isCircle);
// Modes
	DDX_Control(pDX, IDC_PRIMARCAUTO, _MaskAutoArc);
	DDX_Control(pDX, IDC_PRIMARC3PTS, _MaskThreePtArc);
}


BEGIN_MESSAGE_MAP(DPrimArc, DPrimBase)
    ON_BN_CLICKED(IDC_CALC2, OnBnVisibilityClicked)
    ON_BN_CLICKED(IDC_CALC3, OnBnDeleteClicked)
    ON_BN_CLICKED(IDC_CALC4, OnBnAgainClicked)
    ON_BN_CLICKED(IDC_PRIMARCAUTO, OnBnAutomatClicked)
    ON_BN_CLICKED(IDC_PRIMARC3PTS, OnBnThreePtsClicked)
    ON_EN_UPDATE(IDC_ROTCENRSIZE, OnUpdateDialog)
    ON_EN_UPDATE(IDC_ROTCENRSIZE2, OnUpdateDialog)
    ON_BN_CLICKED(IDC_ROTCENTRCOLOR, OnUpdateDialog)
    ON_BN_CLICKED(IDC_CHECK1, OnUpdateDialog)
END_MESSAGE_MAP()


// обработчики сообщений DPrimArc

void DPrimArc::OnBnAutomatClicked()
{
    UpdateData();
    FromDialog();
    _textCoord = _textStatus = strs[0];
    _textHelp = strs[1];
    BnAutomat(true);
    UpdateData(FALSE);
}

void DPrimArc::OnBnThreePtsClicked()
{
    UpdateData();
    FromDialog();
    _textCoord = strs[0];
    _textStatus = strs[2];
    _textHelp = strs[6];
    BnThreePts(true);
    UpdateData(FALSE);
}

void DPrimArc::OnBnVisibilityClicked()
{
    bool hidden = _doc->Dimensions.GetObjects().HideOrShowObjects(DimObjects::DimObjectArcType);
    if (_doc->Dimensions.GetObjects().IsHidden(DimObjects::DimObjectCircleType) != hidden)
        _doc->Dimensions.GetObjects().HideOrShowObjects(DimObjects::DimObjectCircleType);
    
    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimArc::OnBnDeleteClicked()
{
    _doc->Dimensions.GetObjects().RemoveVisibleObjects(DimObjects::DimObjectArcType);
    _doc->Dimensions.GetObjects().RemoveVisibleObjects(DimObjects::DimObjectCircleType);

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimArc::OnBnAgainClicked()
{
    _doc->Dimensions._auxPts.clear();

    _textCoord = strs[0];

	Calc_ind();
    switch (_ind)
    {
    case 0:
        _textStatus = strs[0];
        break;
    case 1:
        _textStatus = strs[2];
        for (int i = _doc->Dimensions.GetObjects().GetSelectedObjectsCount() - 1; i != -1 ; --i) // для того, чтобы точки, созданные для построения, удалились совсем
            _doc->Dimensions.GetObjects().Remove(_doc->Dimensions.GetObjects().GetSelectedObj(i)->GetObjectID());
        _doc->UpdateAllViews(NULL, N_OGL);
        break;
    };

    UpdateData(FALSE);
}

void DPrimArc::UpdateValues()
{
    FillXMLfromDialog();
    FillDataFromXml();
    DDimensions::DialogDataArc* data = &_doc->Dimensions._dPrimArc;
    data->_isCircle = _isCircle;
}

void DPrimArc::FillDataFromXml()
{
    DDimensions::DialogDataArc* data = &_doc->Dimensions._dPrimArc;
    // from XML to data
    CString Key(_T("Defaults/Dimensions/Arc/"));
    NColor Col = NCM_PROJECT.Defaults.GetColor((Key + CString(_T("Color"))).GetBuffer(), NColor());
    data->_color[0] = Col.R();
    data->_color[1] = Col.G();
    data->_color[2] = Col.B();
    Key += _T("Sizes@");
    data->_width = float(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Thickness"))).GetBuffer(), 2));
    data->_length = NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Angle"))).GetBuffer(), 100);
    data->_set = true;
}

void DPrimArc::FillXMLfromDialog()
{
    // from dialog to XML
    CString Key(_T("Defaults/Dimensions/Arc/"));
    NCM_PROJECT.Defaults.SetColor((Key + CString(_T("Color"))).GetBuffer(), NColor().SetRGB(_color.GetColor()));
    Key += _T("Sizes@");
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Thickness"))).GetBuffer(), _width);
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Angle"))).GetBuffer(), _length);
}

void DPrimArc::FromDialog()
{
	DPrimBase::FromDialog();

    UpdateValues();
	Calc_ind();
    switch (_ind)
    {
    case 0: // автомат
        BnAutomat();
        break;
    case 1: // по трём точкам
        BnThreePts();
        break;
    };
}

void DPrimArc::BnAutomat(bool iNoCheck)
{
    if (iNoCheck || (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreateArcWithAuto))
    {
        //_doc->Dimensions.GetObjects().ClearSelectedObjects();

        _doc->Dimensions.SetMouseClickAction(DDimensions::CreateArcWithAuto);
        _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
    }
}

void DPrimArc::BnThreePts(bool iNoCheck)
{
    if (iNoCheck || (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreateArcWithThreePoints))
    {
        _doc->Dimensions.GetObjects().ClearSelectedObjects();

        _doc->Dimensions.SetMouseClickAction(DDimensions::CreateArcWithThreePoints);
        _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
    }
}

void DPrimArc::UpdateDialog()
{
    unsigned int count = (unsigned int)_doc->Dimensions._auxPts.size();
    if (count == 0)
        return;

    if (_doc->Dimensions.GetMouseClickResult())
    {
        // вывод координат указанной точки
        BPoint pt = _doc->Dimensions._auxPts[count - 1];
        _textCoord.Format(strs[8], pt.GetX(), pt.GetY(), pt.GetZ());

        switch (_doc->Dimensions.GetMouseClickAction())
        {
        case DDimensions::CreateArcWithAuto:
            {
                double r = static_cast<DimObjectArc*>(_doc->Dimensions.GetObjects().GetAddedObj(_doc->Dimensions.GetObjects().GetCount() - 1))->GetRadius();
                _textStatus.Format(strs[5], r);
                _doc->Dimensions._auxPts.clear();
                break;
            }
        case DDimensions::CreateArcWithThreePoints:
            {
                if (count == 3)
                {
                    double r = static_cast<DimObjectArc*>(_doc->Dimensions.GetObjects().GetAddedObj(_doc->Dimensions.GetObjects().GetCount() - 1))->GetRadius();
                    _textStatus.Format(strs[5], r);
                    _doc->Dimensions._auxPts.clear();
                }
                else
                    _textStatus = strs[count + 2];

                break;
            }
        };
    }
    else
    {
        _textCoord = strs[0];
        switch (_doc->Dimensions.GetMouseClickAction())
        {
            case DDimensions::CreateArcWithAuto:
                {
                    _textStatus = strs[7];
                    _doc->Dimensions._auxPts.clear();
                    break;
                }
            case DDimensions::CreateArcWithThreePoints:
                if (count == 3)
                {
                    _textStatus = strs[7];
                    _doc->Dimensions._auxPts.clear();
                }
                break;
        };
    }

	UpdateData(FALSE);
}

void DPrimArc::PostNcDestroy()
{
	DPrimBase::PostNcDestroy();
	delete this;
}



BOOL DPrimArc::OnInitDialog()
{
	DPrimBase::OnInitDialog();

// Modes
	CString ToolTip;
	_MaskAutoArc.SetImage(IDB_CREATEAUTOARCOFF);
	_MaskAutoArc.SetCheckedImage(IDB_CREATEAUTOARCON);
	_MaskAutoArc.SizeToContent(); 
	_MaskAutoArc.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskAutoArc.SetCheck(TRUE);
	ToolTip.LoadString(IDS_CREATEAUTOARC);
	_MaskAutoArc.SetTooltip(ToolTip);

	_MaskThreePtArc.SetImage(IDB_CREATE3PTARCOFF);
	_MaskThreePtArc.SetCheckedImage(IDB_CREATE3PTARCON);
	_MaskThreePtArc.SizeToContent(); 
	_MaskThreePtArc.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskThreePtArc.SetCheck(FALSE);
	ToolTip.LoadString(IDS_CREATE3PTARC);
	_MaskThreePtArc.SetTooltip(ToolTip);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DPrimArc::Calc_ind(void)
{
	if(_MaskAutoArc.IsChecked())
		_ind = 0;
	else if(_MaskThreePtArc.IsChecked())
		_ind = 1;
}

void DPrimArc::SetTexts()
{
	Calc_ind();
	switch (_ind)
	{
	case 0: // автомат
		_textCoord = _textStatus = strs[0];
		_textHelp = strs[1];
		break;
	case 1: // по трём точкам
		_textCoord = strs[0];
		_textStatus = strs[2];
		_textHelp = strs[6];
		break;
	}
}
