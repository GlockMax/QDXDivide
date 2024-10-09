#pragma once
#include "COMPARE_API.h"
#include "BPoint.h"
#include "BBox.h"
#include <vector>
#include <map>

class COMPARE_API DimObjects
{
public:
    DimObjects(void);
    ~DimObjects(void);

	void Draw(bool Not4Pick) const;
	//std::vector<class DimObject *>& GetObjArray(void) { return ObjArray;}

    /// ����� ��������� ������
    unsigned int Add(class DimObject* iDimObject);

    /// ����� ������� ������
    void Remove(unsigned int iID) { Remove(ObjArray.find(iID)); }

    /// ����� ������� ���� �������� �������
    void RemoveDuplicate(class DimObject* iDimObject);

    /// ����� ������� ��������� �������
    void RemoveDuplicates(class DimObject* iDimObject);

    /// ����� ���������� ������
    class DimObject* Get(unsigned int iID) const;
    
    /// ������� �������������� ���������� �������
    void SetHighlighted(unsigned int iID, bool iHighlighted);
    bool GetHighlighted(unsigned int iID) const;

    void SetSelected(unsigned int iID, bool iSelected);
    bool GetSelected(unsigned int iID) const;

    unsigned int GetCount() const { return (unsigned int)ObjArray.size(); }

    /// ����� ���������� ������ �� ������ �����������
    class DimObject* GetAddedObj(unsigned int iIndex) const { return Get(_addedIDs[iIndex]); }
    unsigned int GetAddedID(unsigned int iIndex) const { return _addedIDs[iIndex]; }

    std::map<unsigned int, class DimObject *>::iterator GetObjectsBegin() { return ObjArray.begin(); }
    std::map<unsigned int, class DimObject *>::iterator GetObjectsEnd() { return ObjArray.end(); }

    /// ����� ������������� ID �������, ������� ��������� � ������ ������
    void SetHighlightedID(unsigned int iID) { _highlightedID = iID; }
    unsigned int GetHighlightedID() const { return _highlightedID; }
    class DimObject* GetHighlightedObj() const { return Get(_highlightedID); }

    /// ����� ������������� ID �������, ������� ������� � ������ ������
    void SetSelectedID(unsigned int iID) { _selectedID = iID; }
    unsigned int GetSelectedID() { return _selectedID; }
    class DimObject* GetSelectedObj() { return Get(_selectedID); }

    /// ����� ������������� ID ����������� �������
    void SetChangingID(unsigned int iID) { _changingID = iID; }
    unsigned int GetChangingID() const { return _changingID; }
    class DimObject* GetChangingObj() const { return Get(_changingID); }

    /// ���� ��������
    enum DimObjectsType
    {
        DimObjectType = 0,
        DimObjectPointType,
        DimObjectSegmentType,
        DimObjectArcType,
        DimObjectCircleType,
        DimObjectPlaneType,
        DimObjectDimensionType,
        DimObjectDimensionLinearType,
        DimObjectDimensionArcType,
        DimObjectDimensionCircleType,
        DimObjectDimensionAngleType,
		DimObjectStockType,
        DimObjectSegmentEdgeType
    };
	static const int DimObjectsTypeSize = 13; // Must correspond to real DimObjectsType size

    /// ����� �������� ��� ���������� ��� DimObject
    bool HideOrShowObjects(DimObjectsType iType);
    bool HideOrShowObjects(DimObjectsType iType, bool iHide);

    /// ����� �������� ��� ���������� ��� DimObject �� ������ ���������
    bool HideOrShowSelectedObjects(DimObjectsType iType);
    
    /// ������� ��������� �������� ��������� ����
    bool IsHidden(DimObjectsType iType) const { return _hiddenObjExist[(int)iType]; }

    /// ����� ������� ��� ������� DimObject
    bool RemoveVisibleObjects(DimObjectsType iType);

    /// ����� ������� ��� ��������� DimObject
    bool RemoveSelectedObjects(DimObjectsType iType);

    /// ����� ��������� ID ������� � ������ ID ��������� ��������
    void AddSelectedObjectID(unsigned int iID);// { _selectedIDs.push_back(iID); }
    void AddSelectedObjectID(class DimObject* iDimObject);
    //void AddSelectedObjectID() { if (_selectedID != 0) _selectedIDs.push_back(_selectedID); }

    /// ����� ���������� ���������� ��������� ��������
    unsigned int GetSelectedObjectsCount() const { return (unsigned int)_selectedIDs.size(); }

    /// ����� ������� ������ ��������� �������� � ������� ���������������
    void ClearSelectedObjects();// { _selectedIDs.clear(); }

    /// ����� ���������� ������ �� ������ ���������
    /// iIndex - ����� ������� � ������ ���������
    class DimObject* GetSelectedObj(unsigned int iIndex) const { return Get(_selectedIDs[iIndex]); }

    /// ����� ���������� ��������� ���������� �������� ��������� ���� � �����
    bool GetLastFewObjects(unsigned int iCount, DimObjectsType iType, std::vector<class DimObject*>& oObjects) const;
    
    /// ����� ���������� ������� ����������� ���������������
    static double GetScaleKoef() { return _scaleKoef; }

    /// ��������
    BBox GetGabar() const;

	virtual void Serialize(CArchive& ar);
	void Clear(void);
	void Fill_hiddenObjExist(void);
	DimObject *GetTyped(DimObjectsType ObjType);
	void AddObjectStock(class DimObjectStock *pDStock);
	void ChangeObjectStock(class DimObjectStock *pDStock);

protected:

    void Remove(const std::map<unsigned int, class DimObject *>::iterator& iIt);

    std::map<unsigned int, class DimObject *> ObjArray; // ����� ��������

    std::vector<unsigned int> _addedIDs; // ����� �������� �������� � ������� ���������� ��������

    unsigned int _highlightedID, _selectedID, _changingID;

    bool _hiddenObjExist[DimObjectsTypeSize]; // �������� ������� ������� DimObject

    std::vector<unsigned int> _selectedIDs; // ����� �������� �������� � ������� ������ ��������

    static double _scaleKoef; // ����������� ���������������
};

