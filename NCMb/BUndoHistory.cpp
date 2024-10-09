#include "stdafx.h"
#include "BUndoHistory.h"

BUndoHistory::BUndoHistory() noexcept
{
	EndGroup(false);
	stockChanged_ = false;
}

BUndoHistory::~BUndoHistory() noexcept
{
	Clear();
}

bool BUndoHistory::AddAndDo(UndoCommand* cmd)
{
	if (cmd == nullptr)
		return false;
	ClearRedo();
	if (group_)
	{
		if (groupFirst_)
			groupFirst_ = false;
		else
			cmd->MakeInternal();
	}
	redoStack_.push(cmd);
	return Do();
}

bool BUndoHistory::Do()
{
	if (redoStack_.empty())
		return false;
	undoStack_.push(redoStack_.top());
	redoStack_.pop();
	return undoStack_.top()->Do();
}

bool BUndoHistory::Undo()
{
	if (undoStack_.empty())
		return false;
	redoStack_.push(undoStack_.top());
	undoStack_.pop();
	return redoStack_.top()->Undo();
}

bool BUndoHistory::DoGroup()
{
	if (redoStack_.empty())
		return false;
	Do();
	while (!redoStack_.empty())
	{
		if (redoStack_.top()->IsStartGroup())
			break;
		Do();
	}
	return true;
}

bool BUndoHistory::UndoGroup()
{
	if (undoStack_.empty())
		return false;
	while (!undoStack_.empty())
	{
		Undo();
		if (redoStack_.top()->IsStartGroup())
			break;
	}
	return true;
}

bool BUndoHistory::CanUndo() const noexcept
{
	return !undoStack_.empty();
}

bool BUndoHistory::CanRedo() const noexcept
{
	return !redoStack_.empty();
}

void BUndoHistory::Clear()
{
	ClearUndo();
	ClearRedo();
}

bool BUndoHistory::StartGroup() noexcept
{
	// returns true if group is started already
	if (group_)
		return true;
	group_ = true;
	groupFirst_ = true;
	return false;
}

void BUndoHistory::EndGroup(bool internalGroup) noexcept
{
	if (internalGroup)
		return;
	group_ = false;
	groupFirst_ = false;;
}

void BUndoHistory::ClearRedo()
{
	while (!redoStack_.empty())
	{
		redoStack_.top()->Clear(false);
		delete redoStack_.top();
		redoStack_.pop();
	}
}

void BUndoHistory::ClearUndo()
{
	while (!undoStack_.empty())
	{
		undoStack_.top()->Clear(true);
		delete undoStack_.top();
		undoStack_.pop();
	}
}

