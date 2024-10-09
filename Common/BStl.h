#pragma once
#include "MACHINE_API.h"
#include "RenderMode.h"
#include "BDCEL_STL.h"
#include "NColor.h"
#include "BRotSolid.h"
#include <vector>
#include <array>
#include "BStlB.h"

class MACHINE_API BStl :
	public BStlB
{
public:
	BStl(void);
	virtual ~BStl(void);
	void SetDefault();
	void Clear() override;
	void DrawOriginal(enum RenderMode GlobRMode, enum RenderMode ExtRMode, bool WFrame, bool DrawSharpEdges, bool DrawSurf, bool DrawSmoothSurf);
	void Transform(const BMatr &M) override;
	void WriteSTL(CStdioFile & f);
	int WriteSTLmulti(std::vector<CString>& FileNames);// FileNames[0] - start name
	void DrawSelected(void) const;
	void SetSharpEdges(bool SEdges);
	void SetSmoothSurf(bool SSurf);
	bool GetSharpEdges(void) const { return SharpEdges;}
	bool GetSmoothSurf(void) const { return SmoothSurf;}
	RenderMode GetRMode(void) const { return RMode;}
	void SetRMode(RenderMode Mode) { RMode = Mode;}
	void SetEdgesColor(const NColor &Col) { EdgesColor = Col;}
	void SetEdgesWidth(double Width) { EdgesWidth = Width;}
	void SetSmoothAngle(double Angle);
	void SetEdgesAttrs(const class BEdgesAttrs &Attr);
	void ClearRender(void);
	virtual void Serialize(CArchive &ar, double InFileVersion);
	virtual bool Create();
    bool FindSegments(SnapRopes& oSegments);
	void MakeCopy(BStl &object) const;
	bool MakeRDCEL();
	class BDCEL_STL& GetDCEL() { return DCEL; }
	class RDCEL* GetRDCEL() { return pDCELRender; }
	const class RDCEL *GetRDCEL() const { return pDCELRender; }
	bool Is1Connected();
	bool WriteRotCont(bool IgnoreVoids, bool DoOrient);
	bool MakeSpinContours(const BMatr& RelMatr, std::list<BClrContour>& Contours);

protected:
	BDCEL_STL DCEL;
	class RDCEL * pDCELRender;
	bool SharpEdges;
	bool SmoothSurf;
	int *SharpInd;
	RenderMode RMode;
	NColor EdgesColor;
	double EdgesWidth;
public:
	int SplitIntoSolids(std::vector<BStl>& Solids);
public:
	BRotSolid* pRotSolid;
};
