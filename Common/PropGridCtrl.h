#pragma once
class PropEditor;
class PropSingle;

#include "afxpropertygridctrl.h"

class AFX_EXT_CLASS PropGridCtrl : public CMFCPropertyGridCtrl
{
public:
	PropGridCtrl();
	~PropGridCtrl();
	void SetPropListFont();
	void PrEdit(PropEditor *edit);
	void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	void SetEnabled(bool Enable);
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	PropEditor * propEditor;
protected:
	CFont m_Font;
	void ZoomText(int Dir);
};

class AFX_EXT_CLASS PropGridProp : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(PropGridProp)
	public:
		PropGridProp(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE) :
			CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList){};

		PropGridProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
			LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL) :
		CMFCPropertyGridProperty(strName,  varValue, lpszDescr , dwData, lpszEditMask , lpszEditTemplate,  lpszValidChars){};

		~PropGridProp();
};
