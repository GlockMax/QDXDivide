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

    /// Метод добавляет объект
    unsigned int Add(class DimObject* iDimObject);

    /// Метод удаляет объект
    void Remove(unsigned int iID) { Remove(ObjArray.find(iID)); }

    /// Метод удаляет один дубликат объекта
    void RemoveDuplicate(class DimObject* iDimObject);

    /// Метод удаляет дубликаты объекта
    void RemoveDuplicates(class DimObject* iDimObject);

    /// Метод возвращает объект
    class DimObject* Get(unsigned int iID) const;
    
    /// Признак подсвеченности указанного объекта
    void SetHighlighted(unsigned int iID, bool iHighlighted);
    bool GetHighlighted(unsigned int iID) const;

    void SetSelected(unsigned int iID, bool iSelected);
    bool GetSelected(unsigned int iID) const;

    unsigned int GetCount() const { return (unsigned int)ObjArray.size(); }

    /// Метод возвращает объект из списка добавленных
    class DimObject* GetAddedObj(unsigned int iIndex) const { return Get(_addedIDs[iIndex]); }
    unsigned int GetAddedID(unsigned int iIndex) const { return _addedIDs[iIndex]; }

    std::map<unsigned int, class DimObject *>::iterator GetObjectsBegin() { return ObjArray.begin(); }
    std::map<unsigned int, class DimObject *>::iterator GetObjectsEnd() { return ObjArray.end(); }

    /// Метод устанавливает ID объекта, который подсвечен в данный момент
    void SetHighlightedID(unsigned int iID) { _highlightedID = iID; }
    unsigned int GetHighlightedID() const { return _highlightedID; }
    class DimObject* GetHighlightedObj() const { return Get(_highlightedID); }

    /// Метод устанавливает ID объекта, который активен в данный момент
    void SetSelectedID(unsigned int iID) { _selectedID = iID; }
    unsigned int GetSelectedID() { return _selectedID; }
    class DimObject* GetSelectedObj() { return Get(_selectedID); }

    /// Метод устанавливает ID изменяемого объекта
    void SetChangingID(unsigned int iID) { _changingID = iID; }
    unsigned int GetChangingID() const { return _changingID; }
    class DimObject* GetChangingObj() const { return Get(_changingID); }

    /// Типы объектов
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

    /// Метод скрывает или показывает все DimObject
    bool HideOrShowObjects(DimObjectsType iType);
    bool HideOrShowObjects(DimObjectsType iType, bool iHide);

    /// Метод скрывает или показывает все DimObject из списка выбранных
    bool HideOrShowSelectedObjects(DimObjectsType iType);
    
    /// Признак видимости объектов заданного типа
    bool IsHidden(DimObjectsType iType) const { return _hiddenObjExist[(int)iType]; }

    /// Метод удаляет все видимые DimObject
    bool RemoveVisibleObjects(DimObjectsType iType);

    /// Метод удаляет все выбранные DimObject
    bool RemoveSelectedObjects(DimObjectsType iType);

    /// Метод добавляет ID объекта к списку ID выбранных объектов
    void AddSelectedObjectID(unsigned int iID);// { _selectedIDs.push_back(iID); }
    void AddSelectedObjectID(class DimObject* iDimObject);
    //void AddSelectedObjectID() { if (_selectedID != 0) _selectedIDs.push_back(_selectedID); }

    /// Метод возвращает количество выбранных объектов
    unsigned int GetSelectedObjectsCount() const { return (unsigned int)_selectedIDs.size(); }

    /// Метод очищает список выбранных объектов и удаляет вспомогательные
    void ClearSelectedObjects();// { _selectedIDs.clear(); }

    /// Метод возвращает объект из списка выбранных
    /// iIndex - номер объекта в списке выбранных
    class DimObject* GetSelectedObj(unsigned int iIndex) const { return Get(_selectedIDs[iIndex]); }

    /// Метод возвращает указанное количество объектов заданного типа с конца
    bool GetLastFewObjects(unsigned int iCount, DimObjectsType iType, std::vector<class DimObject*>& oObjects) const;
    
    /// Метод возвращает текущий коэффициент масштабирования
    static double GetScaleKoef() { return _scaleKoef; }

    /// Габариты
    BBox GetGabar() const;

	virtual void Serialize(CArchive& ar);
	void Clear(void);
	void Fill_hiddenObjExist(void);
	DimObject *GetTyped(DimObjectsType ObjType);
	void AddObjectStock(class DimObjectStock *pDStock);
	void ChangeObjectStock(class DimObjectStock *pDStock);

protected:

    void Remove(const std::map<unsigned int, class DimObject *>::iterator& iIt);

    std::map<unsigned int, class DimObject *> ObjArray; // набор объектов

    std::vector<unsigned int> _addedIDs; // набор индексов объектов в порядке добавления объектов

    unsigned int _highlightedID, _selectedID, _changingID;

    bool _hiddenObjExist[DimObjectsTypeSize]; // признаки наличия скрытых DimObject

    std::vector<unsigned int> _selectedIDs; // набор индексов объектов в порядке выбора объектов

    static double _scaleKoef; // коэффициент масштабирования
};

