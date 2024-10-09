// CMapStringToNProgram.h: interface for the CMapStringToNProgram class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "PAbstractObject.h"
#include "NProgram.h"
#include "cadrID.h"
#include "..\NCMExt\NExtEnums.h"
#include "NMapStringToOb.h"

enum TrajType
{
	TT_PROG,
	TT_COMP,
	TT_BOTH
};

class CMapStringToNProgram : public BMapStringToNProgram, public PAbstractObject
{
public:
	CMapStringToNProgram();
	~CMapStringToNProgram();
	void SetProgColorMode(PROG_COLOR_MODE Mode);
	bool HaveCompTraj(void) const;
	bool HaveProgTraj(void) const;
	int CalcAllProgramsSize(void);
	int GetAllProgramsSize(void) const;
	void Serialize(CArchive& ar);
	NCadr *GetCadr(cadrID ID) const;
	NCadrGeom* GetCadrGeom(cadrID HistID, int StockPos);
	NCadrGeom* GetCadrCompGeom(cadrID HistID, int StockPos);
	CString GetProgName(cadrID ID) const;
	BProgram *GetProg(cadrID ID) const;
	BProgram *GetProg(const CString &ProgName) const;
	void AddMainProg(LPCTSTR key, CObject* newValue);
	BOOL RemoveKey(LPCTSTR key);
	void RemoveAt(INT_PTR nIndex);
	void RemoveAll();
	int GetMainProgCount() const { return MainProgCount;}
	cadrID GetNearest(cadrID SourceHID, cadrID DestID) const;
	cadrID GetNearestTCFor(cadrID SourceHID, bool *pProjEnd) const;
	cadrID GetNearestTCBack(cadrID SourceHID) const;
	bool IsCadrProtected(cadrID CadrID) const;
	bool IsActive(void) const { return Active;}
	bool GetDrawOnlyActiveToolPart(void) const { return DrawOnlyActiveToolPart; }
	BOOL Lookup(const CString& key, NProgram*& pObj) const;
protected:
	void SetDrawOnlyActiveToolPart(bool Val);
public:
	bool Visible;
	TrajPart PartialDraw;
	TrajType ProgComp;
	unsigned int WormAfter;
	unsigned int WormBefore;
	bool UseAxes;
	bool DrawNodes;
	bool UseToolColor4Prog;
protected:
	bool DrawOnlyActiveToolPart;
	PROG_COLOR_MODE PCMode;
	int AllProgramsSize;
	int MainProgCount;
	bool Active;

	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;
};