// DPrimDimens.cpp: файл реализации
//

#include "stdafx.h"
#include "ConstDef.h"
#include "DPrimDimens.h"
#include "DimObject.h"
#include "DimObjectSegment.h"
#include "DimObjectArc.h"
#include "afxdialogex.h"
#include <string>

static CString strs[25];
//= {"",
//                           /*1*/"DX %.3f DY %.3f DZ %.3f",
//                           /*2*/"Создан! Расстояние %.3f",
//                           /*3*/"Невозможно построить!",
//                           /*4*/"Укажите вторую точку, отрезок, дугу, окружность или плоскость.",
//                           /*5*/"Укажите точку",
//                           /*6*/"Используя привязки, укажите точку, отрезок, плоскость, дугу или окружность.",
//                           /*7*/"Центр X %.3f Y %.3f Z %.3f",
//                           /*8*/"Создан! Радиус %.3f",
//                           /*9*/"Создан! Диаметр %.3f",
//                           /*10*/"Укажите вторую из трех точек",
//                           /*11*/"Укажите последнюю точку",
//                           /*12*/"Создан! Угол %u град %u минут %u секунд",
//                           /*13*/"Используя привязки, укажите три точки или дугу или окружность",
//                           /*14*/"Укажите вторую из трех точек или отрезок",
//                           /*15*/"Укажите последнюю точку",
//                           /*16*/"Укажите второй отрезок или точку",
//                           /*17*/"Укажите вторую плоскость из двух",
//                           /*18*/"Используя привязки укажите точки, отрезок, плоскости или дугу.",
//                           /*19*/"Выберите один или несколько размеров.",
//                           /*20*/"Выбрано объектов: %u"};

// диалоговое окно DPrimDimens

IMPLEMENT_DYNAMIC(DPrimDimens, DPrimBase)

DPrimDimens::DPrimDimens(CNCMDoc* pDoc)
	: DPrimBase(pDoc, DPrimDimens::IDD), _doc(pDoc)
    , _width(2.0f)
    , _dimTextSize(14)
    , _createArc(FALSE)
    , _createPt(TRUE)
    , _hideWhenClosed(TRUE)
    , _textCoord(_T(strs[0]))
    , _textStatus(_T(strs[0]))
    , _textHelp(_T(strs[6]))
    , _ind(0)
	, _dimTextDigits(4)
{
	strs[1].LoadString(IDS_DIM_SIZE1);
	strs[2].LoadString(IDS_DIM_SIZE2);
	strs[3].LoadString(IDS_DIM_SIZE3);
	strs[4].LoadString(IDS_DIM_SIZE4);
	strs[5].LoadString(IDS_DIM_SIZE5);
	strs[6].LoadString(IDS_DIM_SIZE6);
	strs[7].LoadString(IDS_DIM_SIZE7);
	strs[8].LoadString(IDS_DIM_SIZE8);
	strs[9].LoadString(IDS_DIM_SIZE9);
	strs[10].LoadString(IDS_DIM_SIZE10);
	strs[11].LoadString(IDS_DIM_SIZE11);
	strs[12].LoadString(IDS_DIM_SIZE12);
	strs[13].LoadString(IDS_DIM_SIZE13);
	strs[14].LoadString(IDS_DIM_SIZE14);
	strs[15].LoadString(IDS_DIM_SIZE11);
	strs[16].LoadString(IDS_DIM_SIZE16);
	strs[17].LoadString(IDS_DIM_SIZE17);
	strs[18].LoadString(IDS_DIM_SIZE6);
	strs[19].LoadString(IDS_DIM_SIZE19);
	strs[20].LoadString(IDS_DIM_SIZE20);

    FillDataFromXml();
    if (_doc->Dimensions._dPrimDim._set)
    {
        DDimensions::DialogDataDim* data = &_doc->Dimensions._dPrimDim;
        _width = data->_width;
		_dimTextSize = data->_dimTextSize;
		_dimTextDigits = data->_dimTextDigits;
		_createArc = data->_createArc ? 1 : 0;
        _createPt = data->_createPt ? 1 : 0;
        _hideWhenClosed = _doc->Dimensions._hideWhenClosed ? 1 : 0;
        _color0.SetColor(RGB(data->_color[0] * 255.0, data->_color[1] * 255.0, data->_color[2] * 255.0));
        _color1.SetColor(RGB(data->_colorText[0] * 255.0, data->_colorText[1] * 255.0, data->_colorText[2] * 255.0));
        _colorBack.SetColor(RGB(data->_backText[0] * 255.0, data->_backText[1] * 255.0, data->_backText[2] * 255.0));
    }
    else
    {
        _color0.SetColor(0x0000FFFF);
        _color1.SetColor(0x00000000);
        _colorBack.SetColor(0x0000FFFF);
    }
}

DPrimDimens::~DPrimDimens()
{
}

void DPrimDimens::DoDataExchange(CDataExchange* pDX)
{
    DPrimBase::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_PRIMDIMENSWDTH, _width);
    DDX_Text(pDX, IDC_PRIMDIMENSTXTSIZE, _dimTextSize);
	DDX_Text(pDX, IDC_PRIMDIMENSFRAC, _dimTextDigits);
    DDX_Control(pDX, IDC_ROTCENTRCOLOR, _color0);
    DDX_Control(pDX, IDC_ROTCENTRCOLOR2, _color1);
    DDX_Control(pDX, IDC_ROTCENTRCOLOR1, _colorBack);
    DDX_Check(pDX, IDC_PRIMDIMENSCH10, _createArc);
    DDX_Check(pDX, IDC_PRIMDIMENSCH11, _createPt);
    DDX_Check(pDX, IDC_PRIMDIMENSCH12, _hideWhenClosed);
    DDX_Text(pDX, IDC_PRIMDIMENSTXTCOORD, _textCoord);
    DDX_Text(pDX, IDC_PRIMDIMENSTXTSTAT, _textStatus);
    DDX_Text(pDX, IDC_PRIMDIMENSTXTHLP, _textHelp);
// Modes
	DDX_Control(pDX, IDC_PRIMDIMENSRADLEN, _MaskSizeLength);
	DDX_Control(pDX, IDC_PRIMDIMENSRADRAD, _MaskSizeRad);
	DDX_Control(pDX, IDC_PRIMDIMENSRADDIAM, _MaskSizeDiam);
	DDX_Control(pDX, IDC_PRIMDIMENSRADANG, _MaskSizeAngle);
	DDX_Control(pDX, IDC_PRIMDIMENSRADSEL, _MaskSizeSelect);
}


BEGIN_MESSAGE_MAP(DPrimDimens, DPrimBase)
    ON_BN_CLICKED(IDC_PRIMDIMENSCH10, OnUpdateDialog)
    ON_BN_CLICKED(IDC_PRIMDIMENSCH11, OnUpdateDialog)
    ON_BN_CLICKED(IDC_PRIMDIMENSCH12, OnUpdateDialog)
    ON_BN_CLICKED(IDC_PRIMDIMENSBNHIDEALL, OnBnHideAllClick)
    ON_BN_CLICKED(IDC_PRIMDIMENSBNDELALL, OnBnDelAllClick)
    ON_BN_CLICKED(IDC_PRIMDIMENSBNHIDE, OnBnHideClick)
    ON_BN_CLICKED(IDC_PRIMDIMENSBNDEL, OnBnDelClick)
    ON_BN_CLICKED(IDC_PRIMDIMENSRADLEN, OnBnClick)
    ON_BN_CLICKED(IDC_PRIMDIMENSRADRAD, OnBnClick)
    ON_BN_CLICKED(IDC_PRIMDIMENSRADDIAM, OnBnClick)
    ON_BN_CLICKED(IDC_PRIMDIMENSRADANG, OnBnClick)
    ON_BN_CLICKED(IDC_PRIMDIMENSRADSEL, OnBnClick)
    ON_EN_UPDATE(IDC_PRIMDIMENSWDTH, OnUpdateDialog)
    ON_EN_UPDATE(IDC_PRIMDIMENSTXTSIZE, OnUpdateDialog)
	ON_EN_UPDATE(IDC_PRIMDIMENSFRAC, OnUpdateDialog)
    ON_BN_CLICKED(IDC_ROTCENTRCOLOR, OnUpdateDialog)
    ON_BN_CLICKED(IDC_ROTCENTRCOLOR2, OnUpdateDialog)
    ON_BN_CLICKED(IDC_ROTCENTRCOLOR1, OnUpdateDialog)
END_MESSAGE_MAP()


// обработчики сообщений DPrimDimens

void DPrimDimens::OnBnHideAllClick()
{
    DimObjects& objects = _doc->Dimensions.GetObjects();

    bool hidden = objects.HideOrShowObjects(DimObjects::DimObjectDimensionLinearType);
    if (objects.IsHidden(DimObjects::DimObjectDimensionArcType) != hidden)
        objects.HideOrShowObjects(DimObjects::DimObjectDimensionArcType);
    if (objects.IsHidden(DimObjects::DimObjectDimensionCircleType) != hidden)
        objects.HideOrShowObjects(DimObjects::DimObjectDimensionCircleType);
    if (objects.IsHidden(DimObjects::DimObjectDimensionAngleType) != hidden)
        objects.HideOrShowObjects(DimObjects::DimObjectDimensionAngleType);

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimDimens::OnBnDelAllClick()
{
	_doc->Dimensions._dOutDimLin = DDimensions::DialogOutputDimLin();// To prevent access to imObject* _dimLine
    DimObjects& objects = _doc->Dimensions.GetObjects();

    objects.RemoveVisibleObjects(DimObjects::DimObjectDimensionLinearType);
    objects.RemoveVisibleObjects(DimObjects::DimObjectDimensionArcType);
    objects.RemoveVisibleObjects(DimObjects::DimObjectDimensionCircleType);
    objects.RemoveVisibleObjects(DimObjects::DimObjectDimensionAngleType);

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimDimens::OnBnHideClick()
{
    DimObjects& objects = _doc->Dimensions.GetObjects();

    bool hidden = objects.HideOrShowSelectedObjects(DimObjects::DimObjectDimensionLinearType);
    if (objects.IsHidden(DimObjects::DimObjectDimensionArcType) != hidden)
        objects.HideOrShowSelectedObjects(DimObjects::DimObjectDimensionArcType);
    if (objects.IsHidden(DimObjects::DimObjectDimensionCircleType) != hidden)
        objects.HideOrShowSelectedObjects(DimObjects::DimObjectDimensionCircleType);
    if (objects.IsHidden(DimObjects::DimObjectDimensionAngleType) != hidden)
        objects.HideOrShowSelectedObjects(DimObjects::DimObjectDimensionAngleType);

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimDimens::OnBnDelClick()
{
	if(_doc->Dimensions._dOutDimLin._dimLine)
		if (_doc->Dimensions._dOutDimLin._dimLine->GetSelected())
			_doc->Dimensions._dOutDimLin = DDimensions::DialogOutputDimLin();// To prevent access to imObject* _dimLine
	DimObjects& objects = _doc->Dimensions.GetObjects();

    bool deleted = objects.RemoveSelectedObjects(DimObjects::DimObjectDimensionLinearType);
    deleted = deleted || objects.RemoveSelectedObjects(DimObjects::DimObjectDimensionArcType);
    deleted = deleted || objects.RemoveSelectedObjects(DimObjects::DimObjectDimensionCircleType);
    deleted = deleted || objects.RemoveSelectedObjects(DimObjects::DimObjectDimensionAngleType);

    if (deleted)
    {
        _textStatus.Format(strs[20], 0);
        UpdateData(FALSE);
    }

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimDimens::OnBnClick()
{
    _doc->Dimensions.GetObjects().ClearSelectedObjects();
    
    OnUpdateDialog();

	SetTexts();
    
    UpdateData(FALSE);

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimDimens::FromDialog()
{
	DPrimBase::FromDialog();

	if (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreateDimension)
        _doc->Dimensions.GetObjects().ClearSelectedObjects();
    
    _doc->Dimensions.SetMouseClickAction(DDimensions::CreateDimension);
    _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
    _doc->Dimensions._hideWhenClosed = _hideWhenClosed != 0;

	UpdateValues();
 }

void DPrimDimens::UpdateDialog()
{
	Calc_ind();
    switch (_ind)
    {
    case 0:
        {
            DDimensions::DialogOutputDimLin& out = _doc->Dimensions._dOutDimLin;
            if (out._clickResult)
            {
                if (out._created)
                {// значит размер создан
                    BPoint pts[2];
                    out._dimLine->GetEndPoints(pts[0], pts[1]);
                    BPoint vect = pts[1] - pts[0];
                    double vals[3] = {fabs(vect * BPoint(1.0, 0.0, 0.0, 1.0)),
                                      fabs(vect * BPoint(0.0, 1.0, 0.0, 1.0)),
                                      fabs(vect * BPoint(0.0, 0.0, 1.0, 1.0))};

                    _textCoord.Format(strs[1], vals[0], vals[1], vals[2]);
                    _textStatus.Format(strs[2], out._dimVal);
                }
                else
                    _textStatus = out._neededObjType == 0 ? strs[4] : strs[5];
            }
            else
            {
                _textCoord = strs[0];
                _textStatus = strs[3];
            }

            break;
        }
    case 1:
    case 2:
        {
            DDimensions::DialogOutputDimLin& out = _doc->Dimensions._dOutDimLin;
            if (out._clickResult)
            {
                if (out._created)
                {
                    BPoint center = static_cast<DimObjectSegment*>(out._dimLine)->GetStartPoint();

                    _textCoord.Format(strs[7], center.GetX(), center.GetY(), center.GetZ());
                    _textStatus.Format(_ind == 1 ? strs[8] : strs[9], out._dimVal);
                }
                else
                    _textStatus = out._neededObjType == 1 ? strs[10] : strs[11];
            }
            else
            {
                _textCoord = strs[0];
                _textStatus = strs[3];
            }

            break;
        }
    case 3:
        {
            DDimensions::DialogOutputDimLin& out = _doc->Dimensions._dOutDimLin;
            if (out._clickResult)
            {
                if (out._created)
                {
                    BPoint center = static_cast<DimObjectArc*>(out._dimLine)->GetCenter();
                    _textCoord.Format(strs[7], center.GetX(), center.GetY(), center.GetZ());

                    unsigned int deg = (int)out._dimVal,
                                 min = (int)((out._dimVal - deg) * 60.0),
                                 sec = (int)((out._dimVal - (deg + (double)min / 60.0)) * 3600.0);
                    if ((1.0 - out._dimVal + deg) < MIND)
                    {
                        ++deg;
                        min = sec = 0;
                    }
                    _textStatus.Format(strs[12], deg, min, sec);
                }
                else
                {
                    switch (out._neededObjType)
                    {
                    case 1:
                        _textStatus = strs[14];
                        break;
                    case 2:
                        _textStatus = strs[16];
                        break;
                    case 3:
                        _textStatus = strs[17];
                        break;
                    case 4:
                        _textStatus = strs[15];
                        break;
                    };
                }
            }
            else
            {
                _textCoord = strs[0];
                _textStatus = strs[3];
            }

            break;
        }
    case 4:
        {
            _textCoord = strs[0];
            _textStatus.Format(strs[20], _doc->Dimensions.GetObjects().GetSelectedObjectsCount());
        }
    }

    UpdateData(FALSE);
}


BOOL DPrimDimens::OnInitDialog()
{
	DPrimBase::OnInitDialog();

// Modes
	CString ToolTip;
	_MaskSizeLength.SetImage(IDB_SIZELENGTHOFF);
	_MaskSizeLength.SetCheckedImage(IDB_SIZELENGTHON);
	_MaskSizeLength.SizeToContent(); 
	_MaskSizeLength.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskSizeLength.SetCheck(TRUE);
	ToolTip.LoadString(IDS_SIZELENGTH);
	_MaskSizeLength.SetTooltip(ToolTip);

	_MaskSizeRad.SetImage(IDB_SIZERADOFF);
	_MaskSizeRad.SetCheckedImage(IDB_SIZERADON);
	_MaskSizeRad.SizeToContent(); 
	_MaskSizeRad.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskSizeRad.SetCheck(FALSE);
	ToolTip.LoadString(IDS_SIZERAD);
	_MaskSizeRad.SetTooltip(ToolTip);

	_MaskSizeDiam.SetImage(IDB_SIZEDIAMOFF);
	_MaskSizeDiam.SetCheckedImage(IDB_SIZEDIAMON);
	_MaskSizeDiam.SizeToContent(); 
	_MaskSizeDiam.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskSizeDiam.SetCheck(FALSE);
	ToolTip.LoadString(IDS_SIZEDIAM);
	_MaskSizeDiam.SetTooltip(ToolTip);

	_MaskSizeAngle.SetImage(IDB_SIZEANGLEOFF);
	_MaskSizeAngle.SetCheckedImage(IDB_SIZEANGLEON);
	_MaskSizeAngle.SizeToContent(); 
	_MaskSizeAngle.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskSizeAngle.SetCheck(FALSE);
	ToolTip.LoadString(IDS_SIZEANGLE);
	_MaskSizeAngle.SetTooltip(ToolTip);

	_MaskSizeSelect.SetImage(IDB_SIZESELECTOFF);
	_MaskSizeSelect.SetCheckedImage(IDB_SIZESELECTON);
	_MaskSizeSelect.SizeToContent(); 
	_MaskSizeSelect.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskSizeSelect.SetCheck(FALSE);
	ToolTip.LoadString(IDS_SIZESELECT);
	_MaskSizeSelect.SetTooltip(ToolTip);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DPrimDimens::Calc_ind(void)
{
	if(_MaskSizeLength.IsChecked())
		_ind = 0;
	else if(_MaskSizeRad.IsChecked())
		_ind = 1;
	else if(_MaskSizeDiam.IsChecked())
		_ind = 2;
	else if(_MaskSizeAngle.IsChecked())
		_ind = 3;
	else if(_MaskSizeSelect.IsChecked())
		_ind = 4;
}

void DPrimDimens::UpdateValues()
{
    FillXMLfromDialog();
    FillDataFromXml();
    Calc_ind();

	DDimensions::DialogDataDim* data = &_doc->Dimensions._dPrimDim;
	data->_createArc = _createArc != 0;
	data->_createPt = _createPt != 0;
	data->_ind = _ind;
}

void DPrimDimens::FillDataFromXml()
{
    DDimensions::DialogDataDim* data = &_doc->Dimensions._dPrimDim;
    // from XML to data
    CString Key(_T("Defaults/Dimensions/Dimension/"));
    NColor Col = NCM_PROJECT.Defaults.GetColor((Key + CString(_T("ColorLines"))).GetBuffer(), NColor());
    data->_color[0] = Col.R();
    data->_color[1] = Col.G();
    data->_color[2] = Col.B();
    Col = NCM_PROJECT.Defaults.GetColor((Key + CString(_T("ColorBack"))).GetBuffer(), NColor());
    data->_backText[0] = Col.R();
    data->_backText[1] = Col.G();
    data->_backText[2] = Col.B();
    Col = NCM_PROJECT.Defaults.GetColor((Key + CString(_T("ColorText"))).GetBuffer(), NColor());
    data->_colorText[0] = Col.R();
    data->_colorText[1] = Col.G();
    data->_colorText[2] = Col.B();
    Key += _T("Sizes@");
    data->_width = float(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Thickness"))).GetBuffer(), 2));
    data->_dimTextSize = int(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Text"))).GetBuffer(), 10));
    data->_dimTextDigits = int(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Digits"))).GetBuffer(), 2));
    data->_set = true;
}

void DPrimDimens::FillXMLfromDialog()
{
    // from dialog to XML
    CString Key(_T("Defaults/Dimensions/Dimension/"));
    NCM_PROJECT.Defaults.SetColor((Key + CString(_T("ColorLines"))).GetBuffer(), NColor().SetRGB(_color0.GetColor()));
    NCM_PROJECT.Defaults.SetColor((Key + CString(_T("ColorText"))).GetBuffer(), NColor().SetRGB(_color1.GetColor()));
    NCM_PROJECT.Defaults.SetColor((Key + CString(_T("ColorBack"))).GetBuffer(), NColor().SetRGB(_colorBack.GetColor()));
    Key += _T("Sizes@");
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Thickness"))).GetBuffer(), _width);
    NCM_PROJECT.Defaults.SetInt((Key + CString(_T("Text"))).GetBuffer(), _dimTextSize);
    NCM_PROJECT.Defaults.SetInt((Key + CString(_T("Digits"))).GetBuffer(), _dimTextDigits);
}

void DPrimDimens::SetTexts()
{
	Calc_ind();

	_textCoord = _textStatus = strs[0];
	switch (_ind)
	{
	case 0:
		_textHelp = strs[6];
		break;
	case 1:
	case 2:
		_textHelp = strs[13];
		break;
	case 3:
		_textHelp = strs[18];
		break;
	case 4:
		_textHelp = strs[19];
		break;
	}
}
