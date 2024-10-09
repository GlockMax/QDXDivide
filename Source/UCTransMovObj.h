#pragma once
#include "BUndoHistory.h"

class UCTransMovObj : public UndoCommand
{
public:
    UCTransMovObj(class CNCMDoc* pDoc, class NMovableObj* pStock, const BMatr& M);
    virtual ~UCTransMovObj();
    bool Do() override;
    bool Undo() override;
    void Clear(bool undo) override;
protected:
    void Operation(const BMatr& M);

protected:
    class CNCMDoc* pDoc_;
    class NMovableObj* pStock_;
    BMatr Matr_;
};

