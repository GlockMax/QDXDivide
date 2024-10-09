// NMapStringToOb.h: interface for the NMapStringToOb class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "NCTOOL_API.h"
#include "PAbstractObject.h"
#include "NColor.h"
#include "NStock.h"
#include "NMapStringToOb.h"
#include "BEdgesAttrs.h"

class NCTOOL_API CMapStringToNStock : public BMapStringToNStock, public PAbstractObject
{
public:
	CMapStringToNStock(); 
	virtual ~CMapStringToNStock();
	void SetVisible(bool vis = true);
	bool IsVisible(void) const;
	const BEdgesAttrs &GetEdgesAttrs(void) const { return EdgesAttrs; }
	const NColor &GetEdgesColor(void) const { return EdgesAttrs.GetEdgesColor(); }
	void SetEdgesColor(const NColor & Col);
	double GetEdgesWidth(void) const { return EdgesAttrs.GetEdgesWidth();}
	void SetEdgesWidth(double Width);
	double GetSmoothAngle(void) const { return EdgesAttrs.GetSmoothAngle();}
	void SetSmoothAngle(double Angle);
	double GetTransparency(void) const { return Transparency;/* (GetRMode() == RM_TRANSLUCENT) ? Transparency : 1.; */}
	void SetTransparency(double Transp);
	RenderMode GetRMode(void) const { return RMode;}
	void SetRMode(RenderMode Mode) { RMode = Mode;}
	void Draw(RenderMode GlobRMode, bool DrawSurf);
	bool IsThereAnyVisObj(void) const;
	bool IsThere0StPos(void) const;
	void Serialize(CArchive& ar);
	BBox GetGabar(void) const;
protected:
	bool Visible;
	RenderMode RMode;
	double Transparency;
	bool DrawSharpEdgesS;
	bool DrawSmoothSurf;
	bool MooveWithStock;
	BEdgesAttrs EdgesAttrs;
	NColor AxialSectionColor;
protected:
	void SetEdgesAttrs(const BEdgesAttrs &Attr);
public:
	void SetAxialSectionColor(const NColor &C) { AxialSectionColor = C; }
	const NColor &GetAxialSectionColor() const { return AxialSectionColor; }
	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;
	bool HasTranslucent(void);
	bool GetMooveWithStock(void) const { return MooveWithStock;}
	void MakeName(CStringA &Name) const;
	void AddObj(CString& name, int& ind, CObject* pObj);
	CString DeleteObjByInd(int ind);
	bool GetDrawSharpEdges() const { return DrawSharpEdgesS; }
	void SetDrawSharpEdges(bool Val) { DrawSharpEdgesS = Val; }
public:
	static inline const CStringA PrefixStock = _T("@S@");
	static inline const CStringA PrefixPart = _T("@P@");
	static inline const CStringA PrefixFixt = _T("@F@");
};