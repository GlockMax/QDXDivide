// MTAxis.h: interface for the MTAxis class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "StdAfx.h"	
#include <vector>
#include <string>
#include "NColor.h"	
#include "BMatr.h"	

/// Базовый класс для осей
class AFX_EXT_CLASS MTAxis: public CObject
{
public:
	enum AXIS_TYPE
	{
		AT_EMPTY = 0,
		AT_ROT = 2,
		AT_FOR = 1,
		AT_PART = 3,
		AT_TOOL = 4,
		AT_VTOOL = 5,
		AT_STOCK = 6,
		AT_SCHAIN = 7,
		AT_SPIN = 8
	};
public:
	MTAxis();
	MTAxis(const MTAxis &Ax);
	const MTAxis& operator= (const MTAxis& Ax);
	virtual MTAxis* Clone() const = 0;
	virtual ~MTAxis();

	void SetVector(const BPoint & vector);
	virtual void InitVector(const BPoint & vector);
	void InitBasePoint(const BPoint &);
	void InitBasePoint(double x, double y, double z);

	const BPoint GetVector() const;
	const BPoint GetVectorOr() const;

	const BPoint GetBasePoint() const;
	const BPoint GetBasePointOr() const;

	void SetVectorY(const BPoint & p);
	void InitVectorY(const BPoint & p);
	const BPoint & GetLocalDisplVec() const;
	void SetLocalDisplVec(const BPoint& p);
	
	const BPoint GetVectorY() const;
	const BPoint GetVectorYOr() const;

	const BPoint GetVectorZ() const;
	const BPoint GetVectorZOr() const;
	void MakeVectorZ();
	void MakeVectorZOr();

	virtual void Reset();

	void SetColor(COLORREF Col);
	void SetColor(NColor NewC);
	NColor GetColor() const;
	
	virtual AXIS_TYPE GetType() const;

	virtual void Draw(bool selected = false);

	virtual BOOL CreateMoveMatrix(BMatr& MoveMatrix, double param);
	/// Перемещение относительно текущих параметров
	void Move(const BMatr &MoveMatrix);
	/// Перемещение относительно начальных параметров
	void GoTo(const BMatr &MoveMatrix);
	
	const CString & GetParent() const;
	void SetParent(const CString& NewParent);
	const CString & GetName() const;
	void SetName(const CString& NewName);

	double GetMin() const;
	double GetMax() const;
	void SetMin(double min);
	void SetMax(double max);
	const BMatr& GetMatr() const;
	const BMatr& GetMatrOr() const;

	double GetMovement() const;
	void SetMovement(double move);
	bool HaveExpr() const;
	const CString &GetExpr() const;
	void SetExpr(const CString &NewExpr);

	bool GetIsCS() const { return IsCS; }
	void SetIsCS(bool Val) { IsCS = Val; }
	bool IsExprReady() const { return pExpr != nullptr; }
	const std::vector<int>& GetExprIndexes() const;
	double CalcExprValue(const std::vector<double>& VarValues);

protected:
	BMatr XYZB;
	BMatr XYZBOr;
	BPoint LocalDisplVec;

	NColor Color;
	CString Parent;
	CString Name;
	bool Marked;
	bool IsCS;// Is coordinate system

	/// Пределы перемещения
	double MoveMin;
	/// Пределы перемещения
	double MoveMax;

	double Movement;
	double Length;
	bool Visible;

	bool CanMoveWhileSpin;
	bool CanBeSpinned;
	bool Spinning;

	CString Expression;
	class MTExpression* pExpr;
	
public:
	bool GetCanBeSpinned() { return CanBeSpinned;}
	bool GetCanMoveWhileSpin() const noexcept { return CanMoveWhileSpin; }
	double GetLength() const noexcept { return Length; }
	bool GetVisible() const noexcept { return Visible; }
	void SetLength(double val) noexcept { Length = val; }
	void SetVisible(bool val) noexcept { Visible = val; }
	bool IsMarked(void) const;
	void SetMarked(bool Mark);
	void PropParentSpinStatus(const MTAxis* pParent);
	void SetSpinning(bool Val) noexcept { Spinning = Val; }
	bool IsSpinning() const noexcept { return Spinning; }
	virtual BOOL CreateMoveMatrixSt(BMatr &MoveMatrix, const BMatr &M, double param) const;//!!!
	virtual BOOL CreateDerivMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param,  double derivparam) const;//!!!
	virtual BOOL CreateInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param) const;//!!!
	virtual BOOL CreateDerivInvrMoveMatrix(BMatr &MoveMatrix, const BMatr &M, double param, double derivparam) const;//!!!
	/// Отображение стрелки
	void DrawArrow(const BPoint &Dir, NColor & HeadColor);
	/// Отображение габаритного параллелепипеда стрелки
	void DrawGabar(const BPoint &Dir);
	bool CompileExpr(const std::string& exprText, const std::vector<std::string>& varNames, const std::vector<int>& varIndexes);
	virtual int GetTurret(){return -1;}
	virtual int GetAttachment(){return -1;}
	virtual int GetPosition(){return -1;}
	virtual void SetTurret(int turret){return ;}
	virtual void SetAttachment(int attachment){return ;}
	virtual void SetPosition(int position){return ;}

};
