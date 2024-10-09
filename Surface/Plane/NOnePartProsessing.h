#pragma once
#include "NLine.h"
#include "MPlane.h"
#include "SOrigSurf.h"
#include "NMeshArray.h"

class NRectMesh;
class NOneCadrProcessing;
class NOneCadrOneTool;

class NOnePartProcessing
{
	friend class NCylinderProcessing;
protected:
	NOnePartProcessing(void);
	NOnePartProcessing(NLine &Line, NOneCadrProcessing *pCadrInfo, NOneCadrOneTool *pCadrInfoC);
public:
	virtual ~NOnePartProcessing(void); 

	virtual void BuildMesh2d(MeshArr &MeshArray, bool Ext) = 0;
	virtual void BuildMesh3d(MeshArr &MeshArray, bool Ext, const NOnePartProcessing *pPrevPart) = 0;
	virtual void BuildMesh3dBound(MeshArr& MeshArray, bool Ext, const NOnePartProcessing* pPrevPart, const NOnePartProcessing* pSimmPart) const = 0;
	virtual MeshArr* BuildSolid3d(const NOnePartProcessing* pThisPart, bool Ext, const NOnePartProcessing* pPrevPart, const NOnePartProcessing* pSimmPart) const = 0;
	virtual void BuildMesh2dArc(MeshArr &MeshArray, bool Ext, bool PrevIsNearSmall = false) = 0;
	virtual void SetInitCont(bool LastPart) = 0;
	virtual void SetAngles(void) = 0;
	virtual int Cr2dRotSurf(const NLine & Line, const BPoint &Center, double sign) const = 0;
	void MakeFullTool(MeshArr &MeshArray, const NOnePartProcessing* pIntCont);
	NRectMesh* CrDriveMesh();
	static int RotatePoint(int PointInd, const BMatr& m);
	NRectMesh* Create2dAngleMesh();
	virtual NRectMesh* CreateAddTurnMesh() = 0;
	void TrimStartCont(const BPoint& C, const BPoint& N, const NOnePartProcessing* pNextPart);
	void ClearTrimed();
	void BuildMeshFullRot(MeshArr &MeshArray, int AppNum);
	void SetLine(NLine &Line, NOneCadrProcessing *pCadrInfo, NOneCadrOneTool *pCadrInfoC);
	NLine &GetLine();
	void GetContByIntersecPlane(CUIntArray &arr, int St, int En, MPlane &PL1);
	void Copy10();

	const CUIntArray &GetArray(int i) const;
	int AppendTrimedArray(int i, CUIntArray& buf) const;
	static int GetNumAproxLine(const NLine &Line);
	double GetMinAngle();
	double GetMaxAngle();
	double GetTopAngle();
	double GetBotAngle();
protected:
	NRectMesh* CreateParMesh(CUIntArray &arr1, CUIntArray &arr2, bool is_pl);
	NRectMesh* CreateParMeshByMove(CUIntArray &arr, const BPoint &dir, SSurfType Type);
	NRectMesh* CreateParMeshTrans(CUIntArray &arr, const BPoint &dir);
	NRectMesh* CreateMeshByBase(bool First, int Start, int End) const;
	NRectMesh* CreateMeshByBase(bool First, int Start, int End, int PointsNum) const;
	NRectMesh* CreateParMeshByRot(CUIntArray &arr, int St, int En,
								   const BPoint &c, const BPoint &t, 
								   const double &alf, int num);
	NRectMesh* CreateRotMesh(CUIntArray& arr, int St, int En,
		const BPoint& c,
		double alf, int num);
	NRectMesh* CreateFullRotMesh1(const CUIntArray& Start, const CUIntArray& Mid, const BPoint& c);
	NRectMesh* CreateMeshFromArc(bool LastPart, const NOnePartProcessing *pPrevPart);
	NRectMesh* CreateHorCircle(BPoint Center, double Rad, double Sign) const;
	NRectMesh* CreateHorRing(BPoint Center, double Rad0, double R1, double Sign) const;
	double CalcDelta(bool Ext);

protected:
	NOneCadrProcessing	*m_pCInfo;
	NOneCadrOneTool		*m_pCInfoC;
	NLine				m_Line;
	CUIntArray		m_arr[3];
						// 1 - начальный контур
						// 2 - конечный контур
						// 3 - контур "уголка"
	CUIntArray	m_Addarr[2];// Дополнительные контуры для обработки негладких сочленений в контуре инструмента
						// 1 - начальный контур
						// 2 - конечный контур
	double TopAngle; // верхний и нижний углы для линии очерка
	double BotAngle;
	bool IsTrimed;
	bool StartTrimed;
	int TrimPointForInd;// Index of a trim point in the forest; -1 - empty; -2 - the segment coincides with the trim plane
	int TrimPointLocInd;// Index of the trim point line in m_arr
public:
	bool IsLeft(void) const;
	int GetFirstPN() const { return m_arr[0][0];}
};
