// NProgram.h: interface for the NProgram class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "PAbstractObject.h"
#include "BProgram.h"

class NProgram : public BProgram, public PAbstractObject  
{
public:
	NProgram();
	virtual void DrawInt(int StockPos, bool CompOn, bool ProgOn, bool Draw4Pick, const std::vector<BProgVisInterval> & VisInt, int BoldCadr = -1, double t = -1., bool UseAxes = false, bool DrawNodes = false) override;
	DECLARE_SERIAL(NProgram)

	void TranslitText(bool Tr2Ru, char Cs, char Ce);
	void SetSciDoc(void* pDoc);
	void* GetSciDoc() const noexcept { return pSciDoc; }
	static void DeleteObj(NProgram* pProg) noexcept { delete pProg; }
	CString NLoad4LoadNewProgram(const CString& filename, const class NCUnit& ThisUnit) { return NLoad(filename, ThisUnit); }
	bool NReRead(const CString& FName, const class NCUnit& ThisUnit);// don't call this method directly
protected:
	CString NLoad(const CString& filename, const class NCUnit& ThisUnit);
	virtual ~NProgram();
	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;
protected:
	void* pSciDoc;
};
