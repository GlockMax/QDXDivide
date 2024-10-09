// NCMVisualManager.cpp : implementation file
//

#include "stdafx.h"
#include "ResLocal.h"
#include "NCMVisualManager.h"

// NCMVisualManager
IMPLEMENT_DYNCREATE(NCMVisualManager, CMFCVisualManagerVS2005)

NCMVisualManager::NCMVisualManager()
{
}

NCMVisualManager::~NCMVisualManager()
{
}


// NCMVisualManager member functions
COLORREF NCMVisualManager::GetStatusBarPaneTextColor(CMFCStatusBar* pStatusBar, CMFCStatusBarPaneInfo* pPane)
{
	return CMFCVisualManagerVS2005::GetStatusBarPaneTextColor(pStatusBar, pPane);
}
void NCMVisualManager::OnDrawEditBorder(CDC* pDC, CRect rect, BOOL bDisabled, BOOL bIsHighlighted, CMFCToolBarEditBoxButton* pButton)
{
	if (!CMFCToolBarEditBoxButton::IsFlatMode())
	{
		CMFCVisualManager::OnDrawEditBorder(pDC, rect, bDisabled, bIsHighlighted, pButton);
		return;
	}

	if (bIsHighlighted)
	{
		pDC->Draw3dRect(&rect,  m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
	else
	{
		pDC->Draw3dRect(&rect,  RGB(255,255,255), m_clrMenuBorder);
	}

}

COLORREF NCMVisualManager::OnFillRibbonButton(CDC * pDC, CMFCRibbonButton * pButton)
{
	if (pButton->GetData() == 0)
	{
		if (pButton->IsHighlighted())
		{
			if (pButton->IsChecked())
				pDC->FillSolidRect(pButton->GetRect(), RGB(0x9A, 0xBB, 0xD4));
			else
				pDC->FillSolidRect(pButton->GetRect(), RGB(0xB2, 0xD8, 0xF4));
		}
		else if (pButton->IsChecked())
		{
			auto Color = RGB(0xD2, 0xE6, 0xF5);
			pDC->FillSolidRect(pButton->GetRect(), Color);
		}
	}
	else
	{
		switch (pButton->GetData())
		{
		case 1:// red
		{
			const auto Color = RGB(0xE6, 0x92, 0x92);
			pDC->FillSolidRect(pButton->GetRect(), Color);
		}
			break;
		case 2: // green
		{
			const auto Color = RGB(0x92, 0xE6, 0x92);
			pDC->FillSolidRect(pButton->GetRect(), Color);
		}
		break;
		case 3: // yellow
		{
			const auto Color = RGB(0xE6, 0xE6, 0x12);
			pDC->FillSolidRect(pButton->GetRect(), Color);
		}
		break;
		}
	}
	return (COLORREF)-1;
}

void NCMVisualManager::OnDrawHeaderCtrlBorder(CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
	CMFCVisualManager::OnDrawHeaderCtrlBorder(pCtrl, pDC, rect, bIsPressed, bIsHighlighted);
}

COLORREF  NCMVisualManager::OnDrawRibbonCategoryTab(CDC* pDC, CMFCRibbonTab* pTab, BOOL bIsActive)
{
	return CMFCVisualManager::OnDrawRibbonCategoryTab(pDC, pTab, bIsActive);
}

void NCMVisualManager::OnFillHeaderCtrlBackground(CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect rect)
{
}
// NCMVisualManager17
IMPLEMENT_DYNCREATE(NCMVisualManager17, CMFCVisualManagerOffice2007)

NCMVisualManager17::NCMVisualManager17()
{
	SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
}

NCMVisualManager17::~NCMVisualManager17()
{
}


