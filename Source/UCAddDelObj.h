#pragma once
#include "BUndoHistory.h"

class UCAddDelObj : public UndoCommand
{
public:
    UCAddDelObj(class CNCMDoc* pDoc, const CString& name, int ind, class NStock* pStock, bool doAdd);
    virtual ~UCAddDelObj();
    void Clear(bool undo) override;
protected:
    UCAddDelObj();

protected:
    class CNCMDoc* pDoc_;
    CString name_;
    int ind_;
    class NStock* pStock_;
    bool doAdd_;
};

