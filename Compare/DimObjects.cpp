#include "StdAfx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "NObjEnums.h"
#include "DimObject.h"
#include "DimObjects.h"
#include "DimObjectPoint.h"
#include "BaseRender.h"
#include "DimObjectSegmentEdge.h"
#include <algorithm>

double DimObjects::_scaleKoef = 1.0;

DimObjects::DimObjects(void): _highlightedID(0), _selectedID(0), _changingID(0)
{
	for(int i = 0; i < DimObjectsTypeSize; ++i)
		_hiddenObjExist[i] = false;
}

DimObjects::~DimObjects(void)
{
	Clear();
}

void DimObjects::Draw(bool Not4Pick) const
{
    // взять коэффициент масштабирования сцены
    if (Not4Pick)
    {
        double magn[3];
        BaseRender::GetPixelDim(magn[0], magn[1], magn[2]);
        _scaleKoef = magn[0];
	}

    // draw not higlighted first and then highlited objects
    for (auto It = ObjArray.begin(); It != ObjArray.end(); ++It)
    {
        if(It->first != _highlightedID)
            (*It).second->Draw();
    }
	for(auto It = ObjArray.begin(); It != ObjArray.end(); ++It)
    {
        if (It->first == _highlightedID)
            (*It).second->Draw();
    }
}

unsigned int DimObjects::Add(class DimObject* iDimObject)
{
    // удалить возможный дубликат
    RemoveDuplicate(iDimObject);

    // найти свободный индекс
    unsigned int index = 1;
    while (ObjArray.count(index) == 1)
        ++index;

    // добавить элемент
    iDimObject->SetObjectID(index);
    ObjArray[index] = iDimObject;

    // добавить индекс
    _addedIDs.push_back(index);

    return index;
}

void DimObjects::Remove(const std::map<unsigned int, class DimObject *>::iterator& iIt)
{
    if (iIt == ObjArray.end())
        return;

    unsigned int id = iIt->first;

    // удалить индекс из массива индексов
    std::vector<unsigned int>::iterator it = std::find(_addedIDs.begin(), _addedIDs.end(), id);
    if (it != _addedIDs.end())
        _addedIDs.erase(it);

    // удалить индекс из списка выбранных объектов
    if (!_selectedIDs.empty())
    {
        it = std::find(_selectedIDs.begin(), _selectedIDs.end(), id);
        if (it != _selectedIDs.end())
            _selectedIDs.erase(it);
    }

    // очистить индексы характерных объектов
    if (id == _highlightedID)
        _highlightedID = 0;

    if (id == _selectedID)
        _selectedID = 0;

    if (id == _changingID)
        _changingID = 0;

    // удалить объект
	delete iIt->second;
    ObjArray.erase(iIt);
	Fill_hiddenObjExist();
}

void DimObjects::RemoveDuplicate(class DimObject* iDimObject)
{
    for (auto it = ObjArray.begin(); it != ObjArray.end(); ++it)
        if (it->second->IsEqualTo(iDimObject))
        {
            Remove(it);
            break;
        }
}

void DimObjects::RemoveDuplicates(class DimObject* iDimObject)
{
    auto it = ObjArray.begin();
    ++it;
    for (it; it != ObjArray.end(); ++it)
        if (it->second->IsEqualTo(iDimObject))
            Remove(it--);

    it = ObjArray.begin();
    if (it->second->IsEqualTo(iDimObject))
        Remove(it);
}

class DimObject* DimObjects::Get(unsigned int iID) const
{
    auto it = ObjArray.find(iID); // так, чтобы несуществующий ID не добавлялся в map
    if (it == ObjArray.end())
        return nullptr;
    else
        return (*it).second;
}

void DimObjects::SetHighlighted(unsigned int iID, bool iHighlighted)
{
    auto it = ObjArray.find(iID);
    if (it != ObjArray.end())
        (*it).second->SetHighlighted(iHighlighted);
    return;
}

bool DimObjects::GetHighlighted(unsigned int iID) const
{
    auto it = ObjArray.find(iID);
    if (it == ObjArray.end())
        return false;
    else
        return (*it).second->GetHighlighted();
}

void DimObjects::SetSelected(unsigned int iID, bool iSelected)
{
    auto it = ObjArray.find(iID);
    if (it != ObjArray.end())
    {
        bool inList = false;
        for (unsigned int i = 0; i < _selectedIDs.size(); ++i)
            if (_selectedID == _selectedIDs[i])
            {
                inList = true;
                break;
            }

        if (!inList)
            (*it).second->SetSelected(iSelected);
    }
    return;
}

bool DimObjects::GetSelected(unsigned int iID) const
{
    auto it = ObjArray.find(iID);
    if (it == ObjArray.end())
        return false;
    else
        return (*it).second->GetSelected();
}

bool DimObjects::HideOrShowObjects(DimObjectsType iType)
{//true, если есть скрытые
    for (auto it = ObjArray.begin(); it != ObjArray.end(); ++it)
        if (it->second->GetType() == iType)
            it->second->Hide(!_hiddenObjExist[(int)iType]);

    _hiddenObjExist[(int)iType] = !_hiddenObjExist[(int)iType];

    return _hiddenObjExist[(int)iType];
}

bool DimObjects::HideOrShowObjects(DimObjectsType iType, bool iHide)
{//true, если есть скрытые
    for (auto it = ObjArray.begin(); it != ObjArray.end(); ++it)
        if (it->second->GetType() == iType)
            it->second->Hide(iHide);

    _hiddenObjExist[(int)iType] = iHide;

    return iHide;
}

bool DimObjects::HideOrShowSelectedObjects(DimObjectsType iType)
{//true, если есть скрытые
    for (unsigned int i = 0; i < _selectedIDs.size(); ++i)
    {
        DimObject* obj = GetSelectedObj(i);
        if (obj->GetType() == iType)
            obj->Hide(!_hiddenObjExist[(int)iType]);
    }

    _hiddenObjExist[(int)iType] = !_hiddenObjExist[(int)iType];

    return _hiddenObjExist[(int)iType];
}

bool DimObjects::RemoveVisibleObjects(DimObjectsType iType)
{// true, если что-то удалено
    if (ObjArray.empty())
        return false;
    
    unsigned int oldSize = (unsigned int)ObjArray.size();

    _hiddenObjExist[(int)iType] = false;
    auto it = ObjArray.begin();
    for (++it; it != ObjArray.end(); ++it)
    {
        if (it->second->GetType() == iType)
            if (it->second->IsVisible())
                Remove(it--);
            else
                _hiddenObjExist[(int)iType] = true;
    }

    it = ObjArray.begin();
    if (it->second->GetType() == iType)
        if (it->second->IsVisible())
            Remove(it);
        else
            _hiddenObjExist[(int)iType] = true;

    return ObjArray.size() != oldSize;
}

bool DimObjects::RemoveSelectedObjects(DimObjectsType iType)
{
    if (_selectedIDs.empty())
        return false;

    unsigned int oldSize = (unsigned int)ObjArray.size();
    
    _hiddenObjExist[(int)iType] = false;
    auto it = _selectedIDs.begin();
    for (++it; it != _selectedIDs.end(); ++it)
    {
        if (Get(*it)->GetType() == iType)
            if (Get(*it)->IsVisible())
                Remove(Get(*it--)->GetObjectID());
            else
                _hiddenObjExist[(int)iType] = true;
    }

    it = _selectedIDs.begin();
    if (Get(*it)->GetType() == iType)
        if (Get(*it)->IsVisible())
            Remove(Get(*it)->GetObjectID());
        else
            _hiddenObjExist[(int)iType] = true;
    
    return ObjArray.size() != oldSize;
}

void DimObjects::AddSelectedObjectID(unsigned int iID)
{
    if (iID != 0)
    {
        Get(iID)->SetSelected(true);
        _selectedIDs.push_back(iID);
    }
}

void DimObjects::AddSelectedObjectID(class DimObject* iDimObject)
{
    iDimObject->SetSelected(true);
    auto it = std::find(_selectedIDs.begin(), _selectedIDs.end(), iDimObject->GetObjectID());
    if (it != _selectedIDs.end())
    {// значит объект уже в списке выбранных, нужно "развыбрать"
        iDimObject->SetSelected(false);
        _selectedIDs.erase(it);
    }
    else
        _selectedIDs.push_back(iDimObject->GetObjectID());
}

void DimObjects::ClearSelectedObjects()
{
    for (unsigned int i = 0; i < _selectedIDs.size(); ++i)
        Get(_selectedIDs[i])->SetSelected(false);

    _selectedIDs.clear();
}

bool DimObjects::GetLastFewObjects(unsigned int iCount, DimObjectsType iType, std::vector<class DimObject*>& oObjects) const
{
    int k = 0, count = GetCount();
    for (int i = count - 1; i >= 0; --i)
    {
        DimObject* obj = GetAddedObj(i);
        if (obj->GetType() == iType)
        {
            oObjects.push_back(obj);
            ++k;
        }

        if (k == iCount)
            return true;
    }

    return false;
}

BBox DimObjects::GetGabar() const
{
    BBox box;

    for (auto it = ObjArray.begin(); it != ObjArray.end(); ++it)
        box.Expand(it->second->GetGabar());

    return box;
}

void DimObjects::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		for (auto it = ObjArray.begin(); it != ObjArray.end(); ++it)
		{
			if(it->second->GetType() != DimObjectSegmentEdgeType)
				ar << it->second;
		}
		DimObjectPoint Dummy(BPoint(0., 0., 0., -1.));
		ar << &Dummy;
	}
	else
	{
	    ObjArray.clear();
		DimObject *pDimObject;
		while(true)
		{
			ar >> (CObject *&)pDimObject;
			if(!pDimObject)
				break;
			if(pDimObject->IsDummy())
			{
				delete pDimObject;
				break;
			}
			if(pDimObject->GetType() == DimObjects::DimObjectStockType)
				AddObjectStock(static_cast<DimObjectStock *>(pDimObject));
			else
				Add(pDimObject);
		}
	}
}

void DimObjects::Clear()
{
	for (auto it = ObjArray.begin(); it != ObjArray.end(); ++it)
		delete it->second;
	ObjArray.clear();
}

void DimObjects::Fill_hiddenObjExist(void)
{
	for(int i = 0; i < DimObjectsTypeSize; ++i)
		_hiddenObjExist[i] = false;
    for (auto it = ObjArray.begin(); it != ObjArray.end(); ++it)
	{
		if(!it->second->IsVisible())
			_hiddenObjExist[int(it->second->GetType())] = true;
	}
}

DimObject *DimObjects::GetTyped(DimObjectsType ObjType)
{
	// Find an object with given type
	for (auto it = ObjArray.begin(); it != ObjArray.end(); ++it)
		if(it->second->GetType() == ObjType)
			return it->second;
	return nullptr;
}

void DimObjects::AddObjectStock(DimObjectStock *pDStock)
{
	Add(pDStock);
	ChangeObjectStock(pDStock);
}

void DimObjects::ChangeObjectStock(DimObjectStock *pDStock)
{
	RemoveVisibleObjects(DimObjects::DimObjectSegmentEdgeType);
	DimObjectSegmentEdge *Edges[12];
	pDStock->GenEdges(Edges);
	for(int i = 0; i < 12; ++i)
	{
		Add(Edges[i]);
		pDStock->SetEdgeID(i, Edges[i]->GetObjectID());
		Edges[i]->SetParent(pDStock->GetObjectID());
		Edges[i]->SetColor(pDStock->GetColor()._common);
		Edges[i]->SetDepthTest(pDStock->GetDepthTest());
	}
}
