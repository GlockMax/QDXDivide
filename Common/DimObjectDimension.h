#pragma once
#include "DimObject.h"
#include <string>

#define TYPE "courier.ttf"

class AFX_EXT_CLASS DimObjectDimension: public DimObject
{
public:
	DECLARE_SERIAL(DimObjectDimension);
    DimObjectDimension(float iWidth = 2.0, const char* iPrefix = "", const char* iPostfix = "");
    virtual ~DimObjectDimension() { delete _dimLine; delete[] _prefix; delete[] _postfix;}

    /// �������� ���������� �������
    class DimData
    {
	public:
        DimData();

        BPoint _dirExtens[2]; // ����������� �������� �����
        double _magnExtens; // ����� �������� �����
        float _widthExtens; // ������� �������� �����

        enum CursorType
        {
            Empty = 0,
            Opened,
            Closed,
            ClosedFilled
        };

        CursorType _cursorType[2]; // ��� �������
        double _cursorLength; // ������ ������������
        double _cursorAngleSin, _cursorAngleCos; // ����� � ������� ����������� ���� ��� �������

        double _radius; // ������ ������� �� ������ �������
    };
    void SetCursorType(DimData::CursorType iTypeLeft, DimData::CursorType iTypeRight) { _data._cursorType[0] = iTypeLeft; _data._cursorType[1] = iTypeRight; }
    void SetCursorAngle(double iAngle) { _data._cursorAngleSin = sin(iAngle); _data._cursorAngleCos = cos(iAngle); }
    void SetCursorLength(double iLength) { _data._cursorLength = iLength; }
    void SetCursorHeight(double iHeight);
    void SetSpheresRadius(double iRadius) { _data._radius = iRadius; }

    bool FindNearestPoint(const BPoint& iPoint, const BPoint& iViewDir, BPoint& oPoint) const override { return _dimLine ? _dimLine->FindNearestPoint(iPoint, iViewDir, oPoint) : false; }

    BPoint GetMidPoint() const override { return _dimLine->GetMidPoint(); }
    void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const override { _dimLine->GetEndPoints(oPoint0, oPoint1); }

    DimObject* GetDimLine() const { return _dimLine; }

	virtual double GetDimValue() const {return 0.;}
	std::string GetDimValueText(void) const;

    BBox GetGabar() const override;

    DimObjects::DimObjectsType GetType() const override { return DimObjects::DimObjectDimensionType; }

    bool IsDimension() const override { return true; };

    void SetRectColor(double iColor[3]) { _bckgrndColor[0] = iColor[0], _bckgrndColor[1] = iColor[1], _bckgrndColor[2] = iColor[2]; }
    void SetTextColor(double iColor[3]) { _textColor[0] = iColor[0], _textColor[1] = iColor[1], _textColor[2] = iColor[2]; }

    void Serialize(CArchive& ar) override;
    /// ����� ����������� ���������� ��������
    bool IsEqualTo(class DimObject* iDimObject) const override;

protected:
    
    /// ����� ��������� �������
    /// iDimLine0 - ������ ��������� ����� � ���� ���� �����
    /// iDimLine1 - ������ ��������� ����� � ���� ���� �����
    /// iKoefMagn - �����������, �������������� ���������� ������ ������� �� ������ ��� ���������������
    void DrawCursors(const BPoint* iExtLine0, const BPoint* iExtLine1, double iKoefMagn, bool iOrthoToExtens) const;

    /// ����� ��������� ������ �������
    /// iString - �����
    /// iDimPts - ��� ����� - ����� ��������� �����
    /// iKoefMagn - �����������, �������������� ���������� ������ ������� �� ������ ��� ���������������
    /// iMidPoint - ����� ������������ �������
    //void DrawDimValue(char* iString, const BPoint* iDimPts, double iKoefMagn, BPoint* iMidPoint = nullptr) const;
    void DrawDimValue(const std::string& iString, const BPoint* iDimPts, const BPoint& iMidPoint, double iKoefMagn) const;

    /// ��������� ��������� ������ ��������������
    void FindRectCoords(BPoint oPts[4], BPoint& oTextOrigin) const;

    DimData _data; // ��������� �������

    GLdouble _textColor[3];
    GLdouble _bckgrndColor[3];
    GLdouble _borderColor[3];

    DimObject* _dimLine; // ��������� �����

    char* _prefix;
    char* _postfix;
};