#pragma once


// NLineWidthCtrl

class AFX_EXT_CLASS NLineWidthCtrl : public CComboBoxEx
{
	DECLARE_DYNAMIC(NLineWidthCtrl)

public:
	NLineWidthCtrl();
	virtual ~NLineWidthCtrl(); 

protected:
	CImageList IList;
	DECLARE_MESSAGE_MAP()
public:
	void Init();
};


