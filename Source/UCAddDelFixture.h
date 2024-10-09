#pragma once
#include "BUndoHistory.h"
#include "UCAddDelObj.h"

class UCAddDelFixture : public UCAddDelObj
{
public:
    UCAddDelFixture(class CNCMDoc* pDoc, const CString& name, int ind, class NStock* pStock, bool doAdd);
    UCAddDelFixture(class CNCMDoc* pDoc, class NStock* pStock);// for delete only
    ~UCAddDelFixture();
    bool Do() override;
    bool Undo() override;
};

