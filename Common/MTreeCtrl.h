#pragma once
#include "UNITS_API.h"
#include <afxcmn.h>

class UNITS_API MTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(MTreeCtrl)
	DECLARE_MESSAGE_MAP()
public:
	void SetSettings();
protected:
	CFont m_Font;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void ZoomText(int Dir);
protected:
	virtual CString GetBackName() const = 0;
	virtual CString GetFontName() const = 0;
	virtual CString GetColorName() const = 0;
};

