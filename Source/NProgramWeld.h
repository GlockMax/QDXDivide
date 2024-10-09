#pragma once
#include "NMapStringToOb.h"
#include "NSectionSetProg.h"
#include "NWeldTrajectories.h"
#include "NWeldCadr.h"
#include "nprogram.h"

class NProgramWeld :
	public NProgram
{
	friend class NSectProgDlg;
	friend class NWeldProgPropDlg;
protected:
	class NSectNums : public CString
	{
	public:
		NSectNums(void);
		~NSectNums(void);
		bool GetFirst(int *Start, int *End);
		bool GetNext(int *Start, int *End);
		bool AddPair(int Start, int End);
		const CString GetString(void);
		const NSectNums &operator =(const NSectNums &In);
		const NSectNums &operator =(const CString &In);
		void Serialize(CArchive &ar);
	protected:
		int CurrentStrPos;
		const char Delim;
	};
public:
	NProgramWeld(void);
	virtual ~NProgramWeld(void);
	virtual void DrawInt(bool CompOn, bool ProgOn, bool Draw4Pick, const std::vector<BProgVisInterval> & VisInt, int BoldCadr = -1, double t = -1., bool UseAxes = false, bool DrawNodes = false);
	virtual void ClearEvaluated(void);
	virtual void FillGabar(const BMatr *MatrArr);
	int MakePaths(class MTMachineTool &MT, class CMapStringToNStock *pPartsList);
	BPoint CalcPtsTriplet(double t, int Ind, class MTMachineTool &MT, class CMapStringToNStock *pPartsList, class BPoint &FirstP, BPoint &SecontP, BPoint &RootP, double MinW);
	void PrepareSections(const MTMachineTool &MT, class CMapStringToNStock *pPartsList);
	void PreparePaths(MTMachineTool &MT, class CMapStringToNStock *pPartsList);
	virtual void Serialize(CArchive& ar);
	void SetDialog(CWnd *pDlg) { pDialog = pDlg;}
	NWeldTrajectories &GetTrajectories(void) { return Trajectories;}
//	void FillWeldCadrs(const class NCUnitStatePrev &Start, const class NCUnitStatePrev &End, const NCadrGeomArray &CadrGeomArray);
	void DrawPenetration(int Cadr, double t);
	DECLARE_SERIAL(NProgramWeld)

protected:
	int MakeSectionPlanes(class MTMachineTool &MT);
	CWnd *pDialog;
protected:
	typedef CTypedPtrArray <CPtrArray, NWeldCadr *> NWeldCadrArray;
	NWeldTrajectories Trajectories;
	NSectionSetProg ProgSections;
	int IntSections;
	bool VisSections;
	NSectNums SectNums;
	NWeldCadrArray WeldArray;
};
