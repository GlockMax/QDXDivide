// NToolCombined.h: interface for the NToolCombined class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NCMComData.h"
#include "NPassiveObject.h"
#include "NTool.h"
#include "NColor.h"
#include "BBox.h"
#include "InfoTool.h"
#include "NPassiveObject.h"
#include "NToolPalette.h"
#include "BGeomArray.h"
#include "PAbstractObject.h"

typedef CTypedPtrArray <CObArray, NTool*> ToolArray;
int compare_double(const void *a, const void *b);// функция для сравнения двух чисел

class AFX_EXT_CLASS NToolCombined : public NPassiveObject, public PAbstractObject  
{
public:
	friend class NToolsConCrPage;
	static NToolPalette Palet;
	BOOL IsVisible();
	bool IsNameAuto;
	InfoTool & GetInfo();
	InfoTool Info;
	NToolCombined();
	NToolCombined(const CString &Name);
	virtual NCMApplicationType GetClass() const { return AT_UNDEF;}
protected:
	ToolArray pTools;
	CString Name;
	BPoint CompShift;
	bool CompShiftAuto;
	bool IsUsed;
	double LifeTime;
	bool IgnoreUpToolStockColl;
public:
	int Turret;
	int Attach;
	int Position;
	int num_tools;

	int PointSize;
	double Zdisp;
	BOOL WFrame;
	BOOL Visible;
	NColor ColorPassive;
	NColor Color;
	NColor ColorHolder;
	BGeomArray Skeleton;

	virtual NToolCombined* CreateCopyTool() const;
	int AddTool(NTool *pTool);
	bool AddToolSafe(NTool *pTool);
	bool RemoveTool(int index);
	bool RemoveReference(int index);
	void ClearAll();	
	void Draw(RenderMode GlobRMode) const;
	const CString &GetName() const;
	BBox GetGabar() const;
	void SetName(const CString& pName);
	virtual void FillDialog(CDialog *pDial) const;
	virtual void Serialize(CArchive &ar);
	DECLARE_SERIAL(NToolCombined)

	virtual ~NToolCombined();
protected:
	void GenInit();
public:
	void CalcCompShift(void);
	void SetColor(const NColor &Col);
	const NColor& GetColor() const { return Color; }
	void SetColorPassive(const NColor &Col);
	void SetColorHolder(const NColor &Col);
	NTool * GetTool(int n) const;
	NTool * GetLowermostTool(void) const;
	void SetChildrenColors(void);
	virtual void SetChildrenDispl(void){};
	virtual void RestChildrenDispl(void){};
	virtual void Regenerate(void);
	bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height);
	bool NeedAppr(void) const;
	bool MakeApprTool();
	double GetProbeDiam();
	BMatr GetCompMatr(void) const;
	virtual BPoint GetCompVect(void) const;
	void SetVisible(BOOL Val) { Visible = Val;}
	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;
	void SetIsUsed(bool In);
	bool GetIsUsed(void) const;
	NToolCombined * CreateOffsetTool(bool NonCuttingOnly) const;
	NToolCombined* CreateTool4Cutting() const;
	int GetToolsNum(void) const { return int(pTools.GetSize());}
	void RemoveEndBall(void);
	void RestoreEndBall(void);
	double GetLifeTime(void) const { return LifeTime; }
	bool IsLifeTimeDifferent(const NToolCombined *pInTool) const;
	bool IsVertComTool() const;
	bool IsConcave() const;
	BBox GetProbeGabar() const;
	bool IsIgnoreUpToolStockColl() const;
	void DrawGabar(RenderMode GlobRMode) const;
	double GetZDisp() const { return Zdisp; }
	const BGeomArray& GetSkeleton();
	void RemakeSkeleton();
	const NToolCombined* GetTestTool() const;
};

