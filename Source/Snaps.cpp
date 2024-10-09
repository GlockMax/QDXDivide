#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObject.h"
#include "NCMDoc.h"
#include "BaseRender.h"
#include "DimObject.h"
#include "OctreeSkelTool.h"
#include "GrazingCurveElemContour.h"
#include "Snaps.h"

bool Snaps::Snap(const BPoint& iPoint, const BPoint& iViewDir, bool StockView, const RPickID& iPickID)
{
	// point/viewdir is in machine CS now
	if (!_active)
        return false;

    SnapInternal(iPoint, iViewDir, StockView, iPickID);
    if (GetFound())
    {
        const auto& point = GetSnapPoint();
        _cursor.Set(point.GetX(), point.GetY(), point.GetZ());
    }

    return GetFound();
}

bool Snaps::SnapInternal(const BPoint& iPoint, const BPoint& iViewDir, bool StockView, const RPickID& iPickID)
{
	// point/viewdir is in machine CS now
    double eps = GetEpsScaled();

    SetBaseObjectType(Snaps::EmptyBaseObjType);
    
    // учесть матрицу из окна
    BMatr matrInv = _matrAxis.invr();
    BPoint point = iPoint * matrInv,
           viewDir = iViewDir * matrInv;
	// point/viewdir is in stock CS now
    _lastSnap._found = false;
    if (iPickID.GetGroup() == OG_UCS)
    {
        if (!(GetSnapObjectType() & Snaps::UCS))
            return false;
        if (_doc->GetCurChannel()->GetAxesBase(iPickID.GetObjName(), _lastSnap._pt))
        {
            _lastSnap._found = true;
            _lastSnap._snapType = EndPointType;
        }
    }
    else if (iPickID.GetGroup() == OG_DIMENS)
    {
        if (!(GetSnapObjectType() & Snaps::DimObjects))
            return false;
		DimObject* obj = _doc->Dimensions.GetObjects().GetHighlightedObj();
        _lastSnap._found = SnapToDimObjects(obj, point, viewDir, eps, _lastSnap);
        if (_lastSnap._found)
        {
            SetBaseObjectType(Snaps::DimObjectType);
        }
    }
	else if (iPickID.GetGroup() == OG_PROGRAMS)
	{
		if (!(GetSnapObjectType() & Snaps::Trajectory))
			return false;
		CString key;
		NProgram *pProg = NULL;
		_doc->NGetProgramList()->GetAt(iPickID.GetObjName(), key, (CObject*&)pProg);
		const BGeomArray *pGeomArray = &pProg->GetGeomArray();
		cadrID Cadr;
		Cadr.Prog = iPickID.GetObjName();
		Cadr.Cadr = iPickID.GetObjSubName1() > 0 ? iPickID.GetObjSubName1() : iPickID.GetObjSubName0();
		NCadrGeom Geom(*pGeomArray->GetAt(Cadr.Cadr));// Founded geom
//        Geom.Tr(_matrAxis * _matr.invr());
        //Geom is in moved stock CS now
		DimObject* obj = MakeDimObject(&Geom);
		int SnapObjectType = GetSnapObjectType();
		SetSnapObjectType(SnapObjectType | Segment | Circle | Arc);
        _lastSnap._found = SnapToDimObjects(obj, point, viewDir, eps, _lastSnap);
        SetSnapObjectType(SnapObjectType);
		if (_lastSnap._found)
		{
			SetBaseObjectType(Snaps::DimObjectType);
		}
	}
    else if (iPickID.GetGroup() == OG_STOCKS)
    {
        if (!(GetSnapObjectType() & Snaps::Skeleton))
            return false;
        const BGeomArray* pGeomArray = _doc->GetStock(iPickID.GetObjName())->GetSect(iPickID.GetObjSubName0());
        if (pGeomArray)
        {
            cadrID Cadr;
            Cadr.Prog = iPickID.GetObjName();
            Cadr.Cadr = iPickID.GetObjSubName1();
            NCadrGeom Geom(*pGeomArray->GetAt(Cadr.Cadr));// Founded geom
            Geom.Tr(_matr * _matrAxis.invr());
            DimObject* obj = MakeDimObject(&Geom);
            int SnapObjectType = GetSnapObjectType();
            SetSnapObjectType(SnapObjectType | Segment | Circle | Arc);
            _lastSnap._found = SnapToDimObjects(obj, point, viewDir, eps, _lastSnap);
            SetSnapObjectType(SnapObjectType);
            if (_lastSnap._found)
            {
                SetBaseObjectType(Snaps::DimObjectType);
            }
        }
    }
    else if (iPickID.GetGroup() == OG_TOOLS)
    {
        if (!(GetSnapObjectType() & Snaps::Skeleton))
            return false;
        _lastSnap._found = false;
        const NCadrGeom* pGeom = _doc->GetCadrGeomIDost(IDost(iPickID.GetObjName(), iPickID.GetObjSubName0(), iPickID.GetObjSubName1() == 1));// Founded geom
        if (pGeom != nullptr)// pGeom is in stock CS
        {
            DimObject* obj = MakeDimObject(pGeom);
            int SnapObjectType = GetSnapObjectType();
            SetSnapObjectType(SnapObjectType | Segment | Circle | Arc);
            _lastSnap._found = SnapToDimObjects(obj, point, viewDir, eps, _lastSnap);
            SetSnapObjectType(SnapObjectType);
        }
        if (_lastSnap._found)
        {
            SetBaseObjectType(Snaps::DimObjectType);
        }
    }
    else
    {
        if (!(GetSnapObjectType() & Snaps::Skeleton))
			return false;

        EnsurePrepared(viewDir, StockView);
        FindLastSnap(point, viewDir, eps);

        // если точка привязки нашлась
        if (_lastSnap._found)
        {
            SetBaseObjectType(Snaps::SkeletonType);
        }
    }
    if (_lastSnap._found)
        SetCursorType(_lastSnap._snapType);

   return _lastSnap._found;
}

void Snaps::PrepareSegments(const BPoint& viewDir, bool StockView)
{
    // достать отрезки каркаса
    segmPts.clear();
    std::vector<BMatr> matrsInvr;
    _doc->FindSegmRopes(GetSnapObjectType(), StockView, segmPts, matrsInvr);
    // matrsInvr transforms from stock CS to part CS now
    //viewDir is in stock CS
    _proj.e0cong(viewDir, BPoint(0., 0., 1., 0.));// from stock CS to projected stock CS
    _projInv.e0cong(BPoint(0., 0., 1., 0.), viewDir);
    for (unsigned int k = 0; k < segmPts.size(); ++k)
    {
        BMatr Tr = matrsInvr[k].invr() * _proj;// transforms from part CS to projected stock CS
        for (unsigned int i = 0; i < segmPts[k].size(); ++i)
            segmPts[k][i] *= Tr; 
    }
}

void Snaps::EnsurePrepared(const BPoint& viewDir, bool StockView)
{
    if (_prevViewDir.Angle0_180(viewDir) > MINAD || _prevStockView != StockView)
    {
        _prevViewDir = viewDir;
        _prevStockView = StockView;
        _restartFlag = true;
    }
    if (_restartFlag)
    {
        _restartFlag = false;
        PrepareSegments(viewDir, StockView);
        GetRayCastManager().InitializeTools(_doc, StockView);
    }
}

BPoint Snaps::GetNeighRopePoint(int shift, const BMatr& ProjInv) const
{
    const UINT indexOf = _lastSnap._indexOfRope;
    UINT indexIn = _lastSnap._indexInRope;
    if ((int(indexIn) < -shift) || !GetFound())
        return BPoint(0., 0., 0., -1.);
    indexIn += shift;
    if (indexOf < segmPts.size())
    {
        const auto& rope = segmPts.at(indexOf);
        if (indexIn < rope.size())
            return rope.at(indexIn) * _projInv;
    }
    return BPoint(0., 0., 0., -1.);
}

bool Snaps::FindNearestPtInEps(const BPoint& BaseP, double iEps, const std::vector<BPoint>& iSegmPts, SnapPoint& snapPoint)
{
    if (iSegmPts.size() < 2)
        return false;

    const double eps2 = iEps * iEps;
    //BaseP is in projected stock CS here
    bool needEnds = GetSnapType() & Snaps::End;
    bool needMids = GetSnapType() & Snaps::Mid;
    bool needPoints = GetSnapType() & Snaps::Point;

    bool found = false;
    for (unsigned int i = 0; i < iSegmPts.size() - 1; ++i)
    {
        const BPoint& P0 = iSegmPts[i];
        const BPoint& P1 = iSegmPts[i + 1];
        bool F0Left = P0.GetX() < BaseP.GetX() - iEps;
        bool F1Left = P1.GetX() < BaseP.GetX() - iEps;
        if (F0Left && F1Left)
            continue;

        bool F0Right = P0.GetX() > BaseP.GetX() + iEps;
        bool F1Right = P1.GetX() > BaseP.GetX() + iEps;
        if (F0Right && F1Right)
            continue;

        bool F0Bot = P0.GetY() < BaseP.GetY() - iEps;
        bool F1Bot = P1.GetY() < BaseP.GetY() - iEps;
        if (F0Bot && F1Bot)
            continue;

        bool F0Top = P0.GetY() > BaseP.GetY() + iEps;
        bool F1Top = P1.GetY() > BaseP.GetY() + iEps;
        if (F0Top && F1Top)
            continue;
        SnapPoint newSnapPoint;
        bool newPointFound = false;
        if (i == 0 && needEnds) // very first point
        {
            if (!F0Left && !F0Right && !F0Top && !F0Bot)
            {
                newSnapPoint._pt = P0;
                newSnapPoint._indexInRope = i;
                newSnapPoint._snapType = EndPointType;
                newPointFound = true;
            }
        }
        if (!newPointFound && !F1Left && !F1Right && !F1Top && !F1Bot && needEnds)
        {
            newSnapPoint._pt = P1;
            newSnapPoint._indexInRope = i + 1;
            newSnapPoint._snapType = EndPointType;
            newPointFound = true;
        }
        if (!newPointFound && needMids)
        {
            BPoint MidP = (P0 + P1) * 0.5;
            if ((BaseP - MidP).ProjXY().D2() < eps2)
            {
                newSnapPoint._pt = MidP;
                newSnapPoint._indexInRope = i;
                newSnapPoint._snapType = MidPointType;
                newPointFound = true;
            }
        }
        if (!newPointFound && needPoints)
        {
            BPoint pt = GrazingCurveElemContour::TraceOfLines(P0, P1, BaseP, BaseP + BPoint(0., 0., 1., 0.));
            BPoint proj[2] = { GrazingCurveElemContour::PointProjLine(P0, P1, pt),
                              GrazingCurveElemContour::PointProjLine(BaseP, BaseP + BPoint(0., 0., 1., 0.), pt) };
            BPoint vect[2] = { (proj[0] - P0), (proj[0] - P1) };
            double cos = vect[0] * vect[1];
            bool contain = !(cos > 0.0);
            if (contain)
            {
                double magn2 = (proj[0] - proj[1]).D2();
                if (magn2 < eps2)
                {
                    newSnapPoint._pt = proj[0];
                    newSnapPoint._indexInRope = i;
                    newSnapPoint._snapType = PointType;
                    newPointFound = true;
                }
            }
        }
        if (newPointFound)
        {
            bool store = true;
            if (found)
            {
                BPoint vect = newSnapPoint._pt - snapPoint._pt;
                store = (vect * BPoint(0., 0., 1., 0.)) < 0.0;
            }
            if (store)
            {
                snapPoint = newSnapPoint;
                found = true;
            }
        }
    }
    return found;
}

void Snaps::FindLastSnap(const BPoint& point, const BPoint& viewDir, double eps)
{
    // point and viewDir are in stock CS here
    // привязаться, учитывая матрицы из документа
    for (unsigned int i = 0; i < segmPts.size(); ++i)
    {
        SnapPoint snapPoint;
        if (FindNearestPtInEps(point * _proj, eps, segmPts[i], snapPoint))
        {
            // check for distance to the viewer
            snapPoint._pt *= _projInv;// in stock CS now
            bool store = true;
            if (_lastSnap._found)
            {
                BPoint vect = snapPoint._pt - _lastSnap._pt;// _pt in stock CS now
                store = vect * viewDir < 0.0;
            }
            if (store)
            {
                _lastSnap = snapPoint;
                _lastSnap._indexOfRope = i;
                _lastSnap._found = true;
            }
        }
    }
}

bool Snaps::FindVertexes(const BPoint& iPt, const BPoint& iDir, const BPoint iSegm[2], double iEps, const std::vector<std::array<float*, 2>>& iSegments, BPoint oVert[2])
{
    // Поиск вершин отрезка каркаса

    double magns2[2] = {(iSegm[0] - iPt).D2(), (iSegm[1] - iPt).D2()};
    const double eps2 = iEps * iEps;
    /*if (magns2[0] > eps2 && magns2[1] > eps2)
        return false;*/
    
    //BPoint segm[2] = {iSegm[0], iSegm[1]};
    oVert[0] = iSegm[0];
    oVert[1] = iSegm[1];

    for (auto it = iSegments.begin(); it != iSegments.end(); ++it)
    {
        BPoint tmpSegm[2] = {BPoint((*it)[0][0], (*it)[0][1], (*it)[0][2], 1.0),
                             BPoint((*it)[1][0], (*it)[1][1], (*it)[1][2], 1.0)};
        bool segmChanged = UniteSegments(tmpSegm[0], tmpSegm[1], oVert[0], oVert[1]);

        /*if (segmChanged)
        {
            magns2[0] = (oVert[0] - iPt).D2();
            magns2[1] = (oVert[1] - iPt).D2();
            if (magns2[0] > eps2 && magns2[1] > eps2)
                return false;
        }*/
    }

    //oVert = magns2[0] < magns2[1] ? segm[0] : segm[1];

    return true;
}

bool Snaps::UniteSegments(const BPoint& iA0, const BPoint& iA1, BPoint& ioB0, BPoint& ioB1)
{// попытка объединить [A0, A1] и [B0, B1] в [B0, B1]
 // известно, что ни один из них не содержится в другом
 // и их длины не нулевые
    BPoint dirs[2] = {(iA1 - iA0).Unit(), (ioB1 - ioB0).Unit()};
    double cos = dirs[0] * dirs[1];
    if (fabs(1.0 - fabs(cos)) > MIND)
        return false;

    bool success = false;
    if (cos > 0.0)
    {
        double magn2 = (ioB1 - iA0).D2();
        success = magn2 < MIND;
        if (success)
            ioB1 = iA1;
        else
        {
            magn2 = (ioB0 - iA1).D2();
            success = magn2 < MIND;
            if (success)
                ioB0 = iA0;
        }
    }
    else
    {
        double magn2 = (ioB1 - iA1).D2();
        success = magn2 < MIND;
        if (success)
            ioB1 = iA0;
        else
        {
            magn2 = (ioB0 - iA0).D2();
            success = magn2 < MIND;
            if (success)
                ioB0 = iA1;
        }
    }

    return success;
}

bool Snaps::FindIncidentSegments(const BPoint& iPoint, const BPoint& iViewDir, const RPickID& iPickID, unsigned int iMaxCount, std::vector<BPoint>& oSegments)
{
    return false;
}

bool Snaps::FindIncidentSegments(const BPoint& iPt, const BPoint& iDir, unsigned int iMaxCount, std::vector<std::array<float*, 2>>& ioInitSegments, std::vector<BPoint>& oSegments)
{
    // поиск отрезков, исходящих из точки iPt
    // iDir - направление одного из отрезков
    // элемент массива ioInitSegments - два указателя на два массива float[3] - массива координат точек
    
    unsigned int count = 0;
    const double mind2 = MIND * MIND;
    std::vector<BPoint> dirs;
    dirs.push_back(iDir);
    for (auto it = ioInitSegments.begin(); it != ioInitSegments.end(); ++it)
    {
        BPoint segm[2] = {BPoint((*it)[0][0], (*it)[0][1], (*it)[0][2], 1.0),
                          BPoint((*it)[1][0], (*it)[1][1], (*it)[1][2], 1.0)};
        double magn2[2] = {(iPt - segm[0]).D2(), (iPt - segm[1]).D2()};
        int sign = magn2[0] < mind2 ? 1 : (magn2[1] < mind2 ? -1 : 0);
        if (sign != 0)
        {
            BPoint vect = (segm[1] - segm[0]);
            double cos = iDir * vect;
            if ((vect.D2() - cos * cos) > mind2)
            {
                dirs.push_back(vect * (double)sign);
                ++count;
                if (count == iMaxCount)
                    break;
            }
        }
    }

    return CreateSegments(iPt, dirs, ioInitSegments, oSegments);
}

bool Snaps::CreateSegments(const BPoint& iPt, const std::vector<BPoint>& iDirs, std::vector<std::array<float*, 2>>& ioInitSegments, std::vector<BPoint>& oSegments)
{
    unsigned int count = (unsigned int)iDirs.size();
    if (count == 0)
        return false;

    const double mind2 = MIND * MIND;
    oSegments.resize(count, iPt);
    bool success = false;
    for (unsigned int i = 0; i < count; ++i)
    {
        bool Cont = true;
        for (unsigned int j = 0; j < (unsigned int)ioInitSegments.size() && Cont; ++j, success |= Cont)
        {
            Cont = false;
            auto it = ioInitSegments.begin();
            for (++it; it != ioInitSegments.end(); ++it)
            {
                BPoint segm[2] = {BPoint((*it)[0][0], (*it)[0][1], (*it)[0][2], 1.0),
                                  BPoint((*it)[1][0], (*it)[1][1], (*it)[1][2], 1.0)};
                BPoint vect = (segm[1] - segm[0]);
                double cos = iDirs[i] * vect;
                if ((iDirs[i].D2() * vect.D2() - cos * cos) < mind2)
                {
                    double magn2[2] = {(oSegments[i] - segm[0]).D2(), (oSegments[i] - segm[1]).D2()};
                    if (magn2[0] < mind2)
                    {
                        oSegments[i] = segm[1];
                        ioInitSegments.erase(it--);
                        Cont = true;
                    }
                    else if (magn2[1] < mind2)
                    {
                        oSegments[i] = segm[0];
                        ioInitSegments.erase(it--);
                        Cont = true;
                    }
                }
            }

            it = ioInitSegments.begin();
            BPoint segm[2] = {BPoint((*it)[0][0], (*it)[0][1], (*it)[0][2], 1.0),
                              BPoint((*it)[1][0], (*it)[1][1], (*it)[1][2], 1.0)};
            BPoint vect = (segm[1] - segm[0]);
            double cos = iDirs[i] * vect;
            if ((iDirs[i].D2() * vect.D2() - cos * cos) < mind2)
            {
                double magn2[2] = {(oSegments[i] - segm[0]).D2(), (oSegments[i] - segm[1]).D2()};
                if (magn2[0] < mind2)
                {
                    oSegments[i] = segm[1];
                    ioInitSegments.erase(it);
                    Cont = true;
                }
                else if (magn2[1] < mind2)
                {
                    oSegments[i] = segm[0];
                    ioInitSegments.erase(it);
                    Cont = true;
                }
            }
        }
    }

    return success;
}

double Snaps::GetEpsScaled() const
{
    double x, y, z;
    BaseRender::GetPixelDim(x, y, z);
    return GetEps() * x;
}

void Snaps::SetCursorType(SnapPointType iType)
{
    switch (iType)
    {
    case PointType:
        _cursor.SetType(OGLCursor::QuadType);
        break;
    case EndPointType:
        _cursor.SetType(OGLCursor::QuadDiagonalType);
        break;
    case MidPointType:
        _cursor.SetType(OGLCursor::ClockType);
        break;
    case CenterPointType:
        _cursor.SetType(OGLCursor::CircleType);
        break;
    };
}

bool Snaps::SnapToDimObjects(DimObject* obj, const BPoint& iPoint, const BPoint& iViewDir, double iEps, Snaps::SnapPoint& Result)
{
    // выбрать объекты нужных типов
    if (!obj)
        return false;

    bool suitableObj = false;
    int objectType = GetSnapObjectType() == Snaps::AllObjects ? Snaps::Segment | Snaps::Arc | Snaps::Circle : GetSnapObjectType();
    Snaps::SnapObjectType snapObj[3] = { Snaps::Segment, Snaps::Circle, Snaps::Arc };
    DimObjects::DimObjectsType dimObj[3] = { DimObjects::DimObjectSegmentType,
                                            DimObjects::DimObjectCircleType, DimObjects::DimObjectArcType };
    for (unsigned int i = 0; i < 3; ++i)
    {
        if (objectType & snapObj[i])
            suitableObj = obj->GetType() == dimObj[i];

        if (suitableObj)
            break;
    }

    if (!suitableObj)
        return false;

    // найти ближайшую точку
    bool success = obj->FindNearestPoint(iPoint, iViewDir, Result._pt);
    if (!success)
        return false;

    Result._snapType = Snaps::PointType;

    // обработать типы привязок
    BPoint tmpPt(0.0, 0.0, 0.0, -1.0);
    if (GetSnapType() & Snaps::End)
    {
        BPoint endPts[2];
        obj->GetEndPoints(endPts[0], endPts[1]);

        if ((Result._pt - endPts[0]).D2() < iEps * iEps)
        {
            tmpPt = endPts[0];
            Result._snapType = Snaps::EndPointType;
        }
        else if ((Result._pt - endPts[1]).D2() < iEps * iEps)
        {
            tmpPt = endPts[1];
            Result._snapType = Snaps::EndPointType;
        }
    }

    if (GetSnapType() & Snaps::Mid)
    {
        BPoint midPt = obj->GetMidPoint();
        if (tmpPt.GetH() < 0.0)
        {
            if ((Result._pt - midPt).D2() < iEps * iEps)
            {
                tmpPt = midPt;
                Result._snapType = Snaps::MidPointType;
            }
        }
        else
        {
            double magn2[2] = { (Result._pt - midPt).D2(), (Result._pt - tmpPt).D2() };
            if (magn2[0] < magn2[1])
                tmpPt = midPt;
        }
    }

    if (GetSnapType() & Snaps::Center)
        if ((obj->GetType() == DimObjects::DimObjectArcType || obj->GetType() == DimObjects::DimObjectCircleType) && tmpPt.GetH() < 0.0)
        {
            tmpPt = static_cast<DimObjectArc*>(obj)->GetCenter();
            Result._snapType = Snaps::CenterPointType;
        }

    if (tmpPt.GetH() > 0.0)
        Result._pt = tmpPt;
    else if (!(GetSnapType() & Snaps::EmptySnapType))
        success = false; // разрешено привязываться только к характерным точкам, а сделать этого не удалось

    return success;
}

DimObject* Snaps::MakeDimObject(const NCadrGeom* pGeom)
{
    if (pGeom == nullptr)
        return nullptr;
    if (pGeom->IsArc())
    {
        TempObjectArc.SetArc(*pGeom);
        return &TempObjectArc;
    }
    if (pGeom->IsLine())
    {
        NCadrGeom Geom(*pGeom);
        Geom.ApplyMatr();
        TempObjectSegment.SetStartPoint(Geom.GetBP());
        TempObjectSegment.SetEndPoint(Geom.GetEP());
        return &TempObjectSegment;
    }


    TempObjectSegment.SetStartPoint(pGeom->GetBP());
    TempObjectSegment.SetEndPoint(pGeom->GetEP());
    return &TempObjectSegment;
}
