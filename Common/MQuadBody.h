#pragma once
#include "mbody.h"
class AFX_EXT_CLASS MQuadBody :
	public MBody
{
	friend class GQuadRender;
	friend class GQuadRenderMill;
	friend class GQuadRenderDX5X;
	friend class GQuadRenderRot;
	friend class MQuadTreeRot;
	friend class MQuadTreeSliced;
	friend class QMeasure;
protected:
	class RayMarch2D
	{
	public:
		RayMarch2D(const BBox& Gabar, int ColsNum, int RowsNum, const BPoint& P, const BPoint& V);
		~RayMarch2D() {}
		bool GetNextCell(int& i, int& j);
		const BPoint GetP() const;
		const BPoint& GetV() const;
		const BPoint& GetFirstN() const;
	protected:
		BPoint Step;
		BPoint Start;
		double Size[3];
		int MaxInd[3];
		int CurInd[3];
		BPoint CurP;
		BPoint FirstN;
		BPoint CurV;
		BPoint StV;
	};

public:
	class Window
	{
		friend class MQuadTree;
		friend class MQuadTreeRot;
		friend class MQuadTreeSliced;
		friend class DXDirection3X;
		friend class DXDirection5X;
	public:
		Window(void){ Clear();}
		~Window(void){}
		void Set(int xS, int xE, int yS, int yE);
		void Set(int xS, int xE, int yS, int yE, int zS, int zE);
		void Expand(int x, int y);
		void Expand(int x, int y, int z);
		void Clear() { Defined = false;}
		void SetDefined(bool Val) { Defined = Val; }
		void ShiftUpLimits(int Shift) { ixEnd += Shift; iyEnd += Shift; izEnd += Shift; }
		bool IsDefined() const { return Defined; }
		int ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd;
	protected:
		bool Defined;

	};
public:
	MQuadBody( const BPoint & MinPoint, const BPoint & MaxPoint, int RowsNum, int ColsNum, BStockGrID Position);
	void ReCreate( const BPoint & MinPoint, const BPoint & MaxPoint, int RowsNum, int ColsNum);
	virtual void ResetpModifiedCells(void);
	int GetRowsNum(void) const;
	int GetColsNum(void) const;
	void GetCellIndexes(double x, double y, int & ix, int & iy);
	const class MBaseMask &GetModifiedCells(void) const;
	class MBaseMask &GetModifiedCells(void);
	virtual bool IsChanged() const;
	virtual ~MQuadBody(void);
	const Window& GetWindow(void) const { return Win; }
	void ClearWindow() { Win.Clear(); }
	virtual void Save(class MForestSave &SaveData) override;
	virtual void Load(CFile *f) override;

#pragma region LOOP_THROUGH_ALL_BODIES
public:
	virtual void ClearModified(void);
//	void SetWindow(int xS, int xE, int yS, int yE);
	void RemoveWindow();
	virtual void SetWindow(const BBox& Bounds);
	virtual bool CheckCh(void) const = 0;
#pragma endregion LOOP_THROUGH_ALL_BODIES
	void CreateFullCopy(MBody* pQ) override;


protected:
	MQuadBody(BStockGrID Position);
protected:
	int RowsNum;
	int ColsNum;
	Window Win;
	class MBaseMask *pModifiedCells;
	double Xstep;
	double Ystep;
};

