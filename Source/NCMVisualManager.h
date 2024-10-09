#pragma once

// NCMVisualManager command target

class NCMVisualManager : public CMFCVisualManagerVS2005
{
	DECLARE_DYNCREATE(NCMVisualManager)
public:
	NCMVisualManager();
	virtual ~NCMVisualManager();
	virtual COLORREF GetStatusBarPaneTextColor(CMFCStatusBar* pStatusBar, CMFCStatusBarPaneInfo* pPane);
	virtual COLORREF  OnDrawRibbonCategoryTab(CDC* pDC, CMFCRibbonTab* pTab, BOOL bIsActive);
	void OnFillHeaderCtrlBackground(CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect rect) override;
	virtual void OnDrawEditBorder(CDC* pDC, CRect rect, BOOL bDisabled, BOOL bIsHighlighted, CMFCToolBarEditBoxButton* pButton);
	COLORREF OnFillRibbonButton(CDC* pDC, CMFCRibbonButton* pButton) override;
	void OnDrawHeaderCtrlBorder(CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted) override;
};


class NCMVisualManager17 : public CMFCVisualManagerOffice2007
{
	DECLARE_DYNCREATE(NCMVisualManager17)
public:
	NCMVisualManager17();
	virtual ~NCMVisualManager17();
};


