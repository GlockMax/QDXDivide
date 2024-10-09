#pragma once

class NRotTracker : public CRectTracker
{
public:
	NRotTracker(class BOpenGLWnd* pV, char axes/* = 'A'*/, const BPoint& RotBase, WORD LeftButton = WM_LBUTTONUP);
	virtual ~NRotTracker();
	void SetPictureChanged(void) { PictureChanged = true; }
	void DrawRect();
	const BPoint GetBase(void) const { return NewOne ? BPoint(0., 0., 0., -1.) : BPoint(bx, by, bz, 1.); }
	BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert = TRUE);
	void SetMouseButton(WORD Val) { WorkMouseButton = Val; }

protected:
	BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point, CWnd* pWndClipTo);
	void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);
	class BOpenGLWnd* pView;
	char Axes;
	BOOL NewOne;
	HCURSOR PrevCursor;
	void OnChangedRect(const CRect& rectOld);
	bool PictureChanged;
	BPoint New0;
	BPoint New1;
	GLdouble xX, yX, zX, xY, yY, zY, bx, by, bz;// window X and Y axes and base point in object coordinates
	BPoint RotBase;
	WORD WorkMouseButton = WM_LBUTTONUP;
};
