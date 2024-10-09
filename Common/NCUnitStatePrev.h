#pragma once
#include "BIPolar.h"
#include "BICyl.h"
#include "BMTLinkage.h"
#include "NCUnitStatePrevGeom.h"


class AFX_EXT_CLASS NCUnitStatePrev : public NCUnitStatePrevGeom
{
public:
	enum EvaluateCadrResult
	{
		ECR_PROG_END = -1,
		ECR_BREAK = 1,
		ECR_NEED_PROBE = 2,
		ECR_ERROR = -2,
		ECR_OK = 0
	};
public:
	static double FastFeedX;
	static double FastFeedY;
	static double FastFeedZ;
	static double FastFeedA;
	static double FastFeedB;
	static double FastFeedC;
	static int DComp2RegStart;
	static int DComp2RegEnd;

protected:
	union Coords
	{
		_int16 byte;
		struct
		{
			BOOL x : 1;
			BOOL y : 1;
			BOOL z : 1;
			BOOL i : 1;
			BOOL j : 1;
			BOOL k : 1;
			BOOL r : 1;
			BOOL interp : 1;
			BOOL a : 1;
			BOOL b : 1;
			BOOL c : 1;
		};
	};
protected:
public:
	NCUnitStatePrev();
	virtual ~NCUnitStatePrev() {};

	void MakeEqualTo(class NCUnitStatePrev &src);
	void MakeEqualTo(const class NCUnitStatePrev &src);

	double GetPenetrDist(void) const;
	void ChangePlane(enum Plane Src, enum Plane Dest) override;
	virtual void Reset() override;
	class SMachStateFix1 GetMachStateFix(void) const;
	class SMachStateFix1 GetMachStateFixComp(void) const;
	const BInterp & GetInterp(void) const;
	void SetDCompFromRegisters();
	double GetCurLengthComp() const;
	void SetCurLengthComp(double Val);

public:

	//	NFavorConcepts *pFavorConcepts;

protected:
	// Members need to be copied
protected:
	NMapIntToDouble Registers;
	//END: Members need to be copied
public:
	// Must be in Prev
	NCBase CurBase;
	NDraftCode DraftCode;
	NChamfer Chamfer;
	MTCompensations MTComp;
	BIPolar PolarInterp;
	BICyl CylInterp;
	BMatr CSMatr;

	BPoint CurToolDisplacement;
	BPoint PrevToolDisplacement;
	NCadrGeom AddCompGeom;// Temporary additional compensation geometry of current cadr
	NCadrGeom CompGeom;// Temporary compensation geometry of current cadr
	BOOL RetractNext;
protected:
	double CurLengthComp;// can't change place???
public:
	bool OrderChanged;
	int CurLengthCompNum;
	enum UD CurZComp;
	bool CurZCompChanging;
	enum CycleType CurCycle;
	bool ProgramStarted;
	NUSpindle Spindle[MAX_SPINDLE_NUM];
	enum Cool Coolant;
	double CurDiaCompHorVec;//   DiaCompHorizVec		
	double CurDiaCompVerVec;//   DiaCompVerticalVec		
	enum Plane CurPlane;
	double Feed;
	double CurR;
	int CurTurret;
	enum curve CurCurveType;

	double CurX;// Current program defined coord
	double CurY;
	double CurZ;
	double CurI;
	double CurJ;
	double CurK;
	double CurA;
	double CurB;
	double CurC;

	double MashA;
	double MashB;
	double MashC;

	double RCPcX;// Current coord of Mashine Tool with compensation
	double RCPcY;
	double RCPcZ;

	NMapIntToDouble DComp;
	NMapIntToDouble HComp;
	cadrID ModalSubrID; // The ID of modal subroutine or -1 if no modal subr. active
	CString CycleZ;
	CString R1;
	CString R2;
	SMachState AddCoords;
	NToolID CurToolN;
	double ElectrS;
	double ElectrSL;
	double ElectrSF;
	double ElectrT1;
	double ElectrA;
	double ElectrB;
	bool GetNextGeomActive;
	bool AutoFixed;
	bool PolarCoordActive;
	bool GotoElse;
};


