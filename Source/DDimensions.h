#pragma once
#include "DimObjects.h"
#include "DimObjectArc.h"
#include "DimObjectSegment.h"
#include "RPickID.h"
#include "BBox.h"
// DDimensions command target

// DDimensions
// DimObjects
// DimObject
// DimObjectPoint

class DDimensions : public CObject
{
public:
	DDimensions(class Snaps& SnapsGlob);
	virtual ~DDimensions();

	virtual void Serialize(CArchive& ar);
	void Activate(void);
    void Activate(class CNCMDoc* iDoc);
	void DeActivate(void);// { Active = false;}
	bool IsActivated(void) const { return Active; }
	bool IsActivatedForDraw(void) const { return ActiveForDraw; }
	DimObjects & GetObjects(void) { return Objects; }
	void Draw(bool Not4Pick = false) const;
	bool SetHighlighted(int ObjID);
	bool SetSelected(int ObjID);
	bool SelectHighlighted(void);

    void ActivateSnaps();
    void DeActivateSnaps();

    /// ���� �������� ��� ������� ������ ����
	enum MouseClickAction
	{
		EmptyClick = 0,
        CreatePoint,
        CreatePointWithAuto,
        CreatePointWithIntersection,
        CreateSegmentWithAuto,
        CreateSegmentWithTwoPoints,
        CreateSegmentWithTwoPlanes,
        CreateArcWithAuto,
        CreateArcWithThreePoints,
        CreatePlaneWithAuto,
        CreatePlaneWithThreePoints,
        CreateDimension,
		CreateStock
	};
    void SetMouseClickAction(MouseClickAction iAction) { _mouseClickAction.resize(1), _mouseClickAction[0] = iAction; }
    const MouseClickAction& GetMouseClickAction() const { return _mouseClickAction.back(); }
    void PushMouseClickAction(MouseClickAction iAction) { _mouseClickAction.push_back(iAction); }
    void PopMouseClickAction() { _mouseClickAction.size() == 1 ? _mouseClickAction[0] = EmptyClick : _mouseClickAction.resize(_mouseClickAction.size() - 1); }

    /// ��������� ����� �� ������� ������ ����
    void ProcessMouseClickPoint(const BPoint& iPoint, const BPoint* iNormal = nullptr, const BPoint* iViewDir = nullptr, RPickID* iPickID = nullptr, MouseClickAction iInstantAction = EmptyClick);

    /// ���� �������� ��� �������� ����
	enum MouseMoveAction
	{
        MMA_EmptyMove = 0,
		MMA_Change,
        MMA_Snap
	};
    void SetMouseMoveAction(MouseMoveAction iAction) { _mouseMoveAction.resize(1), _mouseMoveAction[0] = iAction; }
    const MouseMoveAction& GetMouseMoveAction() const { return _mouseMoveAction.back(); }
    void PushMouseMoveAction(MouseMoveAction iAction) { _mouseMoveAction.push_back(iAction); }
    void PopMouseMoveAction() { _mouseMoveAction.size() == 1 ? _mouseMoveAction[0] = MMA_EmptyMove : _mouseMoveAction.resize(_mouseMoveAction.size() - 1); }

    /// ����� ��������� �������������� �������
    bool PerformChanges(const BPoint& iPoint, const BPoint& iViewDir);

    /// ��������� �������� ���������� ���������� �������� � ������
    void SetAdditionObjectsToList(bool iAddition);
    bool GetAdditionObjectsToList() const { return _additionObjectsToList; }

    /// ��������
    BBox GetGabar() const { return Objects.GetGabar(); }

    /// ��������� ������� �� ������ ����
    bool GetMouseClickResult() const { return _clickResult; }

    /// �������� ��������������� �������� ��� �������� ��������
    void ClearAuxObjects();

    /// �������� ��������
    void Clear();


	class DimObjectStock * GetDimStock();
protected:

    bool CreatePtWithAuto(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects);
    bool CreatePtWithIntersection(std::vector<DimObject*>& oObjects);
    bool CreateSegmWithAuto(const BPoint& iPoint, const BPoint* iViewDir, std::vector<DimObject*>& oObjects);
	bool CreateSegmWithTwoPts(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects);
    bool CreateSegmWithTwoPlns(const BPoint& iPoint, const BPoint* iNormal, const BPoint* iViewDir, std::vector<DimObject*>& oObjects);
    bool CreateArcWithAutomat(const BPoint& iPoint, const BPoint* iViewDir, RPickID* iPickID, std::vector<DimObject*>& oObjects);
    bool CreateArcWithThreePts(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects, bool iFirstPtAsCenter = false);
    bool CreatePlnWithAuto(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects);
    bool CreatePlnWithThreePts(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects);
    bool CreateBoxStock(const BPoint& iPoint, const BPoint* iViewDir, const BPoint *normal, RPickID* iPickID, std::vector<DimObject*>& oObjects);

    bool CreateDim(const BPoint& iPoint, const BPoint* iViewDir, const BPoint* iNormal, std::vector<DimObject*>& oObjects);
    bool CreateLinDim(const BPoint& iPoint, std::vector<DimObject*>& oObjects);
    bool CreateDiamRadDim(const BPoint& iPoint, std::vector<DimObject*>& oObjects);
    bool CreateAngleDim(const BPoint& iPoint, const BPoint* iViewDir, std::vector<DimObject*>& oObjects);


	DimObjects Objects; // All dimensioning objects
	bool Active;
	bool ActiveForDraw;

    Snaps& _snaps; // ��������

    bool _additionObjectsToList; // ������� ���������� ���������� �������� � ������ DimObjects::_selectedIDs

    float _widthFromDialog; // ������/������ ������� ��� ��� �������� �� �������
    double _lengthFromDialog; // ����� ������� ��� ��� �������� �� �������
    double _colorFromDialog[3]; // ���� ������� ��� ��� �������� �� �������

    std::vector<MouseClickAction> _mouseClickAction; // �������� ������� �� ������ ����
    bool _clickResult; // ������� �� ��������

    std::vector<MouseMoveAction> _mouseMoveAction; // �������� �������� ����

    // <������ ��� ��������>
    
    /// ��������� ���������� �������
    struct DialogData
    {
        DialogData(): _width(2.0f), _set(false) {}

        float _width; // ������/������ ������� ��� ��� �������� �� �������
        double _color[3]; // ���� ������� ��� ��� �������� �� �������
        
        bool _set; // ������� ����, ��� ������ ������
    };

    /// ��������� ���������� ������� �������� �������
    struct DialogDataSegm: public DialogData
    {
        DialogDataSegm(): _length(100.0) {}

        double _length; // ����� ������� ��� ��� �������� �� �������
    };

    /// ��������� ���������� ������� �������� ����
    struct DialogDataArc: public DialogDataSegm
    {
        DialogDataArc(): _isCircle(0) {}
        
        int _isCircle; // ������� ���������� ����������, � �� ����
    };

    /// ��������� ���������� ������� �������� ��������
    struct DialogDataDim: public DialogData
    {
		DialogDataDim() : _dimTextSize(14), _createArc(false), _createPt(true), _ind(0), _dimTextDigits(2) {}

        int _dimTextSize;
        double _colorText[3];
        double _backText[3];
        bool _createArc;
        bool _createPt;
        int _ind; // ������ ������� �����������
		int _dimTextDigits;
    };

    /// ��������� ��� ������ � ������
    class DialogOutputDimLin
    {
	public:
        DialogOutputDimLin() : _created(false), _dimVal(0.), _dimLine(nullptr), _clickResult(false), _neededObjType(0) {}
        bool _created;

        DimObject* _dimLine;
        double _dimVal;

        unsigned int _neededObjType; // ��� ��������� �� �������, ������� ����� ������� ���������, ������� �� �������

        bool _clickResult;
    };

    DialogData _dPrimPoint;
    DialogDataSegm _dPrimSegm;
    DialogDataArc _dPrimArc;
    DialogDataSegm _dPrimPlane;
    DialogDataDim _dPrimDim;

    DialogOutputDimLin _dOutDimLin;

    BPoint _normalInPt; // ������� � �����, ������� �� ������
    std::vector<BPoint> _auxPts; // ��������������� ����� ��� ����������
    std::vector<unsigned int> _auxObjects; // ������� ��������������� �������� ��� ���������� ��������

    bool _hideWhenClosed;

    friend class DPrimDimens;
    friend class DPrimPoint;
    friend class DPrimSegment;
    friend class DPrimArc;
    friend class DPrimPlane;
};


