#pragma once
#include "ntoolcombined.h"

class NToolMill;

class AFX_EXT_CLASS NToolCombinedMill :
	public NToolCombined
{
public:
	NToolCombinedMill(void);
	NToolCombinedMill(const CString &Name);
	~NToolCombinedMill(void);
	NToolMill * GetTool(int n) const;
	virtual NToolCombinedMill* CreateCopyTool() const;
	virtual void FillDialog(CDialog *pDial) const;
	virtual NCMApplicationType GetClass() const { return AT_MILL;}
	DECLARE_SERIAL(NToolCombinedMill)
	void SetChildrenDispl(void);
	void RestChildrenDispl(void);
	virtual BPoint GetCompVect(void) const;
	class NToolCombinedTurn *Convert2TurnTool(void);
	bool MakeShapedTools(NContour &ToolCont, bool CutCont);
	bool MakeVertTool(NContour &ToolCont, bool CutCont);
	bool MakeComplTool(const NToolCombinedMill *pTool);
	void SetCompShiftAuto(bool Val) noexcept { CompShiftAuto = Val; }
	static NTool* MakeConvTool(const NContour& iCont, bool CutCont);
};
