// NTool.h: interface for the NTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NCEnums.h"
#include "RenderMode.h"
#include "NFastColorMode.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "BBox.h"
#include "NColor.h"
#include "NCMComData.h"
#include "BGeomArray.h"
#include "ToolMash.h"

#include "WM_APP_messages.h"

const int START_TURNBLADEMILL = 100;
const int END_TURNBLADEMILL = 199;

class NContour;

class AFX_EXT_CLASS NTool : public CObject  
{
	friend class NMapStringToNTool;
	friend class NToolStd;
public:
	class NColorExchanger
	{
	public:
		NColorExchanger();
		void Store(NTool* pT);
		void Restore();
	protected:
		NTool* pTool;
		NColor Col;
		NColor ColPass;
	};
public:
	static BOOL IsShapeVisible();
	static BOOL IsShapeVisibleHolder();
	static void FlipShapeDraw();
public:
	virtual bool CreateToolContour(GLfloat *pts,TypeLineInContour *type, int size, BOOL f);
	virtual bool CreateToolContour(GLfloat *pts,int *type, BOOL f);
	virtual double GetZdisp() const { return Zdisp;}
	virtual void SetZdisp(double z){ Zdisp = z;}
	//void DrawTransp(int step);
	virtual NTool* CreateCopyTool();
	int GetToolStep(ToolMash m = UNDEF );
	int GetType();
	NTool();
	NTool(const CString &n);
	const CString &GetName() const;
	virtual void FillDialog(CDialog *pDial);
	virtual void FillSheet(CPropertySheet *pSheet);
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual void Draw(RenderMode GlobRMode, int step);
	virtual bool IsBallNose() const { return false; }
	virtual bool IsCylWBallNose() const { return false; }
	virtual bool IsFlatNose() const { return false;}
	virtual bool IsFullBall() const { return false;}
	virtual bool IsDiskTool() const { return false;}
	virtual ~NTool();
public:
	BOOL Visible;
	BOOL WFrame;
	BBox Gabar;
	DECLARE_SERIAL(NTool)
private:
	int ToolType; // изменяется от 10 до 99
		// 10 - инструмент описываемый АРТ 7 (универсальный)
		// 11 - инструмент с фаской
		// 12 - инструмент со скруглением
		// 13 - угловой инструмент
		// 14 - инструмент типа сверло
		// 20 - дисковый инструмент (универсальный)
		// 21 - дисковый инструмент с фасками
		// 22 - дисковый инструмент со скруглениями
		// 23 - дисковый одноугловой инструмент 
		// 24 - дисковый двухугловой инструмент 
		// 3* - зарезервировано для листопробивки
		// 40 - оправка (описывается NToolStd)
		// 41 - прямая оправка
		// 42 - угловая оправка
		// 43 - оправка с фаской
		// 5* - токарный (пластинка)
		// 6* - токарный (holder) (62 - STL)
		// 7* - фасонный
		// 8* - ЭЛС
private:
	bool Probe;
protected:
	NColor Color;
	NColor ColorPassive;
	double Zdisp;
	CString Name;
	bool Cutting;
	bool Shank;
	BGeomArray ToolSkel;

	static BOOL ShapeVisibleTool;
	static BOOL ShapeVisibleHolder;
	static bool IgnoreUpToolStockColl;

	static enum RenderMode RMTool;
	static enum RenderMode RMHolder;
	static enum CollisWithStock ToolStockCollision;
	static double Transparency;
	static double XYOffset;
	static double ZOffset;
	static double XYOffsetSh;
	static double ZOffsetSh;
	static double TurnOffset;
	static enum NFastColorMode FastColorMode;
public:
	static double RoundP(double in);
	static bool IsTranslucent(void);
	static bool IsToolTranslucent(void);
	static enum CollisWithStock GetToolStockCollision(void);
	static void SetToolStockCollision(CollisWithStock val);
	static void SetTurnOffset(double val);
	static void SetXYOffset(double val);
	static void SetZOffset(double val);
	static void SetXYOffsetSh(double val);
	static void SetZOffsetSh(double val);
	static double GetTurnOffset(void);
	static double GetXYOffset(void);
	static double GetZOffset(void);
	static double GetXYOffsetSh(void);
	static double GetZOffsetSh(void);
	static bool GetIgnoreUpToolStockColl(void);
	static void SetIgnoreUpToolStockColl(bool val);
	static void SetShapeVisibleTool(bool Val);
	bool IsVisible(void);
	RenderMode GetRMode(void);
	static NFastColorMode GetFastColorMode();
	static void SetFastColorMode(NFastColorMode Val);
protected:
	void CreateCopyTo(NTool *pTool);
public:
	virtual int GenInit();
	virtual void Regenerate(void);
	static BOOL CreateSimGeneratrix(NTool *pTool, NContour &tmpContour);
	void SetColor(const NColor & Col);
	void SetColorPassive(const NColor & Col);
	void SetColors(const NColor & Col, const NColor & ColP);
	void SetTempColors(const NColor & Col, const NColor & ColP);
	const NColor & GetColor(void) const;
	const NColor & GetColorPassive(void) const;
	virtual bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height);
	virtual bool IsConcave(void) const;
	virtual bool NeedAppr(void) const;
	virtual bool IsStdCC(void) const;
	const BBox & GetGabar(void) const { return Gabar; };
	void SetOpaque(void);
	void SetType(int type);
	bool IsCutting() const { return Cutting; }
	bool IsHolder() const { return !Cutting && !Shank; }
	void SetCutting(bool IsCutting) { Cutting = IsCutting; }
	bool IsShank() const { return Shank; }
	void SetShank(bool IsShank) { Shank = IsShank; Cutting = (IsShank ? false : Cutting); }
	virtual NCMApplicationType GetClass() const { return AT_UNDEF;}
	virtual NTool * CreateOffsetTool(bool NonCuttingOnly) {return CreateCopyTool();}
	virtual bool IsVertTool() const { return false; }
	bool IsProbe() const { return Probe; }
	void SetProbe(bool Val);
	bool NeedDraw(enum RenderMode GlobRMode);
	BGeomArray& GetToolSkel() { return ToolSkel; }
};
