#pragma once
#include <afxwin.h>
#include "ResLocal.h"
class NRotationContour : public CDialog
{
public:
	NRotationContour(CWnd* pParent);   // standard constructor
	virtual ~NRotationContour();

	// Dialog Data
	enum { IDD = IDD_ROTATION_CONTOUR };

	void ShowStartMessage(bool Val) { StartMessage = Val; }
	BOOL GetFindStartEnd() const { return FindStartEnd; }
	BOOL GetCorrectNormal() const { return CorrectNormal; }
protected:
	bool StartMessage;
	BOOL FindStartEnd;
	BOOL CorrectNormal;
public:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
};

