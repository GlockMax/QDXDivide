#include "StdAfx.h"
#include "ConstDef.h"
#include "DimObjectSegment.h"
#include "DimObjectPoint.h"
#include "DimObjectDimension.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(DimObjectDimension, DimObject, 0)

DimObjectDimension::DimObjectDimension(float iWidth, const char* iPrefix, const char* iPostfix) : DimObject(iWidth), _dimLine(nullptr)
{
	if(iPrefix)
	{
		_prefix = new char[strlen(iPrefix) + 1];
		strcpy_s(_prefix, strlen(iPrefix) + 1, iPrefix);
	}
	else
		_prefix = nullptr;
	if(iPostfix)
	{
		_postfix = new char[strlen(iPostfix) + 1];
		strcpy_s(_postfix, strlen(iPostfix) + 1, iPostfix);
	}
	else
		_postfix = nullptr;

    _textColor[0] = _textColor[1] = _textColor[2] = 0.0;

    _bckgrndColor[0] = 1.0;
    _bckgrndColor[1] = 1.0;
    _bckgrndColor[2] = 0.0;

    _borderColor[0] = _textColor[0];
    _borderColor[1] = _textColor[1];
    _borderColor[2] = _textColor[2];
}

DimObjectDimension::DimData::DimData(): _magnExtens(0.0), _widthExtens(1.0), _cursorLength(15.0), _cursorAngleSin(sin(PI / 12.0)), _cursorAngleCos(cos(PI / 12.0)), _radius(3.0)
{
    _cursorType[0] = _cursorType[1] = ClosedFilled;
}

void DimObjectDimension::SetCursorHeight(double iHeight)
{
    double rootInv = 1.0 / sqrt(_data._cursorLength * _data._cursorLength + iHeight * iHeight * 0.25);
    _data._cursorAngleSin = iHeight * 0.5 * rootInv;
    _data._cursorAngleCos = _data._cursorLength * rootInv;
}

void DimObjectDimension::DrawCursors(const BPoint* iExtLine0, const BPoint* iExtLine1, double iKoefMagn, bool iOrthoToExtens) const
{
    BPoint vects[3] = {iExtLine1[1] - iExtLine0[1], iExtLine0[1] - iExtLine0[0], iExtLine1[1] - iExtLine1[0]};
    BPoint normal = vects[1] % vects[0],
           dimDir[2] = {normal % vects[1], normal % vects[2]};
    if (!iOrthoToExtens)
        dimDir[0] = dimDir[1] = iExtLine1[1] - iExtLine0[1];
    dimDir[0].Unit();
    dimDir[1].Unit();
    BPoint orthoDir[2] = {GetViewDir() % dimDir[0], GetViewDir() % dimDir[1]};
    orthoDir[0].Unit();
    orthoDir[1].Unit();
    BPoint dimPts[2] = {iExtLine0[1], iExtLine1[1]};
    double height = _data._cursorLength * iKoefMagn;
    double sign = 1.0;
    for (unsigned int i = 0; i < 2; ++i)
    {
        if (_data._cursorType[i] != DimData::Empty)
        {
            BPoint pts[3] = {dimPts[i],
                             dimPts[i] + (dimDir[i] * _data._cursorAngleCos * sign + orthoDir[i] * _data._cursorAngleSin) * height,
                             dimPts[i] + (dimDir[i] * _data._cursorAngleCos * sign - orthoDir[i] * _data._cursorAngleSin) * height};
        
            glPushAttrib(GL_LINE_BIT);
            glLineWidth(_widthMain);
            switch (_data._cursorType[i])
            {
            case DimData::Opened:
                {
                    BPoint tmpPts[2][2] = {pts[0], pts[1], pts[0], pts[2]};
                    DrawGLPrimitive(tmpPts[0], 2);
                    DrawGLPrimitive(tmpPts[1], 2);
                    break;
                }
            case DimData::Closed:
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    DrawGLPrimitive(pts, 3);
                    break;
                }
            case DimData::ClosedFilled:
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    DrawGLPrimitive(pts, 3);
                    break;
                }
            };
            glPopAttrib();
        }

        sign = -1.0;
    }
}

void DimObjectDimension::DrawDimValue(const std::string& iString, const BPoint* iDimPts, const BPoint& iMidPoint, double iKoefMagn) const
{
    BPoint rectPts[4], textOrigin;
    FindRectCoords(rectPts, textOrigin);

    // нарисовать прямоугольник
    glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT);
    glColor3dv(_bckgrndColor);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    DrawGLPrimitive(rectPts, 4);
 
	// нарисовать текст
	std::string text = _prefix + GetDimValueText() + _postfix;

    glColor3dv(_textColor);
	DimText.Draw3D(textOrigin.GetX(), textOrigin.GetY(), textOrigin.GetZ(), const_cast<char*>(text.c_str()));

    // нарисовать рамку
    BPoint viewDir = GetViewDir();
    rectPts[0] -= viewDir;
    rectPts[1] -= viewDir;
    rectPts[2] -= viewDir;
    rectPts[3] -= viewDir;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3dv(_borderColor);
    DrawGLPrimitive(rectPts, 4);
    glPopAttrib();
}

void DimObjectDimension::FindRectCoords(BPoint oPts[4], BPoint& oTextOrigin) const
{
    BPoint dimPts[2];
    GetEndPoints(dimPts[0], dimPts[1]);
    BPoint mid = _dimLine->GetMidPoint();

    // определить длину текста
    // TODO: определить точно
    // ширину символа можно определить через GetCharABCWidths(hDC, 'a', 'a', abc); GetTextExtentPoint32
	int symbWidth = DimText.height + 2;
    std::string text = _prefix + GetDimValueText() + _postfix;
    double textLen = (double)((int)text.size() * symbWidth);

    // определить плоскость рисования
    BPoint viewDir = GetViewDir();
    double cos = (dimPts[1] - dimPts[0]) * viewDir;
	BPoint origin = BPoint::ProjectPointOnPlane(mid, cos > 0.0 ? dimPts[0] : dimPts[1], viewDir);

    // определить длины сторон прямоугольника
    double a = (textLen + DimText.height) * 0.5;
	double b = DimText.height * 1.5;

    // определить направления прямоугольника
    BPoint xDir = GetXDir(),
           vectA = xDir,
           vectB = viewDir % xDir;

    // определить точку начала текста
    double scaleKoef = DimObjects::GetScaleKoef();
    oTextOrigin = origin - (xDir * textLen * 0.25 - vectB * DimText.height * 0.25) * scaleKoef;
    oTextOrigin -= viewDir;

    vectA *= a * scaleKoef;
    vectB *= b * scaleKoef;

    // определить точки прямоугольника
    BPoint vect[2] = {(vectA + vectB) * 0.5, (vectA - vectB) * 0.5};
    oPts[0] = origin + vect[0];
    oPts[1] = origin + vect[1];
    oPts[2] = origin - vect[0];
    oPts[3] = origin - vect[1];
}

BBox DimObjectDimension::GetGabar() const
{
    if (!_dimLine)
        return BBox();

    BBox box = _dimLine->GetGabar();

    BPoint pts[4], tmp;
    FindRectCoords(pts, tmp);

    box.Expand(pts[0]);
    box.Expand(pts[1]);
    box.Expand(pts[2]);
    box.Expand(pts[3]);

    return box;
}

void DimObjectDimension::Serialize(CArchive& ar)
{
    DimObject::Serialize(ar);
    SerializeElements(ar, &_data, 1);
    SerializeElements(ar, &_textColor[0], 3);
    SerializeElements(ar, &_bckgrndColor[0], 3);
    SerializeElements(ar, &_borderColor[0], 3);
	if(ar.IsStoring())
		ar << _dimLine;
	else
		ar >> _dimLine;
	int StrLen = 0;
	if(ar.IsStoring())
		StrLen = int(strlen(_prefix) + 1);
	SerializeElements(ar, &StrLen, 1);
	if(ar.IsLoading())
	{
		delete[] _prefix;
		_prefix = new char[StrLen];
	}
	SerializeElements(ar, &_prefix[0], StrLen);
	StrLen = 0;
	if(ar.IsStoring())
		StrLen = int(strlen(_postfix) + 1);
	SerializeElements(ar, &StrLen, 1);
	if(ar.IsLoading())
	{
		delete[] _postfix;
		_postfix = new char[StrLen];
	}
	SerializeElements(ar, &_postfix[0], StrLen);
}

bool DimObjectDimension::IsEqualTo(DimObject* iDimObject) const
{
    if (iDimObject->GetType() != GetType())
        return false;


    return false;
}

std::string DimObjectDimension::GetDimValueText(void) const
{
	const int TEXT_BUFFER_SIZE = 400;
	TCHAR szBuffer[TEXT_BUFFER_SIZE];
	long Denominator = long(pow(10., Digits));
	long Value = long(GetDimValue() * Denominator + 0.5);
	ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer) - 1, _T("%.*g"), 15, (double)Value / Denominator));
	return std::string(szBuffer);
}

