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
	// вспомогательна€ информаци€
	int			m_CadrNum;		// номер кадра
	enum ChainType CurChainType;
	bool IsArcExtLeft;// (только дл€ дуги Small)€вл€етс€ ли внешний по отношению к дуге контур левым
	bool IsExtLeft;// €вл€етс€ ли внешний по отношению к повороту в конце кадра контур левым
	NLine CurLine;
	bool		m_IsLastCadr;	// последний кадр или нет
	bool		m_IsFirstCadr;	// первый кадр в цепочке

protected:
	double		m_Alf;
	double		m_BAlf;
	double		m_GAlf;
	double		m_endAlf;
	double		m_endBAlf;
	double		m_endGAlf;
	// —ќ’–јЌя≈ћјя »Ќ‘ќ–ћј÷»я
	bool m_ChainEndBis;
	BPoint		m_BPoint;		// начальна€ точка кадра
	BPoint		m_CurDir;		// вектор направлени€ движени€ текущего кадра
	BPoint		m_NextDir;		// вектор направлени€ движени€ следующего кадра
	bool		m_IsTurnCW;		// в какую сторону происходит поворот в текущем кадре
	bool		m_IsMovInVertPl;// движение на двух кадрах по одной линии


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
