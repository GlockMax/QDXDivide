#include "stdafx.h"
#include "NCMDoc.h"
#include "NStock.h"
#include "UCAddDelPart.h"

UCAddDelPart::UCAddDelPart(CNCMDoc* pDoc, const CString& name, int ind, NStock* pStock, bool doAdd) 
	: UCAddDelObj(pDoc, name, ind, pStock, doAdd)
{
}

UCAddDelPart::UCAddDelPart(CNCMDoc* pDoc, NStock* pStock)
{
	doAdd_ = false;
	pDoc_ = pDoc;
	pStock_ = pStock;
	ind_ = pDoc_->NGetPartList()->GetIndex(static_cast<CObject*>(pStock));
	if (ind_ == -1)
		return;
	CObject* stock;
	pDoc_->NGetPartList()->GetAt(ind_, name_, stock);
	name_ = CMapStringToNStock::PrefixPart + name_;
}

UCAddDelPart::~UCAddDelPart()
{
}

bool UCAddDelPart::Do()
{
	if (doAdd_)
		pDoc_->AddPart(name_, ind_, pStock_); 
	else
	{
		CString key = pDoc_->NGetPartList()->DeleteObjByInd(ind_);
		pDoc_->NGetPosChecks()->RemoveKey(CMapStringToNStock::PrefixPart + key);
	}
	return true;
}

bool UCAddDelPart::Undo()
{
	if (doAdd_)
	{
		CString key = pDoc_->NGetPartList()->DeleteObjByInd(ind_);
		pDoc_->NGetPosChecks()->RemoveKey(CMapStringToNStock::PrefixPart + key);
	}
	else
		pDoc_->AddPart(name_, ind_, pStock_);
	return false;
}
