#pragma once
#include "NCUnitDef.h"
#include "NCTreeCtrlSnapshot.h"

/// ������ ��� �������� � ������� ���������
/// ������������ ��� �������� � ���������� ���������� �� �������� undo/redo
class CNCUnitSnapshot
{
public:
	CNCUnitSnapshot(void);
	~CNCUnitSnapshot(void);
	CNCTreeCtrlSnapshot treeSnaphsot;
	CNCUnitDef unitDef;
};
