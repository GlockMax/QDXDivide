#pragma once
#include "NCUnitDef.h"
#include "NCTreeCtrlSnapshot.h"

/// Объект для хранения в истории изменений
/// Используется для возврата к предыдущим состояниям по командам undo/redo
class CNCUnitSnapshot
{
public:
	CNCUnitSnapshot(void);
	~CNCUnitSnapshot(void);
	CNCTreeCtrlSnapshot treeSnaphsot;
	CNCUnitDef unitDef;
};
