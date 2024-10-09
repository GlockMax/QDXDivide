#pragma once
#include "ppltasks.h"
#include "NCEnums.h"
#include "NCMProject.h"
#include "NCadrGeomArray.h"
#include "NCollisionLog.h"
#include "NCMComData.h"
#include "BSlideBuffer.h"
#include "BMatrPair.h"
#include "BRangePos.h"
#include "CadrData.h"

class NCadrGeom;
class NAnimState;
class GBodyRender;
class NToolCombined;
class NToolCombinedTurn;
class MQuadTree;
class MQuadBody;
class MBody;
class NCadrsProcessing;
class FThreadInter;
class MeshArr;
class BMatr;
class BPoint;
class MTMachineTool;

const int SlideBufSize = 1000;
typedef BSlideBuffer<CadrData, SlideBufSize, 3> SlideBuffer4DX; // A buffer for one cadr data

class AFX_EXT_CLASS FProg2BSP
{
public:
	FProg2BSP(void);
	~FProg2BSP(void);
protected:
	class NMachUnitPair *pMUPair;
	BMatr RotMatr;//StPosData
	BMatr BodyRotMatr;//StPosData
	MBody *pBodyRotStart;
	BMatrPair Matr;//StPosData
	BMatr MCS;
	bool RotMatrActive;//StPosData
	bool PauseOnColl;
	GBodyRender *pRender;
	NAnimState *pAnimState;
	bool Interrupt;
	std::vector< concurrency::task<UINT>> Cutting1DirThreads;
	SlideBuffer4DX SlideBufferCadr;
	MBody *pBody;
protected:
	const int SleepTime = 0;
public:
	std::vector<BRangePosColl> CollPositions;
	bool IsRotationActive;
	bool NeedNewPosition;
	bool Stop;
	bool InterruptDX5X;
public:
	GBodyRender *GetRender();
	NAnimState *GetAnimState();
	void ClearAnimState();
	void SetRender(GBodyRender *pR);
	int ExecGeomArray(NCadrGeom * Geom, int GSize, int GNumber, const NToolCombined *pTool, class CStocksCollection &Stocks, const BRangePos &Pos);
	int ExecGeomArrayDXC(NCadrGeom * Geom, int GSize, int GNumber, const NToolCombined *pTool, class CStocksCollection &Stocks, const BRangePos &Pos);
	int ExecGeomArrayDXC(const class GOneStepGeom & GeomArray, int StartInd, int GNumber, const NToolCombined *pTool, CStocksCollection &Stocks, const BRangePos &Pos);
	int EGAMill3x(NCadrGeom * Geom, int IndStart, int GSize, int GNumber, const NToolCombined *pTool, class CStocksCollection &Stocks, const BRangePos &Pos);
	int EGAMillDX(class GOneStepGeom & Geom, int StartInd, int GNumber, const NToolCombined *pTool, class CStocksCollection &Stocks, const BRangePos &Pos, DX_TOOLTYPE DXType);
	int EGAMill5x(NCadrGeom * Geom, int IndStart, int GSize, int GNumber, const NToolCombined *pTool, class CStocksCollection &Stocks, const BRangePos &Pos);
	int EGATurn(NCadrGeom * Geom, int IndStart, int GNumber, NToolCombinedTurn *pTool, class CStocksCollection &Stocks, const BRangePos &Pos);
	static CollLogElemType CalcCollType(const NToolCombined& Tool, int ApplyRet);
	int EGAGroove(NCadrGeom * Geom, int IndStart, int GSize, int GNumber, const NToolCombinedTurn *pTool, class CStocksCollection &Stocks, const BRangePos &Pos);
	int ApplyMeshs(MeshArr* pPlArray, MQuadBody& Body, int PCNum, cadrID CaID, bool IsFast);
	int ApplyMeshsPack(class MeshsArr& ChMeshArr, MQuadBody& Body, int PCNum, cadrID CaID, bool IsFast);
	int ApplyMeshsBSP(MeshArr *pPlArray, MQuadBody & Body, int PCNum, cadrID CaID, bool IsFast);
	int ApplyMeshsDX(MeshArr *pPlArray, MQuadBody & Body, int PCNum, cadrID CaID, bool IsFast);
	void ResetInterrupt();
	void SetRotMatr(const BMatr &T);
	void SetBodyRotMatr(const BMatr &T);
	void SetBodyRotStart(MBody *pBody);
	void SetMatr(const BMatr& T);
	void SetMatr(const class BMatrPair& T);
	bool ClearRotMatr(void);
	void ReSetRender(class CStocksCollection &Stocks, NCMApplicationType State);
	void Init(bool Pause);
	bool PrepNewAnimState(const NToolCombined *pTool, const NCadrGeom *pGeom, const BRangePos &Pos, int Start);
	bool PrepNewPosition(const BRangePos &Pos, int Start);
	void SetMT(NMachUnitPair* pMU);
	bool WaitAllCuttings(void);
	UINT CuttingThreadProc1Dir( LPVOID pParam );
	int InsSpecCadr(NGlobalState::ModelModes ModelMode, DX_TOOLTYPE DXType,
		const BPoint & P0v, const BPoint & P1v, const BPoint & Nv,
		double R, double H, DX_ID PairID, const BMatr &Shift, bool FirstCadr, bool LastCadr, const std::vector<BStockGrID>& Groups, const CadrData& MaskData);
	int InsSpecCadrLast();
	void StartCutting1DirThreads();
	void StopCutting1DirThreads();
	bool Wait4Empty1DirThreads();
	void RecalcPts(NCadrGeom *pGeom, class BPointsBuf &Pts, const BPoint &Base);
	void PostCollisions(CWnd* pMainWnd);
	void SetMCS(const BMatr& MCS);
	CadrData GetSpecMask(const NCadrGeom* pGeom) const;
};
