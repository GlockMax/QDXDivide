#include "stdafx.h"
#include "NCMDoc.h"
#include "NStock.h"
#include "UCAddDelFixture.h"

UCAddDelFixture::UCAddDelFixture(CNCMDoc* pDoc, const CString& name, int ind, NStock* pStock, bool doAdd) 
	: UCAddDelObj(pDoc, name, ind, pStock, doAdd)
{
}

UCAddDelFixture::UCAddDelFixture(CNCMDoc* pDoc, NStock* pStock)
{
	doAdd_ = false;
	pDoc_ = pDoc;
	pStock_ = pStock;
	ind_ = pDoc_->NGetExObjList()->GetIndex(static_cast<CObject*>(pStock));
	if (ind_ == -1)
		return;
	CObject* stock;
	pDoc_->NGetExObjList()->GetAt(ind_, name_, stock);
	name_ = CMapStringToNStock::PrefixFixt + name_;
}

UCAddDelFixture::~UCAddDelFixture()
{
}

bool UCAddDelFixture::Do()
{
	if (doAdd_)
		pDoc_->AddFixture(name_, ind_, pStock_); 
	else
	{
		CString key = pDoc_->NGetExObjList()->DeleteObjByInd(ind_);
		pDoc_->NGetPosChecks()->RemoveKey(CMapStringToNStock::PrefixFixt + key);
	}
	return true;
}

bool UCAddDelFixture::Undo()
{
	if (doAdd_)
	{
		CString key = pDoc_->NGetExObjList()->DeleteObjByInd(ind_);
		pDoc_->NGetPosChecks()->RemoveKey(CMapStringToNStock::PrefixFixt + key);
	}
	else
		pDoc_->AddFixture(name_, ind_, pStock_);
	return false;
}
