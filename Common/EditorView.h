#pragma once
#include "stdafx.h"
#include "EditorWnd.h"
#include "EditorDEF.h"
#include "scintilla\include\scintilla.h"

#include "AfxTempl.h"
// EditrorView view
constexpr int SC_HOVER_TIME = 400;

class AFX_EXT_CLASS EditorView : public CDockablePane
{
protected:
	enum { STATUS_DEAULT = 0, STATUS_STRINGCOMENT, STATUS_BLOCKCOMENT };
	enum { MARKER_BOOKMARK = 0, MARKER_ERROR_MIN, MARKER_ERROR, MARKER_COLLISION, MARKER_BIND, MARKER_ARROW};
//	DECLARE_DYNCREATE(EditorView)
protected:
	bool fEnableColorise;
	class NTextFindDlg *pFindDlg;
	static int EDITOR_CARET_SLOP;

public:
	EditorView();           // protected constructor used by dynamic creation
	virtual ~EditorView();
	typedef CArray<int, int> NArray;
	void GetBookmarkArr(NArray& arr);
	void SetBookmarkArr(const NArray& arr);
	int GetLineEndPosition(int line);
	ptrdiff_t GetLineWidth(ptrdiff_t line);
	void SetText(const CString &str);
	void AppendText(const CString &str);
	int GetStartLinePos(int line);
	void SetBindMarker(int lineNum);
	CString GetSelText();
	virtual bool IsWordDef(const CString& str);
	virtual void ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate/* = TRUE*/);
	void EnableColorise(bool flag = true);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	int oldType;
protected:
	SciFnDirect pSciMsg;
	sptr_t pSciWndData;
	int status;
	int StartStatusLine;
	bool fEnableBind;
	int m_bookmarkCount;
	double FeedMax;
	double CurrentFeed;
	bool fBind;
	ptrdiff_t LNWidth;
	CBitmap bmp;
	editor_cfg Config;
	EditorWnd m_wndEditor;
	CString Word2Highlight;
	bool AutoUpdate;
	UINT_PTR AutoUpdateTimerID;
	static int AutoUpdateTimeout;

protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNextBookmark();
	afx_msg void OnPrevBookmark();
	afx_msg void OnClearAllBookmark();
	afx_msg void OnUpdatePrevBookmark(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNextBookmark(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearallBookmark(CCmdUI* pCmdUI);
	afx_msg void OnAddSpaces();
	afx_msg void OnRemoveSpaces();
	afx_msg void OnAddComment();
	afx_msg void OnRemoveComment();
	afx_msg void OnModeColorise();
	virtual void OnModePainter(){}
	afx_msg LRESULT OnKeyPressed(WPARAM wparam, LPARAM lparam);
	afx_msg void OnEditFind();
	afx_msg void OnLineNum();
	afx_msg void OnUpdateLineNum(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	CArray <int> Bookmarks;
	ptrdiff_t maxTextLength;
protected:
	CDocument* GetDocument() const;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnBindChangeLine(){}
	void Init();
public:
	void SetStyle(int length,UINT SyleID);
	void StartStyling(int startPos);
	void SaveBookmarks();
	void RestoreBookmarks();
	virtual void Colorise(ptrdiff_t startPos, ptrdiff_t position);
	int GetType(CString &str,int& length);
	LRESULT SendToScentilla(UINT message, WPARAM wparam=0,LPARAM lparam=0) const;
	CString GetText(int lineNumber=-1) const;
	int GetLineLength(int lineNumber) const;
	void SetConfig(editor_cfg cfg);
	editor_cfg GetConfig(void);
	int m_CurrentLine;
	int maxPXTextWidth;
	void GoToLine(int line);
	virtual void OnChangeLine(int lineNum){}
	void ReplaceAll(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr, LPCTSTR lpszReplace);
	int ReplaceText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr, LPCTSTR lpszReplace);
	void SetPos(int pos);
	void MarkAll(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr);
	void SetSelection(int startPos,int length);
	int GetCurPos();
	void PrepareSerch();
	int GetDocLength();
	int FindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord, BOOL bExpr, int startPos = -1);
	void SetDefaultConfig(void);
	bool IsDigit(const CString &str);
	bool IsDigit(char Sym);
	CString GetLexem(const CString& str);
public:
	CString GetLine(int linenum);
	int GetLineCount();
	virtual void SCI_OnDblClick(void);
	virtual void SCI_OnHotDblClick();

	

	int AddLine(CString str, int linenum = -1);
	void ReplaceLine(CString str, int linenum);
	int GetCurLine();
	int GetCurLineWOAnchor();
	virtual void OnAddBookmark();
	void DeleteAllText(bool fRemoveBookmarks = true);
	void Undo();
	void Redo();
	bool CanUndo();
	bool CanRedo();
	void EnableUndo(bool flag = true);
	void Paste();
	bool CanPaste();
	void Cut();
	void Copy();
	bool CanCutCopy();
	void SelectAll();
	void Renum();
	void SetMarkerCurrLine(int line);
	void SetMarkerError(int line);
	void ClearErrorMarkers();
	void SetMarkerErrorMin(int line);
	void SetMarkerCollision(int line);
	void ClearErrorMarkersMin();
	void ClearCollisionMarkers();
	void Renumeration(const CString& N, int nmb, int max, int step, int width, const CString& SP);
	void DeleteNumbers(const CString& N);
	void ReColor();
	void SetPXScreenWidth();
	int GetLineFromPos(int pos);
	virtual void OnEditorDblClick(){}
	void HighlightWord(CString& Word2H);
	void NFindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr);
	void NReplaceAll(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr, LPCTSTR lpszReplace);
	void NReplaceText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr,LPCTSTR lpszReplace);
	void NMarkAll(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr);
	bool m_bFirstSearch;
	afx_msg void OnFindText();

	void SetMarginWidth(int MaxLine);

	void UpdateStat(const CString& prgName, int CurCh, int WholeChannels);
	bool SaveCopy(const CString& FilePath) const;

	void SetSavePoint(void);
	bool GetModify(void);
	void RemoveFindDlg(void);
	void CommentSelection(bool Uncomment);
	virtual CString GetErrorMinText(int curline);
	virtual CString GetCollisionText(int curline) const;
	virtual void AutoUpdateAction() {};
	afx_msg void AutoUpdateReset();
	void AutoUpdateApply();
	static void CALLBACK AutoUpdateTimerProc(
		HWND hWnd,   // handle of CWnd that called SetTimer
		UINT nMsg,   // WM_TIMER
		UINT_PTR nIDEvent,   // timer identification
		DWORD dwTime    // system time
	);
};


