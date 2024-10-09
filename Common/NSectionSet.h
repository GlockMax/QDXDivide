#pragma once
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "NColor.h"
#include "NActiveObject.h"

class NStock;
class NColor;

const int MAX_SECTIONS = 256;

class AFX_EXT_CLASS NSectionSet : public NActiveObject
{
public:
	NSectionSet(int PlanesNum = 0);
	virtual ~NSectionSet(void);
	virtual PObjectStatus OnStatusChanged(int ObjectInd, PObjectStatus NewStatus);
	void Clear(void);
	virtual void DrawSections(int SInd = 0, int EInd = -1) const;
	virtual int MakeAllSections(void);
	int MakeStockSections(const class NStock *pStock);
	void SetPosition(const class BPoint &iP, const class BPoint &iN, int Pos = 0);
	bool IsCreated(void) const { return NSect > 0;}
	int SetArcArray(const BPoint & C, const BPoint & V, const BPoint &BaseDir, int Num , double StartA , double EndA);
	int SetLineArray(const BPoint & P, const BPoint & V, int Num , double StartD, double EndD);
	virtual int SetNPlanes(int Num);
	void SetAllColors(const NColor &Col);
	void SetVisible(bool Vis) { Visible = Vis;}
	bool GetVisible(void) const { return Visible;}
	void SetThickness(GLfloat Th) { Thickness = Th;}
	GLfloat GetThickness(void) const { return Thickness;}
protected:
	GLfloat *Sections[MAX_SECTIONS];
	NColor Colors[MAX_SECTIONS];
	int NSect;
	int NPlanes;
	BPoint *P;
	BPoint *N;
	bool Trimed;
	bool Visible;
	GLfloat Thickness;
protected:
	int MakeOneSection(const class NStock &Stock, const class BPoint &P, const class BPoint &V);
};
