#include "stdafx.h"
#include "NCMDoc.h"
#include "NStock.h"
#include "UCAddDelObj.h"

UCAddDelObj::UCAddDelObj(CNCMDoc* pDoc, const CString& name, int ind, NStock* pStock, bool doAdd)
{
	doAdd_ = doAdd;
	pDoc_ = pDoc;
	name_ = name;
	ind_ = ind;
	pStock_ = pStock;
}

UCAddDelObj::~UCAddDelObj()
{
}

void UCAddDelObj::Clear(bool undo)
{
	if (doAdd_ != undo)// delete command in undo stack or add command in redo stack
		delete pStock_;
}

UCAddDelObj::UCAddDelObj()
{
	doAdd_ = true;
	pDoc_ = nullptr;
	ind_ = -1;
	pStock_ = nullptr;
}
