#pragma once

#include "..\Source\GridCtrl\GridCell.h"
// NGridCellExp command target

class NGridCellExp : public CGridCell
{
	DECLARE_DYNCREATE(NGridCellExp)

public:
	NGridCellExp();

	void OnEndEdit() override;
	void  SetText(LPCTSTR szText) override;
	BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar) override;
protected:
	CString Expression;

};


