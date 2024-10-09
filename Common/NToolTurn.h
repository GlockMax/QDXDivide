// NToolTurn.h: interface for the NToolTurn class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NTool.h"
#include "TBladeCont.h"

class TBladeCont;

class AFX_EXT_CLASS NToolTurn : public NTool  
{
public:
	NToolTurn(const CString &name);
	void Draw(RenderMode GlobRMode, int step) override;
	virtual ~NToolTurn();
	virtual const TBladeCont *GetBladeCont()const{return NULL;} ;
	virtual void FillPage(CDialog *pPage);
	virtual void FillFromPage(CDialog *pPage);
	virtual void Serialize(CArchive &ar);
	virtual NToolTurn* CreateCopyTool();
	virtual bool GetStdParams(double &Diameter , double &CornerRadius , double &ELength , double &FLength , double &BottomAngle , double &SideAngle , double &Height){ return false;}
	DECLARE_SERIAL(NToolTurn)
protected:
	NToolTurn();
	void CreateCopyTo(NToolTurn *pTool);
protected:
	BPoint Displacement;
	double MainAngle;
public:
	void SetDisplacement(const BPoint & RP, double MAngle);
	const BPoint & GetDisplacement(void);
	double GetMainAngle(void);
	BMatr GetPosMatr(void) const;
	virtual NCMApplicationType GetClass() const { return AT_TURN;}
};
