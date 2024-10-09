// NTextViewiew.h : interface of the NTextView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "NCMProject.h"
#include "NCMDoc.h"
#include "NTextRenumDlg.h"
#include "EditorView.h"
#include "SGenMacro.h"


class CntrItem;
class NTextPropertySheet;

#define HORZ_TEXTOFFSET 25
#define VERT_TEXTOFFSET 25

enum ImageType
{	
	GADRPOS_IMAGE = 1,
	ERROR_IMAGE, 
	STATE_IMAGE
};
class NTextToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
public:
	NTextToolBar(void) :
	CMFCToolBar()
	{
		m_bLocked = TRUE;
	}
};


class NTextView : public EditorView
{
protected: // create from serialization only
	DECLARE_DYNCREATE(NTextView)
public:
	bool IsWordDef(const CString& str) override;

public:
	NTextView();
	virtual ~NTextView();
	virtual void OnUpdate(CPane* pSender, LPARAM lHint, CObject* pHint);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NTextView)
	public:
	protected:
	//}}AFX_VIRTUAL

public:	
	void Colorise(ptrdiff_t startPos, ptrdiff_t position) override;
	void MarkErrCadr(int CadrNum, const CString& ProgName);

	
	bool GotoCadr(int CadrNum);
	bool GotoCadrFast(int CadrNum);
	afx_msg void OnEditUpdate();
	void AutoUpdateAction() override { OnEditUpdate(); };

	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnChangeLine(int lineNum);
	void SaveBookMarkInProgram();
	void NUpdatePainter(int mode);
	void NUpdateProperty(CPropertySheet *pDial);
	void FillSheet(CPropertySheet *pDial);
	
	CString LoadedProg;
	int LoadedProgInd;
	
	bool m_DispExecute;
	int ChosenMode;

	NTextToolBar m_wndToolBar;
	SGenMacro GenMacro;

	NTextPropertySheet *m_pTextPropertySheet;
	UINT CurrFeedID;
	bool ShowReplaced;

	void NUpdate();
	void EnsureProperProg();
	void SetCadrPos(bool fBind);
	void SetImageAndState(int CurentCadr, ImageType image);
	void CompleteCurCadr();

	void OnModePainter() override;
	//{{AFX_MSG(NTextView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	afx_msg void OnAutoUpdate();
	afx_msg void OnUpdateAutoUpdate(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnTextNotPaint();
	afx_msg void OnEditRenum();
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnApplyChanges(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
public:
	void NewText(void);
	void ChangeTextReplaced();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnMacro();
	afx_msg void OnInsertToolsStr();
	afx_msg void OnShowReplaced();
	void OnEditorDblClick() override;

	void RefreshMarkers(void);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnAddBookmark();
	afx_msg void OnClearAllBookmark();
	afx_msg void OnUpdateEditUpdate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispExecute(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnCompP();
	afx_msg void OnCompF();
public:
	void ReadWordSettings(void);
	void ReadColors(void);
	void LoadTB(void);
	int LoadParams(void);
	void AdjustLayout(void);
	bool HasFocus(void);
	void SaveSciDoc();
	void ClearLoadedProg() { LoadedProg.Empty(); LoadedProgInd = -1; }
	static const bool IsRibbonInterface(void) { return NCM_PROJECT.GetGlobalState().GetRibbonInterface(); }
	void SetErrorArr(const BProgram* pProg);
	CString GetErrorMinText(int curline) override;
	void SetCollisionArr();
	cadrID CalcCollLocID(const NCollLogElem& LogElem, cadrID& StartID) const;
	bool SetCollision(const NCollLogElem& LogElem);
	CString GetCollisionText(int curline) const override;
	void ApplyMacro();
	int GetLastCadrNum(CString& NumSym) const;
	int GetLoadedProgInd() const { return LoadedProgInd; }
	const CString& GetLoadedProg() const { return LoadedProg; }
protected:
	bool StartMacro();
};


