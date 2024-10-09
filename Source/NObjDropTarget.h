
#pragma once

// NObjDropTarget command target

class NObjDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(NObjDropTarget)

public:
	NObjDropTarget(class CLeftView *pLV);
	virtual ~NObjDropTarget();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,	DROPEFFECT dropEffect, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	bool IsItemAcceptable(CWnd* pWnd, CPoint point, enum DocElements & ItemData);
	enum DocElements GetElemType(CWnd* pWnd, HTREEITEM & hTItem);
protected:
	class CLeftView *pLeftView;
};


