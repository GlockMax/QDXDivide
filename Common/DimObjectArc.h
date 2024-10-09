#pragma once
#include "COMPARE_API.h"
#include "DimObject.h"
#include "NCadrGeom.h"

class AFX_EXT_CLASS DimObjectArc: public DimObject
{
public:
	DECLARE_SERIAL(DimObjectArc);
    /// ���� �� ��� ������
    static bool CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, DimObject*& oArc, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);

    /// ���� �������� �������� �� ��� ������
    static bool CreateOnThreePoints(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, double iAngle, DimObject*& oArc, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);
    
    /// ���� �� ���� ������ � ������
    static bool Create(const BPoint& iCenter, const BPoint& iPoint0, const BPoint& iPoint2, DimObject*& oArc, bool iIsLessThanPI = true, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);

    /// ��������� ����
    struct COMPARE_API ArcData
    {
        ArcData(): _isLine(false), _isCircle(false) {}

        /// ������ ���� �� ��� ������
        ArcData(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2);

        /// ������ ���� �� ��� ������ � ����
        ArcData(const BPoint& iPoint0, const BPoint& iPoint1, const BPoint& iPoint2, double iAngle);

        /// ������ ���� �� ���� ������ � ������
        ArcData(const BPoint& iCenter, const BPoint& iPoint0, const BPoint& iPoint2, bool iIsLessThanPI);

        ~ArcData() {}

        BPoint _center; // �����
        NCadrGeom::Curve _curve; // ����������� ����
        BPoint _normal; // ������� � ��������� ����
        BPoint _pts[2]; // ��������� ����� ����
        bool _isLine; // ������� ����, ��� ���� �������� ������
        bool _isCircle; // ������� ����, ��� ���� �������� �����������
    };
    
    DimObjectArc(const ArcData& iData, double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);
	DimObjectArc(double iCenterRadius = 2.0, float iCenterWidth = 2.0f, float iWidth = 2.0f);
    
    virtual ~DimObjectArc() {}
    
    virtual BPoint GetMidPoint() const;
    virtual void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const { oPoint0 = GetStartPoint(), oPoint1 = GetEndPoint(); }

    /// ����� ���������� �����
    BPoint GetCenter() const { return _arc.GetC(); }

    /// ����� ���������� �������
    BPoint GetNormal() const { return _arc.GetN(); }

    /// ����� ���������� ������ �� ������ � �����
    BPoint GetI() const { return _arc.GetI(); }

    /// ������
    double GetRadius() const { return _arc.GetR(); }

    /// ����� ���������� ������ ����
    BPoint GetStartPoint() const { return _arc.GetB(); }

    /// ����� ������������� ������ ����
    void SetStartPoint(const BPoint& iPoint) { _arc.SetB(iPoint); }
    
    /// ����� ���������� ����� ����
    BPoint GetEndPoint() const { return _arc.GetE(); }

    /// ����� ������������� ����� ����
    void SetEndPoint(const BPoint& iPoint) { _arc.SetE(iPoint); }

    /// ����� ���������� ���� ���� � ��������
    double GetAngle() const;

    /// ����� ������ ���� ���������
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

    /// �������� �������������� ����� ����������
    bool IsOnCircle(const BPoint& iPoint) const;

    /// �������� �������������� �����, ������� �� ����������, ����
    bool IsOnArc(const BPoint& iPoint) const;

    /// ����� ��� �������� ���������� ��������
    virtual bool IsEqualToInternal(class DimObjectArc* iArc) const;
    
    NCadrGeom _arc; // ����
    double _centerRadius;
    float _centerWidth;
};