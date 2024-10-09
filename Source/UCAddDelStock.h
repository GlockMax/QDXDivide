#pragma once
#include "BUndoHistory.h"
#include "UCAddDelObj.h"

class UCAddDelStock : public UCAddDelObj
{
public:
    UCAddDelStock(class CNCMDoc* pDoc, const CString& name, int ind, class NStock* pStock, bool doAdd);
    UCAddDelStock(class CNCMDoc* pDoc, class NStock* pStock);// for delete only
    ~UCAddDelStock();
    bool Do() override;
    bool Undo() override;
};

