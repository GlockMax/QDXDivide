#pragma once

#include "OPENGLRENDER_API.h"
#include "BPoint.h"

class OPENGLRENDER_API OGLCursor
{
public:

    void Clear() { Defined = false; }
	
    void Draw() const;
	
    void Set(double x, double y, double z);
	void Get(double &x, double &y, double &z) const;
	
    bool IsDefined(void) const { return Defined;}
	
    OGLCursor(): size(5), width(1.0f), cR(1.0), cG(0.0), cB(0.0), type(QuadDiagonalType) { Clear(); }
    ~OGLCursor() {}
    
    enum CursorType
    {
        QuadDiagonalType = 0,
        QuadType,
        ClockType,
        CircleType
    };
    
    void SetSize(int iSize) { size = iSize; }
    int GetSize() const { return size; }

    void SetWidth(float iWidth) { width = iWidth; }
    float GetWidth() const { return width; }

    void SetColor(double iR, double iG, double iB) { cR = iR, cG = iG, cB = iB; }
    void GetColor(double& oR, double& oG, double& oB) const { oR = cR, oG = cG, oB = cB; }

    void SetType(CursorType iType) { type = iType; }
    CursorType GetType() { return type; }

protected:
    void DrawQuadDiagonal(const BPoint& iP, const BPoint& iDXW, const BPoint& iDYW) const;
    void DrawQuad(const BPoint& iP, const BPoint& iDXW, const BPoint& iDYW) const;
    void DrawClock(const BPoint& iP, const BPoint& iDXW, const BPoint& iDYW) const;
    void DrawCircle(const BPoint& iP, const BPoint& iDXW, const BPoint& iDYW) const;

	double x, y, z;
	bool Defined;

    int size;
    float width;
    double cR, cG, cB;
    CursorType type;
};