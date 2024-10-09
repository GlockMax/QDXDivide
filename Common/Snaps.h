#pragma once

#include "BPoint.h"
#include "RPickID.h"
#include "OGLCursor.h"
#include "BMatr.h"
#include "RRopes.h"
#include "DimObjectSegment.h"
#include "DimObjectArc.h"
#include "RayCastManager.h"
#include <limits>
#include <array>
#include <vector>

class Snaps
{
public:
    Snaps(class CNCMDoc* iDoc): _doc(iDoc), _active(false), _snapType(EmptySnapType), _objectType(AllObjects), _eps(10.0)
        , _prevStockView(false), _prevViewDir(0., 0., 1., 0.), _baseObjType(EmptyBaseObjType) {}
    ~Snaps() {}

    void Activate() { _active = true; }
    void DeActivate() { _active = false; _cursor.Clear(); }
    bool IsActivated() const { return _active; }
    /// Тип объекта, на котором лежит точка привязки
    enum BaseObjectType
    {
        EmptyBaseObjType = 0,
        DimObjectType,
        SkeletonType
    };
    /// Тип привязки
    enum SnapType
    {
        EmptySnapType = 1, // привязываться ко всем точкам объекта
        End = 2, // привязываться к концам
        Center = 4, // привязываться к центру
        Mid = 8 // привязываться к среденй точке
    };
    void SetSnapType(int iSnapType) { _snapType = iSnapType; }
    int GetSnapType() const { return _snapType; }

    /// Тип объекта для привязки
    enum SnapObjectType
    {
        Empty = 0,
        Point = 1,
        Segment = 2,
        Arc = 4,
        Circle = 8,
        Skeleton = 16,
        Trajectory = 32,
        Model = 64,
        UCS = 128,
        AllObjects = 255,
        DimObjects = Point | Segment | Arc | Circle
    };
    /// Тип точки на объекте, к которой удалось привязаться
    enum SnapPointType
    {
        PointType = 0,
        EndPointType,
        MidPointType,
        CenterPointType,
        No
    };
private:
    class SnapPoint
    {
    public:
        SnapPoint() { _indexOfRope = UINT_MAX; _indexInRope = UINT_MAX; _snapType = No; _found = false;}
    public:
        BPoint _pt; // последняя точка привязки must be in stock (not stock moved) CS
        UINT _indexOfRope; // индекс segmPts
        UINT _indexInRope; // индекс точки в segmPts[_snapIndInd]
        bool _found;
        SnapPointType _snapType;
    };

    bool SnapInternal(const BPoint& iPoint, const BPoint& iViewDir, bool StockView, const RPickID& iPickID);

    bool FindNearestPtInEps(const BPoint& iPoint, double iEps, const std::vector<BPoint>& iSegmPts, SnapPoint& snapPoint);
    void FindLastSnap(const BPoint& point, const BPoint& viewDir, double eps);
    bool FindVertexes(const BPoint& iPt, const BPoint& iDir, const BPoint iSegm[2], double iEps, const std::vector<std::array<float*, 2>>& iSegments, BPoint oVert[2]);
    static bool UniteSegments(const BPoint& iA0, const BPoint& iA1, BPoint& ioB0, BPoint& ioB1);

    static bool CreateSegments(const BPoint& iPt, const std::vector<BPoint>& iDirs, std::vector<std::array<float*, 2>>& ioInitSegments, std::vector<BPoint>& oSegments);
    DimObject* MakeDimObject(const NCadrGeom* pGeom);

    class CNCMDoc* _doc;
    bool _active; // признак включённых привязок
    SnapPoint _lastSnap;

    int _snapType;
    int _objectType;
    BaseObjectType _baseObjType;

    double _eps; // размер области вокруг курсора для определения привязки

    OGLCursor _cursor;

    BMatr _matr; // StockMovedMatr for machine view and StockMovedMatr relative to StockMatr for stock view
    BMatr _matrAxis; // StockMatr for machine view and E matr for stock view
    SnapRopes segmPts;
    bool _restartFlag;
    DimObjectArc TempObjectArc;
    DimObjectSegment TempObjectSegment;
    BMatr _proj;// from stock CS to projected stock CS
    BMatr _projInv;// from projected stock CS to stock CS
    BPoint _prevViewDir;
    bool _prevStockView;
    RayCastManPack _rayCastManager;

public:
    void SetSnapObjectType(int iObjectType) { _objectType = iObjectType; }
    int GetSnapObjectType() const { return _objectType; }

    void SetBaseObjectType(BaseObjectType iType) { _baseObjType = iType; }
    int GetBaseObjectType() const { return _baseObjType; }

    /// Попытка привязаться
	bool Snap(const BPoint& iPoint, const BPoint& iViewDir, bool StockView, const RPickID& iPickID);

    /// Нахождение отрезков, инцидентных вершине каркаса (вершиной является точка привязки)
    bool FindIncidentSegments(const BPoint& iPoint, const BPoint& iViewDir, const RPickID& iPickID, unsigned int iMaxCount, std::vector<BPoint>& oSegments);

    /// Последняя точка привязки
    const BPoint& GetSnapPoint() const { return _lastSnap._pt; }

    void DrawLastPoint() const { if (_active) _cursor.Draw(); }

    /// Размер области вокруг курсора для определения привязки
    void SetEps(double iEps) { _eps = iEps; }
    double GetEps() const { return _eps; }
    double GetEpsScaled() const;

    void SetCursorType(SnapPointType iType);

    /// Параметры курсора
    void SetCursorSize(int iSize) { _cursor.SetSize(iSize); }
    void SetCursorWidth(float iWidth) { _cursor.SetWidth(iWidth); }
    void SetCursorColor(double iR, double iG, double iB) { _cursor.SetColor(iR, iG, iB); }
    int GetCursorSize() const { return _cursor.GetSize(); }
    float GetCursorWidth() const { return _cursor.GetWidth(); }
    void GetCursorColor(double& oR, double& oG, double& oB) const { return _cursor.GetColor(oR, oG, oB); }

    /// Матрица
    void SetMatr(const BMatr& iMatr, const BMatr& iMatrAxis) { _matr = iMatr; _matrAxis = iMatrAxis; }
    const BMatr& GetMatr(void) const { return _matr; }
    const BMatr& GetMatrAxis(void) const { return _matrAxis; }

    /// Метод поиска отрезков, выходящих из iPt и неколлинеарных iDir
    static bool FindIncidentSegments(const BPoint& iPt, const BPoint& iDir, unsigned int iMaxCount, std::vector<std::array<float*, 2>>& ioInitSegments, std::vector<BPoint>& oSegments);

    /// Check for model changes and prepare data
    void  PrepareSegments(const BPoint& viewDir, bool StockView);
    void SetFound(bool val) { _lastSnap._found = val; }
    bool GetFound() const { return _lastSnap._found; }
    void SetRestartFlag() { _restartFlag = true; }
    void EnsurePrepared(const BPoint& viewDir, bool StockView);
    BPoint GetNeighRopePoint(int shift, const BMatr& ProjInv) const;
    /// Ближайшая к направлению взгляда точка на объекте
    bool SnapToDimObjects(class DimObject* obj, const BPoint& iPoint, const BPoint& iViewDir, double iEps, Snaps::SnapPoint& Result);
    auto& GetRayCastManager() { return _rayCastManager; }
};