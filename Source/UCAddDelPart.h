#pragma once
#include "BUndoHistory.h"
#include "UCAddDelObj.h"

class UCAddDelPart : public UCAddDelObj
{
public:
    UCAddDelPart(class CNCMDoc* pDoc, const CString& name, int ind, class NStock* pStock, bool doAdd);
    UCAddDelPart(class CNCMDoc* pDoc, class NStock* pStock);// for delete only
    ~UCAddDelPart();
    bool Do() override;
    bool Undo() override;
};

