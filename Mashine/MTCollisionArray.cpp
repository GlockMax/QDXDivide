#include "stdafx.h"
#include "MTConfig.h"
#include "MTMashineTool.h"
#include "MTCollisionArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(MTCollisionArray, CObject, 0)

const SCollision& SCollision::operator = (const SCollision &C)
{
	//1 Проверка на самоприсваивание
	if(&C == this) return *this;

	//2 Удаление старых значений

	//3 Копирование значений
	Axis1 = C.Axis1;
	Axis1Ch = C.Axis1Ch;
	Axis2 = C.Axis2;
	Axis2Ch = C.Axis2Ch;

	//4 Возвращение указателя *this
	return *this;
}//const SCollision& SCollision::operator = (const SCollision &C)


MTCollisionArray::~MTCollisionArray()
{
	RemoveAll();
}//MTCollisionArray::~MTCollisionArray()

INT_PTR MTCollisionArray::GetSize() const
{
	return Collisions.GetSize();
}//int MTCollisionArray::GetSize()

void MTCollisionArray::RemoveAll()
{
	INT_PTR size = GetSize();
	for(int i = 0; i < size; i++)
		delete Collisions[i];
	Collisions.RemoveAll();
}//void MTCollisionArray::RemoveAll()

void MTCollisionArray::AddCol(SCollision &Col)
{
	SCollision *t = new SCollision();
	*t = Col;
	Collisions.Add(t);
}//void MTCollisionArray::AddCol(SCollision &Col)

void MTCollisionArray::AddCol(MTBase *MT, SCollision &Col)
{
	//1 Добавляем объект столкновения
	SCollision *t = new SCollision();
	*t = Col;
	Collisions.Add(t);
	//2 Добавляем в массив Enemies станка
	int ax1 = MT->GetCurConfig().GetAxisIndex(Col.Axis1);
	int ax2 = MT->GetCurConfig().GetAxisIndex(Col.Axis2);
	if (ax1 < 0 || ax2 < 0)
		return;
	MT->GetCurConfig().GenEnemiesPairs(ax1,Col.Axis1Ch, ax2, Col.Axis2Ch);
}//void MTCollisioinArray::AddCol(MTMachineTool *MT, SCollision &Col)

int MTCollisionArray::GetAxisColNum(const CString& AxisName)
{
	int result = 0;
	for(int i = 0; i < GetSize(); i++)
	{
		if(Collisions[i]->Axis1 == AxisName)
			result++;
	}//for
	return result;
}//int MTCollisionArray::GetAxisColNum(CString AxisName)

SCollision* MTCollisionArray::GetAxisCol(const CString& AxisName, int index)
{
	//index - какой по счёту объект столкновения для данной оси брать
	if(index <= 0)
		return NULL;
	int next = 1; //какой по счёту объект мы ищем в данный момент

	if(GetAxisColNum(AxisName) < index)
		return NULL;

	for(int i = 0; i < GetSize(); i++)
	{
		if(Collisions[i]->Axis1 == AxisName)
		{
			if(index == next)
				return Collisions[i];
			else
				next++;//нужен не этот, ищем следующий
		}//if
	}//for

	return NULL;
}//SCollision* MTCollisionArray::GetAxisCol(CString AxisName, int index)

int MTCollisionArray::GetColIndex(const CString& Axis1, const CString& Axis2)
{
	for(int i = 0; i < GetSize(); i++)
	{
		if(Collisions[i]->Axis1 == Axis1  &&  Collisions[i]->Axis2 == Axis2 )
			return i;
	}//for

	return -1;
}//int MTCollisionArray::GetColIndex(CString Axis1, CString Axis2)

SCollision* MTCollisionArray::operator [] (int index) const
{
	if(index < 0 || index >= GetSize())
		return NULL;

	return Collisions[index];
}//SCollision* MTCollisionArray::operator [] (int index)

void MTCollisionArray::RenameAxis(const CString& OldName, const CString& NewName)
{
	for(int i = 0; i < GetSize(); i++)
	{
		if(Collisions[i]->Axis1 == OldName)
			Collisions[i]->Axis1 = NewName;
		if(Collisions[i]->Axis2 == OldName)
			Collisions[i]->Axis2 = NewName;
	}//for
}//void MTCollisionArray::RenameAxis(CString OldName, CString NewName)

void MTCollisionArray::RemoveCol(int ColIndex)
{
	delete Collisions[ColIndex];
	Collisions.RemoveAt(ColIndex);
}//void MTCollisionArray::RemoveCol(int ColIndex)