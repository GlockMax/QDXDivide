// DPrimPlane.cpp: файл реализации
//

#include "stdafx.h"
#include "DPrimPlane.h"
#include "DimObjectPlane.h"
#include "afxdialogex.h"

static CString strs[9];
//= {"",
//                             1   "Укажите плоскость на модели.",
//                             2   "Укажите первую из трёх точек.",
//                             3   "Укажите вторую из трёх точек.",
//                             4   "Укажите последнюю точку.",
//                             5   "Создана! Нормаль  X=%.3f Y=%.3f Z=%.3f",
//                             6   "Укажите три точки на модели",
//                             7   "Невозможно построить.",
//                             8   "(%.3f, %.3f, %.3f)"};

// диалоговое окно DPrimPlane

IMPLEMENT_DYNAMIC(DPrimPlane, DPrimBase)

DPrimPlane::DPrimPlane(CNCMDoc* iDoc)
	: DPrimBase(iDoc, DPrimPlane::IDD), _doc(iDoc)
    , _textCoord(_T(strs[0]))
    , _textStatus(_T(strs[0]))
    , _textHelp(_T(strs[1]))
    , _sideVal(100.0)
    , _width(2.0f)
    , _ind(0)
{
	strs[1].LoadString(IDS_DIM_PLANE1);
	strs[2].LoadString(IDS_DIM_ARC2);
	strs[3].LoadString(IDS_DIM_SIZE10);
	strs[4].LoadString(IDS_DIM_SIZE11);
	strs[5].LoadString(IDS_DIM_PLANE5);
	strs[6].LoadString(IDS_DIM_ARC6);
	strs[7].LoadString(IDS_DIM_SIZE3);
	strs[8].LoadString(IDS_DIM_PT3);

    FillDataFromXml();
    if (_doc->Dimensions._dPrimPlane._set)
    {
        DDimensions::DialogDataSegm* data = &_doc->Dimensions._dPrimPlane;
        _sideVal = data->_length;
        _width = data->_width;
        _color.SetColor(RGB(data->_color[0] * 255.0, data->_color[1] * 255.0, data->_color[2] * 255.0));
    }
    else
        _color.SetColor(0x00FF0000);
}

DPrimPlane::~DPrimPlane()
{
}

void DPrimPlane::DoDataExchange(CDataExchange* pDX)
{
    DPrimBase::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_PRIMPLANECOORD, _textCoord);
    DDX_Text(pDX, IDC_PRIMPLANESTATUS, _textStatus);
    DDX_Text(pDX, IDC_PRIMPLANEHELP, _textHelp);
    DDX_Control(pDX, IDC_ROTCENTRCOLOR, _color);
    DDX_Text(pDX, IDC_ROTCENRSIZE, _sideVal);
    DDX_Text(pDX, IDC_ROTCENRSIZE2, _width);
//    DDX_Radio(pDX, IDC_PRIMPLANEAUTO, _ind);
// Modes
	DDX_Control(pDX, IDC_PRIMPLANEAUTO, _MaskAutoPlane);
	DDX_Control(pDX, IDC_PRIMPLANE3PTS, _Mask3PtPlane);
}


BEGIN_MESSAGE_MAP(DPrimPlane, DPrimBase)
    ON_BN_CLICKED(IDC_CALC2, OnBnVisibilityClicked)
    ON_BN_CLICKED(IDC_CALC3, OnBnDeleteClicked)
    ON_BN_CLICKED(IDC_CALC4, OnBnAgainClicked)
    ON_BN_CLICKED(IDC_PRIMPLANEAUTO, OnBnAutomatClicked)
    ON_BN_CLICKED(IDC_PRIMPLANE3PTS, OnBnThreePtsClicked)
    ON_EN_UPDATE(IDC_ROTCENRSIZE, OnUpdateDialog)
    ON_EN_UPDATE(IDC_ROTCENRSIZE2, OnUpdateDialog)
    ON_BN_CLICKED(IDC_ROTCENTRCOLOR, OnUpdateDialog)
END_MESSAGE_MAP()


// обработчики сообщений DPrimPlane

void DPrimPlane::OnBnAutomatClicked()
{
    UpdateData();
    FromDialog();
    _textCoord = _textStatus = strs[0];
    _textHelp = strs[1];
    BnAutomat(true);
    UpdateData(FALSE);
}

void DPrimPlane::OnBnThreePtsClicked()
{
    UpdateData();
    FromDialog();
    _textCoord = strs[0];
    _textStatus = strs[2];
    _textHelp = strs[6];
    BnThreePts(true);
    UpdateData(FALSE);
}

void DPrimPlane::OnBnVisibilityClicked()
{
    _doc->Dimensions.GetObjects().HideOrShowObjects(DimObjects::DimObjectPlaneType);
    
    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimPlane::OnBnDeleteClicked()
{
    _doc->Dimensions.GetObjects().RemoveVisibleObjects(DimObjects::DimObjectPlaneType);

    _doc->UpdateAllViews(NULL, N_OGL);
}

void DPrimPlane::OnBnAgainClicked()
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

void DPrimPlane::UpdateValues()
{
    FillXMLfromDialog();
    FillDataFromXml();
}

void DPrimPlane::FillDataFromXml()
{
    DDimensions::DialogDataSegm* data = &_doc->Dimensions._dPrimPlane;
    // from XML to data
    CString Key(_T("Defaults/Dimensions/Plane/"));
    NColor Col = NCM_PROJECT.Defaults.GetColor((Key + CString(_T("Color"))).GetBuffer(), NColor());
    data->_color[0] = Col.R();
    data->_color[1] = Col.G();
    data->_color[2] = Col.B();
    Key += _T("Sizes@");
    data->_width = float(NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Thickness"))).GetBuffer(), 2));
    data->_length = NCM_PROJECT.Defaults.GetDouble((Key + CString(_T("Length"))).GetBuffer(), 100);
    data->_set = true;
}

void DPrimPlane::FillXMLfromDialog()
{
    // from dialog to XML
    CString Key(_T("Defaults/Dimensions/Plane/"));
    NCM_PROJECT.Defaults.SetColor((Key + CString(_T("Color"))).GetBuffer(), NColor().SetRGB(_color.GetColor()));
    Key += _T("Sizes@");
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Thickness"))).GetBuffer(), _width);
    NCM_PROJECT.Defaults.SetDouble((Key + CString(_T("Length"))).GetBuffer(), _sideVal);
}

void DPrimPlane::FromDialog()
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

void DPrimPlane::BnAutomat(bool iNoCheck)
{
    if (iNoCheck || (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreatePlaneWithAuto))
    {
        _doc->Dimensions.GetObjects().ClearSelectedObjects();

        _doc->Dimensions.SetMouseClickAction(DDimensions::CreatePlaneWithAuto);
        _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
    }
}

void DPrimPlane::BnThreePts(bool iNoCheck)
{
    if (iNoCheck || (_doc->Dimensions.GetMouseClickAction() != DDimensions::CreatePlaneWithThreePoints))
    {
        _doc->Dimensions.GetObjects().ClearSelectedObjects();

        _doc->Dimensions.SetMouseClickAction(DDimensions::CreatePlaneWithThreePoints);
        _doc->Dimensions.SetMouseMoveAction(DDimensions::MMA_Snap);
    }
}

void DPrimPlane::UpdateDialog()
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
        case DDimensions::CreatePlaneWithAuto:
            {
                BPoint normal = _doc->Dimensions._normalInPt;
                _textStatus.Format(strs[5], normal.GetX(), normal.GetY(), normal.GetZ());
                _doc->Dimensions._auxPts.clear();
                break;
            }
        case DDimensions::CreatePlaneWithThreePoints:
            {
                if (count == 3)
                {
                    unsigned int count = _doc->Dimensions.GetObjects().GetCount();
                    BPoint normal = static_cast<DimObjectPlane*>(_doc->Dimensions.GetObjects().GetAddedObj(count - 1))->GetNormal();
                    _textStatus.Format(strs[5], normal.GetX(), normal.GetY(), normal.GetZ());
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
            case DDimensions::CreatePlaneWithAuto:
                _textStatus = strs[0];
                break;
            case DDimensions::CreatePlaneWithThreePoints:
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

void DPrimPlane::PostNcDestroy()
{
	DPrimBase::PostNcDestroy();
	delete this;
}



BOOL DPrimPlane::OnInitDialog()
{
	DPrimBase::OnInitDialog();

// Modes
	CString ToolTip;
	_MaskAutoPlane.SetImage(IDB_CREATEAUTOPLANEOFF);
	_MaskAutoPlane.SetCheckedImage(IDB_CREATEAUTOPLANEON);
	_MaskAutoPlane.SizeToContent(); 
	_MaskAutoPlane.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_MaskAutoPlane.SetCheck(TRUE);
	ToolTip.LoadString(IDS_CREATEAUTOPLANE);
	_MaskAutoPlane.SetTooltip(ToolTip);

	_Mask3PtPlane.SetImage(IDB_CREATE3PTPLANEOFF);
	_Mask3PtPlane.SetCheckedImage(IDB_CREATE3PTPLANEON);
	_Mask3PtPlane.SizeToContent(); 
	_Mask3PtPlane.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	_Mask3PtPlane.SetCheck(FALSE);
	ToolTip.LoadString(IDS_CREATE3PTPLANE);
	_Mask3PtPlane.SetTooltip(ToolTip);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DPrimPlane::Calc_ind(void)
{
	if(_MaskAutoPlane.IsChecked())
		_ind = 0;
	else if(_Mask3PtPlane.IsChecked())
		_ind = 1;
}

void DPrimPlane::SetTexts()
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
