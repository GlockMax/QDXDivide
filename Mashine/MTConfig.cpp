#include "stdafx.h"
#include <cmath>
#include <map>
#include <string>
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "ResLocal.h"
#include "tinyxml.h"
#include "BMatr.h"
#include "MTForwardAxis.h"
#include "SMachState.h"
#include "ctype.h"
#include "MNEvalExp.h"
#include "MTCollisionArray.h"
#include "MTMatrFieldAxis.h"
#include "MTStockAxis.h"
#include "MTConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MTConfig::MTConfig()
{
	CurPosIsActual = false;
	CompensationIsActual = false;
	ExprPresent = false;
}

MTConfig::MTConfig(const MTConfig& Conf)
{
	CurPosIsActual = false;
	CompensationIsActual = false;
	ExprPresent = false;
	CopyAxisArray(Conf.Axis, Axis);
	Regularize();
}

MTConfig::~MTConfig()
{
	RemoveAll();
}

int MarkedCount = 0;

void MTConfig::Regularize()
{
	INT_PTR size = Axis.GetSize();
	AxisArrayNode* temp = new AxisArrayNode[size];

	for (int i = 0; i < size; i++)
	{
		MTAxis& Ax = *(*this)[i];
		ExprPresent |= Ax.HaveExpr();

		temp[i].Axis = Axis[i]->Axis;
		temp[i].level = -1;
		delete Axis[i];
	}

	Axis.RemoveAll();
	AxisMap.RemoveAll();

	MarkedCount = 0;
	for (int i = 0; MarkedCount < size && i < size; i++)
	{

		RegularizeInt(temp, size, 0, CString(""));
		for (int i = 0; i < size; i++)
		{
			if (temp[i].level != -100)
			{
				temp[i].Axis->SetParent(CString(""));// if parent does not exist -> set parent to "" and repeat regularization
				break;
			}
		}
	}

	delete[] temp;

	//Теперь создаём словарь по упорядоченному массиву Axis
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		AxisMap.SetAt(Axis[i]->Axis->GetName(), i);
	}
	// propogate spin status and make vectorZ
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		MTAxis* pCurAx = Axis[i]->Axis;
		const MTAxis* pParent = GetAxis(pCurAx->GetParent());
		pCurAx->PropParentSpinStatus(pParent);
		pCurAx->MakeVectorZ();
		pCurAx->MakeVectorZOr();
	}
	PlaceAxisCache.Reset();
	ToolAxisCache.Reset();
}


void MTConfig::RegularizeInt(AxisArrayNode* temp, INT_PTR size, int level, const CString& Parent)
{
	for (int i = 0; i < size; i++)
	{
		if (temp[i].level == -100)
			continue;
		if (temp[i].Axis->GetParent() == Parent)
		{
			AddAxis(temp[i].Axis, level);       // передаем указатель в Axis
			temp[i].level = -100;
			++MarkedCount;
			RegularizeInt(temp, size, level + 1, temp[i].Axis->GetName());
		}
	}
}

bool MTConfig::PrepAxisExpression(MTAxis& CurAxis)
{
	CString Expr = CurAxis.GetExpr();
	// for all axis names
	std::vector<std::string> varNames;
	std::vector<int> varIndexes;
	for (int ind = Expr.Find('{'); ind > -1; ind = Expr.Find('{', ind))
	{
		Expr.SetAt(ind, 'A');
		++ind;
		const int EndInd = Expr.Find('}', ind);
		if (EndInd < 0)
			continue;// error
		Expr.SetAt(EndInd, '_');
		const int AxInd = GetAxisIndex(Expr.Mid(ind, EndInd - ind));// pure name
		if (AxInd < 0)
			continue; // error
		varNames.push_back(std::string(Expr.Mid(ind - 1, EndInd - ind + 2)));// decorated name
		varIndexes.push_back(AxInd);
	}
	return CurAxis.CompileExpr(std::string(Expr), varNames, varIndexes);
}

void MTConfig::AddAxis(MTAxis* Ax, int level)
{
	AxisArrayNode* t = new AxisArrayNode;
	t->Axis = Ax;
	t->level = level;
	Axis.Add(t);
}

MTAxis* MTConfig::operator [](int i) const
{
	return Axis[i]->Axis;
}

const MTConfig& MTConfig::operator=(const MTConfig& B)
{
	CopyAxisArray(B.Axis, Axis);
	Regularize();
	Enemies.clear();
	CurPosIsActual = false;
	CompensationIsActual = false;
	return *this;
}

void MTConfig::RemoveAll()
{
	//При выполнении этой функции Axis и AxisMap могут иметь разные длины

	//Удаляем Axis.
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		delete Axis[i]->Axis;
		delete Axis[i];
	}//for
	Axis.RemoveAll();

	//Удаляем AxisMap
	AxisMap.RemoveAll();

}

int MTConfig::GetLevel(int index)
{
	return Axis[index]->level;
}

int MTConfig::GetSize() const
{
	return int(Axis.GetSize());
}

int MTConfig::GetAxisIndex(const CString& AxName) const
{
	int a;
	if (AxisMap.Lookup(AxName, a)) //nonzero if element was found
		return a;
	return -1;
}

void MTConfig::Reset()
{
	CurPosIsActual = false;
	CompensationIsActual = false;
	for (int i = 0; i < Axis.GetSize(); i++)
		(*this)[i]->Reset();
}

void MTConfig::ResetComp()
{
	if (CompensationIsActual)
		CurPosIsActual = false;
	CompensationIsActual = false;
}

//В случае, если одна из осей, или несколько осей выходят за свои пределы значений,
//функция возвращает номер одной из таких осей, в противном случае -1
int MTConfig::MoveToCurPos(const CooArray& Now, bool ForCopy /*= false*/)
{
	CurPosIsActual = true;
	double* values = new double[Axis.GetSize()];
	bool* slave = new bool[Axis.GetSize()];
	bool slavesExist = false;
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		values[i] = 0;
		MTAxis& Ax = *(*this)[i];
		Ax.SetMovement(0);
		if (Ax.HaveExpr())
		{
			slave[i] = true;
			slavesExist = true;
		}
		else
			slave[i] = false;
	}

	for (int i = 0; i < Now.GetSize(); i++)
	{
		int ind = GetAxisIndex(Now[i].AxisName);
		if (ind >= 0)
		{
			values[ind] = Now[i].Value;
			slave[ind] = false;
		}
	}

	// Apply expressions
	bool oneIsChanged = true;
	while (slavesExist && oneIsChanged)
	{
		oneIsChanged = false;// cycling prevention
		slavesExist = false;
		for (int i = 0; i < Axis.GetSize(); ++i)
		{
			if (!slave[i])
				continue;
			MTAxis& CurAxis = *(*this)[i];
			if (!CurAxis.HaveExpr())
				continue;
			if (!CurAxis.IsExprReady())
				PrepAxisExpression(CurAxis);
			std::vector<double> VarValues;
			// for all axis indexes
			const auto Indexes = CurAxis.GetExprIndexes();
			for each(auto AxInd in Indexes)
			{
				if (AxInd < 0)
					break; // error
				if (slave[AxInd])
				{
					slavesExist = true;
					break; // can not calculate expr yet
				}
				MTRelocateAxis* pAx = dynamic_cast<MTRelocateAxis*>((*this)[AxInd]);
				if (pAx == nullptr)
					break; // error
				double Val = values[AxInd];
				if (std::isnan(Val) || std::isinf(Val))
					Val = 0.;
				VarValues.push_back(Val);
			}
			if (VarValues.size() == Indexes.size())
			{// all values founded successfully 
				slave[i] = false;
				oneIsChanged = true;
				// Calculate expr
				values[i] = CurAxis.CalcExprValue(VarValues);
			}
		}
	}
	delete[] slave;
	// END:Apply expressions
	int Result = -1;
	MoveStack MStack;
	const MoveStackNode* SNode;
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		while (((SNode = MStack.Peek()) != nullptr) && SNode->level >= GetLevel(i))
			delete MStack.Pop();

		SNode = MStack.Peek();
		MTAxis* pCurAxis = (*this)[i];
		if (SNode != nullptr)
		{
			if (SNode->pMatrField != nullptr)
			{
				BMatr FieldMatr;
				SNode->pMatrField->CreateFieldMatr(FieldMatr, pCurAxis);
				pCurAxis->GoTo(SNode->MoveMatrix * FieldMatr);
			}
			else
			{
				pCurAxis->GoTo(SNode->MoveMatrix);
			}
			pCurAxis->SetSpinning(SNode->Spinning);
		}
		else
			pCurAxis->Reset();

		if (values[i] != 0 || (*this)[i]->GetIsCS())
		{
			BMatr MoveMatrix;
			if (!pCurAxis->CreateMoveMatrix(MoveMatrix, values[i]))
				Result = i;
			MTMatrFieldAxis* pMatrField = dynamic_cast<MTMatrFieldAxis*>(pCurAxis);
			MoveStackNode* NewSNode = new MoveStackNode;
			if (pMatrField != nullptr)
				NewSNode->pMatrField = pMatrField;
			if (SNode != nullptr && !(*this)[i]->GetIsCS())
				NewSNode->MoveMatrix = SNode->MoveMatrix * MoveMatrix;
			else
			{
				if (ForCopy && SNode != nullptr)
					NewSNode->MoveMatrix.SetE();
				else
					NewSNode->MoveMatrix = MoveMatrix;
			}
			NewSNode->Spinning = pCurAxis->IsSpinning();
			NewSNode->level = GetLevel(i);
			MStack.Push(NewSNode);
		}//if
	}//for

	//Очистка стека
	while (MStack.Peek() != nullptr)
		delete MStack.Pop();

	delete[] values;

	return Result;
}

int MTConfig::GetBranchAmount(const CString& BranchRoot)
{
	int res = 0;
	int index = GetAxisIndex(BranchRoot);
	if (index == -1)
		return -1;

	int level = Axis[index]->level;

	for (int i = index + 1; i < GetSize(); i++)
	{
		if (Axis[i]->level <= level)
			break;
		res = i;
	}//for
	return res - index; // кол-во = индекс последнего элемента в ветви  -  индекс корня
}//int MTBase::GetBranchAmount(CString BranchRoot)

void MTConfig::RemoveAxis(int index)
{
	//1 - Переименовываем родителя у непосредственных потомков
	for (int i = index + 1; (i < Axis.GetSize()) && (GetLevel(i) > GetLevel(index)); i++)
	{
		if ((*this)[i]->GetParent() == (*this)[index]->GetName())
			(*this)[i]->SetParent((*this)[index]->GetParent());
	}//for

	//2 - Удаляем ось

	if (GetSize() == 0 || index >= GetSize())
		return;
	delete Axis[index]->Axis;
	Axis.RemoveAt(index);
	Regularize();
}

void MTConfig::RemoveAxis(const CString& AxName)
{
	int index = GetAxisIndex(AxName);
	if (index >= 0)
		RemoveAxis(index);
}

void MTConfig::CopyAxisArray(const AxisArray& AxSrc, AxisArray& AxDst)
{
	//Освобождение второго массива
	INT_PTR size = AxDst.GetSize();
	for (int i = 0; i < size; i++)
	{
		delete AxDst[i]->Axis;     //потенциально опасное место
		delete AxDst[i];
	}//for

	size = AxSrc.GetSize();
	AxDst.SetSize(size);
	for (int i = 0; i < size; i++)
	{
		AxisArrayNode* AxN = new AxisArrayNode;
		AxN->level = AxSrc[i]->level;
		AxN->Axis = AxSrc[i]->Axis->Clone();
		AxDst[i] = AxN;
	}
}

//В случае если ось выходит за свои пределы, функция возвращает номер оси,
//в противном случае -1
int MTConfig::MoveAxis(const CString& AxisName, double Param)
{
	int Result = -1;
	const int k = GetAxisIndex(AxisName);
	if (k < 0)
		return -1;

	MTAxis* pMainAxis = (*this)[k];
	BMatr MoveMatrix;
	if (!pMainAxis->CreateMoveMatrix(MoveMatrix, Param))
		Result = k;

	MTMatrFieldAxis* pMatrField = dynamic_cast<MTMatrFieldAxis*>(pMainAxis);
	const int level = GetLevel(k);
	for (int i = k + 1; i < GetSize(); ++i)
	{
		if (Axis[i]->level <= level)
			break;
		MTAxis* pCurAxis = (*this)[i];
		if (pMatrField != nullptr)
			pMatrField->CreateFieldMatr(MoveMatrix, pCurAxis);
		pCurAxis->Move(MoveMatrix);
	}// For

	return Result;
}//int MTBase::MoveAxis(CString AxisName, double Param)

bool MTConfig::CreateParentAxis(MTAxis* Axis, const CString& AxisName)
{
	MTAxis* CurrAxis = GetAxis(AxisName);
	Axis->SetParent(CurrAxis->GetParent());
	CurrAxis->SetParent(Axis->GetName());
	AddAxis(Axis);
	Regularize();
	return TRUE;
}//bool MTBase::CreateParentAxis(CString ChildName, CString &NewAxisName)

bool MTConfig::CreateChildAxis(MTAxis* Axis, const CString& AxisName)
{
	Axis->SetParent(AxisName);
	AddAxis(Axis);
	Regularize();

	return TRUE;
}//bool MTBase::CreateChildAxis(MTAxis *Axis, CString AxisName)

bool MTConfig::MoveBranch(const CString& BaseAxis, const CString& DestinationAxis)
{
#ifdef _DEBUG
	//	DebugPrint(CString(_T("MTBase::MoveBranch...")));
#endif
	int index = GetAxisIndex(BaseAxis);
	if (index < 0)
		return false;

	if ((*this)[index]->GetType() == MTAxis::AT_STOCK) //stock, если есть компенсация перевешиваем stock_X
		if (IsCompStock(index))
		{
			int Vectx = GetAxisIndex(CString("stock_X"));
			index = Vectx;
			ASSERT(index != -1);
		}//if if

	(*this)[index]->SetParent(DestinationAxis);

	Regularize();
#ifdef _DEBUG
	//DebugPrint(CString(_T("MTBase::  Branch moved")));
#endif

	return true;
}//bool MTBase::MoveBranch(CString BaseAxis, CString DestinationAxis)

MTAxis* MTConfig::GetAxis(const CString& N) const//Возвращает ось по имени
{
	int i = GetAxisIndex(N);
	return GetAxis(i);
}

MTAxis* MTConfig::GetAxis(int Ind) const
{
	if (Ind < 0)
		return nullptr;
	MTAxis* pAxis = (*this)[Ind];
	return pAxis;
}

bool MTConfig::IsItAxisChild(int AxisIndex, const CString& Name)
{
	for (int i = AxisIndex + 1; i < Axis.GetSize(); i++)
	{
		if ((*this)[i]->GetParent() == Name)
			return TRUE;
	}//for
	return FALSE;
}//bool MTBase::IsItAxisChild(int AxisIndex, CString &Name)

bool MTConfig::IsCompAxis(int AxisIndex)
{
	CString ax1, ax2, ax3;

	if (AxisIndex < 0 || AxisIndex >= GetSize())
		return false;

	ax1 = (*this)[AxisIndex]->GetName();

	if (ax1 == "stock_X")
	{
		if (IsItAxisChild(AxisIndex, "stock_Y"))
		{
			int index = GetAxisIndex("stock_Y");
			if (IsItAxisChild(index, "stock_Z"))
				return true;
			else
				return false;
		}//if
		else
			return false;
	}//if
	else if (ax1 == "stock_Y")
	{
		int ind = GetAxisIndex(ax1);
		if (ind < 0)
			return false;
		ax2 = (*this)[ind]->GetParent();
		if (ax2 == "stock_X")
		{
			if (IsItAxisChild(AxisIndex, "stock_Z"))
				return true;
			else
				return false;
		}//if
		else
			return false;
	}//if
	else if (ax1 == "stock_Z")
	{
		int ind = GetAxisIndex(ax1);
		if (ind < 0)
			return false;
		ax2 = (*this)[ind]->GetParent();
		if (ax2 == "stock_Y")
		{
			int ind = GetAxisIndex(ax2);
			if (ind < 0)
				return false;
			ax3 = (*this)[GetAxisIndex(ax2)]->GetParent();
			if (ax3 == "stock_X")
				return true;
			else
				return false;
		}//if
		else
			return false;
	}//if
	else
		return false;
}//bool MTBase::IsCompAxis(int AxisIndex)

bool MTConfig::IsCompStock(int AxisIndex)
{
	if (GetLevel(AxisIndex) < 3)  //Если заготовка находится ниже чем на 4 уровне, она не может иметь 3 родителей
		return false;

	int ParentIndex = GetAxisIndex((*this)[AxisIndex]->GetParent());

	return IsCompAxis(ParentIndex);
}//bool MTBase::IsCompStock(int AxisIndex)

BPoint MTConfig::GetCompXVect(int AxisIndex)
{
	int index = GetAxisIndex("stock_X");
	if (index >= 0)
		return (*this)[index]->GetVectorOr();
	else
		return BPoint(0., 0., 0., 0);
}//BPoint MTBase::GetCompXVect(int AxisIndex)

BPoint MTConfig::GetCompYVect(int AxisIndex)
{
	int index = GetAxisIndex("stock_Y");
	if (index >= 0)
		return (*this)[index]->GetVectorOr();
	else
		return BPoint(0., 0., 0., 0);
}//BPoint MTBase::GetCompYVect(int AxisIndex)

BPoint MTConfig::GetCompZVect(int AxisIndex)
{
	int index = GetAxisIndex("stock_Z");
	if (index >= 0)
		return (*this)[index]->GetVectorOr();
	else
		return BPoint(0., 0., 0., 0);
}//BPoint MTBase::GetCompZVect(int AxisIndex)

bool MTConfig::SetCompXVect(const BPoint& vect, int AxisIndex)
{
	int index = GetAxisIndex("stock_X");
	if (index < 0)
		return FALSE;

	(*this)[index]->InitVector(vect);

	return TRUE;
}//void MTBase::SetCompXVect(BPoint &vect)

bool MTConfig::SetCompYVect(const BPoint& vect, int AxisIndex)
{
	int index = GetAxisIndex("stock_Y");
	if (index < 0)
		return FALSE;

	(*this)[index]->InitVector(vect);

	return TRUE;
}//bool MTBase::SetCompYVect(BPoint &vect, int AxisIndex)

bool MTConfig::SetCompZVect(const BPoint& vect, int AxisIndex)
{
	int index = GetAxisIndex("stock_Z");
	if (index < 0)
		return FALSE;

	(*this)[index]->InitVector(vect);

	return TRUE;
}//bool MTBase::SetCompZVect(BPoint &vect, int AxisIndex)

void MTConfig::DeleteComp(int StockIndex)
{
	RemoveAxis(CString("stock_X"));
	RemoveAxis(CString("stock_Y"));
	RemoveAxis(CString("stock_Z"));
}//void MTBase::DeleteComp(int StockIndex)

void MTConfig::CreateComp(int StockIndex)
{
	MTAxis* stock_X, * stock_Y, * stock_Z;
	stock_X = new MTForwardAxis();
	stock_X->SetMin(0.);
	stock_X->SetMax(0.);
	stock_Y = new MTForwardAxis();
	stock_Y->SetMin(0.);
	stock_Y->SetMax(0.);
	stock_Z = new MTForwardAxis();
	stock_Z->SetMin(0.);
	stock_Z->SetMax(0.);

	stock_X->SetName(CString("stock_X"));
	stock_Y->SetName(CString("stock_Y"));
	stock_Z->SetName(CString("stock_Z"));

	stock_X->InitVector(BPoint(1., 0., 0., 0.));
	stock_Y->InitVector(BPoint(0., 1., 0., 0.));
	stock_Z->InitVector(BPoint(0., 0., 1., 0.));

	CString Name = (*this)[StockIndex]->GetName();

	CreateParentAxis(stock_X, Name);
	CreateParentAxis(stock_Y, Name);
	CreateParentAxis(stock_Z, Name);

}//void MTBase::CreateComp(int StockIndex)

bool MTConfig::IsDescendant(const CString& BaseAxis, const CString& Axis)
{
	int ind1, ind2, level1;
	ind1 = GetAxisIndex(BaseAxis);
	ind2 = GetAxisIndex(Axis);
	if (ind1 < 0 || ind2 < 0)
		return false;
	level1 = GetLevel(ind1);
	if (ind2 < ind1)
		return FALSE;

	for (int i = ind1 + 1; i <= ind2; i++)
	{
		if (GetLevel(i) <= level1)
			return FALSE;
	}//for

	return TRUE;
}//BOOL MTBase::IsDescendant(CString BaseAxis, CString Axis)

const CString MTConfig::CrInitStr(void) const
{
	CString InitStr;
	for (int i = 0; i < GetSize(); ++i)
	{
		const auto pAxis = (*this)[i];
		auto Type = pAxis->GetType();
		if (Type == MTAxis::AT_FOR || Type == MTAxis::AT_ROT || Type == MTAxis::AT_SCHAIN)
			if (!pAxis->HaveExpr())
				InitStr = InitStr + "\n" + pAxis->GetName();
	}
	InitStr.Delete(0, 1);
	return InitStr;
}

bool MTConfig::IsDefined(void) const
{
	return (Axis.GetSize() > 0);
}

bool MTConfig::IsDescendant(const MTAxis& Parent, const MTAxis& Child) const
{
	const MTAxis* pAxis = &Child;
	CString PaName = Parent.GetName();
	while (pAxis->GetParent() != PaName)
	{
		int i = GetAxisIndex(pAxis->GetParent());
		if (i < 0)
			return false;
		pAxis = Axis[i]->Axis;
	}
	return true;
}

int MTConfig::FindAllChildren(int AxisNum, int* ChildrenNums)
{
	MTAxis& Ax = *((*this)[AxisNum]);
	int Ind = 0;
	for (int i = 0; i < Axis.GetSize(); ++i)
	{
		if (IsDescendant(Ax, *((*this)[i])))
			ChildrenNums[Ind++] = i;
	}
	return Ind;
}

size_t MTConfig::FindAllDescendant(const MTAxis* pAxis, std::vector<MTAxis*>& DescAxes)
{
	for (int i = 0; i < Axis.GetSize(); ++i)
	{
		MTAxis* pCurAxis = ((*this)[i]);
		if (IsDescendant(*pAxis, *pCurAxis))
			DescAxes.push_back(pCurAxis);
	}
	return DescAxes.size();
}

void MTConfig::ReGenEnemiesPairs(const MTCollisionArray& Collisions)
{
	Enemies.clear();
	for (int i = 0; i < Collisions.GetSize(); i++)
	{
		const int ax1 = GetAxisIndex(Collisions[i]->Axis1);
		const int ax2 = GetAxisIndex(Collisions[i]->Axis2);
		if (ax1 < 0 || ax2 < 0)
			continue;
		GenEnemiesPairs(ax1, Collisions[i]->Axis1Ch, ax2, Collisions[i]->Axis2Ch);
	}//for
}//void MTCollisionArray::RegenEnemiesForMT(MTMachineTool *MT)

void MTConfig::GenEnemiesPairs(int MyNum, int MyChildren, int AxisNum, int AxisChildren)
{
	auto& ConfAxis = Axis;
	// Find all nums
	int MyChildrenNum = 0;
	int* MyChildrenArr = new int[MyChildren ? ConfAxis.GetSize() : 1];
	if (MyChildren)
		MyChildrenNum = FindAllChildren(MyNum, MyChildrenArr);
	MyChildrenArr[MyChildrenNum++] = MyNum;

	int AxisChildrenNum = 0;
	int* AxisChildrenArr = new int[AxisChildren ? ConfAxis.GetSize() : 1];
	if (AxisChildren)
		AxisChildrenNum = FindAllChildren(AxisNum, AxisChildrenArr);
	AxisChildrenArr[AxisChildrenNum++] = AxisNum;
	// Make all pairs
	for (int i = 0; i < MyChildrenNum; ++i)
	{
		const MTAxis* pAxisi = (*this)[MyChildrenArr[i]];
		if (dynamic_cast<const MTRelocateAxis*>(pAxisi) != nullptr)
			continue;
		for (int j = 0; j < AxisChildrenNum; ++j)
		{
			const MTAxis* pAxisj = (*this)[AxisChildrenArr[j]];
			if (dynamic_cast<const MTRelocateAxis*>(pAxisj) != nullptr)
				continue;
			Enemies.insert(std::pair<int, int>(MyChildrenArr[i], AxisChildrenArr[j]));
		}
	}
	delete[] MyChildrenArr;
	delete[] AxisChildrenArr;
}

void MTConfig::Draw(BOOL flag, const CString& SelectedAxis)
{
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		auto& Ax = *(*this)[i];
		bool selected = (SelectedAxis == Ax.GetName());
		auto Type = Ax.GetType();
		if (Type == MTAxis::AT_PART || Type == MTAxis::AT_TOOL || Type == MTAxis::AT_VTOOL || Type == MTAxis::AT_STOCK)
		{
			if (flag)
			{
				glPushName(GetAxisID(i));
				Ax.Draw(selected);
				glPopName();
			}
		}//if
		else
		{
			glPushName(GetAxisID(i));
			Ax.Draw(selected);
			glPopName();
		}//else
	}//for
}//void MTBase::Draw(BOOL flag)

int MTConfig::GetPartID(int ind)
{
	return 2 * ind + 1;
}

int MTConfig::GetAxisID(int ind)
{
	return 2 * ind;
}

MTPartAxis* MTConfig::GetToolPlaceAxis(int Num, int Tur, int Att) const
{
	bool needNewValue = false;
	void* pA = PlaceAxisCache.Get(Att, Tur, Num, needNewValue);
	if (needNewValue)
	{
		pA = nullptr;
		int i = GetAxisIndex(GetToolPlaceAxisName(Num, Tur, Att));
		if (i >= 0)
		{
			MTAxis* pAxis = (*this)[i];
			if (pAxis->IsKindOf(RUNTIME_CLASS(MTPartAxis)))
				pA = pAxis;
		}
		PlaceAxisCache.Set(Att, Tur, Num, pA);
	}
	return (MTPartAxis*)pA;
}

MTPartAxis* MTConfig::GetToolAxis(int Tur, int Att) const
{
	bool needNewValue = false;
	void* pA = ToolAxisCache.Get(0, Att, Tur, needNewValue);
	if (needNewValue)
	{
		pA = nullptr;
		int i = GetAxisIndex(GetToolAxisName(Tur, Att));
		if (i >= 0)
		{
			MTAxis* pAxis = (*this)[i];
			if (pAxis->IsKindOf(RUNTIME_CLASS(MTPartAxis)))
				pA = pAxis;
		}
		ToolAxisCache.Set(0, Att, Tur, pA);
	}
	return (MTPartAxis*)pA;
}


bool MTConfig::RemoveBranch(const CString& AxisName)
{
	int index = GetAxisIndex(AxisName);
	if (index < 0)
		return false;
	return RemoveBranch(index);
}

bool MTConfig::RemoveBranch(int AxisIndex)
{
	int level = Axis[AxisIndex]->level;

	//delete all childs
	for (int i = AxisIndex + 1; i < Axis.GetSize(); )
	{
		if (Axis[i]->level <= level)
			break;
		delete Axis[i]->Axis;
		delete Axis[i];
		Axis.RemoveAt(i);
	}//for

	//delete branch root
	delete Axis[AxisIndex]->Axis;
	delete Axis[AxisIndex];
	Axis.RemoveAt(AxisIndex);

	Regularize();
	return true;
}

bool MTConfig::SwapAxisNames(int First, int Second, const MTCollisionArray& Collisions)
{
	// it is assumed First and Second are right indexes
	//1 - переименовываем родителя у непосредственных потомков
	const CString& FirstName = Axis[First]->Axis->GetName();
	const CString& SecondName = Axis[Second]->Axis->GetName();
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		const CString& Parent = Axis[i]->Axis->GetParent();
		if (Parent == FirstName)
			Axis[i]->Axis->SetParent(SecondName);
		else if (Parent == SecondName) // else is necessary (prevent double replacing)
			Axis[i]->Axis->SetParent(FirstName);
	}

	//2 - переименовываем саму ось
	CString b = Axis[First]->Axis->GetName();
	Axis[First]->Axis->SetName(Axis[Second]->Axis->GetName());
	Axis[Second]->Axis->SetName(b);
	Regularize();
	ReGenEnemiesPairs(Collisions);
	return true;
}

bool MTConfig::MakeAutoEnemies()
{
	std::map<int, std::vector<int>> STLGroups; // index of relocate axis, indexes of the axes from it's group
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		const MTAxis* pAxis = Axis[i]->Axis;
		if (pAxis == nullptr)
			continue;
		if (dynamic_cast<const MTRelocateAxis*>(pAxis) != nullptr)
			continue;

		const int RelInd = GetNearestMoveRotAxisInd(i);
		STLGroups[RelInd].push_back(i);
	}
	std::vector<const std::vector<int>*> Ptrs;
	Ptrs.reserve(STLGroups.size());
	for (auto it = STLGroups.begin(); it != STLGroups.end(); ++it)
		Ptrs.push_back(&it->second);
	for (int k = 0; k < Ptrs.size(); ++k)
		for (int l = k + 1; l < Ptrs.size(); ++l)
			for each (auto n1 in *Ptrs[k])
				for each (auto n2 in *Ptrs[l])
					Enemies.insert(std::pair(n1, n2));
	return false;
}

MTConfig::MoveStackNode::MoveStackNode()
{
	pMatrField = nullptr;
	Spinning = false;
}

MTConfig::MoveStackNode::~MoveStackNode()
{
}

const MTAxis* MTConfig::GetNearestMoveRotAxis(const MTAxis* pAxisInp) const
{
	const MTAxis* pAxis = pAxisInp;
	if (pAxis == nullptr)
		return nullptr;
	pAxis = GetAxis(pAxis->GetParent());
	if (pAxis == nullptr)
		return nullptr;
	while (dynamic_cast<const MTRelocateAxis*>(pAxis) == nullptr)
	{
		pAxis = GetAxis(pAxis->GetParent());
		if (pAxis == nullptr)
			return nullptr;
	}
	return pAxis;
}

int MTConfig::GetNearestMoveRotAxisInd(int AxisInd) const
{
	const MTAxis* pAxis = GetNearestMoveRotAxis(operator[](AxisInd));
	if (pAxis != nullptr)
		return GetAxisIndex(pAxis->GetName());
	return -1;
}

bool MTConfig::IsAncestor(const MTAxis* pAncestor, MTAxis::AXIS_TYPE ChildType) const
{
	if (pAncestor == nullptr)
		return false;
	for (int i = 0; i < Axis.GetSize(); ++i)
	{
		const auto& TermAx = *Axis[i]->Axis;
		if (TermAx.GetType() == ChildType)
		{
			if (IsDescendant(*pAncestor, TermAx))
				return true;
		}
	}
	return false;
}

void MTConfig::GetStockPos(std::set<int>& Pos) const
{
	for (int i = 0; i < Axis.GetSize(); i++)
	{
		if (Axis[i]->Axis->GetType() == MTAxis::AT_STOCK)
		{
			const MTStockAxis& StockAxis = static_cast<const MTStockAxis&>(*Axis[i]->Axis);
			Pos.insert(StockAxis.GetPos());
		}
	}
}

const CString MTConfig::GetToolPlaceAxisName(int Num, int Tur, int Att)
{
	if (Tur == 0 && Att == 0) // speedup only
	{
		switch (Num)
		{
		case 0:
			return _T("0000tool");
		case 1:
			return _T("0001tool");
		case 2:
			return _T("0002tool");
		case 3:
			return _T("0003tool");
		case 4:
			return _T("0004tool");
		case 5:
			return _T("0005tool");
		case 6:
			return _T("0006tool");
		case 7:
			return _T("0007tool");
		case 8:
			return _T("0008tool");
		case 9:
			return _T("0009tool");
		}
	}
	CString PlaceAxisName;
	PlaceAxisName.Format("%04u", Num);
	return PlaceAxisName + GetToolAxisName(Tur, Att);
}

const CString MTConfig::GetToolAxisName(int Tur, int Att)
{
	// Tur - number of a turret
	// Att - number of an attachment
	// Axis name constructs like this:
	// if Tur and Att are equal to 0 -> tool
	// if Tur is not equal to 0 ->tool<Tur>
	// if Att is not equal to 0 -><prev result>:<Att>
	if (Tur < 0 || Att < 0)
		return CString("");

	CString key("tool");
	if (Tur > 0)
	{
		CString n;
		n.Format("%d", Tur);
		key += n;
	}
	if (Att > 0)
	{
		CString n;
		n.Format(":%d", Att);
		key += n;
	}
	return key;
}


