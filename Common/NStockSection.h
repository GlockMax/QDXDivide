#pragma once
#include "BPoint.h"
#include "NColor.h"
#include "CutObjectGL.h"
#include "BBox.h"

class MBody;
class NStockSectionDlg;

class AFX_EXT_CLASS NStockSection
{
	friend class NStockSectionDlg;
public:
	NStockSection(CWnd *pParent);
	~NStockSection(void);
protected:
	BPoint P;
	BPoint N;
	bool Active;
	bool FillSection;
	bool CutAll;
	bool WaitPoint;
	NStockSectionDlg *pDlg;
	NColor Color;
	double Vmin;
	double Vmax;
	BBox StockGabar;
	CutGL LocalCut;
public:
	void RemoveClipPlane(void);
	void SetClipPlane(bool ClipNear = true);
	void EnableClipPlane(void);
	void DisableClipPlane(void);
	void DrawClipPlane(void);
	void Activate(const BBox &Gabar);
	void Deactivate(void);
	bool IsActive(void);
	bool IsWaitingPoint(void);
	void SetPoint(const BPoint & InP);
	void SetPlane(int PlNum);
	void CalcVMinMax();
	bool NeedFillSection(void);
	bool NeedCutAll(void);
	void EndTest(void);
	void BeginTest1(void);
	void BeginTest2(void);
	bool ContainsP(const BPoint &P, double Eps);
	bool CutsOffP(const BPoint &iP, const BPoint &View);
	const BPoint &GetN(void) { return N;}
	const NColor &GetColor(void) const{ return Color;}
	bool GetActive() const { return Active; }
	void SetActive(bool val) { Active = val; }
};
