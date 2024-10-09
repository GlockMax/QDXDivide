
#pragma once

#include "NCUnitTreeCtrl.h"
#include "NCEditCtrl.h"
#include "gdiplus.h"
using namespace Gdiplus;

class CNCUnitHelpPane : public CDockablePane
{
// Construction
public:
	CNCUnitHelpPane();

	void AdjustLayout();
	void OnChangeVisualStyle();

// Attributes
protected:

	//CStatic m_Static;
	CNCEditCtrl m_Edit;
	//UINT height, width; ///< Размеры изображения, deprecated
	Bitmap *m_pBitmap;
	UINT imageHeight, imageWidth;

// Implementation
public:
	virtual ~CNCUnitHelpPane();

	void ShowHelp(CString text, CString PicName);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

};