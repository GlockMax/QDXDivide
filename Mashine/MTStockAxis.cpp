#include "stdafx.h"
#include "MTStockAxis.h"

MTStockAxis::MTStockAxis()
{
	LocalDisplVec = BPoint(0.,0.,0.,0.);
}

MTStockAxis::~MTStockAxis()
{
}

MTStockAxis* MTStockAxis::Clone() const
{
	return new MTStockAxis(*this);
}

int MTStockAxis::GetPos() const
{
	const CString& Name = GetName();
	auto Ind = Name.Find("stock_moved");
	if (Ind == 0)
	{
		CString Pos = Name.Right(Name.GetLength() - 11);// 11 is a length of "stock_moved"
		return atoi(Pos);
	}
	Ind = Name.Find("stock");
	if (Ind == 0)
	{
		CString Pos = Name.Right(Name.GetLength() - 5);// 5 is a length of "stock"
		return atoi(Pos);
	}
	return 0;
}

MTAxis::AXIS_TYPE MTStockAxis::GetType() const
{
	return AT_STOCK;
}