#include "stdafx.h"
#include "NCMDoc.h"
#include "NStock.h"
#include "NCUnit.h"
#include "UCTransMovObj.h"

UCTransMovObj::UCTransMovObj(CNCMDoc* pDoc, NMovableObj* pStock, const BMatr& M)
{
	pDoc_ = pDoc;
	pStock_ = pStock;
	Matr_ = M;
}

UCTransMovObj::~UCTransMovObj()
{
}

bool UCTransMovObj::Do()
{
	Operation(Matr_);
	return true;
}

bool UCTransMovObj::Undo()
{
	Operation(Matr_.invr());
	return true;
}

void UCTransMovObj::Clear(bool undo)
{
}

void UCTransMovObj::Operation(const BMatr& M)
{
	if (pStock_ == nullptr)
	{// move UCSs
		pDoc_->GetUnit()->RecalcAllUcss(M);
		pDoc_->GetUnit()->Reset();
		pDoc_->RegeneratePrograms();
		return;
	}

	NStock* pRealStock = dynamic_cast<NStock*>(pStock_);
	if (pRealStock != nullptr)
		if (pRealStock->IsRealStock())
			pDoc_->GetStocksList().ResetBodiesAndRendersLists();

	pStock_->Transformm(M);

	if (pRealStock != nullptr)
		if (pRealStock->IsRealStock())
			pDoc_->GetUndoHistory().SetStockChanged();
}

