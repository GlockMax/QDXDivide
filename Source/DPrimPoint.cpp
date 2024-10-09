// ..\Source\DPrimPoint.cpp : implementation file
//

#include "stdafx.h"
#include "DPrimPoint.h"
#include "afxdialogex.h"
#include "DimObjectPoint.h"
#include "NColor.h"

static CString strs[14];
//= {"",
//                                 "Укажите на объект.",
//                                 "Нет пересечения!",
//                                 "Точка  X=%.3f Y=%.3f Z=%.3f",
//                                 "Нормаль  X=%.3f Y=%.3f Z=%.3f",
//                                 "Создана! Координаты (%.3f, %.3f, %.3f)",
//                                 "Создана! На размерном примитиве.",
//                                 "Создана! На каркасе.",
//                                 "Создана! На модели.",
//                                 "Создана! Пересечение размерных примитивов.",
//                                 "Нормаль невозможно вычислить.",
//                                 "Укажите первый объект.",
//                                 "Укажите второй объект."};

// DPrimPoint dialog

IMPLEMENT_DYNAMIC(DPrimPoint, DPrimBase)

DPrimPoint::DPrimPoint(CNCMDoc* pDoc)
	: DPrimBase(pDoc, DPrimPoint::IDD), _doc(pDoc)
    , _x(0)
    , _y(0)
    , _z(0)
    , _r(2.0f)
    , _textCoord(_T(strs[0]))
    , _textNormal(_T(strs[0]))
    , _textStatus(_T(strs[1]))
	, _ind(0)
{
	strs[1].LoadString(IDS_DIM_PT1);
	strs[2].LoadString(IDS_DIM_PT2);
	strs[3].LoadString(IDS_DIM_PT3);
	strs[4].LoadString(IDS_DIM_PT4);
	strs[5].LoadString(IDS_DIM_PT5);
	strs[6].LoadString(IDS_DIM_PT6);
	strs[7].LoadString(IDS_DIM_PT7);
	strs[8].LoadString(IDS_DIM_PT8);
	strs[9].LoadString(IDS_DIM_PT9);
	strs[10].LoadString(IDS_DIM_PT10);
	strs[11].LoadString(IDS_DIM_PT11);
	strs[12].LoadString(IDS_DIM_PT12);

    FillDataFromXml();
    if (_doc->Dimensions._dPrimPoint._set)
    {
        DDimensions::DialogData* data = &_doc->Dimensions._dPrimPoint;
        _r = data->_width;
        _color.SetColor(RGB(data->_color[0] * 255.0, data->_color[1] * 255.0, data->_color[2] * 255.0));
    }
    else
        _color.SetColor(0x00FF0000);
}

DPrimPoint::~DPrimPoint()
{
}

void DPrimPoint::DoDataExchange(CDataExchange* pDX)
{
    DPrimBase::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FIRST_X, _x);
    DDX_Text(pDX, IDC_FIRST_Y, _y);
    DDX_Text(pDX, IDC_FIRST_Z, _z);
    DDX_Text(pDX, IDC_ROTCENRSIZE, _r);
	DDX_Control(pDX, IDC_ROTCENTRCOLOR, _color);
    DDX_Text(pDX, IDC_END_POINT, _textCoord);
    DDX_Text(pDX, IDC_RESULT, _textNormal);
    DDX_Text(pDX, IDC_RESULT2, _textStatus);
	DDX_Control(pDX, IDC_PRIMPOINTRADIO1, _autoPT);
	DDX_Control(pDX, IDC_PRIMPOINTRADIO2, _intersectPT);
}	

BEGIN_MESSAGE_MAP(DPrimPoint, DPrimBase)
    ON_BN_CLICKED(IDC_PRIMPOINTRADIO1, OnBnAutomatClicked)
    ON_BN_CLICKED(IDC_PRIMPOINTRADIO2, OnBnIntersectClicked)
    ON_BN_CLICKED(IDC_CALC, OnBnCreateClicked)
    ON_BN_CLICKED(IDC_CALC2, OnBnVisibilityClicked)
    ON_BN_CLICKED(IDC_CALC3, OnBnDeleteClicked)
    ON_EN_UPDATE(IDC_ROTCENRSIZE, OnUpdateDialog)
    ON_BN_CLICKED(IDC_ROTCENTRCOLOR, OnUpdateDialog)
END_MESSAGE_MAP()

void DPrimPoint::OnBnAutomatClicked()
{
    UpdateData();
    FromDialog();
	_textNormal = strs[0];
	_textStatus = strs[1];

    _doc->Dimensions.SetMouseClickAction(DDimensions::CreatePointWithAuto);
    _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);

    UpdateData(FALSE);
}

void DPrimPoint::OnBnIntersectClicked()
{
    UpdateData();
    FromDialog();
    _textNormal = strs[11];
    _textStatus = strs[1];

    _doc->Dimensions.SetMouseClickAction(DDimensions::CreatePointWithIntersection);
    _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
    _doc->Dimensions.DeActivateSnaps();

    UpdateData(FALSE);
}

void DPrimPoint::OnBnCreateClicked()
{
    UpdateData();
    FromDialog();

    _doc->Dimensions.PushMouseClickAction(DDimensions::CreatePoint);
    _doc->Dimensions.ProcessMouseClickPoint(BPoint(_x, _y, _z, 1.0));
    _doc->Dimensions.PopMouseClickAction();

    UpdateDialog();

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimPoint::OnBnVisibilityClicked()
{
    _doc->Dimensions.GetObjects().HideOrShowObjects(DimObjects::DimObjectPointType);
    
    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimPoint::OnBnDeleteClicked()
{
    _doc->Dimensions.GetObjects().RemoveVisibleObjects(DimObjects::DimObjectPointType);

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimPoint::UpdateValues()
{
    FillXMLfromDialog();
    FillDataFromXml();
}

void DPrimPoint::FillDataFromXml()
{
    DDimensions::DialogData* data = &_doc->Dimensions._dPrimPoint;
    // from XML to data
    CString Key(_T("Defaults/Dimensions/Point/"));
    NColor Col = NCM_PROJECT.Defaults.GetColor((Key + CString(_T("Color"))).GetBuffer(), NColor());
    data->_color[0] = Col.R();
    data->_color[1] = Col.G();
    data->_color[2] = Col.B();
    Key += _T("Sizes@");
    data->_width = float(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Diam"))).GetBuffer(), 2));
    data->_set = true;
}

void DPrimPoint::FillXMLfromDialog()
{
    // from dialog to XML
    CString Key(_T("Defaults/Dimensions/Point/"));
    NCM_PROJECT.Defaults.SetColor((Key + CString(_T("Color"))).GetBuffer(), NColor().SetRGB(_color.GetColor()));
    Key += _T("Sizes@");
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Diam"))).GetBuffer(), _r);
}

void DPrimPoint::FromDialog()
{
	DPrimBase::FromDialog();

	UpdateValues();

	Calc_ind();

    switch (_ind)
    {
    case 0:
        if (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreatePointWithAuto)
        {
            _doc->Dimensions.SetMouseClickAction(DDimensions::CreatePointWithAuto);
            _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
        }
        break;
    case 1:
        if (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreatePointWithIntersection)
        {
            _doc->Dimensions.GetObjects().ClearSelectedObjects();

            _doc->Dimensions.SetMouseClickAction(DDimensions::CreatePointWithIntersection);
            _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
        }
        break;
    };
}

void DPrimPoint::UpdateDialog()
{
    unsigned int count = _doc->Dimensions.GetObjects().GetCount();
    if (count == 0)
        return;

    if (_doc->Dimensions.GetMouseClickResult())
    {
        DimObject* objPt = _doc->Dimensions.GetObjects().GetAddedObj(count - 1);
        if (objPt != nullptr)
        {
            if (objPt->GetType() == DimObjects::DimObjectPointType)
            {
                BPoint pt = static_cast<DimObjectPoint*>(objPt)->GetBPoint();
                _x = ((int)(pt.GetX() * 1000.0)) / 1000.0;
                _y = ((int)(pt.GetY() * 1000.0)) / 1000.0;
                _z = ((int)(pt.GetZ() * 1000.0)) / 1000.0;
                _textCoord.Format(strs[3], _x, _y, _z);
            }
        }

        switch (_doc->Dimensions.GetMouseClickAction())
        {
        case DDimensions::CreatePoint:
            {
                // при нажатии этой кнопки последним объектом всегда будет точка
                 _textStatus.Format(strs[5], _x, _y, _z);
                _textNormal = strs[0];
                break;
            }
        case DDimensions::CreatePointWithAuto:
            {
                if (_doc->Dimensions._normalInPt.GetH() != -1.0)
                {
                    _textNormal.Format(strs[4], _doc->Dimensions._normalInPt.GetX(), _doc->Dimensions._normalInPt.GetY(), _doc->Dimensions._normalInPt.GetZ());
                    _textStatus = strs[8];
                }
                else
                {
                    _textNormal = strs[10];

                    switch (_doc->GetSnaps().GetBaseObjectType())
                    { // имена конкретных объектов
                    case Snaps::DimObjectType:
                        _textStatus = strs[6];
                        break;
                    case Snaps::SkeletonType:
                        _textStatus = strs[7];
                        break;
                    };
                }

                break;
            }
        case DDimensions::CreatePointWithIntersection:
            {
                switch (_doc->Dimensions.GetObjects().GetSelectedObjectsCount())
                {
                case 0:
                    switch (_doc->GetSnaps().GetBaseObjectType())
                    { // имена конкретных объектов
                    case Snaps::EmptyBaseObjType:
                        _textCoord = _textNormal = strs[0];
                        _textStatus = strs[2];
                        break;
                    case Snaps::DimObjectType:
                        _textStatus = strs[9];
                        _textNormal = strs[11];
                        break;
                    };
                    break;
                case 1:
                    _textNormal = strs[12];
                    break;
                };

                break;
            }
        };
    }
    else
    {
        if (_doc->Dimensions.GetMouseClickAction() == DDimensions::CreatePointWithIntersection)
        {
            _textCoord = _textNormal = strs[0];
            _textStatus = strs[2];
        }
        else if (_doc->Dimensions.GetMouseClickAction() == DDimensions::CreatePointWithAuto)
        {
            _textStatus = strs[1];
        }
    }

    UpdateData(FALSE);
}

void DPrimPoint::PostNcDestroy()
{
	DPrimBase::PostNcDestroy();
	delete this;
}

BOOL DPrimPoint::OnInitDialog()
{
	DPrimBase::OnInitDialog();
	_autoPT.SetImage(IDB_CREATEAUTPTOFF);
	_autoPT.SetCheckedImage(IDB_CREATEAUTPTON);
	_autoPT.SizeToContent(); 
	_autoPT.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_autoPT.SetCheck(TRUE);
	CString ToolTip;
	ToolTip.LoadString(IDS_CREATEAUTPT);
	_autoPT.SetTooltip(ToolTip);
	_intersectPT.SetImage(IDB_CREATEINTERSECTPTOFF);
	_intersectPT.SetCheckedImage(IDB_CREATEINTERSECTPTON);
	_intersectPT.SizeToContent(); 
	_intersectPT.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	ToolTip.LoadString(IDS_CREATEINTERSECTPT);
	_intersectPT.SetTooltip(ToolTip);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DPrimPoint::Calc_ind(void)
{
	if(_autoPT.IsChecked())
		_ind = 0;
	else if(_intersectPT.IsChecked())
		_ind = 1;
}

void DPrimPoint::SetTexts()
{
	Calc_ind();
	switch (_ind)
	{
	case 0: // автомат
		_textNormal = strs[0];
		_textStatus = strs[1];
		break;
	case 1: // пересечение
		_textNormal = strs[11];
		_textStatus = strs[1];
		break;
	}
}
