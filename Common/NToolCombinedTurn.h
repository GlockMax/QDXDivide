#pragma once
#include "ntoolcombined.h"

class NToolTurn;

class AFX_EXT_CLASS NToolCombinedTurn :
	public NToolCombined
{
	friend class NToolCombinedMill;
public:
	NToolCombinedTurn(void);
	NToolCombinedTurn(const CString &Name);
	~NToolCombinedTurn(void);
	NToolTurn * GetTool(int n) const;
	virtual NToolCombinedTurn* CreateCopyTool() const;
	virtual void FillDialog(CDialog *pDial) const;
	virtual NCMApplicationType GetClass() const { return AT_TURN;}
	virtual void Serialize(CArchive &ar);
	DECLARE_SERIAL(NToolCombinedTurn)
public:
	BPoint Displacement;
	double MainAngle;
	void SetChildrenDispl(void);
	void RestChildrenDispl(void);
	virtual BPoint GetCompVect(void) const;
	bool MakeShapedTools(const class TBladeCont &ToolCont, bool CutCont, double Thick, double Angle);
};
