#pragma once
#include "StdAfx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "glText.h"
#include "DimObjects.h"
#include "BPoint.h"
//#include "ConstDef.h"
#include "GrazingCurveElemContour.h"
#include <vector>
#include "BBox.h"



class AFX_EXT_CLASS DimObject : public CObject
{
	friend class DDimensions;
public:
    struct Color
    {
        Color();

        double _common[3]; // цвет в обычном состоянии
        double _whenAimed[3]; // цвет, когда курсор наведён на объект
        double _whenActive[3]; // цвет в активном состоянии
        double _whenActiveNAimed[3]; // цвет в активном состоянии, когда курсор наведён на объект
    };
    DimObject(float iWidth): ObjectID(0), Highlighted(false), Selected(false), _changeState(false), _hidden(false), _widthMain(iWidth), _depthTest(false) {}
    virtual ~DimObject(void) {}
	DimObject(const DimObject &iObject);
	DimObject(): ObjectID(0), Highlighted(false), Selected(false), _changeState(false), _hidden(false), _widthMain(5), _depthTest(false) {}

    void Draw() const;
	virtual void DrawSpecific() const {};
	
    void SetHighlighted(bool In) { Highlighted = In;}
	bool GetHighlighted(void) const { return Highlighted;}
	
    void SetSelected(bool In) { Selected = In;}
	bool GetSelected(void) const { return Selected;}
    
    void SetObjectID(unsigned int iID) { ObjectID = iID; }
	int GetObjectID(void) const { return ObjectID; }

    /// Цвет в обычном состоянии
    void SetColor(const double iColor[3]) { _color._common[0] = iColor[0], _color._common[1] = iColor[1], _color._common[2] = iColor[2]; }

    /// Ширина
    void SetWidth(float iWidth) { _widthMain = iWidth; }
    
    /// Метод устанавливает признак того, что объект не рисуется
    void Hide(bool iHide) { _hidden = iHide; }
    bool IsVisible() { return !_hidden; }

    /// Метод указывает объекту, что его сейчас будут редактировать
    void EnableChangeState(const BPoint& iPoint, const BPoint& iViewDir); // { _changeState = true; _oldPt = iPoint; }

    /// Метод прекращает редактирование
    void DisableChangeState() { _changeState = false; }

    /// Метод возвращает признак редактирования объекта
    bool GetChangeState() { return _changeState; }

    /// Метод редактирует объект
    virtual void Change(const BPoint& iPoint, const BPoint& iViewDir) { _oldPt = iPoint; }
    
    /// Метод находит ближайшую точку на объекте
    virtual BPoint FindNearestPoint(const BPoint& iPoint) const { return BPoint(0.0, 0.0, 0.0, 0.0); }
	virtual bool FindNearestPoint(const BPoint& iPoint, const BPoint& iViewDir, BPoint& oPoint) const {return false;};

    /// Метод определения совпадения объектов
    virtual bool IsEqualTo(class DimObject* iDimObject) const { return false; }
    
    /// Метод определения принадлежности точки объекту
    virtual bool Contains(const BPoint& iPoint) const { return false; }

    /// Метод пересечения объектов
    static void IntersectObjects(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);

    /// Метод возвращает особые точки объекта
	virtual BPoint GetMidPoint() const {return BPoint(0., 0., 0., -1.);};
	virtual void GetEndPoints(BPoint& oPoint0, BPoint& oPoint1) const {};

    /// Габариты
	virtual BBox GetGabar() const {return BBox();};

    /// Тип объекта
    virtual DimObjects::DimObjectsType GetType() const { return DimObjects::DimObjectType; }
    
    /// Направление оси
    /// oX - 0, oY - 1, oZ - 2
    static BPoint GetAxisDir(int iAxisIndex);

    /// Проверка нахождения точек на одной прямой
    static bool IsOnLine(const BPoint& iP0, const BPoint& iP1, const BPoint& iP2);

    /// Признак того, что объект является размером
    virtual bool IsDimension() const { return false; };

    /// Ширина/толщина объекта
    double GetWidth() { return _widthMain; }

	virtual void Serialize(CArchive& ar);

	virtual bool IsDummy(void) const { return false;}

	static void SetTextHeight(int iHeight) { DimText.height = iHeight; DimText.width = iHeight / 2; DimText.Init(wglGetCurrentDC(), USEFONTBITMAPS, ""); }
	const Color &GetColor(void) const { return _color;}
	const BPoint &Get_oldPt(void) const { return _oldPt;}
	void SetDepthTest(bool iV) { _depthTest = iV;}
	bool GetDepthTest(void) const { return _depthTest;}
	static void SetDigits(int iDigits) { Digits = iDigits; }

	DECLARE_SERIAL(DimObject);
protected:
    static BPoint GetViewDir();
    static BPoint GetXDir();

    static void DrawGLPrimitive(const BPoint* iPoints, unsigned int iCount);
    
    static void IntersectSegments(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);
    static void IntersectSegmentNArc(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);
    static void IntersectSegmentNCircle(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);
    static void IntersectSegmentNPlane(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);

    static void IntersectArcs(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);
    static void IntersectArcNCircle(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);
    static void IntersectArcNPlane(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);

    static void IntersectCircles(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);
    static void IntersectCircleNPlane(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);

    static void IntersectPlanes(class DimObject* iDimObject0, class DimObject* iDimObject1, std::vector<BPoint>& oIntersectPoints);

	static int Digits;

	unsigned int ObjectID; // Empty value is 0
	bool Highlighted;
	bool Selected;

    bool _changeState; // признак редактирования объекта
    
    BPoint _oldPt;

    Color _color;

    float _widthMain; // основная ширина/радиус

    bool _hidden; // признак того, что объект не рисуется
	static Text DimText;

	bool _depthTest;
};