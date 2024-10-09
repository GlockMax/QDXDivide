#pragma once
#include "StdAfx.h"
#include "MACHINE_API.h"
#include <set>
#include "MTAxis.h"
#include "BMatr.h"
#include "..\Mashine\MTAxisCache3D.h"
#include "MTCoordinate.h"


/**
Кинематика станка моделируется деревом осей.
Дерево хранится в массиве.Каждый элемент содержит объект "ось" и уровень элемента в дереве.
Класс содержит словарь для поиска осей в массиве по имени, ключ - имя, значение - номер в массиве.
\sa MTMachine, MTMachineTool, MTAxis, AxisArrayNode
*/
class MACHINE_API MTConfig
{
	friend class MTMachineTool;
	friend class MTBase;
	class MoveStackNode : public CObject
	{
	public:
		MoveStackNode();
		~MoveStackNode();
		int level;
		BMatr MoveMatrix;
		const class MTMatrFieldAxis* pMatrField;
		bool Spinning;
	};

	class MoveStack : public CTypedPtrList<CObList, MoveStackNode*>
	{
	public:
		//Add element to top of stack
		void Push(MoveStackNode* newNode)
		{
			AddHead(newNode);
		}

		//Peek at top element of stack
		MoveStackNode* Peek()
		{
			return IsEmpty() ? nullptr : GetHead();
		}

		//Pop top element off stack
		MoveStackNode* Pop()
		{
			return RemoveHead();
		}
	};//class MoveStack : public CTypedPtrList <CObList, *MoveStackNode>

	class AxisArrayNode :public CObject
	{
	public:
		int level;
		MTAxis* Axis;
	};//struct SArrayNode

	typedef CTypedPtrArray<CObArray, AxisArrayNode*> AxisArray;
	typedef CMap<CString, LPCSTR, int, int> AxisM;
public:
	MTConfig();
	MTConfig(const MTConfig& MT);
	virtual ~MTConfig();

private:
	AxisM AxisMap;
protected:
	AxisArray Axis;
	bool CurPosIsActual;
	bool CompensationIsActual;
	bool ExprPresent;
	std::set<std::pair<int, int>> Enemies; ///< Полный список пар перемещательных осей, между которыми контролируются столкновения
	mutable MTAxisCache3D PlaceAxisCache;
	mutable MTAxisCache3D ToolAxisCache;
public:
	void Regularize();///< упорядочение массива.
	void AddAxis(MTAxis* Ax, int level = -1);   //Добавление оси в хвост массива
												//После добавления необходимо упорядочить - Regularize()
	MTAxis* operator [](int i) const;           //Обращение к Axis
	const MTConfig& operator = (const MTConfig& B);
	void RemoveAll();
	int GetLevel(int index);
	int GetSize() const;
	int GetAxisIndex(const CString& AxName) const;
	static void CopyAxisArray(const AxisArray& AxSrc, AxisArray& AxDst);   //Не имеет значения длина и структура массивов
	int MoveAxis(const CString& AxisName, double Param);  //двигает одну ось
	void Reset(); //Копирование NullAxis в Axis
	void ResetComp();
	int MoveToCurPos(const CooArray& Now, bool ForCopy = false);
	int GetBranchAmount(const CString& BranchRoot); //кол-во узлов в ветви
	//Edit functions
	void RemoveAxis(int index);//Delete axis, not childs
	void RemoveAxis(const CString& AxName);//Delete axis, not childs
	bool CreateParentAxis(MTAxis* Axis, const CString& AxisName);//create new non-terminal axis
	bool CreateChildAxis(MTAxis* Axis, const CString& AxisName);//create new terminal axis
	bool MoveBranch(const CString& BaseAxis, const CString& DestinationAxis);
	MTAxis* GetAxis(const CString& N) const;//Возвращает ось по имени
	MTAxis* GetAxis(int Ind) const;//Возвращает ось по индексу
	//Compensation related. used by MTEdit
	bool IsItAxisChild(int AxisIndex, const CString& Name);
	bool IsCompAxis(int AxisIndex);//Is it one of compensation axes?
	bool IsCompStock(int AxisIndex);// Does the stock have compensation?
	BPoint GetCompXVect(int AxisIndex);//Get stock_X
	BPoint GetCompYVect(int AxisIndex);//Get stock_Y
	BPoint GetCompZVect(int AxisIndex);//Get stock_Z
	bool SetCompXVect(const BPoint& vect, int AxisIndex);//Set stock_X
	bool SetCompYVect(const BPoint& vect, int AxisIndex);//Set stock_Y
	bool SetCompZVect(const BPoint& vect, int AxisIndex);//Set stock_Z
	void DeleteComp(int StockIndex);//Delete compensation axes for the stock
	void CreateComp(int StockIndex);//Create default compensation axes
	bool IsDescendant(const CString& BaseAxis, const CString& Axis);
	const CString CrInitStr(void) const;
	bool IsDefined(void) const;
	bool IsDescendant(const MTAxis& Parent, const MTAxis& Child) const;
	int FindAllChildren(int AxisNum, int* ChildrenNums);
	size_t FindAllDescendant(const MTAxis* pAxis, std::vector<MTAxis*>& DescAxes);
	void ReGenEnemiesPairs(const class MTCollisionArray& Collisions);//regenerate Enemies array 
	void GenEnemiesPairs(int MyNum, int MyChildren, int AxisNum, int AxisChildren);
	void Draw(BOOL flag, const CString& SelectedAxis);
	bool RemoveBranch(const CString& AxisName);
	bool RemoveBranch(int AxisIndex);
	bool SwapAxisNames(int First, int Second, const class MTCollisionArray& Collisions);
	bool MakeAutoEnemies();
	const MTAxis* GetNearestMoveRotAxis(const MTAxis* pAxis) const;
	int GetNearestMoveRotAxisInd(int AxisInd) const;
	bool IsAncestor(const MTAxis* pAncestor, MTAxis::AXIS_TYPE ChildType) const;
	void GetStockPos(std::set<int>& Pos) const;
	bool HaveExpressions() const { return ExprPresent; }
	static int GetPartID(int ind);
	static int GetAxisID(int ind);
	class MTPartAxis* GetToolPlaceAxis(int Num, int Tur, int Att) const;
	MTPartAxis* GetToolAxis(int Tur, int Att) const;
	static const CString GetToolPlaceAxisName(int Num, int Tur, int Att);
	static const CString GetToolAxisName(int Tur, int Att);

protected:
	void RegularizeInt(AxisArrayNode* temp, INT_PTR size, int level, const CString& Parent);  //вспомогательная рекурсивная функция
	bool PrepAxisExpression(MTAxis& CurAxis);
};
