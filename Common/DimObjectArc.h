#pragma once
#include "COMPARE_API.h"
#include "DimObject.h"
#include "NCadrGeom.h"

class AFX_EXT_CLASS DimObjectArc: public DimObject
{
public:
	DECLARE_SERIAL(DimObjectArc);
    /// Дуга по трём точкам
    static bool CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oArc, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);

    /// Дуга заданной величины по трём точкам
    static bool CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, double iAngle, DimObject*& oArc, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);
    
    /// Дуга по двум точкам и центру
    static bool Create(const BPoint& iCenter, const BPoint& iPoint0, const BPoint& iPoint2, DimObject*& oArc, bool iIsLessThanPI = true, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);

    /// Параметры дуги
    struct COMPARE_API ArcData
    {
        ArcData(): _isLine(false), _isCircle(false) {}

        /// Данные дуги по трём точкам
        ArcData(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2);

        /// Данные дуги по трём точкам и углу
        ArcData(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, double iAngle);

        /// Данные дуги по двум точкам и центру
        ArcData(const BPoint& iCenter, const BPoint& iPoint0, const BPoint& iPoint2, bool iIsLessThanPI);

        ~ArcData() {}

        BPoint _center; // центр
        NCadrGeom::Curve _curve; // направление дуги
        BPoint _normal; // нормаль к плоскости дуги
        BPoint _pts[2]; // граничные точки дуги
        bool _isLine; // признак того, что дуга является прямой
        bool _isCircle; // признак того, что дуга является окружностью
    };
    
    DimObjectArc(const ArcData& iData, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);
	DimObjectArc(double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);
    
    virtual ~DimObjectArc() {}
    
    virtual BPoint GetMidPoint() const;
    virtual void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const { oPoint0 = GetStartPoint(), oPoint1 = GetEndPoint(); }

    /// Метод возвращает центр
    BPoint GetCenter() const { return _arc.GetC(); }

    /// Метод возвращает нормаль
    BPoint GetNormal() const { return _arc.GetN(); }

    /// Метод возвращает вектор из начала в центр
    BPoint GetI() const { return _arc.GetI(); }

    /// Радиус
    double GetRadius() const { return _arc.GetR(); }

    /// Метод возвращает начало дуги
    BPoint GetStartPoint() const { return _arc.GetB(); }

    /// Метод устанавливает начало дуги
    void SetStartPoint(const BPoint& iPoint) { _arc.SetB(iPoint); }
    
    /// Метод возвращает конец дуги
    BPoint GetEndPoint() const { return _arc.GetE(); }

    /// Метод устанавливает конец дуги
    void SetEndPoint(const BPoint& iPoint) { _arc.SetE(iPoint); }

    /// Метод возвращает угол дуги в радианах
    double GetAngle() const;

    /// Метод делает дугу замкнутой
    void MakeClosed() { _arc.SetE(_arc.GetB()); }

    virtual void DrawSpecific() const;

    virtual BPoint FindNearestPoint(const BPoint& iPoint) const;
    virtual bool FindNearestPoint(const BPoint& iPoint, const BPoint& iViewDir, BPoint& oPoint) const;
    
    virtual bool Contains(const BPoint& iPoint) const;

    virtual BBox GetGabar() const;

    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectArcType; }

    virtual bool IsLine() { return _arc.IsLine(); }

    virtual bool IsEqualTo(class DimObject* iDimObject) const;

    bool IsLessThanPI() const;
    
    virtual void Serialize(CArchive& ar);

	void SetArc(const NCadrGeom &iArc) { _arc = iArc; _arc.ApplyMatr();}

protected:

    /// Проверка принадлежности точки окружности
    bool IsOnCircle(const BPoint& iPoint) const;

    /// Проверка принадлежности точки, лежащей на окружности, дуге
    bool IsOnArc(const BPoint& iPoint) const;

    /// Метод для проверки совпадения объектов
    virtual bool IsEqualToInternal(class DimObjectArc* iArc) const;
    
    NCadrGeom _arc; // дуга
    double _centerRadius;
    float _centerWidth;
};