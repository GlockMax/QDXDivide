#pragma once

// MTEditDropTarget command target

class MTEditTreeCtrl;
class MTEditDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(MTEditDropTarget)

public:
	MTEditDropTarget();
	virtual ~MTEditDropTarget();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual void OnDragLeave( CWnd* pWnd );
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

public:
	static HWND m_shWndTreeCtrl;
	MTEditTreeCtrl *pWndTreeCtrl;
	BOOL draging;

private:
	// Pointer to source tree control
	MTEditTreeCtrl *m_pSourceTreeCtrl;
	// Pointer to destination tree  control
	MTEditTreeCtrl *m_pDestTreeCtrl;
	// Recursive Function for adding items from source tree to destination tree
	void AddItem(HTREEITEM hSrcTItem,HTREEITEM hDestTItem );
	// Function to get the index of the item’s image and its 
	// selected image within the tree control’s image list
	void GetItemImages(HTREEITEM hSrcTItem, int &nSelItem, int &nNonSelItem);
	HTREEITEM m_hDestItem;
	
	
};


