#include "stdafx.h"
#include "NCMDoc.h"
#include "NStock.h"
#include "UCAddDelStock.h"

UCAddDelStock::UCAddDelStock(CNCMDoc* pDoc, const CString& name, int ind, NStock* pStock, bool doAdd) 
	: UCAddDelObj(pDoc, name, ind, pStock, doAdd)
{
}

UCAddDelStock::UCAddDelStock(CNCMDoc* pDoc, NStock* pStock)
{
	doAdd_ = false;
	pDoc_ = pDoc;
	pStock_ = pStock;
	ind_ = pDoc_->NGetStockList()->GetIndex(static_cast<CObject*>(pStock));
	if (ind_ == -1)
		return;
	CObject* stock;
	pDoc_->NGetStockList()->GetAt(ind_, name_, stock);
	name_ = CMapStringToNStock::PrefixStock + name_;
}

UCAddDelStock::~UCAddDelStock()
{
}

bool UCAddDelStock::Do()
{
	if (doAdd_)
		pDoc_->NGetStockList()->AddStock(name_, ind_, pStock_); 
	else
	{
		CString key = pDoc_->NGetStockList()->DeleteStockByInd(ind_);
		pDoc_->NGetPosChecks()->RemoveKey(CMapStringToNStock::PrefixStock + key);
	}
	return true;
}

bool UCAddDelStock::Undo()
{
	if (doAdd_)
	{
		CString key = pDoc_->NGetStockList()->DeleteStockByInd(ind_);
		pDoc_->NGetPosChecks()->RemoveKey(CMapStringToNStock::PrefixStock + key);
	}
	else
		pDoc_->NGetStockList()->AddStock(name_, ind_, pStock_);
	return false;
}
