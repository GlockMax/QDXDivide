#pragma once

#include "NCMB_API.h"
#include <stack>

class UndoCommand
{
public:
	UndoCommand() noexcept { startGroup_ = true; }
	~UndoCommand() noexcept {};
	void MakeInternal() noexcept { startGroup_ = false; }

	virtual bool Do() = 0;
	virtual bool Undo() = 0;
	virtual void Clear(bool undo) = 0;
	bool IsStartGroup() const noexcept { return startGroup_; }
protected:
	bool startGroup_;
};
class NCMB_API BUndoHistory
{
public:
	BUndoHistory() noexcept;
	~BUndoHistory() noexcept;
	bool AddAndDo(UndoCommand* cmd);
	bool Do();
	bool Undo();
	bool DoGroup();
	bool UndoGroup();
	bool CanUndo() const noexcept;
	bool CanRedo() const noexcept;
	void Clear();
	bool StartGroup() noexcept;
	void EndGroup(bool internalGroup) noexcept;
	bool IsGroupStarted() const noexcept { return group_; }
	void ResetStockChanged() { stockChanged_ = false; }
	void SetStockChanged() { stockChanged_ = true; }
	bool IsStockChanged() const { return stockChanged_; }
protected:
	void ClearRedo();
	void ClearUndo();
protected:
	std::stack<UndoCommand*> undoStack_;
	std::stack<UndoCommand*> redoStack_;
	bool group_;
	bool groupFirst_;
	bool stockChanged_;
};

