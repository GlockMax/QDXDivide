#pragma once
#include "NMapIntToDouble.h"
#include "NUCSs.h"
#include "BGeomArray.h"
#include "NCoordLimits.h"
#include "NFavorConcepts.h"
#include "CadrID.h"
#include "NCEnums.h"
#include "NMirrorRotate.h"
#include "NCBase.h"
#include "NCadrGeom.h"
#include "NToolID.h"
#include "NChamfer.h"
#include "NMultiCounter.h"
#include "SMachState.h"
#include "NDraftCode.h"
#include "NCPar.h"
#include "NUSpindle.h"
#include "NTiltPlane.h"
#include "NCallStack.h"
#include "MTCompensations.h"
#include "BSplineParams.h"
#include "BRegsMap.h"
#include <map>
class IntToolID
{
public:
	IntToolID(int iPosition, int iTurret) { Position = iPosition;  Turret = iTurret; }
	IntToolID() { Position = -1;  Turret = -1; }
	bool operator< (const IntToolID& ID) const { return Position < ID.Position ? true : (Position > ID.Position ? false : (Turret < ID.Turret ? true : false)); }
	bool IsEmpty() const { return Position < 0 || Turret < 0; }
protected:
	int Position;
	int Turret;
};

class NMapToolID2Double : public std::map<IntToolID, double>
{
public:
	NMapToolID2Double();
	~NMapToolID2Double();

	NMapToolID2Double & operator= (const NMapToolID2Double &In) { std::map<IntToolID, double >::operator=(In); return *this; };
};

enum ZPType
{
	ZPT_STD,
	ZPT_REL,
	ZPT_SHT
};
enum PlaceToolType
{
	PT_EMPTY = 0,
	PT_IJK
};
const int MAX_REG = INT_MAX;// Maximum number of the registers
const int MAX_SPINDLE_NUM = 5;// Maximum number of the spindles
const int SPINDLE_MILL = 0;
const int SPINDLE_TURN = 1;
const int REG_STACK_BASE = 50000;
const int REG_COPY_BASE = 50950;

class AFX_EXT_CLASS NCUnitStatePrevGeom
{
public:
	NCUnitStatePrevGeom();
	virtual void ChangePlane(enum Plane Src, enum Plane Dest);
	virtual void Reset();
	static void TrCoord(double * pX, double * pY, double * pZ, double H, const BMatr & Tr);
	static const BMatr ChangePlaneMatr(enum Plane Src, enum Plane Dest);
	bool IsAngleConcave(const NCadrGeom & NextGeom) const;
	static bool DIntOtrArc(const NCadrGeom &Geom0, const NCadrGeom &Geom1, double d, BOOL OtrFirst, BPoint &p);
	static bool DIntArcArc(const NCadrGeom &Geom0, const NCadrGeom &Geom1, double d, BPoint &p);
	static bool DIntOtrOtr(const NCadrGeom &Geom0, const NCadrGeom &Geom1, double d, BPoint &p);

	double MashX;// Current coord of Mashine Tool
	double MashY;
	double MashZ;

	double MashcX;// Current coord of Mashine Tool with compensation
	double MashcY;
	double MashcZ;

	NCadrGeom Geom;// Temporary geometry of current cadr
	enum LR CurDirComp;
	BMTLinkage MachLink;
};

