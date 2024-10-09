#pragma once

#include <vector>
#include "NMapStringToOb.h"
#include "PAbstractObject.h"

class NMapStringToNTool :
	public CMapStringToNTool, public PAbstractObject
{
public:
	NMapStringToNTool(void);
	virtual ~NMapStringToNTool(void);
	void Serialize(CArchive& ar);
	bool MakeActual(void);
	void FullCopy(const NMapStringToNTool & Source);
	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int , enum UpdateModes &AdditionalModes, int &Group) override;
	void ClearUsed(void);
	void DeleteUnUsed(void);
	void SetNeedProgRegen(bool Val) { NeedProgRegen = Val; }
	bool GetNeedProgRegen(void) const { return NeedProgRegen; }
	void GetFullNames(std::vector<CString> &Names);
	static const CString GetFullName(class NToolID ToolID, const NToolCombined *pTc);
	BOOL Lookup(const CString& key, NToolCombined*& pObj) const;
protected:
	bool NeedProgRegen;
};
