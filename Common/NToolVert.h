#pragma once
#include "NToolShaped.h"
class NToolVert : public NToolShaped
{
public:
	NToolVert(const CString & Name, const CString & FileName, const NContour& Cont, bool CutCont);
	NToolVert(const CString &Name, double a, double D, double H, double h, double L, double W, double R);
	void Serialize(CArchive & ar);
	virtual ~NToolVert();
	NToolVert * CreateCopy();
	bool IsConcave(void) const override { return true;}
	virtual int GenInit() override;
	static NToolVert * MakeTool(const CString &Name, const CString & FileName, NContour &ToolCont, bool CutCont);
	void FillSheet(CPropertySheet * pDial) override;
	const CString &GetFileName() const { return m_FileName; }
	bool MakeContour();
	bool IsDiskTool() const override { return true; }
	DECLARE_SERIAL(NToolVert);
protected:
	virtual bool GetStdParams(double &Diameter, double &CornerRadius, double &ELength, double &FLength, double &BottomAngle, double &SideAngle, double &Height) override;
protected:
	NToolVert();
	double	m_D;
	double	m_H;
	double	m_h;
	double	m_L;
	double	m_W;
	double	m_a;
	double	m_R;
	CString m_FileName;
};

