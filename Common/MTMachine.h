// MTMachineTool.h: interface for the MTMachineTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "StdAfx.h"
#include "BBox.h"	
#include "MTPartArray.h"	
#include "mtcoordinate.h"
#include "PAbstractObject.h"
#include "MTBase.h"
#include "BStockGrID.h"

class MTPartAxis;

/// –одитель MTMachineTool, инкапсулирует список деталей станка и методы работы с ним.
/** 
∆елательно все методы и данные, относ€щиес€ к детал€м станка, хранить в этом классе.
\sa MTBase, MTMachineTool, MTPart, MTPartArray
*/
class AFX_EXT_CLASS MTMachine : public MTBase
{
protected:
	bool Visible;
	bool DrawAxis;
	bool DrawPartAxis;
	bool DrawPart;
	bool DrawSharpEdges;
	bool DrawSmoothSurf;
	MTPartArray Part;

public:
	MTPartAxis * GetToolAxis(int Tur, int Att) const;
	MTPartAxis * GetStockAxis(int StockPosition) const;
	MTPartAxis* GetStockMovedAxis(int StockPosition) const;
	static const CString GetStockMovedAxisName(int StPos);
public:
	static const CString GetStockAxisName(int StPos);
	static const int GetToolTurret(const CString& ToolName);
	static const int GetToolAttachement(const CString& ToolName);
	static const int GetToolPosition(const CString& ToolName);

	MTMachine();
	virtual ~MTMachine();
	virtual void RemoveAll();
	DECLARE_SERIAL(MTMachine)
	virtual void Serialize(CArchive &ar);
	const MTPartArray & GetParts(void) const;
	MTPartArray & GetParts(void);
	MTPart* GetPart(const CString& AxisName, int StartInd = 0) const;
	void RemovePart(int PartIndex);

	int GetPartIndex(int AxisIndex) const;
	int GetMaxSpecPtsSize() const;
	void SetDrawParams(	bool iDrawAxis = false, bool iDrawPartAxis = false, bool iDrawPart = true);

	/// ”становка подсвечиваемой при отображении оси
	/** ≈сли ничего не нужно подсвечивать, передать "" */
	void SelectAxis(const CString& AxisName);
	BPoint GetToolVector(int Tur, int Att) const;
	int GetStockAxisInd(int StPos) const;
	int GetStockMovedAxisInd(int StPos) const;
	int GetStockMovedAxisIndSGr(BStockGrID StGr) const;
	void FullCopyWOVis(const MTMachine& MT);
	void DelFullCopyWOVis();
	void ClearFullCopyWOVis();
};
