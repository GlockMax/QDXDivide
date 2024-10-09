/*!
\file MTEditSliderButton.h
\brief Классы для ползунка в панели инструментов.
Взято из примеров Visual Studio MFC Feature Pack, пример называется Slider
*/

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CCustomSliderCtrl window

class MTEditSliderButton;

class MTEditSliderCtrl : public CSliderCtrl
{
// Construction
public:
	MTEditSliderCtrl(MTEditSliderButton& btnSlider);

// Attributes
public:
	MTEditSliderButton& m_btnSlider;

// Operations
public:
// Overrides

// Implementation
public:
	virtual ~MTEditSliderCtrl();

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
class MTEditSliderButton : public CMFCToolBarButton  
{
	DECLARE_SERIAL(MTEditSliderButton)

public:
	MTEditSliderButton();
	MTEditSliderButton (UINT uiId,
			int iImage = -1,
			DWORD dwStyle = 0,
			int iWidth = 0);

	virtual ~MTEditSliderButton();

	void SetRange (int iMin, int iMax);
	void SetValue (int iValue, BOOL bNotify = TRUE);

	static int GetPos (UINT uiCmd);
	static void SetPos (UINT uiCmd, int nPos);

// Overrides:
	virtual void Serialize(CArchive& ar);
	virtual void CopyFrom (const CMFCToolBarButton& src);
	virtual SIZE OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz);
	virtual void OnShow (BOOL bShow);
	virtual void OnChangeParentWnd (CWnd* pWndParent);
	virtual void OnMove ();
	virtual void OnSize (int iSize);
	virtual HWND GetHwnd ()
	{	
		return m_wndSlider.GetSafeHwnd ();
	}

	virtual BOOL CanBeStretched () const
	{	
		return TRUE;	
	}

	virtual BOOL HaveHotBorder () const
	{
		return FALSE;
	}

	virtual void OnDraw (CDC* /*pDC*/, const CRect& /*rect*/, CMFCToolBarImages* /*pImages*/,
						BOOL /*bHorz*/ = TRUE, BOOL /*bCustomizeMode*/ = FALSE,
						BOOL /*bHighlight*/ = FALSE,
						BOOL /*bDrawBorder*/ = TRUE,
						BOOL /*bGrayDisabledButtons*/ = TRUE) {}
// Attributes:
public:
	MTEditSliderCtrl	m_wndSlider;
	int				m_iWidth;
	DWORD			m_dwStyle;
	int				m_nMin;
	int				m_nMax;
	int				m_nValue;
};
