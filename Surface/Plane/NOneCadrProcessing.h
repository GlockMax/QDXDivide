// NOneCadrProcessing.h: interface for the NOneCadrProcessing class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "MBSPTree.h"
#include "NContour.h"
#include "NLine.h"
#include "MFace.h"
#include "NPlaneProcessing.h"
#include "NHorPlaneProcessing.h"
#include "NCylinderProsessing.h"
#include "NOneCadrOneTool.h"
#include "NMeshArray.h"
#include "NSortedArray.h"

class MBSPNode;
class NOneCadrOneTool;

class NOneCadrProcessing  
{
	friend class NOnePartProcessing;
	friend class NPlaneProcessing;
	friend class NHorPlaneProcessing;
	friend class NCylinderProcessing;
	friend class NOneCadrOneTool;

public:
	bool SetInitConts();
	bool CreateWConts();
	void CrFaces4Trim(MeshsArr& PlaneArray, bool StartFace);
	void BuildAddTurnMesh(MeshsArr& PlaneArray);
	void MakeFullTool(MeshsArr& PlaneArray);
	bool TrimStartCont(const BPoint& C, const BPoint& N);
	void CrEndFaces(MeshsArr &PlaneArray);
	void CrBaseFaces(MeshsArr &PlaneArray, bool StartFace);
	void SetToolsContour(int iTool, NContour *pToolsContour, bool IsCutting, bool IsShank, const NColor& ToolColor, bool IsConcave);
	void SetDataConcave(int iTool, double CRad);
	void BuildSurf2d(const NLine &Line, BPoint &nextDir, MeshsArr &MeshArray, bool ChainEndBis);
	void BuildSurf3d(const NLine &bLine, BPoint &nextDir, MeshsArr &MeshArray, bool ChainEndBis);
	void BuildSurfVert(const NLine &Line, MeshsArr &MeshArray);
	void BuildSurf2dArc(const NLine& Line, BPoint& nextDir, MeshsArr& MeshArray, bool ChainEndBis, bool LastArcPart);
	void BuildSurf2dArcNew(const NLine& Line, BPoint& nextDir, MeshsArr& MeshArray);
	int DefBaseAngles();
	NOneCadrProcessing();
	virtual ~NOneCadrProcessing();

public:
	// ��������������� ����������
	int			m_CadrNum;		// ����� �����
	enum ChainType CurChainType;
	bool IsArcExtLeft;// (������ ��� ���� Small)�������� �� ������� �� ��������� � ���� ������ �����
	bool IsExtLeft;// �������� �� ������� �� ��������� � �������� � ����� ����� ������ �����
	NLine CurLine;
	bool		m_IsLastCadr;	// ��������� ���� ��� ���
	bool		m_IsFirstCadr;	// ������ ���� � �������

protected:
	double		m_Alf;
	double		m_BAlf;
	double		m_GAlf;
	double		m_endAlf;
	double		m_endBAlf;
	double		m_endGAlf;
	// ����������� ����������
	bool m_ChainEndBis;
	BPoint		m_BPoint;		// ��������� ����� �����
	BPoint		m_CurDir;		// ������ ����������� �������� �������� �����
	BPoint		m_NextDir;		// ������ ����������� �������� ���������� �����
	bool		m_IsTurnCW;		// � ����� ������� ���������� ������� � ������� �����
	bool		m_IsMovInVertPl;// �������� �� ���� ������ �� ����� �����


	int ToolsNum;
	CArray <NOneCadrOneTool, NOneCadrOneTool &> ToolSpecArr;
	MPlane		m_P2, m_P4;
	
protected:
	void SetBasePlanes2d();
	static BOOL ComputeAngles(const NLine &Line, TypeLineInContour type,
					  const BPoint &dir, 
					  double &alf, double &beta,
					  double &maxR, bool &is_top);
	static BOOL ComputeAngles(NLine &Line, TypeLineInContour type,
					  BPoint &dir,
					  double &alf, double &beta);
	void SwapConts();
public:
	void SetInitData2d(const BPoint &bPoint, BPoint &curDir, BPoint &nextDir);
	bool IsLast(void);
	bool CompareSECont(void);
	bool CheckMesh(NRectMesh *pMesh);
	void SetToolsNum(int num);
	int GetToolsNum() const { return ToolsNum; }
	void FixIMStart(const MeshsArr &MeshsArray);
	void FixIMEnd(const MeshsArr &MeshsArray);
	void TrimStartFaces(MeshsArr& MeshsArray, const BPoint& C, const BPoint& N);
	void TrimAllFaces(MeshsArr& MeshsArray, const BPoint& C, const BPoint& N);
	void ApproxContour();
	void RestoreContour();
};
