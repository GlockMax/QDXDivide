// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "RPickID.h"
#include "NObjTreeCtrl.h"

class PropPane;
class PAbstractObject;
class CNCMDoc;
class NToolCombined;
class NToolsConCrTurnPage;
class NToolsConCrMillPage;
class NToolsConCrWeldPage;
class NToolID;
class NColor;
class NProgram;
class NPositionDlg;
class NSectProgDlg;

/// Окно со списком объектов, с которыми работает NCManager
class CLeftView : public CDockablePane
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CLeftView)

public:
	CLeftView();
	virtual ~CLeftView();
	CNCMDoc* GetDocument();
	NObjTreeCtrl &GetTreeCtrl();
	void AdjustLayout();
	int CalcContextMenuID();
	void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void InitialUpdate(); // called first time after construct
	/// Обработка нажатия левой кнопки мыши
	void ProcClick(CPoint &point);
	NProgram * GetSelProg(void);
	void SetSelection(bool ExObjListChanged, bool PartListChanged, bool StockListChanged
		, bool ToolListChanged, bool MachineChanged, HTREEITEM NewItem ,HTREEITEM ParentItemNew);
	bool ClosePositionDlg();
	void CloseSectProgDlg();
	void MoveItem(HTREEITEM Source, HTREEITEM Target);
	void MoveExObj(HTREEITEM Source, HTREEITEM Target);
	void FillPropPane(HTREEITEM NewItem, HTREEITEM ParentItemNew);
	void SetPropPane(PropPane *pPP) noexcept { pPropPane = pPP;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMenu * pPopup;// This variable was local variable in
//						OnRclick. I insert it to CLeftView
//						for DEFENCE porposes only, but it is not
//						in use yet.
//						
	std::vector<CString> names;

protected:
	NPositionDlg *pPosDlg;
	NSectProgDlg *pSectProgDlg;
	NObjTreeCtrl TreeCtrl;
	CImageList ImageList;
	CImageList AddImageList;
	class PropPane *pPropPane;
	static const int InitialSubNum = 20;
	CRect LastPosDlgRect;
protected:
	void OnChToolCopyAndCut(bool flag);
	void OnChSimToolEdit(int index, const NToolID &ToolID);
	int GetSelProgNum();
	bool FixtureCrStl();
	bool PartCrStl();
	void PrepareTreeCtrl();
	void RecreateTreeCtrl(INT_PTR count);
	void NUpdate();
	void DeleteItem(HTREEITEM hItem);
	bool PaStockCanCreate(void);
	bool CreateStockBox(class NStock *pExStock, int ReplIndex = -1);
	bool CreateStockCyl(class NStock *pExStock, int ReplIndex = -1);
	bool CreateStockContbox(class NStock *pExStock, int ReplIndex = -1);
	bool CreateStockStl(class NStock *pExStock, int ReplIndex = -1);
	bool CreateStockCone(class NStock *pExStock, int ReplIndex = -1);
	bool CreateStockPipe(class NStock *pExStock, int ReplIndex = -1);
	bool CreateStockProfile(class NStock *pExStock, const CString &InitFileName = CString(), int ReplIndex = -1);


	//{{AFX_MSG(CLeftView)
	afx_msg void OnLoad();
	afx_msg void OnLoadAux();
	afx_msg void OnChProgrSave();
	afx_msg void OnDelete();
	afx_msg void OnOnChProgTextComp();
	void ProcPosChecks();
	afx_msg void OnDestroy();
	afx_msg void OnChProgrText();
	afx_msg void OnChProgramColor();
	afx_msg void OnPaToolsCr();
	afx_msg void OnChToolDelete();
	afx_msg void OnChToolColor();
	int CreateTreeCtrl(INT_PTR subItemsCount);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	/// Смена видимости детали станка
	afx_msg void OnChMachVisible();
	/// Смена способа отображения станка(каркасный/обычный)
	afx_msg void OnChMachWFrm();
	afx_msg void OnChProgrVisible();
	afx_msg void OnChToolEdit();
	afx_msg void OnPaStockDelete();
	afx_msg void OnChToolVisible();
	afx_msg void OnPaStockCrBox();
	afx_msg void OnChStockCrBox();
	afx_msg void OnChStockRepBox();
	afx_msg void OnChStockRepCyl();
	afx_msg void OnChStockCrCyl();
	afx_msg void OnChStockCrContbox();
	afx_msg void OnChStockRepContbox();
	afx_msg void OnChStockRepPipe();
	afx_msg void OnChStockRepProfile();
	afx_msg void OnChStockRepCone();
	afx_msg void OnChStockCrStl();
	afx_msg void OnChStockRepStl();
	afx_msg void OnChStockCrCone();
	afx_msg void OnChStockCrPipe();
	afx_msg void OnChStockCrProfile();
	afx_msg void OnPaStockCrCyl();
	afx_msg void OnPaStockAllvis();
	afx_msg void OnPaStockAllunv();
	afx_msg void OnPaStockInv();
	afx_msg void OnPaStockTemp();
	afx_msg void OnPaStockTempRem();
	afx_msg void OnPaToolsAllvis();
	afx_msg void OnPaToolsAllunv();
	afx_msg void OnPaToolsInv();
	afx_msg void OnPaProgAllvis();
	afx_msg void OnPaProgInv();
	afx_msg void OnPaProgAllunv();
	afx_msg void OnPaProgFPTraj();
	afx_msg void OnNcunitLoad();
	afx_msg void OnNcunitStart();
	afx_msg void OnChToolWf();
	afx_msg void OnPaProgrCr();
	afx_msg void OnPaProgrCrAux();
	afx_msg void OnPaProgPart();
	afx_msg void OnPaProgRelo();
	afx_msg void OnChToolPasColor();
	afx_msg void OnChToolCut();
	afx_msg void OnChToolCopy();
	afx_msg void OnChToolPaste();
	afx_msg void OnPaStockCrStl();
	afx_msg void OnCHPROGRDXFExport();
	afx_msg void OnCHPROGRAPTexport();
	afx_msg void OnPaProgFull();
	afx_msg void OnPaProgWorm();
	afx_msg void OnPaProgDelAllProg();
	afx_msg void OnPaProgDelAllSubProg();
	afx_msg void OnPaToolsUse();
	afx_msg void OnPaToolsImpncm();
	afx_msg void OnUpdatePaToolsImpncm(CCmdUI *pCmdUI);
	afx_msg void OnPaToolsShape();
	afx_msg void OnPaMachLoad();
	afx_msg void OnPaMachVisible();
	afx_msg void OnChStockExstl();
	afx_msg void OnChStockExp();
	afx_msg void OnStockImp();
	afx_msg void OnStockExstc();
	afx_msg void OnStockExdxf();
	afx_msg void OnPaExobjCrStl();
	afx_msg void OnPaPartCrStl();
	afx_msg void OnPaPartDelete();
	afx_msg void OnPaToolsTurn();
	afx_msg void OnPaToolsMt();
	afx_msg void OnPaStockCrPipe();
	afx_msg void OnChProgrStcontexport();
	afx_msg void OnPaStockCrProfile();
	afx_msg void OnPaStockCrContbox();
	afx_msg void OnPaStockCrCone();
	afx_msg void OnChProgrRelo();
	afx_msg void OnPaPartCrProfile();
	afx_msg void OnChExobjReplace();
	afx_msg void OnChExobjDelete();
	afx_msg void OnChExobjDisassemble();
	afx_msg void OnChExobjSendToStock();
	afx_msg void OnUpdateChExobjSendToStock(CCmdUI* pCmdUI);
	afx_msg void OnChExobjSendToPart();
	afx_msg void OnChExobjColor();
	afx_msg void OnChExobjVisible();
	afx_msg void OnUpdateChExobjVisible(CCmdUI *pCmdUI);
	afx_msg void OnPaExobjAllvis();
	afx_msg void OnPaExobjAllunv();
	afx_msg void OnPaExobjInv();
	afx_msg void OnChExobjPos();
	afx_msg void OnUpdatePaExobjAllvis(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaExobjAllunv(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaExobjInv(CCmdUI *pCmdUI);
	afx_msg void OnPaExobjDelete();
	afx_msg void OnUpdatePaExobjDelete(CCmdUI *pCmdUI);
	afx_msg void OnPaExobjPos();
	afx_msg void OnUpdatePaExobjPos(CCmdUI *pCmdUI);
	afx_msg void OnChExobjExportstl();
	afx_msg void OnChExobjWf();
	afx_msg void OnUpdateChExobjWf(CCmdUI *pCmdUI);
	afx_msg void OnPaPartAllvis();
	afx_msg void OnPaPartAllunv();
	afx_msg void OnPaPartInv();
	afx_msg void OnPaPartPos();
	afx_msg void OnUpdatePaPartAllvis(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaPartAllunv(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaPartInv(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockAllvis(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockAllunv(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockInv(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockTemp(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockTempRem(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockRepBox(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockRepCone(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockRepContbox(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockRepCyl(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockRepPipe(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockRepProfile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockRepStl(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockPos(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChStockDivide(CCmdUI *pCmdUI);
	afx_msg void OnChPartReplace();
	afx_msg void OnChPartDelete();
	afx_msg void OnChPartColor();
	afx_msg void OnPaStockColor();
	afx_msg void OnChStockDelete();
	afx_msg void OnChPartVisible();
	afx_msg void OnChStockVisible();
	afx_msg void OnChStockWFrame();
	afx_msg void OnChPartWf();
	afx_msg void OnChPartPos();
	afx_msg void OnPaStockPos();
	afx_msg void OnChStockPos();
	afx_msg void OnChPartExportstl();
	afx_msg void OnChPartExportdxf();
	afx_msg void OnUpdateChPartVisible(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChPartWf(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaPartDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockPos(CCmdUI *pCmdUI);
	//afx_msg void OnUpdatePaStockDivide(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaPartPos(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChPartPos(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChPartExportstl(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockPolyhromeTurn(CCmdUI *pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaMachToolVisible();
	afx_msg void OnUpdatePaMachToolVisible(CCmdUI *pCmdUI);
	afx_msg void OnPaStockSecView();
	afx_msg void OnUpdateChProgrDxfexport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChProgrAptexport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStockExstl(CCmdUI *pCmdUI);
	afx_msg void OnPaMachEdit();
	afx_msg void OnPaMachEditEnd();
	afx_msg void OnUpdatePaMachEdit(CCmdUI *pCmdUI);
	afx_msg void OnPaToolsCrWeld();
	afx_msg void OnChPartWsect();
	afx_msg void OnChProgrWeldSect();
	afx_msg void OnChPsectVisible();
	afx_msg void OnUpdateChPsectVisible(CCmdUI *pCmdUI);
	afx_msg void OnPropertiesEd();
	afx_msg void OnUpdateChProgrVisible(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChProgrActive(CCmdUI *pCmdUI);
	afx_msg void OnPaToolsDelunused();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdatePaMachVisible(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChMachVisible(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChMachWFrm(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaToolsShape(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaPartVisible(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaExobjVisible(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaStockVisible(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePaProgVisible(CCmdUI *pCmdUI);
	afx_msg void OnPaPartSubtract();
	afx_msg void OnChStockDivide();
	afx_msg void OnPaStockVisible();
	afx_msg void OnPaPartVisible();
	afx_msg void OnPaExobjVisible();
	afx_msg void OnPaProgVisible();
	void ChangeProgVisMode();
	PAbstractObject * GetAbsObj(HTREEITEM NewItem, HTREEITEM ParentItemNew);
	PAbstractObject * GetAbsObj(void);
	void GetCheckedToolList(CStringArray &CheckedToolList);
	void SetStockVisible();
	void SetStockColor();
	void LoadStocks(const std::vector<CString>& names);
	void LoadPrograms(const std::vector<CString>& names);
	void LoadProgramsAux(const std::vector<CString>& names);
	void LoadParts(const std::vector<CString>& names);
	void LoadPartsWDial(const std::vector<CString>& names);
	void LoadFixtures(const std::vector<CString>& names);
	class NStockCont * LoadStockFrCont(const CString & FileName);
	void AutoPosition(class NSetupSetBoxPointDlg &SetBoxPointDlg, const class NStock &Stock);
	bool GetSelListElem(CString &key, class NStock *&pStock);
	BOOL IsFrameWnd() const noexcept override { return TRUE; }// Added for NLeftViewToolBar::OnIdleUpdateCmdUI stops here when it looks for FrameWnd
	void UpdatePropPane();
	void ExpandTools(void);
	bool IsToolbar(const CCmdUI * pCmdUI) const;
	void PaPartCrProfile(const CString &InitFileName);
	bool TvnBegindrag();
	bool IsPosDlgActive() const;
	void OnPick(RPickID ObjID);
	NPositionDlg* MakePosDlg(CWnd* pParent, CNCMDoc* pDoc);
	LRESULT OnLeftDrop(WPARAM wParam, LPARAM lParam);
	auto GetPosDlg() const { return pPosDlg; }
};


