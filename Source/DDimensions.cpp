// DDimensions.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "NCM.h"
#include "NCMDoc.h"
#include "NColor.h"
#include "BPoint.h"
#include "Snaps.h"
#include "DimObjectPoint.h"
#include "DimObjectSegmentEdge.h"
#include "DimObjectArc.h"
#include "DimObjectCircle.h"
#include "DimObjectPlane.h"
#include "DimObjectDimensionLinear.h"
#include "DimObjectDimensionArc.h"
#include "DimObjectDimensionCircle.h"
#include "DimObjectDimensionAngle.h"
#include "DimObjectStock.h"
#include "DDimensions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// DDimensions

DDimensions::DDimensions(Snaps& SnapsGlob) : Active(false), ActiveForDraw(false), _snaps(SnapsGlob), _widthFromDialog(2.0), _additionObjectsToList(false),
    _mouseClickAction(1, EmptyClick), _clickResult(false), _mouseMoveAction(1, MMA_EmptyMove), _hideWhenClosed(true)
{
}

DDimensions::~DDimensions()
{
}


// DDimensions member functions


void DDimensions::Serialize(CArchive& ar)
{
	Objects.Serialize(ar);
	SerializeElements(ar, &DimObject::DimText.height, 1);
	SerializeElements(ar, &DimObject::DimText.width, 1);
	if(ar.IsLoading())
		DimObject::DimText.Init(wglGetCurrentDC(), USEFONTBITMAPS, "");
}

void DDimensions::Draw(bool Not4Pick) const
{
	if(!ActiveForDraw)
		return;

    glPushName(OG_DIMENS);
	Objects.Draw(Not4Pick);
	glPopName();
}

void DDimensions::Activate(class CNCMDoc* iDoc)
{
    Activate();
}

void DDimensions::DeActivate(void)
{
	if (_hideWhenClosed)
	{
		ActiveForDraw = false;
	}
    else
    {
        SetMouseClickAction(EmptyClick);
        SetMouseMoveAction(MMA_EmptyMove);
    }
    _snaps.DeActivate();
	Active = false;
}

void DDimensions::Activate(void)
{
	Active = true;
	ActiveForDraw = true;
    _snaps.SetRestartFlag();
}

bool DDimensions::SetHighlighted(int ObjID)
{
	// Returns true if attribute changed

    if (ObjID == Objects.GetHighlightedID())
        return false;

    if (Objects.GetHighlightedID() != 0)
        Objects.SetHighlighted(Objects.GetHighlightedID(), false);

    if (ObjID != 0)
        Objects.SetHighlighted(ObjID, true);

    Objects.SetHighlightedID(ObjID);

	return true;
}

bool DDimensions::SetSelected(int ObjID)
{
	// Returns true if attribute changed

    if (Objects.GetSelectedID() == ObjID)
        return false;

    if (Objects.GetSelectedID() != 0)
        Objects.SetSelected(Objects.GetSelectedID(), false);

    if (ObjID != 0)
        Objects.SetSelected(ObjID, true);

    Objects.SetSelectedID(ObjID);

	return true;
}

bool DDimensions::SelectHighlighted(void)
{
    return SetSelected(Objects.GetHighlightedID());
}

void DDimensions::ProcessMouseClickPoint(const BPoint& iPoint, const BPoint* iNormal, const BPoint* iViewDir, RPickID* iPickID, MouseClickAction iInstantAction)
{
    SelectHighlighted();

    std::vector<DimObject*> objs;

    bool clickResult = false;

    // считается, что, если нормали нет, значит iPoint не лежит на модели, и её обрабатывать не нужно
    const BPoint* normal = (_snaps.GetSnapObjectType() & Snaps::Model) ? iNormal : nullptr;

    switch (iInstantAction == EmptyClick ? GetMouseClickAction() : iInstantAction)
    {
    case CreatePoint:
        {
            objs.push_back(new DimObjectPoint(iPoint, _dPrimPoint._width));
            objs[0]->SetColor(_dPrimPoint._color);
            
            clickResult = true;

            break;
        }
    case CreatePointWithAuto:
        {
            clickResult = CreatePtWithAuto(iPoint, normal, objs);
            break;
        }
    case CreatePointWithIntersection:
        {
            clickResult = CreatePtWithIntersection(objs);
            break;
        }
    case CreateSegmentWithAuto:
        {
            clickResult = CreateSegmWithAuto(iPoint, iViewDir, objs);
            break;
        }
    case CreateSegmentWithTwoPoints:
        {
            clickResult = CreateSegmWithTwoPts(iPoint, normal, objs);
            break;
        }
    case CreateSegmentWithTwoPlanes:
        {
            clickResult = CreateSegmWithTwoPlns(iPoint, normal, iViewDir, objs);
            break;
        }
    case CreateArcWithAuto:
        {
            clickResult = CreateArcWithAutomat(iPoint, iViewDir, iPickID, objs);
            break;
        }
    case CreateArcWithThreePoints:
        {
            clickResult = CreateArcWithThreePts(iPoint, normal, objs);
            break;
        }
    case CreatePlaneWithAuto:
        {
            clickResult = CreatePlnWithAuto(iPoint, normal, objs);
            break;
        }
    case CreatePlaneWithThreePoints:
        {
            clickResult = CreatePlnWithThreePts(iPoint, normal, objs);
            break;
        }
    case CreateDimension:
        {
            clickResult = CreateDim(iPoint, iViewDir, normal, objs);
            break;
        }
    case CreateStock:
        {
			clickResult = CreateBoxStock(iPoint, iViewDir, normal, iPickID, objs);
            break;
        }
    default:
        break;
    }

    if (!objs.empty())
        for (unsigned int i = 0; i < (unsigned int)objs.size(); ++i)
            Objects.Add(objs[i]);

    _clickResult = clickResult;
}

bool DDimensions::CreatePtWithAuto(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects)
{
    _normalInPt = BPoint(0.0, 0.0, 0.0, -1.0);

    BPoint pt = _snaps.GetSnapPoint();
    if (!_snaps.GetFound())
        if (iNormal)
        {
            pt = iPoint;
            _normalInPt = *iNormal;
        }
        else
            return false;

    oObjects.push_back(new DimObjectPoint(pt, _dPrimPoint._width));
    oObjects[0]->SetColor(_dPrimPoint._color);

    return true;
}

bool DDimensions::CreatePtWithIntersection(std::vector<DimObject*>& oObjects)
{
    if (Objects.GetSelectedID() == 0)
        return true;

    // добавить объект в список выбранных
    Objects.AddSelectedObjectID(Objects.GetSelectedID());
    if (Objects.GetSelectedObjectsCount() < 2)
        return true;

    // пересечь выбранные объекты
    bool result = false;
    std::vector<BPoint> pts;
    DimObject::IntersectObjects(Objects.GetSelectedObj(0), Objects.GetSelectedObj(1), pts);
    if (!pts.empty() && !((dynamic_cast<DimObjectPlane*>(Objects.GetSelectedObj(0)) && dynamic_cast<DimObjectPlane*>(Objects.GetSelectedObj(1)))))
    {
        for (unsigned int i = 0; i < (unsigned int)pts.size(); ++i)
        {
            oObjects.push_back(new DimObjectPoint(pts[i], _dPrimPoint._width));
            oObjects[i]->SetColor(_dPrimPoint._color);
        }

        _snaps.SetBaseObjectType(Snaps::DimObjectType);

        result = true;
    }

    // очистить список выбранных
    Objects.ClearSelectedObjects();

    return result;
}

bool DDimensions::CreateSegmWithAuto(const BPoint& iPoint, const BPoint* iViewDir, std::vector<DimObject*>& oObjects)
{
    if (!_snaps.GetFound())
        return false;

    const BMatr ProjInv(BMatr().e0cong(BPoint(0., 0., 1., 0.), *iViewDir));

    _auxPts.push_back(_snaps.GetSnapPoint()); // нужно для вывода координат указанной точки
    
    BPoint p0 = _snaps.GetNeighRopePoint(0, ProjInv);
    if (!p0.IsPoint())
        return false;
    BPoint p1 = _snaps.GetNeighRopePoint(1, ProjInv);
    if (!p1.IsPoint())
        return false;

    BPoint dir = (p1 - p0).Unit() * (_dPrimSegm._length * 0.5);
    BPoint pts[2] = {_snaps.GetSnapPoint() - dir, _snaps.GetSnapPoint() + dir};
    oObjects.push_back(new DimObjectSegment(pts[0], pts[1], _dPrimSegm._width));
    oObjects[0]->SetColor(_dPrimSegm._color);

    return true;
}

bool DDimensions::CreateSegmWithTwoPts(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects)
{
    if (Objects.GetSelectedID() != 0)
    {
        DimObject* obj = Objects.GetSelectedObj();
        if (obj->GetType() == DimObjects::DimObjectPointType)
        {
            Objects.AddSelectedObjectID(obj->GetObjectID());
            _auxPts.push_back(static_cast<DimObjectPoint*>(obj)->GetBPoint());
        }
        else
        {
            DimObject* obj;
            if (_dPrimPoint._set)
            {
                obj = new DimObjectPoint(_snaps.GetSnapPoint(), _dPrimPoint._width);
                obj->SetColor(_dPrimPoint._color);
            }
            else
            {
                obj = new DimObjectPoint(_snaps.GetSnapPoint(), _dPrimSegm._width);
                obj->SetColor(_dPrimSegm._color);
            }
            Objects.Add(obj);
            Objects.AddSelectedObjectID(obj->GetObjectID());

            _auxPts.push_back(_snaps.GetSnapPoint());
        }
    }
    else 
    {
		BPoint pt = _snaps.GetSnapPoint();
		if (!_snaps.GetFound())
			if (iNormal)
				pt = iPoint;
			else
				return false;
		DimObject* obj;
        if (_dPrimPoint._set)
        {
            obj = new DimObjectPoint(pt, _dPrimPoint._width);
            obj->SetColor(_dPrimPoint._color);
        }
        else
        {
            obj = new DimObjectPoint(pt, _dPrimSegm._width);
            obj->SetColor(_dPrimSegm._color);
        }
        Objects.Add(obj);
        Objects.AddSelectedObjectID(obj->GetObjectID());

        _auxPts.push_back(_snaps.GetSnapPoint());
    }

    if (Objects.GetSelectedObjectsCount() < 2)
        return false;

    // создать с помощью точек отрезок
    bool result = false;
    BPoint pts[2] = {static_cast<DimObjectPoint*>(Objects.GetSelectedObj(0))->GetBPoint(),
                     static_cast<DimObjectPoint*>(Objects.GetSelectedObj(1))->GetBPoint()};

    double magn2 = (pts[1] - pts[0]).D2();
    if (magn2 > MIND * MIND)
    {
        oObjects.push_back(new DimObjectSegment(pts[0], pts[1], _dPrimSegm._width));
        oObjects[0]->SetColor(_dPrimSegm._color);
        result = true;
    }

    //// удалить точки
    //Objects.Remove(Objects.GetSelectedObj(1)->GetObjectID());
    //Objects.Remove(Objects.GetSelectedObj(0)->GetObjectID());
    Objects.ClearSelectedObjects();
    if (Objects.GetSelectedID() != 0)
    {
        Objects.GetSelectedObj()->SetSelected(false);
        Objects.SetSelectedID(0);
    }

    return result;
}

bool DDimensions::CreateSegmWithTwoPlns(const BPoint& iPoint, const BPoint* iNormal, const BPoint* iViewDir, std::vector<DimObject*>& oObjects)
{
    if (Objects.GetSelectedID() == 0)
    {
        if (!iNormal)
            return false;

        DimObject* plane;
        if (_dPrimPlane._set)
        {
            DimObjectPlane::CreateOnPointAndNormal(iPoint, *iNormal, plane, _dPrimPlane._length, _dPrimPlane._width);
            plane->SetColor(_dPrimPlane._color);
        }
        else
        {
            DimObjectPlane::CreateOnPointAndNormal(iPoint, *iNormal, plane, _dPrimSegm._length, _dPrimSegm._width);
            plane->SetColor(_dPrimSegm._color);
        }
        int id = Objects.Add(plane);
        Objects.AddSelectedObjectID(id);
        SetSelected(id);
        
        _auxPts.push_back(iPoint);
    }
    else if (Objects.GetSelectedObj()->GetType() == DimObjects::DimObjectPlaneType)
    {
        if (!iViewDir)
            return false;

        Objects.AddSelectedObjectID(Objects.GetSelectedID());

        // добавить точку, в которую указал курсор, чтобы потом определить длину отрезка
        DimObjectPlane* plane = static_cast<DimObjectPlane*>(Objects.GetSelectedObj());
        BPoint pt;
        AvailableTrace result = GrazingCurveElemContour::LinePlane(iPoint, *iViewDir, plane->GetOrigin(), plane->GetNormal(), pt);
        _auxPts.push_back(result == ONE_TRACE ? pt : iPoint);
    }
            
    if (Objects.GetSelectedObjectsCount() < 2)
        return false;

    // пересечь выбранные объекты
    DimObjectPlane* objPlanes[2] = {static_cast<DimObjectPlane*>(Objects.GetSelectedObj(0)), static_cast<DimObjectPlane*>(Objects.GetSelectedObj(1))};
    std::vector<BPoint> pts;
    DimObject::IntersectObjects(objPlanes[0], objPlanes[1], pts);
    if (pts.empty())
    {
        Objects.ClearSelectedObjects();
        return false;
    }

    BPoint tmpPts[2] = {GrazingCurveElemContour::PointProjLine(pts[0], pts[0] + pts[1], _auxPts[0]),
                        GrazingCurveElemContour::PointProjLine(pts[0], pts[0] + pts[1], _auxPts[1])};

    BPoint mid = (tmpPts[0] + tmpPts[1]) * 0.5;

    BPoint dir = pts[1] * _dPrimSegm._length * 0.5;

    oObjects.push_back(new DimObjectSegment(mid + dir, mid - dir, _dPrimSegm._width));
    oObjects[0]->SetColor(_dPrimSegm._color);

    // очистить список выбранных объектов
    Objects.ClearSelectedObjects();

    return true;
}

bool DDimensions::CreateArcWithAutomat(const BPoint& iPoint, const BPoint* iViewDir, RPickID* iPickID, std::vector<DimObject*>& oObjects)
{
    if (!iViewDir)
        return false;

    if (!iPickID)
        return false;

    if (!_snaps.GetFound())
        return false;
    
    _auxPts.push_back(_snaps.GetSnapPoint());
    
    std::vector<BPoint> segms;
    const BMatr ProjInv(BMatr().e0cong(BPoint(0., 0., 1., 0.), *iViewDir));
    BPoint prevPoint = _snaps.GetNeighRopePoint(-1, ProjInv);
    if (!prevPoint.IsPoint())
        return false;
    BPoint nextPoint = _snaps.GetNeighRopePoint(+1, ProjInv);
    if (!nextPoint.IsPoint())
        return false;
    bool success = false;
    DimObject* arc;
    if (_dPrimArc._isCircle)
        success = DimObjectCircle::CreateOnThreePoints(prevPoint, _snaps.GetSnapPoint(), nextPoint, arc, _dPrimArc._width, _dPrimArc._width, _dPrimArc._width);
    else
    {
        static const double k = PI / 180.0;
        success = DimObjectArc::CreateOnThreePoints(prevPoint, _snaps.GetSnapPoint(), nextPoint, _dPrimArc._length * k, arc, _dPrimArc._width, _dPrimArc._width, _dPrimArc._width);
    }
    if (success)
    {
        arc->SetColor(_dPrimArc._color);
        oObjects.push_back(arc);
    }

    return success;
}

bool DDimensions::CreateArcWithThreePts(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects, bool iFirstPtAsCenter)
{
    if (Objects.GetSelectedID() != 0)
    {
        DimObject* obj = Objects.GetSelectedObj();
        if (obj->GetType() == DimObjects::DimObjectPointType)
        {
            Objects.AddSelectedObjectID(obj->GetObjectID());
            _auxPts.push_back(static_cast<DimObjectPoint*>(obj)->GetBPoint());
        }
        else
        {
            BPoint pt = _snaps.GetSnapPoint();
            if (!_snaps.GetFound())
                if (iNormal)
                    pt = iPoint;
                else
                    return false;

            _auxPts.push_back(pt); // вектор очистится при выводе в диалог

            DimObject* objPt;
            if (_dPrimPoint._set)
            {
                objPt = new DimObjectPoint(pt, _dPrimPoint._width);
                objPt->SetColor(_dPrimPoint._color);
            }
            else
            {
                objPt = new DimObjectPoint(pt, _dPrimArc._width);
                objPt->SetColor(_dPrimArc._color);
            }
            SetSelected(Objects.Add(objPt));
            Objects.AddSelectedObjectID(Objects.GetSelectedID());
        }
    }
    else if (Objects.GetSelectedObjectsCount() < 3)
    {// создать точку на объекте через привязки
        BPoint pt = _snaps.GetSnapPoint();
        if (!_snaps.GetFound())
            if (iNormal)
                pt = iPoint;
            else
                return false;

        _auxPts.push_back(pt); // вектор очистится при выводе в диалог

        DimObject* objPt;
        if (_dPrimPoint._set)
        {
            objPt = new DimObjectPoint(pt, _dPrimPoint._width);
            objPt->SetColor(_dPrimPoint._color);
        }
        else
        {
            objPt = new DimObjectPoint(pt, _dPrimArc._width);
            objPt->SetColor(_dPrimArc._color);
        }
        SetSelected(Objects.Add(objPt));
        Objects.AddSelectedObjectID(Objects.GetSelectedID());
    }

    if (Objects.GetSelectedObjectsCount() < 3)
        return true;

    // создать с помощью точек дугу
    DimObjectPoint* objPts[3] = {static_cast<DimObjectPoint*>(Objects.GetSelectedObj(0)),
                                 static_cast<DimObjectPoint*>(Objects.GetSelectedObj(1)),
                                 static_cast<DimObjectPoint*>(Objects.GetSelectedObj(2))};
    
    DimObject* arc;
    bool success;
    if (_dPrimArc._isCircle)
        success = DimObjectCircle::CreateOnThreePoints(objPts[0]->GetBPoint(), objPts[1]->GetBPoint(), objPts[2]->GetBPoint(), arc, _dPrimArc._width, _dPrimArc._width, _dPrimArc._width);
    else if (iFirstPtAsCenter)
        success = DimObjectArc::Create(objPts[0]->GetBPoint(), objPts[1]->GetBPoint(), objPts[2]->GetBPoint(), arc, true, _dPrimArc._width, _dPrimArc._width, _dPrimArc._width);
    else
        success = DimObjectArc::CreateOnThreePoints(objPts[0]->GetBPoint(), objPts[1]->GetBPoint(), objPts[2]->GetBPoint(), arc, _dPrimArc._width, _dPrimArc._width, _dPrimArc._width);
    if (success)
    {
        arc->SetColor(_dPrimArc._color);
        oObjects.push_back(arc);
    }

    //// удалить точки
    //Objects.Remove(objPts[2]->GetObjectID());
    //Objects.Remove(objPts[1]->GetObjectID());
    //Objects.Remove(objPts[0]->GetObjectID());
    Objects.ClearSelectedObjects();
    if (Objects.GetSelectedID() != 0)
    {
        Objects.GetSelectedObj()->SetSelected(false);
        Objects.SetSelectedID(0);
    }

    return success;
}

bool DDimensions::CreatePlnWithAuto(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects)
{
    if (!iNormal)
        return false;

    _auxPts.push_back(iPoint);
    _normalInPt = *iNormal;

    oObjects.resize(1);
    DimObjectPlane::CreateOnPointAndNormal(iPoint, _normalInPt, oObjects[0], _dPrimPlane._length, _dPrimPlane._width);
    oObjects[0]->SetColor(_dPrimPlane._color);

    return true;
}

bool DDimensions::CreatePlnWithThreePts(const BPoint& iPoint, const BPoint* iNormal, std::vector<DimObject*>& oObjects)
{
    if (Objects.GetSelectedID() != 0)
    {
        DimObject* obj = Objects.GetSelectedObj();
        if (obj->GetType() == DimObjects::DimObjectPointType)
        {
            Objects.AddSelectedObjectID(obj->GetObjectID());
            _auxPts.push_back(static_cast<DimObjectPoint*>(obj)->GetBPoint());
        }
        else
        {
            BPoint pt = _snaps.GetSnapPoint();
            if (!_snaps.GetFound())
                if (iNormal)
                    pt = iPoint;
                else
                    return false;

            _auxPts.push_back(pt); // вектор очистится при выводе в диалог

            DimObject* objPt;
            if (_dPrimPoint._set)
            {
                objPt = new DimObjectPoint(pt, _dPrimPoint._width);
                objPt->SetColor(_dPrimPoint._color);
            }
            else
            {
                objPt = new DimObjectPoint(pt, _dPrimPlane._width);
                objPt->SetColor(_dPrimPlane._color);
            }
            SetSelected(Objects.Add(objPt));
            Objects.AddSelectedObjectID(Objects.GetSelectedID());
        }
    }
    else
    {// создать точку на объекте через привязки
        BPoint pt = _snaps.GetSnapPoint();
        if (!_snaps.GetFound())
            if (iNormal)
                pt = iPoint;
            else
                return false;

        _auxPts.push_back(pt); // вектор очистится при выводе в диалог

        DimObject* objPt;
        if (_dPrimPoint._set)
        {
            objPt = new DimObjectPoint(pt, _dPrimPoint._width);
            objPt->SetColor(_dPrimPoint._color);
        }
        else
        {
            objPt = new DimObjectPoint(pt, _dPrimPlane._width);
            objPt->SetColor(_dPrimPlane._color);
        }
        SetSelected(Objects.Add(objPt));
        Objects.AddSelectedObjectID(Objects.GetSelectedID());
    }

    if (Objects.GetSelectedObjectsCount() < 3)
        return true;

    // создать с помощью точек плоскость
    DimObjectPoint* objPts[3] = {static_cast<DimObjectPoint*>(Objects.GetSelectedObj(0)),
                                 static_cast<DimObjectPoint*>(Objects.GetSelectedObj(1)),
                                 static_cast<DimObjectPoint*>(Objects.GetSelectedObj(2))};

    DimObject* plane;
    bool success = DimObjectPlane::CreateOnThreePoints(objPts[0]->GetBPoint(), objPts[1]->GetBPoint(), objPts[2]->GetBPoint(), plane, _dPrimPlane._width);
    if (success)
    {
        plane->SetColor(_dPrimPlane._color);
        oObjects.push_back(plane);
    }

    //// удалить точки
    //Objects.Remove(objPts[2]->GetObjectID());
    //Objects.Remove(objPts[1]->GetObjectID());
    //Objects.Remove(objPts[0]->GetObjectID());
    Objects.ClearSelectedObjects();
    if (Objects.GetSelectedID() != 0)
    {
        Objects.GetSelectedObj()->SetSelected(false);
        Objects.SetSelectedID(0);
    }

    return success;
}

bool DDimensions::CreateDim(const BPoint& iPoint, const BPoint* iViewDir, const BPoint* iNormal, std::vector<DimObject*>& oObjects)
{
    bool success = false;

    // добавить объект в список объектов, по которым построится размер
    if (_dPrimDim._ind == 4)
    {
        if (Objects.GetSelectedID() != 0)
            if (Objects.GetSelectedObj()->IsDimension())
            {
                Objects.AddSelectedObjectID(Objects.GetSelectedObj());
                success = true;
            }
    }
    else if (Objects.GetSelectedID() != 0 && Objects.GetSelectedObj()->IsDimension())
    {// если указали на размер, и редактирование не включено, то начать, иначе прекратить
        if (Objects.GetChangingID() == 0)
        {
			Objects.GetSelectedObj()->EnableChangeState(iPoint, *iViewDir);
			Objects.SetChangingID(Objects.GetSelectedID());
			PushMouseMoveAction(MMA_Change);
        }
        else
        {
            Objects.GetChangingObj()->DisableChangeState();
            Objects.SetChangingID(0);
            PopMouseMoveAction();
        }
        success = true;
    }
    else if (Objects.GetChangingID() != 0)
    {
        Objects.GetChangingObj()->DisableChangeState();
        Objects.SetChangingID(0);
        PopMouseMoveAction();
        success = true;
    }
    else
    {
        BPoint pt(0.0, 0.0, 0.0, -1.0);
        if (_snaps.GetFound())
            pt = _snaps.GetSnapPoint();
        else if (iNormal)
            pt = iPoint;

        if (Objects.GetSelectedID() != 0 && !Objects.GetSelectedObj()->IsDimension() && !_snaps.GetFound())
        {
            Objects.AddSelectedObjectID(Objects.GetSelectedObj());
            success = true;
        }
		else if (pt.GetH() > 0.0)
		{
			DimObject* objPt;
			if (_dPrimPoint._set)
			{
				objPt = new DimObjectPoint(pt, _dPrimPoint._width);
				objPt->SetColor(_dPrimPoint._color);
			}
			else
			{
				objPt = new DimObjectPoint(pt, _dPrimDim._width);
				objPt->SetColor(_dPrimDim._color);
			}
			Objects.AddSelectedObjectID(Objects.Add(objPt));
			_auxObjects.push_back(Objects.GetSelectedObj(Objects.GetSelectedObjectsCount() - 1)->GetObjectID());
			success = true;
		}
	}

    if (Objects.GetSelectedObjectsCount() == 0)
        return true;

    // построить размер
    switch (_dPrimDim._ind)
    {
    case 0: // линейный размер
        success = CreateLinDim(iPoint, oObjects);
        break;
    case 1: // радиус
		_dPrimArc._isCircle = FALSE;
        success = CreateDiamRadDim(iPoint, oObjects);
        break;
	case 2: // диаметр
		_dPrimArc._isCircle = TRUE;
        success = CreateDiamRadDim(iPoint, oObjects);
        break;
    case 3: // угол
        success = CreateAngleDim(iPoint, iViewDir, oObjects);
        break;
    };

    _dOutDimLin._clickResult = success;
    _dOutDimLin._created = !oObjects.empty();

    if (_dOutDimLin._created)
    {
        _dOutDimLin._dimLine = static_cast<DimObjectDimension*>(oObjects[0])->GetDimLine();
        _dOutDimLin._dimVal = static_cast<DimObjectDimension*>(oObjects[0])->GetDimValue();

        static_cast<DimObjectDimension*>(oObjects[0])->SetRectColor(_dPrimDim._backText); // не должен совпадать с цветом линий
        static_cast<DimObjectDimension*>(oObjects[0])->SetColor(_dPrimDim._color);
        static_cast<DimObjectDimension*>(oObjects[0])->SetTextColor(_dPrimDim._colorText);
        static const float spheresKoef = 1.5f;
        static_cast<DimObjectDimension*>(oObjects[0])->SetSpheresRadius(_dPrimDim._width * spheresKoef);
		DimObject::SetTextHeight(_dPrimDim._dimTextSize);
		DimObject::SetDigits(_dPrimDim._dimTextDigits);

        ClearAuxObjects(); // удалить созданные точки, если они не нужны
        
        _auxPts.clear();
        Objects.ClearSelectedObjects();

        if (Objects.GetSelectedID() != 0)
        {
            Objects.GetSelectedObj()->SetSelected(false);
            Objects.SetSelectedID(0);
        }
    }

    if (!success)
    {
        ClearAuxObjects(); // удалить созданные точки, если они не нужны

        _auxPts.clear();
        Objects.ClearSelectedObjects();
    }

    return success;
}

bool DDimensions::CreateLinDim(const BPoint& iPoint, std::vector<DimObject*>& oObjects)
{
    if (Objects.GetSelectedObjectsCount() < 2)
    {
        
        _dOutDimLin._neededObjType = 0;
        const DimObject* pObj = Objects.GetSelectedObj(0);
        if (pObj == nullptr)
            return false;
        const auto Type = pObj->GetType();
        if (Type == DimObjects::DimObjectPointType)
            _dOutDimLin._neededObjType = 0; // подойдёт любой допустимый объект
        else if (Type == DimObjects::DimObjectSegmentType || Type == DimObjects::DimObjectPlaneType || Type == DimObjects::DimObjectArcType || Type == DimObjects::DimObjectCircleType)
            _dOutDimLin._neededObjType = 1; // подойдёт только точка
        else
            return false; // указан неверный объект

        return true;
    }

    BPoint pts[3];
    bool success = false;
    for (unsigned int ind = 0; ind < 2; ++ind)
        if (Objects.GetSelectedObj(ind)->GetType() == DimObjects::DimObjectPointType)
        {
            unsigned int ind2 = 1 - ind;

            pts[ind] = static_cast<DimObjectPoint*>(Objects.GetSelectedObj(ind))->GetBPoint();

            DimObjects::DimObjectsType type = Objects.GetSelectedObj(ind2)->GetType();
            if (type == DimObjects::DimObjectSegmentType)
            {
                BPoint a, b;
                Objects.GetSelectedObj(ind2)->GetEndPoints(a, b);
                pts[ind2] = GrazingCurveElemContour::PointProjLine(a, b, pts[ind]);
            }
            else if (type == DimObjects::DimObjectPlaneType)
            {
                pts[ind2] = BPoint::ProjectPointOnPlane(pts[ind], static_cast<DimObjectPlane*>(Objects.GetSelectedObj(ind2))->GetOrigin(), static_cast<DimObjectPlane*>(Objects.GetSelectedObj(ind2))->GetNormal());
            }
            else if (type == DimObjects::DimObjectPointType)
            {
                pts[ind2] = static_cast<DimObjectPoint*>(Objects.GetSelectedObj(ind2))->GetBPoint();
            }
            else
            {// здесь будет дуга или окружность
                pts[ind2] = static_cast<DimObjectArc*>(Objects.GetSelectedObj(ind2))->GetCenter();
            }

            success = true;
            break;
        }

    if (!success)
    {
        // two arcs case should be processed
        if ((Objects.GetSelectedObj(0)->GetType() == DimObjects::DimObjectArcType || Objects.GetSelectedObj(0)->GetType() == DimObjects::DimObjectCircleType)
            &&
            (Objects.GetSelectedObj(1)->GetType() == DimObjects::DimObjectArcType || Objects.GetSelectedObj(1)->GetType() == DimObjects::DimObjectCircleType))
        {
            pts[0] = static_cast<DimObjectArc*>(Objects.GetSelectedObj(0))->GetCenter();
            pts[1] = static_cast<DimObjectArc*>(Objects.GetSelectedObj(1))->GetCenter();
            success = true;
        }
    }
    if (!success)
        return false;

    oObjects.push_back(new DimObjectDimensionLinear(pts[0], pts[1], pts[1], _dPrimDim._width));

    return true;
}

bool DDimensions::CreateDiamRadDim(const BPoint& iPoint, std::vector<DimObject*>& oObjects)
{
    bool success = false;

    bool arc = false, none = false;
    unsigned int ptsCount = 0;
    for (ptsCount; ptsCount < Objects.GetSelectedObjectsCount(); ++ptsCount)
    {
        DimObjects::DimObjectsType type = Objects.GetSelectedObj(ptsCount)->GetType();
        if (type == DimObjects::DimObjectPointType)
        {
            _dOutDimLin._neededObjType = ptsCount + 1;
            continue;
        }
        else if (type == DimObjects::DimObjectArcType || type == DimObjects::DimObjectCircleType)
        {
            arc = true;
            break;
        }
        else
        {
            none = true;
            break;
        }
    }

    if (Objects.GetSelectedObjectsCount() < 3 && !arc)
        return true;

    if (none)
        return false;

    BPoint pts[2];
    std::vector<DimObject*> objArc;
    if (ptsCount == 3)
    {// размер по трём точкам
        if (Objects.GetSelectedID() != 0)
        {
            Objects.GetSelectedObj()->SetSelected(false);
            Objects.SetSelectedID(0);
        }
        
        if (!_dPrimArc._set)
        {
            _dPrimArc._color[0] = _dPrimDim._color[0];
            _dPrimArc._color[1] = _dPrimDim._color[1];
            _dPrimArc._color[2] = _dPrimDim._color[2];
        }
        success = CreateArcWithThreePts(iPoint, nullptr, objArc);
    }
    else if (arc)
    {
        pts[0] = static_cast<DimObjectArc*>(Objects.GetSelectedObj(ptsCount))->GetCenter();
        pts[1] = _snaps.GetFound() ? _snaps.GetSnapPoint() : Objects.GetSelectedObj(ptsCount)->GetMidPoint();
        // make copy to prevent conflicts while adding
        const auto pObj = Objects.GetSelectedObj(ptsCount);
        DimObject* pCopy = nullptr;
        switch (pObj->GetType())
        {
        case DimObjects::DimObjectCircleType:
            pCopy = new DimObjectCircle(*static_cast<DimObjectCircle*>(pObj));
            break;
        case DimObjects::DimObjectArcType:
            pCopy = new DimObjectArc(*static_cast<DimObjectArc*>(pObj));
            break;
        }
        objArc.push_back(pCopy);
        success = true;
    }

    if (success)
    {
        pts[0] = static_cast<DimObjectArc*>(objArc[0])->GetCenter();
        pts[1] = objArc[0]->GetMidPoint();

        DimObject* obj;
        if (_dPrimDim._ind == 2)
            if (objArc[0]->GetType() == DimObjects::DimObjectCircleType)
                obj = new DimObjectDimensionCircle(pts[0], pts[1], static_cast<DimObjectArc*>(objArc[0])->GetNormal(), _dPrimDim._width);
            else
                obj = new DimObjectDimensionArc(pts[0], pts[1], static_cast<DimObjectArc*>(objArc[0])->GetNormal(), _dPrimDim._width, false);
        else
            obj = new DimObjectDimensionArc(pts[0], pts[1], static_cast<DimObjectArc*>(objArc[0])->GetNormal(), _dPrimDim._width);
        
        oObjects.push_back(obj);

        if (_dPrimDim._createArc)
            oObjects.push_back(objArc[0]);
    }

    return success;
}

bool DDimensions::CreateAngleDim(const BPoint& iPoint, const BPoint* iViewDir, std::vector<DimObject*>& oObjects)
{
    int type = -1;

    unsigned int size = Objects.GetSelectedObjectsCount();
    switch (size)
    {
    case 1:
        {
            DimObjects::DimObjectsType objType = Objects.GetSelectedObj(0)->GetType();
            switch (objType)
            {
            case DimObjects::DimObjectArcType:
                type = 1; // дуга
                _dOutDimLin._neededObjType = 0;
                break;
            case DimObjects::DimObjectPointType:
                type = 0;
                _dOutDimLin._neededObjType = 1; // нужна вторая точка или отрезок
                break;
            case DimObjects::DimObjectSegmentType:
                type = 0;
                _dOutDimLin._neededObjType = 2; // нужен второй отрезок или точка
                break;
            case DimObjects::DimObjectPlaneType:
                type = 0;
                _dOutDimLin._neededObjType = 3; // нужна вторая плоскость
                break;
            };

            break;
        }
    case 2:
        {
            DimObjects::DimObjectsType objType = Objects.GetSelectedObj(1)->GetType();
            switch (_dOutDimLin._neededObjType)
            {
            case 1:
                switch (objType)
                {
                case DimObjects::DimObjectPointType:
                    type = 0;
                    _dOutDimLin._neededObjType = 4; // нужна последняя точка
                    break;
                case DimObjects::DimObjectSegmentType:
                    type = 3; // точка и отрезок
                    _dOutDimLin._neededObjType = 0;
                    break;
                };

                break;
            case 2:
                switch (objType)
                {
                case DimObjects::DimObjectSegmentType:
                    _dOutDimLin._neededObjType = 0;
                    type = 4; // отрезок и отрезок
                    break;
                case DimObjects::DimObjectPointType:
                    type = 2; // отрезок и точка
                    _dOutDimLin._neededObjType = 0;
                    break;
                };

                break;
            case 3:
                if (objType == DimObjects::DimObjectPlaneType && iViewDir)
                {
                    type = 5; // плоскость и плоскость
                    _dOutDimLin._neededObjType = 0;
                }
                break;
            };

            break;
        }
    case 3:
        {
            DimObjects::DimObjectsType objType = Objects.GetSelectedObj(2)->GetType();
            if (objType == DimObjects::DimObjectPointType)
            {
                type = 6; // три точки
                _dOutDimLin._neededObjType = 0;
            }
            break;
        }
    };

    if (type == 0)
        return true; // выбор объектов не закончен
    else if (type == -1)
        return false; // выбраны неподходящие объекты

    if (Objects.GetSelectedID() != 0)
    {
        Objects.GetSelectedObj()->SetSelected(false);
        Objects.SetSelectedID(0);
    }

    BPoint pts[3];
    switch (type)
    {
    case 1:
        pts[0] = static_cast<DimObjectArc*>(Objects.GetSelectedObj(0))->GetCenter();
        Objects.GetSelectedObj(0)->GetEndPoints(pts[1], pts[2]);
        break;
    case 2:
        Objects.GetSelectedObj(0)->GetEndPoints(pts[1], pts[2]);
        pts[0] = static_cast<DimObjectPoint*>(Objects.GetSelectedObj(1))->GetBPoint();
        break;
    case 3:
        Objects.GetSelectedObj(1)->GetEndPoints(pts[1], pts[2]);
        pts[0] = static_cast<DimObjectPoint*>(Objects.GetSelectedObj(0))->GetBPoint();
        break;
    case 4:
        {
            BPoint endPts[4];
            Objects.GetSelectedObj(0)->GetEndPoints(endPts[0], endPts[1]);
            Objects.GetSelectedObj(1)->GetEndPoints(endPts[2], endPts[3]);

            pts[0] = GrazingCurveElemContour::TraceOfLines(endPts[0], endPts[1], endPts[2], endPts[3]);

            BPoint proj = GrazingCurveElemContour::PointProjLine(endPts[0], endPts[1], pts[0]);
            //double magn2 = (proj - pts[0]).D2();
            //if (magn2 < 0.5 * MIND * MIND)
            //    return false;

            BPoint vect[2] = {endPts[0] - endPts[1], endPts[2] - endPts[3]};
            double magn = (sqrt(vect[0].D2()) + sqrt(vect[1].D2())) * 0.25;
            vect[0] = vect[0].Unit() * magn;
            vect[1] = vect[1].Unit() * magn;

            pts[1] = pts[0] + vect[0];
            pts[2] = pts[0] + vect[1];

            break;
        }
    case 5:
        {
            std::vector<BPoint> intersectPts;
            DimObject::IntersectObjects(Objects.GetSelectedObj(0), Objects.GetSelectedObj(1), intersectPts);
            if (intersectPts.size() != 2)
                return false;

            AvailableTrace result = GrazingCurveElemContour::LinePlane(iPoint, *iViewDir, static_cast<DimObjectPlane*>(Objects.GetSelectedObj(1))->GetOrigin(), static_cast<DimObjectPlane*>(Objects.GetSelectedObj(1))->GetNormal(), pts[1]);
            if (result != ONE_TRACE)
            {
                result = GrazingCurveElemContour::LinePlane(iPoint, *iViewDir, static_cast<DimObjectPlane*>(Objects.GetSelectedObj(0))->GetOrigin(), static_cast<DimObjectPlane*>(Objects.GetSelectedObj(0))->GetNormal(), pts[1]);
                if (result != ONE_TRACE)
                    return false;
                
				pts[2] = BPoint::ProjectPointOnPlane(pts[1] + static_cast<DimObjectPlane*>(Objects.GetSelectedObj(0))->GetNormal(), static_cast<DimObjectPlane*>(Objects.GetSelectedObj(1))->GetOrigin(), static_cast<DimObjectPlane*>(Objects.GetSelectedObj(1))->GetNormal());
            }
            else
				pts[2] = BPoint::ProjectPointOnPlane(pts[1] + static_cast<DimObjectPlane*>(Objects.GetSelectedObj(1))->GetNormal(), static_cast<DimObjectPlane*>(Objects.GetSelectedObj(0))->GetOrigin(), static_cast<DimObjectPlane*>(Objects.GetSelectedObj(0))->GetNormal());

            pts[0] = GrazingCurveElemContour::PointProjLine(intersectPts[0], intersectPts[0] + intersectPts[1], pts[1]);

            break;
        }
    case 6:
        {
            pts[0] = static_cast<DimObjectPoint*>(Objects.GetSelectedObj(0))->GetBPoint();
            pts[1] = static_cast<DimObjectPoint*>(Objects.GetSelectedObj(1))->GetBPoint();
            pts[2] = static_cast<DimObjectPoint*>(Objects.GetSelectedObj(2))->GetBPoint();
            bool onLine = DimObject::IsOnLine(pts[0], pts[1], pts[2]);
            if (onLine)
                return false;

            break;
        }
    };

    DimObject* obj;
    if (type == 1)
        if (Objects.GetSelectedObj(0)->GetType() == DimObjects::DimObjectCircleType)
            return false;
        else
            obj = new DimObjectDimensionAngle(static_cast<DimObjectArc*>(Objects.GetSelectedObj(0)), _dPrimDim._width);
    else
    {
        bool success = DimObjectDimensionAngle::CreateWithCenter(pts[0], pts[1], pts[2], obj, _dPrimDim._width);
        if (!success)
            return false;
    }

    oObjects.push_back(obj);

    return true;
}

bool DDimensions::PerformChanges(const BPoint& iPoint, const BPoint& iViewDir)
{
	// point/viewdir is in view CS now
	BMatr M(_snaps.GetMatrAxis().invr());
	BPoint P(iPoint * M);
	BPoint V(iViewDir * M);
	// P/V is in stock CS now
	DimObject *pDObj = Objects.GetChangingObj();
    if (pDObj)
	{
		if(pDObj->GetType() == DimObjects::DimObjectSegmentEdgeType)
		{
			DimObjectSegmentEdge * pDOSEObj = static_cast<DimObjectSegmentEdge *>(pDObj);
			DimObjectStock *pDStock = static_cast<DimObjectStock *>(Objects.Get(pDOSEObj->GetParentID()));
			if(!pDStock)
				return false;
			pDOSEObj->Change(P, V);
			pDStock->ChangeByEdge(P, V, pDOSEObj);
			unsigned int OldID = pDOSEObj->GetObjectID();
			Objects.ChangeObjectStock(pDStock);
			SetSelected(OldID);
			Objects.GetSelectedObj()->EnableChangeState(P, V);
			Objects.SetChangingID(Objects.GetSelectedID());
		}
		else
			pDObj->Change(P, V);
	}

    return true;
}

void DDimensions::SetAdditionObjectsToList(bool iAddition)
{
    if (iAddition)
    {
        SetMouseClickAction(EmptyClick);
        SetMouseMoveAction(MMA_EmptyMove);
    }
    _additionObjectsToList = iAddition;
}

void DDimensions::ClearAuxObjects()
{
    if (!_dPrimDim._createPt)
    {
		for (unsigned int i = 0; i < _auxObjects.size(); ++i)
		{
			auto pCur = Objects.Get(_auxObjects[i]);
			if(pCur != nullptr)
				Objects.Remove(pCur->GetObjectID());
		}
    }
        
    _auxObjects.clear(); // индексы вспомогательных объектов нужно очищать всегда, т.к. этот метод вызывается, когда они уже не нужны
}

void DDimensions::Clear()
{
	Active = false;
	ActiveForDraw = false;
	_dOutDimLin = DialogOutputDimLin();
	Objects.Clear();
}

bool DDimensions::CreateBoxStock(const BPoint& iPoint, const BPoint* iViewDir, const BPoint *normal, RPickID* iPickID, std::vector<DimObject*>& oObjects)
{
	if (Objects.GetSelectedID() != 0)
	{
		if(Objects.GetSelectedObj()->GetType() == DimObjects::DimObjectSegmentEdgeType)
		{// если указали на DimObjectSegmentEdge, и редактирование не включено, то начать, иначе прекратить
			if (Objects.GetChangingID() == 0)
			{
				Objects.GetSelectedObj()->EnableChangeState(iPoint, *iViewDir);
				Objects.SetChangingID(Objects.GetSelectedID());
				PushMouseMoveAction(MMA_Change);
			}
			else
			{
				Objects.GetChangingObj()->DisableChangeState();
				Objects.SetChangingID(0);
				PopMouseMoveAction();
			}
			return true;
		}
	}
    else if (Objects.GetChangingID() != 0)
    {
        Objects.GetChangingObj()->DisableChangeState();
        Objects.SetChangingID(0);
        PopMouseMoveAction();
		return true;
    }

	// Move Box
	CreatePtWithAuto(iPoint, normal, oObjects);
	if(oObjects.empty())
		return false;
	DimObjectStock *pDStock = GetDimStock();
	BPoint V(10., 10., 10., 0.);
	bool DepthTest = false;
	if(pDStock)
	{
		V = (pDStock->GetPointMax() - pDStock->GetPointMin()) * 0.5;
		DepthTest = pDStock->GetDepthTest();
	}
	DimObjectPoint *pPoint = dynamic_cast<DimObjectPoint *>(oObjects.back());
	if(!pPoint)
		return false;
	pDStock = new DimObjectStock(pPoint->GetBPoint() - V, pPoint->GetBPoint() + V, _dPrimPoint._width);
    // get Color from xml
    NColor Col = NCM_PROJECT.Defaults.GetColor(_T("Defaults/Dimensions/TempStock/Color"), NColor());
    const double Color[3] = { Col.R(), Col.G(), Col.B() };
    pDStock->SetColor(Color);
    pDStock->SetDepthTest(DepthTest);
	Objects.AddObjectStock(pDStock);
	for (auto it = oObjects.begin(); it != oObjects.end(); ++it)
		delete (*it);
	oObjects.clear();

    return true;
}

DimObjectStock * DDimensions::GetDimStock()
{
	return static_cast<DimObjectStock *>(Objects.GetTyped(DimObjects::DimObjectStockType));
}

void DDimensions::ActivateSnaps()
{
    _snaps.Activate(); 
}

void DDimensions::DeActivateSnaps()
{
    _snaps.DeActivate(); 
}

