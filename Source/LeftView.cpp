// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "zlib.h"
#include "NOCImport.h"
#include "NColor.h"
#include "NFont.h"
#include "NCM.h"
#include "NCMDoc.h"
#include "NTextCompDlg.h"
#include "NProgram.h"
#include "NProgramWeld.h"
#include "Security.h"
#include "NCUnit.h"
#include "mainfrm.h"
#include "NToolStd.h"
#include "NToolTurn.h"
#include "NToolsCrDial.h"
#include "NToolsConCrSheet.h"
#include "NToolsCrDiskSheet.h"
#include "NStockStl.h"
#include "NStockBSP.h"
#include "NWeldPart.h"
#include "NStockBox.h"
#include "NStockBar.h"
#include "NStockCyl.h"
#include "NStockCont.h"
#include "NStockSweep.h"
#include "NStockCrBoxDial.h"
#include "NStockCrCylDial.h"
#include "NStockCrStlDial.h"
#include "NStockCrPipDial.h"
#include "NStockCrContDial.h"
#include "NStockCrContMDial.h"
#include "DXFOut.h"
#include "StContOut.h"
#include "NMendrelDial.h"
#include "NToolCombinedTurn.h"
#include "NToolCombinedMill.h"
#include "NToolCombinedWeld.h"
#include "NToolTurnBlade.h"
#include "NToolTurnHolder.h"
#include "NToolsConCrPage.h"
#include "NToolsConCrTurnPage.h"
#include "NToolsConCrMillPage.h"
#include "NToolsConCrWeldPage.h"
#include "NPositionDlg.h"
#include "NSectArrDlg.h"
#include "NSectArrSheet.h"
#include "NWeldProgPropDlg.h"
#include "NSectProgDlg.h"
#include "MTPart.h"
#include "NRotationContour.h"
#include "NTextsComp.h"

#include "PAbstractObject.h"
#include "PropEditor.h"//PropEditor
#include "BFileDialogWPath.h"
#include "PropGridCtrl.h"//PropEditor
#include "NUnitNameDlg.h"//PropEditor
#include "ConstDef.h"
#include "UCAddDelStock.h"
#include "DocElements.h"
#include "MTConfig.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MAXPROG = 256;

extern CNCMApp theApp;
NToolCombined *pCopyTool = NULL;

// define status images
//#define UNIT_DEFINED_IMAGE 10
//#define UNIT_UNDEFINED_IMAGE 2
//#define MACH_DEFINED_IMAGE 17
//#define MACH_UNDEFINED_IMAGE 2
//#define MACH_VISIBLE_IMAGE 16
//
//#define VISIBLE_BIT 8
//#define ACTIVE_BIT 4
//#define TOOLPA_BIT 2
//#define PARTIAL_BIT 2
//
//#define PROGRAM_AUX_NUM 7
//#define PROGRAMS_AUX_NUM 18
//#define PROGRAM_AUX_NUM_PROT 18

#define TREE_VIEW_ICON_WIDTH 52
#define TREE_VIEW_ICON_HEIGHT 22
#define TREE_VIEW_ADDICON_WIDTH 32
#define TREE_VIEW_INDENT 22

/////////////////////////////////////////////////////////////////////////////
// CLeftView
int compare_double(const void *a, const void *b)
{
	if(*(double*)a>*(double*)b)
		return -1;
	return (*(double*)a==*(double*)b) ? 0 : 1;
}

IMPLEMENT_DYNCREATE(CLeftView, CDockablePane)

BEGIN_MESSAGE_MAP(CLeftView, CDockablePane)
	//{{AFX_MSG_MAP(CLeftView)
#ifndef NC_DEMO
	ON_COMMAND(ID_LOAD, &CLeftView::OnLoad)
	ON_COMMAND(ID_PA_PROG_RELO, OnPaProgRelo)
	ON_COMMAND(ID_SAVE, OnChProgrSave)
	ON_COMMAND(ID_CH_PROGR_RELO, OnChProgrRelo)
#endif
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_CH_PROGR_TEXT, OnChProgrText)
	ON_COMMAND(ID_CH_PROGRAM_COLOR, OnChProgramColor)
	ON_COMMAND(ID_PA_TOOLS_CR, OnPaToolsCr)
	ON_COMMAND(ID_CH_TOOL_DELETE, OnChToolDelete)
	ON_COMMAND(ID_CH_TOOL_COLOR, OnChToolColor)
	ON_COMMAND(ID_CH_PROGR_VISIBLE, OnChProgrVisible)
	ON_COMMAND(ID_CH_TOOL_EDIT, OnChToolEdit)
	ON_COMMAND(ID_PA_STOCK_DELETE, OnPaStockDelete)
	ON_COMMAND(ID_PA_STOCK_COLOR, OnPaStockColor)
	ON_COMMAND(ID_CH_TOOL_VISIBLE, OnChToolVisible)
	ON_COMMAND(ID_PA_STOCK_CR_BOX, OnPaStockCrBox)
	ON_COMMAND(ID_PA_STOCK_CR_CYL, OnPaStockCrCyl)
	ON_COMMAND(ID_PA_TOOLS_ALLVIS, OnPaToolsAllvis)
	ON_COMMAND(ID_PA_TOOLS_ALLUNV, OnPaToolsAllunv)
	ON_COMMAND(ID_PA_TOOLS_INV, OnPaToolsInv)
	ON_COMMAND(ID_PA_PROG_ALLVIS, OnPaProgAllvis)
	ON_COMMAND(ID_PA_PROG_INV, OnPaProgInv)
	ON_COMMAND(ID_PA_PROG_ALLUNV, OnPaProgAllunv)
	ON_COMMAND(ID_PA_PROG_FP_TRAJ, OnPaProgFPTraj)
	ON_COMMAND(ID_NCUNIT_LOAD, OnNcunitLoad)
	ON_COMMAND(ID_NCUNIT_START, OnNcunitStart)
	ON_COMMAND(ID_CH_TOOL_WF, OnChToolWf)
	ON_COMMAND(ID_PA_PROGR_CR, OnPaProgrCr)
	ON_COMMAND(ID_PA_PROGR_CR_AUX, OnPaProgrCrAux)
	ON_COMMAND(ID_PA_PROG_PART, OnPaProgPart)
	ON_COMMAND(ID_CH_TOOL_PASCOLOR, OnChToolPasColor)
	ON_COMMAND(ID_CH_TOOL_CUT, OnChToolCut)
	ON_COMMAND(ID_CH_TOOL_COPY, OnChToolCopy)
	ON_COMMAND(ID_CH_TOOL_PASTE, OnChToolPaste)
	ON_COMMAND(ID_CH_STOCK_DELETE, OnChStockDelete)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_DELETE, OnUpdateChStockDelete)
	ON_COMMAND(ID_PA_STOCK_CR_STL, OnPaStockCrStl)
	ON_COMMAND(ID_CH_STOCK_CR_STL, OnChStockCrStl)
	ON_COMMAND(ID_CH_STOCK_CR_BOX, OnChStockCrBox)
	ON_COMMAND(ID_CH_STOCK_REPLACE_BOX, OnChStockRepBox)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_REPLACE_BOX, OnUpdateChStockRepBox)
	ON_COMMAND(ID_CH_STOCK_REPLACE_CYL, OnChStockRepCyl)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_REPLACE_CYL, OnUpdateChStockRepCyl)
	ON_COMMAND(ID_CH_STOCK_REPLACE_STL, OnChStockRepStl)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_REPLACE_STL, OnUpdateChStockRepStl)
	ON_COMMAND(ID_CH_STOCK_REPLACE_CONTBOX, OnChStockRepContbox)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_REPLACE_CONTBOX, OnUpdateChStockRepContbox)
	ON_COMMAND(ID_CH_STOCK_REPLACE_PIPE, OnChStockRepPipe)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_REPLACE_PIPE, OnUpdateChStockRepPipe)
	ON_COMMAND(ID_CH_STOCK_REPLACE_PROFILE, OnChStockRepProfile)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_REPLACE_PROFILE, OnUpdateChStockRepProfile)
	ON_COMMAND(ID_CH_STOCK_REPLACE_CONE, OnChStockRepCone)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_REPLACE_CONE, OnUpdateChStockRepCone)
	ON_COMMAND(ID_CH_STOCK_CR_CYL, OnChStockCrCyl)
	ON_COMMAND(ID_CH_STOCK_CR_CONTBOX, OnChStockCrContbox)
	ON_COMMAND(ID_CH_STOCK_CR_CONE, OnChStockCrCone)
	ON_COMMAND(ID_CH_STOCK_CR_PIPE, OnChStockCrPipe)
	ON_COMMAND(ID_CH_STOCK_CR_PROFILE, OnChStockCrProfile)
	ON_COMMAND(ID_CH_STOCK_POS, OnChStockPos)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_POS, OnUpdateChStockPos)
	ON_COMMAND(ID_CH_PROGR_DXFExport, OnCHPROGRDXFExport)
	ON_COMMAND(ID_CH_PROGR_APTexport, OnCHPROGRAPTexport)
	ON_COMMAND(ID_PA_PROG_FULL, OnPaProgFull)
	ON_COMMAND(ID_PA_PROG_DELAllPROG, OnPaProgDelAllProg)
	ON_COMMAND(ID_PA_PROG_DELAllSUBPROG, OnPaProgDelAllSubProg)
	ON_COMMAND(ID_PA_PROG_WORM, OnPaProgWorm)
	ON_COMMAND(ID_PA_TOOLS_USE, OnPaToolsUse)
	ON_COMMAND(ID_PA_TOOLS_IMPNCM, OnPaToolsImpncm)
	ON_COMMAND(ID_PA_TOOLS_DELUNUSED, OnPaToolsDelunused)
	ON_UPDATE_COMMAND_UI(ID_PA_TOOLS_IMPNCM, OnUpdatePaToolsImpncm)
	ON_COMMAND(ID_PA_TOOLS_SHAPE, OnPaToolsShape)
	ON_UPDATE_COMMAND_UI(ID_PA_TOOLS_SHAPE, &CLeftView::OnUpdatePaToolsShape)
	ON_COMMAND(ID_PA_PROG_VISIBLE, OnPaProgVisible)
	ON_UPDATE_COMMAND_UI(ID_PA_PROG_VISIBLE, &CLeftView::OnUpdatePaProgVisible)
	ON_COMMAND(ID_PA_MACH_LOAD, OnPaMachLoad)
	ON_COMMAND(ID_PA_MACH_VISIBLE, OnPaMachVisible)
	ON_UPDATE_COMMAND_UI(ID_PA_MACH_VISIBLE, OnUpdatePaMachVisible)
	ON_COMMAND(ID_CH_MACH_VISIBLE, OnChMachVisible)
	ON_UPDATE_COMMAND_UI(ID_CH_MACH_VISIBLE, OnUpdateChMachVisible)
	ON_COMMAND(ID_CH_MACH_WF, OnChMachWFrm)
	ON_UPDATE_COMMAND_UI(ID_CH_MACH_WF, OnUpdateChMachWFrm)
	ON_COMMAND(ID_CH_STOCK_EXSTL, OnChStockExstl)
	ON_COMMAND(ID_CH_STOCK_EXP, OnChStockExp)
	ON_COMMAND(ID_STOCK_IMP, OnStockImp)
	ON_COMMAND(ID_STOCK_EXSTC, OnStockExstc)
	ON_COMMAND(ID_STOCK_EXDXF, OnStockExdxf)
	ON_COMMAND(ID_PA_PART_CR_STL, OnPaPartCrStl)
	ON_COMMAND(ID_PA_STOCK_VISIBLE, OnPaStockVisible)
	ON_COMMAND(ID_CH_STOCK_VISIBLE, OnChStockVisible)
	ON_UPDATE_COMMAND_UI(ID_PA_STOCK_VISIBLE, OnUpdatePaStockVisible)
	ON_COMMAND(ID_PA_PART_VISIBLE, OnPaPartVisible)
	ON_COMMAND(ID_PA_EXOBJ_VISIBLE, OnPaExobjVisible)
	ON_COMMAND(ID_PA_PART_DELETE, OnPaPartDelete)
	ON_COMMAND(ID_PA_EXOBJ_CR_STL, OnPaExobjCrStl)
	ON_COMMAND(ID_PA_TOOLS_TURN, OnPaToolsTurn)
	ON_COMMAND(ID_PA_TOOLS_MT, OnPaToolsMt)
	ON_COMMAND(ID_PA_STOCK_CR_PIPE, OnPaStockCrPipe)
	ON_COMMAND(ID_CH_PROGR_StContExport, OnChProgrStcontexport)
	ON_COMMAND(ID_PA_STOCK_CR_PROFILE, OnPaStockCrProfile)
	ON_COMMAND(ID_PA_STOCK_CR_CONTBOX, OnPaStockCrContbox)
	ON_COMMAND(ID_PA_STOCK_CR_CONE, OnPaStockCrCone)
	ON_COMMAND(ID_PA_PART_CR_PROFILE, OnPaPartCrProfile)
	ON_COMMAND(ID_CH_EXOBJ_REPLACE, &CLeftView::OnChExobjReplace)
	ON_COMMAND(ID_CH_EXOBJ_DELETE, &CLeftView::OnChExobjDelete)
	ON_COMMAND(ID_CH_EXOBJ_DISASSAMBLE, &CLeftView::OnChExobjDisassemble)
	ON_COMMAND(ID_CH_EXOBJ_SENDTOSTOCK, &CLeftView::OnChExobjSendToStock)
	ON_UPDATE_COMMAND_UI(ID_CH_EXOBJ_SENDTOSTOCK, &CLeftView::OnUpdateChExobjSendToStock)
	ON_COMMAND(ID_CH_EXOBJ_SENDTOPART, &CLeftView::OnChExobjSendToPart)
	ON_COMMAND(ID_CH_EXOBJ_COLOR, &CLeftView::OnChExobjColor)
	ON_COMMAND(ID_CH_EXOBJ_VISIBLE, &CLeftView::OnChExobjVisible)
	ON_UPDATE_COMMAND_UI(ID_CH_EXOBJ_VISIBLE, &CLeftView::OnUpdateChExobjVisible)
	ON_COMMAND(ID_PA_EXOBJ_ALLVIS, &CLeftView::OnPaExobjAllvis)
	ON_COMMAND(ID_PA_EXOBJ_ALLUNV, &CLeftView::OnPaExobjAllunv)
	ON_COMMAND(ID_PA_EXOBJ_INV, &CLeftView::OnPaExobjInv)
	ON_COMMAND(ID_CH_EXOBJ_POS, &CLeftView::OnChExobjPos)
	ON_UPDATE_COMMAND_UI(ID_PA_EXOBJ_ALLVIS, &CLeftView::OnUpdatePaExobjAllvis)
	ON_UPDATE_COMMAND_UI(ID_PA_EXOBJ_ALLUNV, &CLeftView::OnUpdatePaExobjAllunv)
	ON_UPDATE_COMMAND_UI(ID_PA_EXOBJ_INV, &CLeftView::OnUpdatePaExobjInv)
	ON_UPDATE_COMMAND_UI(ID_PA_EXOBJ_VISIBLE, &CLeftView::OnUpdatePaExobjVisible)
	ON_UPDATE_COMMAND_UI(ID_PA_PART_VISIBLE, &CLeftView::OnUpdatePaPartVisible)
	ON_COMMAND(ID_PA_EXOBJ_DELETE, &CLeftView::OnPaExobjDelete)
	ON_UPDATE_COMMAND_UI(ID_PA_EXOBJ_DELETE, &CLeftView::OnUpdatePaExobjDelete)
	ON_COMMAND(ID_PA_EXOBJ_POS, &CLeftView::OnPaExobjPos)
	ON_UPDATE_COMMAND_UI(ID_PA_EXOBJ_POS, &CLeftView::OnUpdatePaExobjPos)
	ON_COMMAND(ID_CH_EXOBJ_EXPORTSTL, &CLeftView::OnChExobjExportstl)
	ON_COMMAND(ID_CH_EXOBJ_WF, &CLeftView::OnChExobjWf)
	ON_UPDATE_COMMAND_UI(ID_CH_EXOBJ_WF, &CLeftView::OnUpdateChExobjWf)
	ON_COMMAND(ID_PA_PART_ALLVIS, &CLeftView::OnPaPartAllvis)
	ON_COMMAND(ID_PA_PART_ALLUNV, &CLeftView::OnPaPartAllunv)
	ON_COMMAND(ID_PA_PART_INV, &CLeftView::OnPaPartInv)
	ON_COMMAND(ID_PA_PART_POS, &CLeftView::OnPaPartPos)
	ON_COMMAND(ID_PA_STOCK_ALLVIS, &CLeftView::OnPaStockAllvis)
	ON_COMMAND(ID_PA_STOCK_ALLUNV, &CLeftView::OnPaStockAllunv)
	ON_COMMAND(ID_PA_STOCK_INV, &CLeftView::OnPaStockInv)
	ON_COMMAND(ID_PA_STOCK_TEMP, &CLeftView::OnPaStockTemp)
	ON_COMMAND(ID_PA_STOCK_TEMPREM, &CLeftView::OnPaStockTempRem)
	ON_UPDATE_COMMAND_UI(ID_PA_PART_ALLVIS, &CLeftView::OnUpdatePaPartAllvis)
	ON_UPDATE_COMMAND_UI(ID_PA_PART_ALLUNV, &CLeftView::OnUpdatePaPartAllunv)
	ON_UPDATE_COMMAND_UI(ID_PA_PART_INV, &CLeftView::OnUpdatePaPartInv)
	ON_UPDATE_COMMAND_UI(ID_PA_STOCK_ALLVIS, &CLeftView::OnUpdatePaStockAllvis)
	ON_UPDATE_COMMAND_UI(ID_PA_STOCK_ALLUNV, &CLeftView::OnUpdatePaStockAllunv)
	ON_UPDATE_COMMAND_UI(ID_PA_STOCK_INV, &CLeftView::OnUpdatePaStockInv)
	ON_UPDATE_COMMAND_UI(ID_PA_STOCK_TEMP, &CLeftView::OnUpdatePaStockTemp)
	ON_UPDATE_COMMAND_UI(ID_PA_STOCK_TEMPREM, &CLeftView::OnUpdatePaStockTempRem)
	ON_COMMAND(ID_CH_PART_REPLACE, &CLeftView::OnChPartReplace)
	ON_COMMAND(ID_CH_PART_DELETE, &CLeftView::OnChPartDelete)
	ON_COMMAND(ID_CH_PART_COLOR, &CLeftView::OnChPartColor)
	ON_COMMAND(ID_CH_PART_VISIBLE, &CLeftView::OnChPartVisible)
	ON_COMMAND(ID_CH_PART_WF, &CLeftView::OnChPartWf)
	ON_COMMAND(ID_CH_PART_POS, &CLeftView::OnChPartPos)
	ON_COMMAND(ID_PA_STOCK_POS, &CLeftView::OnPaStockPos)
	ON_UPDATE_COMMAND_UI(ID_PA_STOCK_DELETE, &CLeftView::OnUpdatePaStockDelete)
	ON_UPDATE_COMMAND_UI(ID_PA_STOCK_POS, &CLeftView::OnUpdatePaStockPos)
	ON_COMMAND(ID_CH_PART_ROTATE, &CLeftView::OnChPartExportdxf)
	ON_COMMAND(ID_CH_PART_EXPORTSTL, &CLeftView::OnChPartExportstl)
	ON_UPDATE_COMMAND_UI(ID_CH_PART_VISIBLE, &CLeftView::OnUpdateChPartVisible)
	ON_UPDATE_COMMAND_UI(ID_CH_PART_WF, &CLeftView::OnUpdateChPartWf)
	ON_UPDATE_COMMAND_UI(ID_PA_PART_DELETE, &CLeftView::OnUpdatePaPartDelete)
	ON_UPDATE_COMMAND_UI(ID_PA_PART_POS, &CLeftView::OnUpdatePaPartPos)
	ON_UPDATE_COMMAND_UI(ID_CH_PART_POS, &CLeftView::OnUpdateChPartPos)
	ON_UPDATE_COMMAND_UI(ID_CH_PART_EXPORTSTL, &CLeftView::OnUpdateChPartExportstl)
	ON_COMMAND(ID_PA_MACH_TOOL_VISIBLE, &CLeftView::OnPaMachToolVisible)
	ON_UPDATE_COMMAND_UI(ID_PA_MACH_TOOL_VISIBLE, &CLeftView::OnUpdatePaMachToolVisible)
	ON_COMMAND(ID_PA_STOCK_SEC_VIEW, &CLeftView::OnPaStockSecView)
	ON_UPDATE_COMMAND_UI(ID_CH_PROGR_DXFExport, &CLeftView::OnUpdateChProgrDxfexport)
	ON_UPDATE_COMMAND_UI(ID_CH_PROGR_APTexport, &CLeftView::OnUpdateChProgrAptexport)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_EXSTL, &CLeftView::OnUpdateStockExstl)
	ON_COMMAND(ID_PA_MACH_EDIT, &CLeftView::OnPaMachEdit)
	ON_COMMAND(ID_PA_MACH_EDIT_END, &CLeftView::OnPaMachEditEnd)
	ON_UPDATE_COMMAND_UI(ID_PA_MACH_EDIT, &CLeftView::OnUpdatePaMachEdit)
	ON_COMMAND(ID_PA_TOOLS_CR_WELD, &CLeftView::OnPaToolsCrWeld)
	ON_COMMAND(ID_CH_PART_WSECT, &CLeftView::OnChPartWsect)
	ON_COMMAND(ID_CH_PROGR_WELD_SECT, &CLeftView::OnChProgrWeldSect)
	ON_COMMAND(ID_CH_PSECT_VISIBLE, &CLeftView::OnChPsectVisible)
	ON_UPDATE_COMMAND_UI(ID_CH_PSECT_VISIBLE, &CLeftView::OnUpdateChPsectVisible)
	ON_COMMAND(ID_PROPERTIES_ED, &CLeftView::OnPropertiesEd)
	ON_UPDATE_COMMAND_UI(ID_CH_PROGR_VISIBLE, &CLeftView::OnUpdateChProgrVisible)
	ON_WM_HELPINFO()
	//ON_COMMAND(ID_PA_PART_SUBTRACT, &CLeftView::OnPaPartSubtract)
	ON_COMMAND(ID_CH_STOCK_DIVIDE, &CLeftView::OnChStockDivide)
	ON_UPDATE_COMMAND_UI(ID_CH_STOCK_DIVIDE, &CLeftView::OnUpdateChStockDivide)
	ON_COMMAND(ID_LOAD_AUX, &CLeftView::OnLoadAux)
	ON_MESSAGE(WM_APP_LEFT_DROP, &CLeftView::OnLeftDrop)
	ON_COMMAND(ID_GCODE_COMPARE, &CLeftView::OnOnChProgTextComp)
	//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView() : TreeCtrl(this)
{
	pCopyTool = nullptr;
	pPosDlg = nullptr;
	pSectProgDlg = nullptr;
	pPropPane = nullptr;
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= TVS_HASBUTTONS | TVS_INFOTIP ;//| TVS_TRACKSELECT;
//	cs.style |= TVS_CHECKBOXES | TVS_HASBUTTONS | TVS_INFOTIP;//| TVS_TRACKSELECT;
	return CDockablePane::PreCreateWindow(cs);
}

void CLeftView::InitialUpdate()
{
	TreeCtrl.ShowAllTB(false);
	//Initialise TreeCtrl
	TreeCtrl.SetImageList(&ImageList,TVSIL_NORMAL);
	TreeCtrl.SetImageList(&AddImageList,TVSIL_STATE );
	
	TreeCtrl.ModifyStyle(NULL,TVS_NOTOOLTIPS | TVS_HASBUTTONS  | TVS_LINESATROOT /*| TVS_HASLINES */ );
	TreeCtrl.DeleteAllItems();
	CString str;
	str.LoadString(IDS_UNIT);
	TreeCtrl.NCUnitItem = TreeCtrl.InsertItem(str);
	TreeCtrl.SetItemData(TreeCtrl.NCUnitItem,NCUnit);

	str.LoadString(IDS_MACHINE);
	TreeCtrl.NMachineItem = TreeCtrl.InsertItem(str);
	TreeCtrl.SetItemData(TreeCtrl.NMachineItem,NMachine);

	str.LoadString(IDS_NCPROGRAMS);
	TreeCtrl.NProgramItem = TreeCtrl.InsertItem(str);
	TreeCtrl.SetItemData(TreeCtrl.NProgramItem,NProgramList);

	TreeCtrl.NProgramItemAux = NULL;
#ifndef _2D
	str.LoadString(IDS_TOOLS);
	TreeCtrl.NToolItem = TreeCtrl.InsertItem(str);
	TreeCtrl.SetItemData(TreeCtrl.NToolItem,NToolList);

	TreeCtrl.NStockItem = NULL;
	TreeCtrl.NStocksItem = NULL;

	if(NCMComData::GetType() != AT_WELD)
	{
		CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
		if (pDoc)
		{
			if (pDoc->IsThereMoreThanOneStock())
			{
				// заготовки
				str.LoadString(IDS_STOCKS);
				TreeCtrl.NStocksItem = TreeCtrl.InsertItem(str,0,0,TVI_ROOT,TVI_LAST);
				TreeCtrl.SetItemData(TreeCtrl.NStocksItem,NStocksList);
			}
			else
			{
				// заготовка
				str.LoadString(IDS_STOCK);
				TreeCtrl.NStockItem = TreeCtrl.InsertItem(str);
				TreeCtrl.SetItemData(TreeCtrl.NStockItem,NStock);
			}
		}
	}

	str.LoadString(IDS_PART);
	TreeCtrl.NPartItem = TreeCtrl.InsertItem(str,0,0,TVI_ROOT,TVI_LAST);
	TreeCtrl.SetItemData(TreeCtrl.NPartItem,NPartList);
	
	str.LoadString(IDS_EXTEND);
	TreeCtrl.NExObjItem = TreeCtrl.InsertItem(str,0,0,TVI_ROOT,TVI_LAST);
	TreeCtrl.SetItemData(TreeCtrl.NExObjItem,NExObjList);
#endif
	TreeCtrl.SelectItem(NULL);

	TreeCtrl.SetSettings();
	// Initialize tool bars
	COLORREF BkColor = GetTreeCtrl().GetBkColor();
	for (int i = 0; i < TreeCtrl.ItemTypesNum; ++i)
		TreeCtrl.ItemToolbars[i].SetBkColor(BkColor);
	for (size_t i = 0; i < TreeCtrl.SubItemToolBars.size(); ++i)
		TreeCtrl.SubItemToolBars[i].SetBkColor(BkColor);
	TreeCtrl.SetIndent(TREE_VIEW_INDENT);
	NUpdate();
	TreeCtrl.RedrawWindow();
}

void CLeftView::LoadPrograms(const std::vector<CString>& names)
{
#ifndef NC_DEMO
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	for(int i = 0; i < int(names.size()) ; ++i)
	{
		CString filename = names[i];
		//Load program
		NProgram *pP = pDoc->LoadNewProgram(filename);
		if(pP == nullptr) 
		{
			CString str;
			str.Format(IDS_MES_READ,filename);

			if(AfxMessageBox(str,
					MB_ICONQUESTION | 
					MB_YESNO )
				== IDYES)
			
				continue;
			else
				return;
		}
		pDoc->NAddMainProgram(filename, pP );
		pDoc->SetProgrTDT(filename);
	}

	pDoc->UpdateAllViews(NULL, N_LEFT | N_NEW_TEXT | N_SETTINGS | N_PROGRAMCHANGED); // Should be before RegeneratePrograms to ensure correct errors processing
	pDoc->RegeneratePrograms();
	pDoc->UpdateAllViews(NULL, N_LEFT | N_PROGRAMCHANGED | N_TEXT); // Needed to set auto tools; N_TEXT needed to remove error icon
#endif
}
#ifndef NC_DEMO
void CLeftView::OnLoad() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	
	CString name;
	CString filter;
	CString AddExt("");//Additional file extension defined in the unit definition
	if(!pDoc->GetUnit()->GetFileExt(AddExt))
		AddExt = "*.tap";
	filter.Format(IDS_PROG_FILTER,AddExt,AddExt);
	CString filename;
	BFileDialogWPath fd(_T("LastProgPath"), TRUE, NULL, NULL, NULL, filter);
	fd.m_ofn.Flags = fd.m_ofn.Flags | OFN_ALLOWMULTISELECT;
	CHAR buf[4096] = "";
	fd.m_ofn.lpstrFile = buf;
	fd.m_ofn.nMaxFile = 4096;
	if(fd.DoModal() != IDOK)
		return;
	std::vector<CString> names;
	// For all selected files
	for(POSITION pos = fd.GetStartPosition() ; pos != NULL ;)
	{
		names.push_back(fd.GetNextPathName(pos));
	}
	LoadPrograms(names);
}

void CLeftView::OnPaProgRelo() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CloseSectProgDlg();
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->ReloadAllProg();
	pDoc->UpdateAllViews(NULL,N_LEFT | N_NEW_TEXT | N_OGL | N_STATE | N_PROGRAMCHANGED);
}

void CLeftView::OnChProgrSave() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
// Save selected program

// Take pProg
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	int k = GetSelProgNum();
	if(k<0)
		return;
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg;
	CString key;
	pPrL->GetAt(k,key,(CObject *&)pProg);

// Get output file name	
	CString filename(key);
	CString filter,ext("");
	key.LoadString(IDS_ALLFILES_FILTER);
	int index = filename.ReverseFind('.');
	if( index > 0 )
	{
		for(int i = index + 1; i < filename.GetLength(); i++)
			ext += filename[i];
		filter = "*." + ext +"|*." + ext + "|";
		filter += key;
	}
	else
		filter = key;
	CFileDialog fd(FALSE,ext,filename,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		filter);
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return;


// Write output file
	pDoc->SaveProgText(filename, pProg);
}

void CLeftView::OnChProgrRelo()
{
// Reload selected program from the corresponding files
// create all child items

	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CloseSectProgDlg();

	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	int i = GetSelProgNum();
	if(i < 0)
		return;
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg = nullptr;
	CString key;
	pPrL->GetAt(i, key, (CObject *&)pProg);


	pDoc->GetUnit()->Reset();

	CString mes;
	if(pProg->IsManChanged())
	{
		mes.Format(IDS_MES_PROG_CHANGED, key);
		if(AfxMessageBox(mes, MB_YESNO) != IDYES)
			return; // Do not reload
	}
	((NTextView*)((CMainFrame*)AfxGetMainWnd())->GetTextView())->ClearLoadedProg();
	if (pDoc->ReloadProgram(pProg, key))
	{
		pDoc->ImplRepl(pProg);
	}
	else // Can't open file
	{
		mes.Format(IDS_MES_DEL_PROG, key);
		if(AfxMessageBox(mes, MB_YESNO) == IDYES)
		{
			pDoc->DeleteProg(key);
		}
	}

	pDoc->RegeneratePrograms();
	pDoc->UpdateAllViews(NULL, N_LEFT | N_NEW_TEXT | N_OGL | N_STATE | N_PROGRAMCHANGED);
}
#endif

void CLeftView::OnDelete() 
{
//Delete program
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CloseSectProgDlg();

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int i = GetSelProgNum();
	if(i < 0)
		return;
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg;
	CString key;
	pPrL->GetAt(i,key,(CObject *&)pProg);

	pDoc->DeleteProg(key);

	pDoc->UpdateAllViews(NULL,N_OGL | N_NEW_TEXT | N_LEFT | N_PROGRAMCHANGED);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnOnChProgTextComp()
{
	const int SelProgInd = GetSelProgNum();
	NTextCompDlg Dlg(GetDocument(), nullptr);
	if (Dlg.DoModal(SelProgInd) == IDOK)
	{
		NTextsComp Comp(*GetDocument());
		if (!Comp.MakeArg1(SelProgInd))
			return;
		if (Dlg.IsFileSelected())
		{
			if (!Comp.MakeArg2(Dlg.GetName()))
				return;
		}
		else
		{
			if (!Comp.MakeArg2(Dlg.GetProgInd()))
				return;
		}
		Comp.DoComp();
	}
}

void CLeftView::ProcPosChecks()
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc == nullptr)
		return;
	const bool SetChecks = IsPosDlgActive();
	if (SetChecks)
	{
		pPosDlg->StartPosDlg();
		pPosDlg->FillAbsFields();
	}
	GetTreeCtrl().SetPosChecks(pDoc, SetChecks);
}

void CLeftView::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint) 
{
	if(lHint & N_STOCKSTATECHANGE)
	{
		// Save expanded items
		bool ExpMach = TreeCtrl.IsItemExpanded(TreeCtrl.NMachineItem);
		bool ExpProg = TreeCtrl.IsItemExpanded(TreeCtrl.NProgramItem);
		bool ExpProgAux = false;
		if (TreeCtrl.NProgramItemAux)
			ExpProgAux = TreeCtrl.IsItemExpanded(TreeCtrl.NProgramItemAux);
		bool ExpStocks = false;
		if (TreeCtrl.NStocksItem)
			ExpStocks = TreeCtrl.IsItemExpanded(TreeCtrl.NStocksItem);
		bool ExpTools = TreeCtrl.IsItemExpanded(TreeCtrl.NToolItem);
		bool ExpParts = TreeCtrl.IsItemExpanded(TreeCtrl.NPartItem);
		bool ExpExObj = TreeCtrl.IsItemExpanded(TreeCtrl.NExObjItem);

		InitialUpdate();

		// Restore expanded items
		if (ExpMach) TreeCtrl.SetItemExpanded(TreeCtrl.NMachineItem);
		if (ExpProg) TreeCtrl.SetItemExpanded(TreeCtrl.NProgramItem);
		if (ExpProgAux) TreeCtrl.SetItemExpanded(TreeCtrl.NProgramItemAux);
		if (ExpStocks) TreeCtrl.SetItemExpanded(TreeCtrl.NStocksItem);
		if (ExpTools) TreeCtrl.SetItemExpanded(TreeCtrl.NToolItem);
		if (ExpParts) TreeCtrl.SetItemExpanded(TreeCtrl.NPartItem);
		if (ExpExObj) TreeCtrl.SetItemExpanded(TreeCtrl.NExObjItem);
	}
	if (lHint & N_POS_CHECK)
	{
		ProcPosChecks();
		Invalidate();
	}
	if (lHint & N_PROGRAMCHANGED)
	{
		const CNCMDoc* pDoc = GetDocument();
		if (!pDoc)
			return;
		((CMainFrame*)AfxGetMainWnd())->ShowErrorsCaption(pDoc->GetCurChannel()->HaveMinorErrors() ? TRUE : FALSE);
	}
	// Update colors
	if(lHint & N_SETTINGS)
	{
		TreeCtrl.SetSettings();
		NUpdate();
		Invalidate();
		return;
	}
	if(lHint & N_CADRCHANGED)
	{
		CNCMDoc* pDoc = GetDocument();
		if (!pDoc)
			return;
		if (IsPaneVisible())
			TreeCtrl.SetHighlights(pDoc);
		((CMainFrame *)AfxGetMainWnd())->SetFlags();
		return;
	}
	if (lHint & (N_LEFT | N_NEWDOC))
	{
		((CMainFrame *)AfxGetMainWnd())->UpdateRibbonToolsCombo();
		NUpdate();
		return;
	}
	if (lHint & N_LEFT_ATTR)
	{
		CNCMDoc* pDoc = GetDocument();
		if (!pDoc)
			return;
		TreeCtrl.MakeTB(pDoc);
	}
	if (lHint & N_DELETECONT)
	{
		ClosePositionDlg();
		CloseSectProgDlg();
		((CMainFrame *)AfxGetMainWnd())->CloseDimDialogs();
	}
}

int compare_int( const void *a, const void *b )
{
	if(*(int*)a<*(int*)b)
		return -1;
	return (*(int*)a==*(int*)b) ? 0 : 1;
}

void CLeftView::PrepareTreeCtrl()
{
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	size_t count = pDoc->CalcSubObjects();
	if (count != TreeCtrl.SubItemToolBars.size())
		RecreateTreeCtrl(count);
}

void CLeftView::RecreateTreeCtrl(INT_PTR count)
{
	// Save selected item
	DocElements SelElement = NCUnit;
	HTREEITEM SelItem = GetTreeCtrl().GetSelectedItem();
	if (SelItem == TreeCtrl.NExObjItem)
		SelElement = NExObjList;
	else if (SelItem == TreeCtrl.NProgramItem)
		SelElement = NProgramList;
	else if (SelItem == TreeCtrl.NToolItem)
		SelElement = NToolList;
	else if (SelItem == TreeCtrl.NStockItem)
		SelElement = NStock;
	else if (SelItem == TreeCtrl.NPartItem)
		SelElement = NPartList;
	else if (SelItem == TreeCtrl.NCUnitItem)
		SelElement = NCUnit;
	else if (SelItem == TreeCtrl.NMachineItem)
		SelElement = NMachine;
	else if (SelItem == TreeCtrl.NProgramItemAux)
		SelElement = NProgramListAux;
	else if (SelItem == TreeCtrl.NStocksItem)
		SelElement = NStocksList;

	// Save expanded items
	bool ExpMach = TreeCtrl.IsItemExpanded(TreeCtrl.NMachineItem);
	bool ExpProg = TreeCtrl.IsItemExpanded(TreeCtrl.NProgramItem);
	bool ExpProgAux = false;
	if (TreeCtrl.NProgramItemAux)
		ExpProgAux = TreeCtrl.IsItemExpanded(TreeCtrl.NProgramItemAux);
	bool ExpStocks = false;
	if (TreeCtrl.NStocksItem)
		ExpStocks = TreeCtrl.IsItemExpanded(TreeCtrl.NStocksItem);
	bool ExpTools = TreeCtrl.IsItemExpanded(TreeCtrl.NToolItem);
	bool ExpParts = TreeCtrl.IsItemExpanded(TreeCtrl.NPartItem);
	bool ExpExObj = TreeCtrl.IsItemExpanded(TreeCtrl.NExObjItem);

	TreeCtrl.DestroyWindow();
	CreateTreeCtrl(count);

	// Restore expanded items
	if (ExpMach) TreeCtrl.SetItemExpanded(TreeCtrl.NMachineItem);
	if (ExpProg) TreeCtrl.SetItemExpanded(TreeCtrl.NProgramItem);
	if (ExpProgAux) TreeCtrl.SetItemExpanded(TreeCtrl.NProgramItemAux);
	if (ExpStocks) TreeCtrl.SetItemExpanded(TreeCtrl.NStocksItem);
	if (ExpTools) TreeCtrl.SetItemExpanded(TreeCtrl.NToolItem);
	if (ExpParts) TreeCtrl.SetItemExpanded(TreeCtrl.NPartItem);
	if (ExpExObj) TreeCtrl.SetItemExpanded(TreeCtrl.NExObjItem);
	// Restore selected item
	if (SelItem != nullptr)
	{
		switch (SelElement)
		{
		case NExObjList:
			TreeCtrl.SelectItem(TreeCtrl.NExObjItem);
			break;
		case NProgramList:
			TreeCtrl.SelectItem(TreeCtrl.NProgramItem);
			break;
		case NToolList:
			TreeCtrl.SelectItem(TreeCtrl.NToolItem);
			break;
		case NStock:
			TreeCtrl.SelectItem(TreeCtrl.NStockItem);
			break;
		case NPartList:
			TreeCtrl.SelectItem(TreeCtrl.NPartItem);
			break;
		case NCUnit:
			TreeCtrl.SelectItem(TreeCtrl.NCUnitItem);
			break;
		case NMachine:
			TreeCtrl.SelectItem(TreeCtrl.NMachineItem);
			break;
		case NProgramListAux:
			TreeCtrl.SelectItem(TreeCtrl.NProgramItemAux);
			break;
		case NStocksList:
			TreeCtrl.SelectItem(TreeCtrl.NStocksItem);
			break;
		}
	}
	TreeCtrl.ProcessItemPosChanged();
}

void CLeftView::NUpdate()
{
//	TreeCtrl.CloseContextToolBar();
 //Erase insert mark
	GetTreeCtrl().SetInsertMark(NULL);

	CNCMDoc* pDoc = GetDocument();

	POSITION pos;
	CString key;
	NProgram *pP;
	if(!pDoc)
		return;
	ASSERT_VALID(pDoc);
/// Get the CTreeCtrl
	NObjTreeCtrl& tc = GetTreeCtrl();
	ASSERT(tc);

	CArray<int, int> arExpandTool;

	for(HTREEITEM Item = tc.GetRootItem(),ItemB = NULL;
			  Item != NULL;
			  Item = tc.GetNextSiblingItem(Item))
	{
		for(HTREEITEM ChildItem = tc.GetChildItem(Item);
					  ChildItem != NULL;)
		{
			ItemB = tc.GetNextSiblingItem(ChildItem);
			/// сохранение развёрнутых инструментов
			if (GetTreeCtrl().GetItemData(Item) == NToolList)
			{
				CString key_str;
				CString ToolText = GetTreeCtrl().GetItemText(ChildItem);
				HTREEITEM tpmItem = tc.GetNextVisibleItem(ChildItem);
				if (tpmItem != NULL &&  
					GetTreeCtrl().GetParentItem(tpmItem) != TreeCtrl.NToolItem &&
												tpmItem != TreeCtrl.NStockItem &&
												tpmItem != TreeCtrl.NStocksItem &&
												tpmItem != TreeCtrl.NPartItem)
				{
					for(int i = 0; i <ToolText.Find(" "); i++ )
						  key_str += ToolText[ i ];
					arExpandTool.Add(atoi(key_str));
				}
			}

			ChildItem = ItemB;
		}
	}
	//
	PrepareTreeCtrl();

/// update all child items
	HTREEITEM cItem, pItem;
	/// Machine
	{
		const MTPartArray &Parts = pDoc->GetMachTool().GetParts();
		HTREEITEM ChildItem = tc.GetChildItem(TreeCtrl.NMachineItem);
		int i = 0;
		for(; i < Parts.GetSize(); ++i)
		{
			CString NewText = Parts[i]->GetAxisName();
			if(ChildItem == NULL)
			{
				cItem = tc.InsertItem(NewText, TreeCtrl.NMachineItem);
			}
			else
			{
				tc.SetItemText(ChildItem, NewText);
				ChildItem = tc.GetNextSiblingItem(ChildItem);
			}
		}//for
		// Delete all items from ChildItem
		for(; ChildItem != NULL;)
		{
			HTREEITEM ItemB = tc.GetNextSiblingItem(ChildItem);
			tc.DeleteItem(ChildItem);
			ChildItem = ItemB;
		}
	}

	/// Список программ
	CMapStringToNProgram *plist = pDoc->NGetProgramList();
	{
		HTREEITEM ChildItem = tc.GetChildItem(TreeCtrl.NProgramItem);
		for(int ip = 0; ip < plist->GetMainProgCount(); ++ip)
		{
			plist->GetAt( ip, key, (CObject*&)pP );
			int index = key.ReverseFind( '\\' );
			CString NewText;
			for( int i = index + 1; i < key.GetLength(); i++ )
				NewText += key[ i ];
			if(ChildItem == NULL)
			{
				cItem = tc.InsertItem(NewText, TreeCtrl.NProgramItem);
			}
			else
			{
				tc.SetItemText(ChildItem, NewText);
				ChildItem = tc.GetNextSiblingItem(ChildItem);
			}
		}//for
		// Delete all items from ChildItem
		for(; ChildItem != NULL; )
		{
			HTREEITEM ItemB = tc.GetNextSiblingItem(ChildItem);
			DeleteItem(ChildItem);
			ChildItem = ItemB;
		}
		// Aux programs
		if(plist->GetSize() > plist->GetMainProgCount())
		{
			if(!TreeCtrl.NProgramItemAux)
			{
				CString str;
				str.LoadString(IDS_NCPROGRAMS_AUX);
				TreeCtrl.NProgramItemAux = tc.InsertItem(str, TVI_ROOT, TreeCtrl.NProgramItem);
				tc.SetItemData(TreeCtrl.NProgramItemAux, NProgramListAux);
			}
			ChildItem = tc.GetChildItem(TreeCtrl.NProgramItemAux);
			for(int ip = plist->GetMainProgCount(); ip < plist->GetSize(); ++ip)
			{
				plist->GetAt( ip, key, (CObject*&)pP );
				int index = key.ReverseFind( '\\' );
				CString NewText;
				for( int i = index + 1; i < key.GetLength(); i++ )
					NewText += key[ i ];
				if(ChildItem == NULL)
				{
					cItem = tc.InsertItem(NewText, TreeCtrl.NProgramItemAux);
				}
				else
				{
					tc.SetItemText(ChildItem, NewText);
					ChildItem = tc.GetNextSiblingItem(ChildItem);
				}
			}//for
			// Delete all items from ChildItem
			for(; ChildItem != NULL; )
			{
				HTREEITEM ItemB = tc.GetNextSiblingItem(ChildItem);
				DeleteItem(ChildItem);
				ChildItem = ItemB;
			}
		}
		else
		{
			if(TreeCtrl.NProgramItemAux)
			{
				DeleteItem(TreeCtrl.NProgramItemAux);
				TreeCtrl.NProgramItemAux = NULL;
			}
		}
	}
/// list Tool
	/// list Combined Tool
#ifndef _2D
	NMapStringToNTool *tlist = pDoc->NGetToolList();
	tlist->MakeActual();
	NToolCombined* pTc;
/// Calc a turrets number and turret sizes
	NToolID ToolID;
	CMap<int, int, int, int> TurretsSizes, BTurretsSizes; // We shall decrement elements and need original Turrets for the further use
	for each (const auto & el in *tlist)
	{
		const NToolID& ToolID = el.first;
		NToolCombined* pTc = static_cast<NToolCombined*>(el.second);;
		++TurretsSizes[ToolID.GetTurretNum()] ;
	}
/// Create and fill turret arrays
	CMap< int, int, int *, int *> Turrets;
	for( pos = TurretsSizes.GetStartPosition(); pos != NULL;)
	{
		int ikey, TurSize;
		TurretsSizes.GetNextAssoc( pos, ikey, TurSize);
		BTurretsSizes[ikey] = TurSize ; // Make copy
		Turrets[ikey] = new int [ TurSize ];
	}
	for each (const auto & el in *tlist)
	{
		const NToolID& ToolID = el.first;
		NToolCombined* pTc = static_cast<NToolCombined*>(el.second);
		Turrets	[ ToolID.GetTurretNum() ]
				[ -- BTurretsSizes[ ToolID.GetTurretNum() ] ]
			= ToolID.GetToolNum();
	}

	HTREEITEM ChildItem = tc.GetChildItem(TreeCtrl.NToolItem);
	for( pos = Turrets.GetStartPosition(); pos != NULL;)
	{
		int TurNum, *TurArray;
		Turrets.GetNextAssoc( pos, TurNum, TurArray);

		qsort((void *)TurArray, TurretsSizes[TurNum], sizeof(int), compare_int);
		double * Zdis_array = NULL;
		double * Zdis_vspom = NULL;
		CString str;
		int index;
		for(int i = 0; i < TurretsSizes[TurNum]; ++i)
		{
			ToolID.Set(TurArray[i], TurNum);
			tlist->Lookup(ToolID, pTc);


			CString NewText = tlist->GetFullName(ToolID, pTc);
			if(ChildItem == NULL)
			{
				pItem = tc.InsertItem(NewText, TreeCtrl.NToolItem);
			}
			else
			{
				tc.SetItemText(ChildItem, NewText);
				pItem = ChildItem;
				ChildItem = tc.GetNextSiblingItem(ChildItem);
			}

			HTREEITEM SubChildItem = tc.GetChildItem(pItem);
			if (pTc->num_tools != 1)
			{
				if (Zdis_array) delete Zdis_array;
				Zdis_array = new double [pTc->num_tools];
				if (Zdis_vspom) delete Zdis_vspom;
				Zdis_vspom = new double [pTc->num_tools];
				for (int j = 0; j < pTc->num_tools; ++j)
					Zdis_array[j] = Zdis_vspom[j] = 
					(pTc->GetClass() == AT_MILL) ? ((NToolCombinedMill *)pTc)->GetTool(j)->GetZdisp() : 0.;
				qsort((void *)Zdis_array, pTc->num_tools, sizeof(double), compare_double);
				for (int j=0; j<pTc->num_tools; j++)
				{
					for (int k=0; k<pTc->num_tools; k++)
						if (Zdis_array[j] == Zdis_vspom[k] &&
							Zdis_vspom[k] != -10000.)
						{
							index = k;
							Zdis_vspom[k] = -10000.;
							k = pTc->num_tools;
						}
					if (index != -1)
					{
						CString NewText = pTc->GetTool(index)->GetName();
						if(SubChildItem == NULL)
						{
							cItem = tc.InsertItem(NewText, pItem);
						}
						else
						{
							tc.SetItemText(SubChildItem, NewText);
							cItem = SubChildItem;
							SubChildItem = tc.GetNextSiblingItem(SubChildItem);
						}
						tc.SetItemData(cItem, index);
					}
				}
			}
			// Delete all items from SubChildItem
			for(; SubChildItem != NULL; )
			{
				HTREEITEM ItemB = tc.GetNextSiblingItem(SubChildItem);
				DeleteItem(SubChildItem);
				SubChildItem = ItemB;
			}
			for (int j=0; j<arExpandTool.GetSize(); j++)
				if (ToolID.GetToolNum() == arExpandTool[j])
				{
					tc.Expand(pItem,TVE_EXPAND);
					arExpandTool.RemoveAt(j);
				}
		}
		delete[] Zdis_array;
		delete[] Zdis_vspom;
		delete[] TurArray;
	}
	// Delete all items from ChildItem
	for(; ChildItem != NULL; )
	{
		HTREEITEM ItemB = tc.GetNextSiblingItem(ChildItem);
		DeleteItem(ChildItem);
		ChildItem = ItemB;
	}
	//tc.SortChildren(NToolItem);

	/// Детали
	CMapStringToNStock *slist = pDoc->NGetPartList();
	{
		HTREEITEM ChildItem = tc.GetChildItem(TreeCtrl.NPartItem);
		for each (const auto & el in *slist)
		{
			class NStock* pStock = static_cast<class NStock*>(el.second);
			const CString& key = el.first;
			CString NewText = key;
			if(ChildItem == NULL)
			{
				cItem = tc.InsertItem(NewText, TreeCtrl.NPartItem);
			}
			else
			{
				tc.SetItemText(ChildItem, NewText);
				ChildItem = tc.GetNextSiblingItem(ChildItem);
			}
		}//for
		// Delete all items from ChildItem
		for(; ChildItem != NULL; )
		{
			HTREEITEM ItemB = tc.GetNextSiblingItem(ChildItem);
			DeleteItem(ChildItem);
			ChildItem = ItemB;
		}
	}

	/// Заготовки
	CMapStringToNStock *stockslist = pDoc->NGetStockList();
	if (TreeCtrl.NStocksItem)
	{
		HTREEITEM ChildItem = tc.GetChildItem(TreeCtrl.NStocksItem);
		for each (const auto & el in *stockslist)
		{
			class NStock* pStock = static_cast<class NStock*>(el.second);
			const CString& key = el.first;
			CString NewText = key;
			if(ChildItem == NULL)
			{
				cItem = tc.InsertItem(NewText, TreeCtrl.NStocksItem);
			}
			else
			{
				tc.SetItemText(ChildItem, NewText);
				ChildItem = tc.GetNextSiblingItem(ChildItem);
			}
		}//for
		// Delete all items from ChildItem
		for(; ChildItem != NULL; )
		{
			HTREEITEM ItemB = tc.GetNextSiblingItem(ChildItem);
			DeleteItem(ChildItem);
			ChildItem = ItemB;
		}
	}
	/// Оснастка
	CMapStringToNStock *elist = pDoc->NGetExObjList();
	{
		HTREEITEM ChildItem = tc.GetChildItem(TreeCtrl.NExObjItem);
		for each (const auto & el in *elist)
		{
			class NStock* pStock = static_cast<class NStock*>(el.second);
			const CString& key = el.first;
			CString NewText = key;
			if(ChildItem == NULL)
			{
				cItem = tc.InsertItem(NewText, TreeCtrl.NExObjItem);
			}
			else
			{
				tc.SetItemText(ChildItem, NewText);
				ChildItem = tc.GetNextSiblingItem(ChildItem);
			}
		}//for
		// Delete all items from ChildItem
		for(; ChildItem != NULL; )
		{
			HTREEITEM ItemB = tc.GetNextSiblingItem(ChildItem);
			DeleteItem(ChildItem);
			ChildItem = ItemB;
		}
	}
#endif

	ImageList.SetImageCount(1);
	TreeCtrl.ProcImportMode(((CMainFrame *)AfxGetMainWnd())->IsInImportMode());
	TreeCtrl.ClearTB();
	ProcPosChecks();// MakeTB is called inside ProcPosChecks
}

void CLeftView::OnDestroy() 
{
	pPropPane = NULL;// To prevent FillPropPane problems
	CDockablePane::OnDestroy();
	GetTreeCtrl().DeleteAllItems();
	ImageList.Detach();
}

void CLeftView::OnChProgrText() 
{
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	int i = GetSelProgNum();
	if(i<0)
		return;
	pDoc->ShowProgrText(i);
	((CMainFrame *)AfxGetMainWnd())->ShowTextView();
}

void CLeftView::OnChProgramColor() 
{
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	int i = GetSelProgNum();
	if(i<0)
		return;
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg;
	CString key;
	pPrL->GetAt(i, key, (CObject *&)pProg);

	COLORREF clrInit = pProg->GetColor().GetRGB();
	CColorDialog ColorDialog(clrInit,0,this);
	if(ColorDialog.DoModal() != IDOK)
		return;
	NColor Col;
	Col.SetRGB(ColorDialog.GetColor());
	pProg->SetColor(Col);
	pProg->PrepRenders(NCM_PROJECT.GetDouble("TolLineApp", 0.01));

	GetDocument()->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);
	UpdatePropPane();
}

void CLeftView::OnPaToolsCr() 
{
	((CMainFrame *)AfxGetMainWnd())->PaToolsCr();
	SetFocus();
}

void CLeftView::OnChToolDelete() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
// Take pTool
	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);
	GetTreeCtrl().SelectItem(NULL);// To clear PropPane

	CString ToolText = GetTreeCtrl().GetItemText(CurItem);
	NToolCombined *pToolCon;
	if(ParentItem != NULL) // Item has parent
		if (GetTreeCtrl().GetItemData(ParentItem) == NToolList)
		{
			NToolID ToolID;
			int i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if(!GetDocument()->NGetToolList()->Lookup(ToolID,pToolCon))
				return;
			pDoc->NGetToolList()->RemoveKey(ToolID);
			delete pToolCon;
		}
		else // удаление простого из состава сложного
		{
			ToolText = GetTreeCtrl().GetItemText(ParentItem);
			NToolID ToolID;
			int i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if(!pDoc->NGetToolList()->Lookup(ToolID,pToolCon))
				return;
			pToolCon->RemoveTool((int)GetTreeCtrl().GetItemData(CurItem));
			pToolCon->Changed();
		}
	pDoc->UpdateAllViews(NULL, N_LEFT | N_OGL | N_TOOLCHANGED);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnChToolColor() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);
	
	CString ToolText = GetTreeCtrl().GetItemText(CurItem);
	NToolID ToolID;
	int i = ToolText.Find(" ");
	ToolID.CrFromPrintStr(ToolText.Left(i));

	NToolCombined *pToolCon;
	if (ParentItem != NULL) // Item has parent
	{
		if (GetTreeCtrl().GetItemData(ParentItem) == NToolList)
		{
			if (!GetDocument()->NGetToolList()->Lookup(ToolID, pToolCon))
				return;
			COLORREF clrInit = pToolCon->Color.GetRGB();
			CColorDialog ColorDialog(clrInit, 0, this);
			ColorDialog.m_cc.lpCustColors = NToolCombined::Palet.GetPalPtr(0);
			if (ColorDialog.DoModal() != IDOK)
				return;
			NColor col;
			col.SetRGB(ColorDialog.GetColor());
			col.A() = pToolCon->Color.A();
			pToolCon->SetColor(col);
			GetDocument()->UpdateAllViews(NULL, N_LEFT_ATTR);
		}
		else
		{
			ToolText = GetTreeCtrl().GetItemText(ParentItem);
			NToolID ToolID;
			auto i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if (!GetDocument()->NGetToolList()->Lookup(ToolID, pToolCon))
				return;
			auto index = GetTreeCtrl().GetItemData(CurItem);
			COLORREF clrInit = pToolCon->GetTool(int(index))->GetColor().GetRGB();
			CColorDialog ColorDialog(clrInit, 0, this);
			ColorDialog.m_cc.lpCustColors = NToolCombined::Palet.GetPalPtr(0);
			if (ColorDialog.DoModal() != IDOK)
				return;
			GetDocument()->UpdateAllViews(NULL, N_OGL | N_LEFT);
			NColor TCol = pToolCon->GetTool(int(index))->GetColor();
			TCol.SetRGB(ColorDialog.GetColor());
			pToolCon->GetTool(int(index))->SetColor(TCol);
		}
	}
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	if(!pDoc->Comparison.IsActive())
		if(pDoc->IsThereAnyStock())
			pDoc->CreateStocksRender();
	pDoc->UpdateAllViews(NULL, N_OGL | N_SETTINGS | N_TOOLCHANGED); // N_SETTINGS required for text view regeneration with new tool color
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnChToolPasColor() 
{
	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);

	CString ToolText = GetTreeCtrl().GetItemText(CurItem);
	NToolCombined *pToolCon;
	
	if(ParentItem != NULL) // Item has parent
		if (GetTreeCtrl().GetItemData(ParentItem) == NToolList)
		{
			NToolID ToolID;
			int i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if(!GetDocument()->NGetToolList()->Lookup(ToolID,pToolCon))
				return;
			COLORREF clrInit = pToolCon->ColorPassive.GetRGB();
			CColorDialog ColorDialog(clrInit,0,this);
			ColorDialog.m_cc.lpCustColors = NToolCombined::Palet.GetPalPtr(0);
			if(ColorDialog.DoModal() != IDOK)
				return;
			NColor col;
			col.SetRGB(ColorDialog.GetColor());
			col.A() = pToolCon->Color.A();
			pToolCon->SetColorPassive(col);
		}
		else // простой инструмент
		{
			ToolText = GetTreeCtrl().GetItemText(ParentItem);
			NToolID ToolID;
			int i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if(!GetDocument()->NGetToolList()->Lookup(ToolID,pToolCon))
				return;
			int index = int(GetTreeCtrl().GetItemData(CurItem));
			COLORREF clrInit = pToolCon->GetTool(i)->GetColorPassive().GetRGB();
			CColorDialog ColorDialog(clrInit,0,this);
			ColorDialog.m_cc.lpCustColors = NToolCombined::Palet.GetPalPtr(0);
			if(ColorDialog.DoModal() != IDOK)
				return;
			NColor col;
			col.SetRGB(ColorDialog.GetColor());
			col.A() = pToolCon->Color.A();
			pToolCon->GetTool(index)->SetColorPassive(col);
			if (pToolCon->GetTool(index)->GetType() == 40)
				pToolCon->GetTool(index)->SetColor(col);
		}
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	if(!pDoc->Comparison.IsActive())
		if(pDoc->IsThereAnyStock())
		{
			pDoc->CreateStocksRender();
		}
	pDoc->UpdateAllViews(NULL, N_OGL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

int CLeftView::CreateTreeCtrl(INT_PTR subItemsCount)
{
	TreeCtrl.SubItemToolBars.resize(subItemsCount);
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE /* | TVS_HASLINES | TVS_LINESATROOT */| TVS_HASBUTTONS | TVS_SHOWSELALWAYS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!TreeCtrl.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Tree Control\n");
		return -1;      // fail to create
	}
	if (HIMAGELIST(ImageList) == NULL)
		ImageList.Create(TREE_VIEW_ICON_WIDTH, TREE_VIEW_ICON_HEIGHT, ILC_COLORDDB | ILC_MASK, 1, 10);

	CBitmap AddBmp;
	AddBmp.LoadBitmap(IDB_TREE_VIEW_ADDICONS);
	if (HIMAGELIST(AddImageList) == NULL)
		AddImageList.Create(TREE_VIEW_ADDICON_WIDTH, TREE_VIEW_ICON_HEIGHT, ILC_COLORDDB | ILC_MASK, 4, 10);
	AddImageList.Add(&AddBmp, RGB(0, 0, 0));

	AdjustLayout();

	InitialUpdate();
	return 0;
}

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (CreateTreeCtrl(InitialSubNum) == -1)
		return -1;

	return 0;
}

void CLeftView::OnChMachVisible()
{
	HTREEITEM Item = GetTreeCtrl().GetSelectedItem();
	CString AxisName = GetTreeCtrl().GetItemText(Item);

	CNCMDoc* pDoc = GetDocument();
	MTMachineTool &Mach = pDoc->GetMachTool();
	MTPart *pPart = Mach.GetPart(AxisName);
	if(pPart)
		pPart->SetVisible(!pPart->IsVisible());
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnChMachWFrm()
{
	HTREEITEM Item = GetTreeCtrl().GetSelectedItem();
	CString AxisName = GetTreeCtrl().GetItemText(Item);

	CNCMDoc* pDoc = GetDocument();
	MTMachineTool &Mach = pDoc->GetMachTool();
	MTPart *pPart = Mach.GetPart(AxisName);
	if(pPart)
		pPart->SetWFrame(!pPart->IsWFrame());
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnChProgrVisible() 
{
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	int i = GetSelProgNum();
	if(i<0)
		return;
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg;
	CString key;
	pPrL->GetAt(i,key,(CObject *&)pProg);
	pProg->InvertVisible();
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

// редактирование инструмента (составной или простой)
void CLeftView::OnChToolEdit() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
// Get tool position
	HTREEITEM Item = GetTreeCtrl().GetSelectedItem();
	GetTreeCtrl().SelectItem(NULL);// To clear PropPane
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(Item);
	CString ToolText = GetTreeCtrl().GetItemText(Item);
	NToolID ToolID;
	int i = ToolText.Find(" ");
	ToolID.CrFromPrintStr(ToolText.Left(i));
	bool IsSimpleTool = false;

// определение составной выбран инструмент или нет
	int index;
	if(ParentItem != NULL) // Item has parent
		if (GetTreeCtrl().GetItemData(ParentItem) != NToolList)
		{
			index = int(GetTreeCtrl().GetItemData(Item));
			IsSimpleTool = true;
		}
//Get selected tool

	if (IsSimpleTool)
	{
		ToolText = GetTreeCtrl().GetItemText(ParentItem);
		int i = ToolText.Find(" ");
		ToolID.CrFromPrintStr(ToolText.Left(i));
		OnChSimToolEdit(index, ToolID);
	}
	else
		((CMainFrame *)AfxGetMainWnd())->ChConToolEdit(ToolID);

	GetTreeCtrl().SelectItem(Item);// To restore PropPane
}

// редактирование простого инструмента
void CLeftView::OnChSimToolEdit(int index, const NToolID &ToolID)
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	NToolCombined *pExConTool;
	NTool *pExTool;

	if( !pDoc->NGetToolList()->Lookup(ToolID, pExConTool))
		return;//error
	
 //	NToolCombined *pOldConTool;
	NTool *pTool;
	CString ToolName;
	int ToolType = pExConTool->GetTool(index)->GetType();
	pExTool = pExConTool->GetTool(index);
	
	if( 11 <= ToolType && ToolType <= 14)
	{
		NToolsCrDial CreateDial;
		pExTool->FillDialog(&CreateDial);
		CreateDial.SetOtherTool(true);
		if( CreateDial.DoModal() == IDOK ) 
		{ 			
			pTool=CreateDial.GetNewTool();
			if ( pTool == NULL )
				return;
			pTool->SetColor(pExTool->GetColor());
			pTool->SetColorPassive(pExTool->GetColorPassive());
			pTool->Visible = pExTool->Visible;
			pExConTool->RemoveTool(index);
			pExConTool->AddTool(pTool);
			(*(pDoc->NGetToolList()))[ToolID] = pExConTool;
			pExConTool->Changed();
			pDoc->UpdateAllViews(NULL, N_LEFT | N_TOOLCHANGED);
			pDoc->SetModifiedFlag();
		}
	}
	if( ((20 <= ToolType && ToolType <= 24) || ToolType == 10))
	{
		NToolsCrDiskSheet DiskSheet(IDS_DISK_TOOL);
		pExTool->FillSheet(&DiskSheet);

		if( DiskSheet.DoModal() == IDOK ) 
		{
			pTool=DiskSheet.GetNewTool();
			if ( pTool == NULL )
				return;
			pTool->SetColor(pExTool->GetColor());
			pTool->SetColorPassive(pExTool->GetColorPassive());
			pTool->Visible = pExTool->Visible;
			pExConTool->RemoveTool(index);
			pExConTool->AddTool(pTool);
			(*(pDoc->NGetToolList()))[ToolID] = pExConTool;
			pExConTool->Changed();
			pDoc->UpdateAllViews(NULL, N_LEFT | N_TOOLCHANGED);
			pDoc->SetModifiedFlag();
		}
	}
	if (40 <= ToolType && ToolType <= 43)
	{ 
		NMendrelDial CreateDial;
		
		pExTool->FillDialog(&CreateDial);
		if( CreateDial.DoModal() == IDOK ) 
		{
			pTool=CreateDial.GetNewTool();
			if ( pTool == NULL )
				return;
			pTool->SetColor(pExTool->GetColor());
			pTool->SetColorPassive(pExTool->GetColorPassive());
			pTool->Visible = pExTool->Visible;
			pExConTool->RemoveTool(index);
			pExConTool->AddTool(pTool);
			(*(pDoc->NGetToolList()))[ToolID] = pExConTool;
			
			pDoc->UpdateAllViews(NULL, N_LEFT | N_TOOLCHANGED);
			pDoc->SetModifiedFlag();
		}
	}
}

// вырезать простой инструмент
void CLeftView::OnChToolCut() 
{
	OnChToolCopyAndCut(true);

	GetDocument()->UpdateAllViews(NULL, N_OGL | N_LEFT | N_TOOLCHANGED);
	GetDocument()->SetModifiedFlag();
}

// скопированить простой инструмент
void CLeftView::OnChToolCopy() 
{
	OnChToolCopyAndCut(false);
}

void CLeftView::OnChToolCopyAndCut(bool IsCut)
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);
	GetTreeCtrl().SelectItem(NULL);// To clear PropPane
	
	CString ToolText = GetTreeCtrl().GetItemText(CurItem);
	NToolCombined *pToolCon;
	NTool *pTool;

	// удаление старого скопированного инструмента
	if (pCopyTool)
	{
		delete pCopyTool;
		pCopyTool = NULL;
	}
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(ParentItem != NULL) // Item has parent
		if (GetTreeCtrl().GetItemData(ParentItem) == NToolList)
		{
			// если работаем с составным инструментом
			NToolID ToolID;
			int i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if(!pDoc->NGetToolList()->Lookup(ToolID,pToolCon))
				return;
			pCopyTool = pToolCon->CreateCopyTool();
			if (!pCopyTool)
				return;
			if (IsCut)
			{
				pToolCon->ClearAll();
				pDoc->NGetToolList()->RemoveKey(ToolID);
				delete pToolCon;
			}
		}
		else // если работаем с простым инструментом из состава сложного
		{
			ToolText = GetTreeCtrl().GetItemText(ParentItem);
			NToolID ToolID;
			auto i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if(!pDoc->NGetToolList()->Lookup(ToolID,pToolCon))
				return;
			if (!pCopyTool)
				pCopyTool = new NToolCombined;
			int index = int(GetTreeCtrl().GetItemData(CurItem));
			pTool = pToolCon->GetTool(index);
			pCopyTool->AddTool(pTool->CreateCopyTool());
			if (!pCopyTool)
				return;
			if (IsCut)
				pToolCon->RemoveTool(index);
		}
}

// вставить скопированный инструмент
void CLeftView::OnChToolPaste() 
{
	if (!pCopyTool)
		return;

	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);
	NMapStringToOb *plist = pDoc->NGetToolList();
	
	if(ParentItem != NULL) // Item has parent
	{
		CString ToolText = GetTreeCtrl().GetItemText(CurItem);
		NToolCombined *pToolCon;

		// вставка нового составного инструмента:
		if (GetTreeCtrl().GetItemData(ParentItem) == NToolList &&
			pCopyTool->GetName() != _T("No name"))
		{   	
			// вставка нового (независимого) составного со всеми 
			// его простыми инструментами
			int next_tool_pos = 0, next_con_pos = 0;
			for each (const auto & el in *plist)
			{
				const NToolID& ToolID = el.first;
				NToolCombined* pTc = static_cast<NToolCombined*>(el.second);;
				int tool_pos = ToolID.GetToolNum();
				if (next_tool_pos < tool_pos)
					next_tool_pos = tool_pos;
			}
			next_tool_pos++;
			NToolID ToolID;
			ToolID.Set( next_tool_pos, ToolID.GetTurretNum());

			pToolCon = nullptr;
			auto it = plist->find(ToolID);
			if (it != plist->end())
			{
				pToolCon = static_cast<NToolCombined*>(it->second);
				delete pToolCon;
			}

			pToolCon = pCopyTool->CreateCopyTool();

			pToolCon->Turret = ToolID.GetTurretNum();
			pToolCon->Position = ToolID.GetToolNum();
			(*(pDoc->NGetToolList()))[ToolID]=pToolCon;
		}
		else 
		{	
		// вставка новых простых инструментов:

			// - вставка всех простых из скопированнного составного 
			// в выбранный составной
			// - вставка одного простого
			NToolCombined *pOldToolCon;
			
			if (GetTreeCtrl().GetItemData(ParentItem) != NToolList) 
				ToolText = GetTreeCtrl().GetItemText(ParentItem);
			
			NToolID ToolID;
			int i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));

			auto it = plist->find(ToolID);
			if(it == plist->end())
				return;
			pOldToolCon = static_cast<NToolCombined*>(it->second);
			pToolCon = pOldToolCon->CreateCopyTool();

			
			bool Success = true;
			for (i = 0; i < pCopyTool->num_tools; ++i)
			{
				NTool *pSimCopy = pCopyTool->GetTool(i)->CreateCopyTool();
				if(!pToolCon->AddToolSafe(pSimCopy))
				{
					Success = false;
					delete pSimCopy;
				}
			}
			if(Success)
			{
				delete pOldToolCon;
				(*(pDoc->NGetToolList()))[ToolID]=pToolCon;
				pDoc->SetModifiedFlag();
			}
			else
			{
				delete pToolCon;
				AfxMessageBox(IDS_WRONG_TOOL);
			}
		}
		pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_TOOLCHANGED);
	}
}

void CLeftView::OnPaStockDelete() 
{
	if(!PaStockCanCreate())
		return;
	ClosePositionDlg();
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	std::vector<class NStock*> Stocks;
	for each (const auto & el in pDoc->GetStocksList())
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		Stocks.push_back(pS);
	}
	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	for each (auto pS in Stocks)
		pDoc->DeleteStockWU(pS);
	pDoc->GetUndoHistory().EndGroup(InternalGroup);
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::SetStockVisible() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pStock->Visible = !pStock->IsVisible();
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::SetStockColor() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CString key;
	class NStock *pS = NULL;
	if (!this->GetSelListElem(key, pS))
		return;
	int index = pDoc->NGetStockList()->GetIndex(key);
	
	COLORREF clrInit = pDoc->GetStockColor(index);
	CColorDialog ColorDialog(clrInit,0,this);
	if(ColorDialog.DoModal() != IDOK)
		return;
	pDoc->SetStockColor(ColorDialog.GetColor(), index);
	if(!pDoc->Comparison.IsActive())
		if(pDoc->GetStock(index))
			pDoc->GetStock(index)->CreateRender();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnChToolVisible() 
{
 // Get tool position
	HTREEITEM Item = GetTreeCtrl().GetSelectedItem();
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(Item);
	bool IsSimpleTool = false;

// определение составной выбран инструмент или нет
	int index;
	if(ParentItem != NULL) // Item has parent
		if (GetTreeCtrl().GetItemData(ParentItem) != NToolList)
		{
			index = int(GetTreeCtrl().GetItemData(Item));
			IsSimpleTool = true;
		}
// Set Visible flag to selected tool
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	NToolID ToolID;
	if (IsSimpleTool)
	{
		CString ToolText = GetTreeCtrl().GetItemText(ParentItem);
		int i = ToolText.Find(" ");
		ToolID.CrFromPrintStr(ToolText.Left(i));
	}
	else
	{
		CString ToolText = GetTreeCtrl().GetItemText(Item);
		int i = ToolText.Find(" ");
		ToolID.CrFromPrintStr(ToolText.Left(i));
	}

	NToolCombined *pTool;
	if(!pDoc->NGetToolList()->Lookup(ToolID, pTool))
		return;

	if (IsSimpleTool)
	{
		NTool *pSimTool = pTool->GetTool(index);
		pSimTool->Visible = !pSimTool->Visible;
	}
	else
	{
		pTool->Visible = !pTool->IsVisible();
		for (int i=0; i<pTool->num_tools; i++)
			pTool->GetTool(i)->Visible = !pTool->GetTool(i)->Visible;
	}
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

bool CLeftView::PaStockCanCreate()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return false;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return false;
	}
	pDoc->Measure.DeActivate();
	pDoc->Dimensions.DeActivate();
	return true;
}

void CLeftView::OnPaStockCrBox()
{
	CreateStockBox(NULL);
}

bool CLeftView::CreateStockBox(class NStock *pExStock, int ReplIndex /* = -1*/)
{
	if(!PaStockCanCreate())
		return false;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	class NStock *pStock = NULL;
	if(pExStock == NULL)
		pDoc->pCreateBoxDlg->UnSet();
	else
		if(!pExStock->IsKindOf(RUNTIME_CLASS(NStockBox)))
			pDoc->pCreateBoxDlg->UnSet();
	if (pDoc->pCreateBoxDlg->DoModal(pDoc->GetProgDim(), pExStock) == IDOK)
	{
		if ((pStock = pDoc->pCreateBoxDlg->GetNewStock()) != NULL)
		{
			pStock->SetEdgesAttrs(pDoc->NGetStockList()->GetEdgesAttrs());
			if (ReplIndex == -1)
				pDoc->AddStock(pStock);
			else
				pDoc->ReplaceStock(ReplIndex, pStock);
			AutoPosition(*pDoc->pCreateBoxDlg, *pStock);
			pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_STOCKSTATECHANGE, NULL);
			pDoc->SetModifiedFlag();
			return true;
		}
	}
	delete pStock;
	return false;
}

void CLeftView::OnChStockCrBox()
{
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
	{
		CreateStockBox(NULL);
		return;
	}
	CreateStockBox(pStock);
}

void CLeftView::OnChStockRepBox()
{
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
		return;

	CString key;
	class NStock *pStock = NULL; 	

	if (pDoc->NGetStockList()->size() == 1)
	{
		pDoc->NGetStockList()->GetAt(0, key, (CObject *&)pStock);
	}
	else
	{
		if (!GetSelListElem(key, pStock))
			return;
	}

	int current = pDoc->NGetStockList()->GetIndex(key);
	CreateStockBox(pStock, current);
}

void CLeftView::OnChStockRepCyl()
{
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
		return;

	CString key;
	class NStock *pStock = NULL; 	

	if (pDoc->NGetStockList()->size() == 1)
	{
		pDoc->NGetStockList()->GetAt(0, key, (CObject *&)pStock);
	}
	else
	{
		if (!GetSelListElem(key, pStock))
			return;
	}

	int current = pDoc->NGetStockList()->GetIndex(key);
	CreateStockCyl(pStock, current);
}

void CLeftView::OnChStockRepContbox()
{
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
		return;

	CString key;
	class NStock *pStock = NULL; 

	if (pDoc->NGetStockList()->size() == 1)
	{
		pDoc->NGetStockList()->GetAt(0, key, (CObject *&)pStock);
	}
	else
	{
		if (!GetSelListElem(key, pStock))
			return;
	}

	int current = pDoc->NGetStockList()->GetIndex(key);
	CreateStockContbox(pStock, current);
}

void CLeftView::OnChStockRepStl()
{
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
		return;

	CString key;
	class NStock *pStock = NULL;

	if (pDoc->NGetStockList()->size() == 1)
	{
		pDoc->NGetStockList()->GetAt(0, key, (CObject *&)pStock);
	}
	else
	{
		if (!GetSelListElem(key, pStock))
			return;
	}

	int current = pDoc->NGetStockList()->GetIndex(key);
	CreateStockStl(pStock, current);
}

void CLeftView::OnChStockRepPipe()
{
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
		return;

	CString key;
	class NStock *pStock = NULL;

	if (pDoc->NGetStockList()->size() == 1)
	{
		pDoc->NGetStockList()->GetAt(0, key, (CObject *&)pStock);
	}
	else
	{
		if (!GetSelListElem(key, pStock))
			return;
	}

	int current = pDoc->NGetStockList()->GetIndex(key);
	CreateStockPipe(pStock, current);
}

void CLeftView::OnChStockRepProfile()
{
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
		return;

	CString key;
	class NStock *pStock = NULL;

	if (pDoc->NGetStockList()->size() == 1)
	{
		pDoc->NGetStockList()->GetAt(0, key, (CObject *&)pStock);
	}
	else
	{
		if (!GetSelListElem(key, pStock))
			return;
	}

	int current = pDoc->NGetStockList()->GetIndex(key);
	CreateStockProfile(pStock, CString(), current);
}

void CLeftView::OnChStockRepCone()
{
	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
		return;

	CString key;
	class NStock *pStock = NULL;

	if (pDoc->NGetStockList()->size() == 1)
	{
		pDoc->NGetStockList()->GetAt(0, key, (CObject *&)pStock);
	}
	else
	{
		if (!GetSelListElem(key, pStock))
			return;
	}

	int current = pDoc->NGetStockList()->GetIndex(key);
	CreateStockCone(pStock, current);
}

void CLeftView::ExpandTools(void)
{
	TreeCtrl.Expand(TreeCtrl.NToolItem, TVE_EXPAND);
}

void CLeftView::OnChStockCrCyl()
{
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
	{
		CreateStockCyl(NULL);
		return;
	}
	CreateStockCyl(pStock);
}

void CLeftView::OnChStockCrContbox()
{
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
	{
		CreateStockContbox(NULL);
		return;
	}
	CreateStockContbox(pStock);
}

void CLeftView::OnChStockCrStl()
{
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
	{
		CreateStockStl(NULL);
		return;
	}
	CreateStockStl(pStock);
}

void CLeftView::OnChStockCrCone()
{
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
	{
		CreateStockCone(NULL);
		return;
	}
	CreateStockCone(pStock);
}

void CLeftView::OnChStockCrPipe()
{
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
	{
		CreateStockPipe(NULL);
		return;
	}
	CreateStockPipe(pStock);
}

void CLeftView::OnChStockCrProfile()
{
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
	{
		CreateStockProfile(NULL);
		return;
	}
	CreateStockProfile(pStock);
}

void CLeftView::OnPaStockCrCyl()
{
	CreateStockCyl(NULL);
}

bool CLeftView::CreateStockCyl(class NStock *pExStock, int ReplIndex /* = -1*/)
{
	if(!PaStockCanCreate())
		return false;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	class NStock *pStock = NULL;
	if(pExStock == NULL)
		pDoc->pCreateCylDlg->UnSet();
	else
		if(!pExStock->IsKindOf(RUNTIME_CLASS(NStockCyl)))
			pDoc->pCreateCylDlg->UnSet();
	if(pDoc->pCreateCylDlg->DoModal(pDoc->GetProgCyl()) == IDOK) 
		if((pStock=pDoc->pCreateCylDlg->GetNewStock()) != NULL)
		{
			pStock->SetEdgesAttrs(pDoc->NGetStockList()->GetEdgesAttrs());
			if (ReplIndex == -1)
				pDoc->AddStock(pStock);
			else
				pDoc->ReplaceStock(ReplIndex, pStock);
			AutoPosition(*pDoc->pCreateCylDlg, *pStock);
			pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_STOCKSTATECHANGE, NULL);
			pDoc->SetModifiedFlag();
			return true;
		}	
	delete pStock;
	return false;
}

void CLeftView::OnPaToolsAllvis() 
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	NMapStringToNTool *plist = pDoc->NGetToolList();
	for each (const auto & el in *plist)
	{
		NToolCombined* pTool = static_cast<NToolCombined*>(el.second);;
		pTool->Visible = TRUE;
	}
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaToolsAllunv() 
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	NMapStringToNTool *plist = pDoc->NGetToolList();
	for each (const auto & el in *plist)
	{
		NToolCombined* pTool = static_cast<NToolCombined*>(el.second);;
		pTool->Visible = FALSE;
	}
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaToolsInv() 
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	NMapStringToNTool *plist = pDoc->NGetToolList();
	for each (const auto & el in *plist)
	{
		NToolCombined* pTool = static_cast<NToolCombined*>(el.second);;
		pTool->Visible = !pTool->IsVisible();
	}
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaProgAllvis() 
{
// Make all programs visible
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	for each (const auto & el in *pPrL)
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);
		if(!pProg->IsVisible())
			pProg->InvertVisible();
	}
	
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaProgInv() 
{
// Invert visibility of all programs
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	for each (const auto & el in *pPrL)
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);
		pProg->InvertVisible();
	}
	
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaProgAllunv() 
{
// Make all programs Invisible
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	for each (const auto & el in *pPrL)
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);
		if(pProg->IsVisible())
			pProg->InvertVisible();
	}
	
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaProgFPTraj()
{
// Переключить между вся и пройденная часть траектории
	ChangeProgVisMode();
	UpdatePropPane();
}

void CLeftView::OnNcunitLoad() 
{
// Load configuration file
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	int res = 0;
	NUnitNameDlg und;
	if (und.DoModal(pDoc->GetUnit()->GetConfPath(), pDoc->GetUnit()->GetConfFileName()) != IDOK)
		return;// User did not select new file
	res = pDoc->GetUnit()->SetDef(und.m_name);
// Remove IsEvaluated flag
	static_cast<CMainFrame*>(AfxGetMainWnd())->SetNCUnitName(pDoc->GetUnit()->GetConfFileName());
	pDoc->ImplReplAll();
	pDoc->RegeneratePrograms();
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_OGL | N_STATE | N_LEFT | N_UNIT_CHANGED  | N_PROGRAMCHANGED);
}

void CLeftView::OnNcunitStart() 
{
	((CMainFrame *)AfxGetMainWnd())->RunNcunitStartDlg(0);
}

void CLeftView::OnChToolWf() 
{
	CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
	ASSERT(pDoc);
	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);
	CString ToolText = GetTreeCtrl().GetItemText(CurItem);
	NToolCombined *pToolCon;
	
	if(ParentItem != NULL) // Item has parent
		if (GetTreeCtrl().GetItemData(ParentItem) == NToolList)
		{
			NToolID ToolID;
			auto i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if(!GetDocument()->NGetToolList()->Lookup(ToolID,pToolCon))
				return;
			pToolCon->WFrame = !pToolCon->WFrame;
			for (int i = 0; i < pToolCon->num_tools; ++i)
				pToolCon->GetTool(i)->WFrame = pToolCon->WFrame;
		}
		else // простой в составе сложного
		{
			ToolText = GetTreeCtrl().GetItemText(ParentItem);
			NToolID ToolID;
			auto i = ToolText.Find(" ");
			ToolID.CrFromPrintStr(ToolText.Left(i));
			if(!pDoc->NGetToolList()->Lookup(ToolID,pToolCon))
				return;
			int index = int(GetTreeCtrl().GetItemData(CurItem));
			pToolCon->GetTool(index)->WFrame = !pToolCon->GetTool(index)->WFrame;
		}
	pDoc->UpdateAllViews(NULL, N_OGL,NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

int CLeftView::GetSelProgNum()
{
// Get zero based index of selected program
// returns -1 on error
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NObjTreeCtrl& tc = GetTreeCtrl();
	HTREEITEM item;
	HTREEITEM citem;
	item = tc.GetSelectedItem();
	if(!item)
		return -1;
	citem = tc.GetParentItem(item);
	if(!citem)
		return(-1);
	if(tc.GetItemData(citem) != NProgramList && tc.GetItemData(citem) != NProgramListAux)
		return -1;
	int i = 0;
	for(; (item = tc.GetPrevSiblingItem(item)) != NULL && i < MAXPROG ; ++i);
	if(i == MAXPROG)
		return -1;

	if(tc.GetItemData(citem) == NProgramList)
		return i;
	return pPrL->GetMainProgCount() + i;
}
bool CLeftView::GetSelListElem(CString &key, class NStock *&pStock)
{
// Text of selected list element
// returns "" on error
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	NObjTreeCtrl& tc = GetTreeCtrl();
	HTREEITEM item;
	HTREEITEM citem;
	item = tc.GetSelectedItem();
	if(!item)
		return false;
	citem = tc.GetParentItem(item);
	if(!citem)
	{
		if (tc.GetItemData(item) == NStock)
			citem = item;
		else
			return false;
	}
	CMapStringToNStock *pStL = nullptr;
	bool IsSingleStock = false;
	switch(tc.GetItemData(citem))
	{
	case NExObjList:
		pStL = pDoc->NGetExObjList();
		break;
	case NPartList:
		pStL = pDoc->NGetPartList();
		break;
	case NStock:
		IsSingleStock = true;
	case NStocksList:
		pStL = pDoc->NGetStockList();
		break;
	default:
		return false;
	}
	if(pStL->GetSize() == 1)
	{
		pStL->GetAt(0, key, (CObject *&)pStock);
		if(IsSingleStock)
			key = tc.GetItemText(citem);
	}
	else
	{
		key = tc.GetItemText(item);
		auto it = pStL->find(key);
		if (it == pStL->end())
			return false;
		pStock = static_cast<class NStock*>(it->second);
	}
	return true;
}

void CLeftView::OnPaProgrCr() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
// Create empty program file and load it to project and to text view
	CString filename("new.nc");
	CString filter,ext;
	int index = filename.ReverseFind('.');
	for(int i = index + 1; i < filename.GetLength(); i++)
		ext += filename[i];
	filter = "*." + ext +"|*." + ext + "|";
	CString key;
	key.LoadString(IDS_ALLFILES_FILTER);
	filter += key;
	BFileDialogWPath fd(_T("LastProgPath"), FALSE, ext, filename,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		filter);
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return;
	if(filename == "") 
		return;
	CStdioFile f(filename, CFile::typeText | CFile::modeWrite | CFile::modeCreate);
	f.Close();
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	NProgram *pP = pDoc->LoadNewProgram(filename);

	pDoc->NAddMainProgram(filename, pP );
	pDoc->RegeneratePrograms();
	pDoc->SetProgrTDT(filename);

	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_LEFT | N_TEXT  | N_PROGRAMCHANGED);
}

void CLeftView::OnPaProgrCrAux() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
// Create empty program file and load it to project and to text view
	CString filename("new.nc");
	CString filter,ext;
	int index = filename.ReverseFind('.');
	for(int i = index + 1; i < filename.GetLength(); i++)
		ext += filename[i];
	filter = "*." + ext +"|*." + ext + "|";
	CString key;
	key.LoadString(IDS_ALLFILES_FILTER);
	filter += key;
	BFileDialogWPath fd(_T("LastProgPath"), FALSE, ext, filename,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		filter);
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return;
	if(filename == "") 
		return;
	CString str;
	CStdioFile f(filename,
		CFile::typeText | CFile::modeWrite | CFile::modeCreate);
	f.Close();
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	NProgram *pP = pDoc->LoadNewProgram(filename);

	pDoc->NAddAuxProgram(filename, pP );
	pDoc->RegeneratePrograms();
	pDoc->SetProgrTDT(filename);

	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_LEFT | N_TEXT  | N_PROGRAMCHANGED);
}

void CLeftView::AutoPosition(NSetupSetBoxPointDlg &SetBoxPointDlg, const class NStock &Stock)
{
	// Make auto positioning
	if(SetBoxPointDlg.NeedAuto())
	{
		((CMainFrame *)AfxGetMainWnd())->OnSetupAutops();
	}
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	SetBoxPointDlg.StockGabar = Stock.GetGabar();	
	BPoint P, N;
	SetBoxPointDlg.CalcToolPos(P, N);
	pDoc->SetBoxPointApply(P, N);
	if (SetBoxPointDlg.NeedFillG54())
		((CMainFrame *)AfxGetMainWnd())->WriteCS_OK(0);
	pDoc->SetBoxPointEnd(); // To deactivate PosTool
}

void CLeftView::OnPaStockCrStl()
{
	CreateStockStl(nullptr);
}

bool CLeftView::CreateStockStl(class NStock *pExStock, int ReplIndex /* = -1*/)
{
	if(!PaStockCanCreate())
		return false;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	class NStock *pStock = nullptr;
	if(pExStock == NULL)
		pDoc->pCreateStlDlg->UnSet();
	else
	{
		if(!(pExStock->IsKindOf(RUNTIME_CLASS(NStockStl)) || (pExStock->IsKindOf(RUNTIME_CLASS(NStockBSP)))))
		{
			pDoc->pCreateStlDlg->UnSet();
			pExStock = NULL;
		}
	}
	const bool SaveStock = (pDoc->pCreateStlDlg->DoModal(pExStock) == IDOK);
	if(SaveStock)
	{
		if((pStock = pDoc->pCreateStlDlg->GetNewStock()) != NULL)
		{
			pStock->SetEdgesAttrs(pDoc->NGetStockList()->GetEdgesAttrs());
			AutoPosition(*pDoc->pCreateStlDlg, *pStock);
			const MBody *pBody = pStock->GetBody();
			if(pBody)
			{
				if(!pBody->Check())
					if(AfxMessageBox(IDS_MES_STOCKERRORS, MB_YESNO) != IDYES)
					{
						delete pStock;
						return false;
					}
			}
			if (ReplIndex == -1)
				pDoc->AddStock(pStock);
			else
				pDoc->ReplaceStock(ReplIndex, pStock);
			pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_STOCKSTATECHANGE, NULL);
			pDoc->SetModifiedFlag();
			return true;
		}
	}
	return false;
}

void CLeftView::OnCHPROGRDXFExport() 
{
	NProgram *pProg = GetSelProg();
	if(!pProg)
		return;
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if(pDoc->NGetProgramList()->HaveCompTraj())
		DXFOut Export(&pProg->GetCompGeomArray());
	if(pDoc->NGetProgramList()->HaveProgTraj())
		DXFOut Export(&pProg->GetGeomArray());
}

BOOL CLeftView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	if(message ==TVN_GETINFOTIP)
	{
		message++;
	}
	if(message == WM_LBUTTONDOWN)
	{
		message++;
	}
	return CDockablePane::OnChildNotify(message, wParam, lParam, pLResult);
}

void CLeftView::OnCHPROGRAPTexport() 
{
#ifdef _2D
	AfxMessageBox(IDS_MESSAGE_2D);
#endif
#ifndef _2D
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	int i = GetSelProgNum();
	if(i<0)
		return;
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg;
	CString key;
	pPrL->GetAt(i,key,(CObject *&)pProg);
	pDoc->GetUnit()->AptExport(key);
	pDoc->UpdateAllViews(NULL, N_OGL, NULL);
#endif
}

void CLeftView::OnPaProgPart() 
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->NGetProgramList()->PartialDraw = TrajPart::TP_PART;
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);
}

void CLeftView::OnPaProgFull() 
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->NGetProgramList()->PartialDraw = TrajPart::TP_WHOLE;
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);
}

void CLeftView::OnPaProgWorm() 
{
// Set pDoc->PartialDraw = WORM
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->NGetProgramList()->PartialDraw = TrajPart::TP_WORM;
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);
}

void CLeftView::OnPaProgDelAllProg()
{
	if (AfxMessageBox(IDS_MES_DELALLPROG, MB_ICONQUESTION | MB_YESNO) != IDYES)
		return;

	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CloseSectProgDlg();

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//Clear ProgramList
	CString key;
	NProgram* pProg;
	for (auto i = pDoc->GetCurChannel()->NGetProgramListCn()->GetMainProgCount() - 1; i >= 0; --i)
	{
		if (pDoc->GetCurChannel()->NGetProgramListCn()->GetAt(i, key, (CObject*&)pProg))
			CNCMDoc::DeleteProgObj(pProg);
	}
	pDoc->GetCurChannel()->DeleteAllProgCn();
	pDoc->RegeneratePrograms();

	pDoc->UpdateAllViews(NULL, N_OGL | N_NEW_TEXT | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaProgDelAllSubProg()
{
	if (AfxMessageBox(IDS_MES_DELALLSUBPROG, MB_ICONQUESTION | MB_YESNO) != IDYES)
		return;

	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CloseSectProgDlg();

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//Clear ProgramList
	CString key;
	NProgram* pProg;
	for (auto i = pDoc->GetCurChannel()->NGetProgramListCn()->size() - 1; i >= pDoc->GetCurChannel()->NGetProgramListCn()->GetMainProgCount(); --i)
	{
		if (pDoc->GetCurChannel()->NGetProgramListCn()->GetAt(i, key, (CObject*&)pProg))
			CNCMDoc::DeleteProgObj(pProg);
	}
	pDoc->GetCurChannel()->DeleteAllSubProgCn();
	pDoc->RegeneratePrograms();

	pDoc->UpdateAllViews(NULL, N_OGL | N_NEW_TEXT | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::ChangeProgVisMode()
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->NGetProgramList()->PartialDraw != TrajPart::TP_WHOLE)
		pDoc->NGetProgramList()->PartialDraw = TrajPart::TP_WHOLE;
	else
		pDoc->NGetProgramList()->PartialDraw = TrajPart::TP_PART;
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);

}

void CLeftView::OnPaToolsUse() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	for (int ChInd = 0; ChInd < pDoc->GetChannelsCount(); ++ChInd)
	{
		auto pChannel = pDoc->GetChannel(ChInd);
		pChannel->MakeUsedTools();
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_TOOLCHANGED);	
}

void CLeftView::OnPaToolsImpncm() 
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	theApp.SetImportMode();
	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL, N_LEFT | N_TOOLCHANGED);	
}

void CLeftView::OnPaToolsShape() 
{
// Set/remove ShapeVisible
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	NTool::FlipShapeDraw();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);
	UpdatePropPane();
}



void CLeftView::OnPaMachLoad()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();
	pDoc->OnMtLoad();
	static_cast<CMainFrame*>(AfxGetMainWnd())->SetMachName(pDoc->GetMachTool().GetFileName());
}

void CLeftView::OnPaMachVisible()
{
	MTMachineTool &MT = GetDocument()->GetMachTool();
	MT.SetVisible(!MT.IsVisible());
	CNCMDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	UpdatePropPane();
}

void CLeftView::OnUpdatePaMachVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	CNCMDoc* pDoc = GetDocument();
	MTMachineTool &MachTool = pDoc->GetMachTool();
	pCmdUI->Enable(MachTool.GetCurConfig().IsDefined());
	pCmdUI->SetCheck(MachTool.IsVisible());
}

void CLeftView::OnUpdateChMachVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;

	HTREEITEM Item = GetTreeCtrl().GetSelectedItem();
	CString AxisName = GetTreeCtrl().GetItemText(Item);

	CNCMDoc* pDoc = GetDocument();
	MTMachineTool &Mach = pDoc->GetMachTool();
	MTPart *pPart = Mach.GetPart(AxisName);
	if (pPart)
		pCmdUI->SetCheck(pPart->IsVisible());
}

void CLeftView::OnUpdateChMachWFrm(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;

	HTREEITEM Item = GetTreeCtrl().GetSelectedItem();
	CString AxisName = GetTreeCtrl().GetItemText(Item);

	CNCMDoc* pDoc = GetDocument();
	MTMachineTool &Mach = pDoc->GetMachTool();
	MTPart *pPart = Mach.GetPart(AxisName);
	if (pPart)
		pCmdUI->SetCheck(pPart->IsWFrame());
}

void CLeftView::OnUpdatePaToolsShape(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	CNCMDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->IsThereAnyTool());
	pCmdUI->SetCheck(NTool::IsShapeVisible());
}

void CLeftView::OnChStockExstl()
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
	{
		AfxMessageBox(IDS_COMP_NOSTOCK);
		return;
	}

	CString filename;
	CString ext;
	ext.LoadString(IDS_STL_EXT);
	CString filter;
	filter = "*." + ext;
	filter = filter + _T("|") + filter + _T("||");
	BFileDialogWPath fd(_T("LastStockPath"), FALSE, ext, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else
		return;

	CStdioFile f;
	if (!f.Open(filename,
		CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return;

	CString key;
	class NStock *pStock = NULL;
	if (pDoc->IsThereMoreThanOneStock())
	{
		if (GetSelListElem(key, pStock))
		{
			pStock->ExportSTL(&f);
		}
	}
	else
		pDoc->GetStock(0)->ExportSTL(&f);
	f.Close();
}

void CLeftView::OnChStockExp()
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();
	if (!pDoc->IsThereAnyStock())
	{
		AfxMessageBox(IDS_COMP_NOSTOCK);
		return;
	}

	CString filename;
	CString ext;
	ext.LoadString(IDS_BSP_EXT);
	CString filter;
	filter = "*." + ext;
	filter = filter + _T("|") + filter + _T("||");
	BFileDialogWPath fd(_T("LastStockPath"), FALSE, ext, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else
		return;
	CFile fu;//WRITE PACKED
	if (!fu.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		AfxMessageBox(IDS_BSP_SAVE_ERR);
		return;
	}
	CMemFile f(2 * 1024 * 1024);
	if (pDoc->IsThereMoreThanOneStock())
	{
		CString key;
		class NStock *pStock = NULL;
		if (GetSelListElem(key, pStock))
		{
			if (theApp.GetGlobalState().GetModelMode() == NGlobalState::BSP)
				pStock->ExportBSP(&f);
			else
				pStock->ExportDX(&f);
		}
	}
	else
	{
		if (theApp.GetGlobalState().GetModelMode() == NGlobalState::BSP)
			pDoc->GetStock(0)->ExportBSP(&f);
		else
			pDoc->GetStock(0)->ExportDX(&f);
	}
	CNCMDoc::WritePacked(f, fu);
}

void CLeftView::OnStockImp()
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	AfxMessageBox(IDS_BSP_LOAD_ERR);
}

void CLeftView::OnStockExstc()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();	
	if(!pDoc->IsThereAnyStock())
	{
		AfxMessageBox(IDS_COMP_NOSTOCK);
		return;
	}
	CString key;
	class NStock *pStock = NULL;
	if (pDoc->IsThereMoreThanOneStock())
	{
		if(GetSelListElem(key, pStock))
		{
			if(!pStock->ExportStC())
				AfxMessageBox(IDS_EXSTC_ORDER);
			return;
		}
	}
	if(!pDoc->GetStock(0)->ExportStC())
		AfxMessageBox(IDS_EXSTC_ORDER);
}

void CLeftView::OnStockExdxf()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;

	CNCMDoc* pDoc = GetDocument();	
	if(!pDoc->IsThereAnyStock())
	{
		AfxMessageBox(IDS_COMP_NOSTOCK);
		return;
	}
	CString key;
	class NStock *pStock = NULL;
	if (pDoc->IsThereMoreThanOneStock())
	{
		if(GetSelListElem(key, pStock))
		{
			if(!pStock->ExportDXF())
				AfxMessageBox(IDS_EXSTC_ORDER);
			return;
		}
	}
	if(!pDoc->GetStock(0)->ExportDXF())
		AfxMessageBox(IDS_EXSTC_ORDER);
}

void CLeftView::OnPaExobjCrStl()
{
	FixtureCrStl();
}

void CLeftView::OnPaPartCrStl()
{
	PartCrStl();
}

void CLeftView::OnPaStockVisible()
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = GetDocument();
	if (pDoc->NGetStockList()->GetSize() == 1)
	{
		pDoc->NGetStockList()->GetStock(0)->Visible = !pDoc->NGetStockList()->GetStock(0)->Visible;
		pDoc->NGetStockList()->SetVisible(pDoc->NGetStockList()->GetStock(0)->IsVisible());
	}
	else
	{
		pDoc->NGetStockList()->SetVisible(!pDoc->NGetStockList()->IsVisible());
	}
	AfxGetMainWnd()->SendMessage(WM_APP_STOCK_VISIBLE);
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	UpdatePropPane();
}

void CLeftView::OnPaProgVisible()
{
	CNCMDoc* pDoc = GetDocument();
	pDoc->NGetProgramList()->Visible = !pDoc->NGetProgramList()->Visible;
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);
	UpdatePropPane();
}

void CLeftView::OnPaPartVisible()
{
	CNCMDoc* pDoc = GetDocument();
	pDoc->NGetPartList()->SetVisible(!pDoc->NGetPartList()->IsVisible());
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);
	UpdatePropPane();
}

void CLeftView::OnPaExobjVisible()
{
	CNCMDoc* pDoc = GetDocument();
	pDoc->NGetExObjList()->SetVisible(!pDoc->NGetExObjList()->IsVisible());
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR);
	UpdatePropPane();
}

void CLeftView::LoadFixtures(const std::vector<CString>& names)
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMapStringToNStock *elist = pDoc->NGetExObjList();
	// For all selected files
	for (size_t i = 0; i < names.size(); ++i)
	{
		CString NameCopy = names[i];
		const StockFileType StockType = MTEdit::Conv2STL(NameCopy, NExObjList);// NameCopy can be changed here
		if(StockType == SFT_UNKNOWN)
			continue;// ignore file if error while convertion
		class NStock *pPart = nullptr;
		switch (StockType)
		{
		case SFT_STL:
		{
			NStockStl *pPartStl = (NCMComData::GetType() == AT_WELD) ? new NWeldPart : new NStockStl;
			if (pPartStl == nullptr)
				continue;
			pPartStl->CreateEx(NameCopy);
			if (!pPartStl->IsCreated)
			{
				delete pPartStl;
				pPartStl = nullptr;
				continue;
			}
			pPart = pPartStl;
			break;
		}
		case  SFT_CONT:
		{
			auto Type = NCMComData::GetType();
			if (Type != AT_MILL_TURN && Type != AT_TURN)
				continue;
			NStockCont *pPartCont = LoadStockFrCont(NameCopy);
			if (pPartCont == nullptr)
				continue;
			pPart = pPartCont;
			break;
		}
		default:
			continue;
		}
		NColor Col;
		Col.SetRGB(NToolCombined::Palet.GetRecurColor(0));
		pPart->SetColor(Col);
		pDoc->AddExobjWU(pPart);
	}
	AfxGetMainWnd()->PostMessage(WM_APP_UPDATE_PANES, N_OGL | N_LEFT);// PostMessage is nesessary for NObjDropTarget::OnDrop works properly
	pDoc->SetModifiedFlag();
}

NStockCont *CLeftView::LoadStockFrCont(const CString &FileName)
{
	StContOut Import;
	NCadrGeom *GeomArr;
	auto Num = Import.ReadFromFile(GeomArr, FileName);
	if (Num == 0)
	{
		AfxMessageBox(IDS_STOCK_ERREAD);
		return nullptr;
	}
	NStockCont *pBufStock = new NStockCont();
	pBufStock->Create(GeomArr, int(Num), BPoint(0., 0., 0., 1.), FileName, false);
	delete[] GeomArr;
	if (!pBufStock->IsCreated)
	{
		delete pBufStock;
		pBufStock = nullptr;
	}
	return pBufStock;
}

//#include "..\mashine\StlRend.h"
bool CLeftView::FixtureCrStl()
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString filter;
	filter.LoadString(IDS_PART_FILTER);
	BFileDialogWPath fd(_T("LastFixtPath"), TRUE, NULL, NULL, NULL, filter);
	fd.m_ofn.Flags = fd.m_ofn.Flags | OFN_ALLOWMULTISELECT;
	CHAR buf[4096] = "";
	fd.m_ofn.lpstrFile = buf;
	fd.m_ofn.nMaxFile = 4096;
	if(fd.DoModal() != IDOK)
		return false;
	std::vector<CString> names;
	for(POSITION pos = fd.GetStartPosition() ; pos != NULL ;)
	{
		names.push_back(fd.GetNextPathName(pos));
	}
	//TEMP StlRend test
	//StlRend Tst;
	//Tst.create(names[0]);
	//int facesCount, stride;
	//int* sharpInd;
	//std::vector<float> points;
	//Tst.getMesh(0, facesCount, stride, points, sharpInd);
	//int iMax = 0;
	//for (int i = 1; i < sharpInd[0]; ++i)
	//	iMax = max(iMax, sharpInd[i]);
	//END:TEMP
	LoadFixtures(names);
	return true;
}

void CLeftView::LoadParts(const std::vector<CString>& names)
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	CMapStringToNStock *elist = pDoc->NGetPartList();
	// For all selected files
	for (size_t i = 0; i < names.size(); ++i)
	{
		CString NameCopy = names[i];
		const StockFileType StockType = MTEdit::Conv2STL(NameCopy, NPartList);// NameCopy can be changed here
		if (StockType == SFT_UNKNOWN)
			continue;// ignore file if error while convertion
		class NStock *pPart = nullptr;
		switch (StockType)
		{
		case SFT_STL:
		{
			NStockStl *pPartStl = (NCMComData::GetType() == AT_WELD) ? new NWeldPart : new NStockStl;
			if (pPartStl == nullptr)
				continue;
			pPartStl->CreateEx(NameCopy);
			if (!pPartStl->IsCreated)
			{
				delete pPartStl;
				pPartStl = nullptr;
				continue;
			}
			pPart = pPartStl;
			break;
		}
		case  SFT_CONT:
		{
			auto Type = NCMComData::GetType();
			if (Type != AT_MILL_TURN && Type != AT_TURN)
				continue;
			NStockCont *pPartCont = LoadStockFrCont(NameCopy);
			if (pPartCont == nullptr)
				continue;
			pPart = pPartCont;
			break;
		}
		default:
			continue;
		}
		NColor Col;
		if (!(!pDoc->IsThereAnyPart() && i == 0))
			Col.SetRGB(NToolCombined::Palet.GetRecurColor(0));
		else
		{
			NColor *c;
			if (NCM_PROJECT.Colors.Lookup("ДетальНомер1", c) && (c->r() != 0 || c->g() != 0 || c->b() != 0))
				Col = *c;
			else
				Col.SetRGB(NToolCombined::Palet.GetRecurColor(0));
		}
		pPart->SetColor(Col);
		pDoc->AddPartWU(pPart);
	}
	AfxGetMainWnd()->PostMessage(WM_APP_UPDATE_PANES, N_OGL | N_LEFT);// PostMessage is nesessary for NObjDropTarget::OnDrop works properly
	pDoc->SetModifiedFlag();
}

void CLeftView::LoadPartsWDial(const std::vector<CString>& names)
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	CMapStringToNStock *elist = pDoc->NGetPartList();
	// For all selected files
	for (size_t i = 0; i < names.size(); ++i)
	{
		CString NameCopy = names[i];
		const StockFileType StockType = MTEdit::Conv2STL(NameCopy, NPartList);// NameCopy can be changed here
		if (StockType == SFT_UNKNOWN)
			continue;// ignore file if error while convertion
		class NStock *pPart = nullptr;
		switch (StockType)
		{
		case SFT_STL:
		{
			NStockStl *pPartStl = (NCMComData::GetType() == AT_WELD) ? new NWeldPart : new NStockStl;
			if (pPartStl == nullptr)
				continue;
			pPartStl->CreateEx(NameCopy);
			if (!pPartStl->IsCreated)
			{
				delete pPartStl;
				pPartStl = nullptr;
				continue;
			}
			pPart = pPartStl;
			NColor Col;
			if (!(!pDoc->IsThereAnyPart() && i == 0))
				Col.SetRGB(NToolCombined::Palet.GetRecurColor(0));
			else
			{
				NColor *c;
				if (NCM_PROJECT.Colors.Lookup("ДетальНомер1", c) && (c->r() != 0 || c->g() != 0 || c->b() != 0))
					Col = *c;
				else
					Col.SetRGB(NToolCombined::Palet.GetRecurColor(0));
			}
			pPart->SetColor(Col);
			pDoc->AddPartWU(pPart);
			break;
		}
		case  SFT_CONT:
		{
			auto Type = NCMComData::GetType();
			if (Type != AT_MILL_TURN && Type != AT_TURN)
				continue;
			PaPartCrProfile(NameCopy);
		}
		default:
			continue;
		}
	}
	AfxGetMainWnd()->PostMessage(WM_APP_UPDATE_PANES, N_OGL | N_LEFT);// PostMessage is nesessary for NObjDropTarget::OnDrop works properly
	pDoc->SetModifiedFlag();
}

bool CLeftView::PartCrStl()
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return false;
	}
	CString filter;
	filter.LoadString(IDS_PART_FILTER);
	BFileDialogWPath fd(_T("LastPartPath"), TRUE, NULL, NULL, NULL, filter);
	fd.m_ofn.Flags = fd.m_ofn.Flags | OFN_ALLOWMULTISELECT;
	CHAR buf[32768] = "";
	fd.m_ofn.nMaxFile = 32768;
	fd.m_ofn.lpstrFile = buf;
	if(fd.DoModal() != IDOK)
		return false;
	CWaitCursor w;
	std::vector<CString> names;
	for(POSITION pos = fd.GetStartPosition() ; pos != NULL ;)
	{
		names.push_back(fd.GetNextPathName(pos));
	}
	LoadParts(names);
	return true;
}

void CLeftView::OnPaPartDelete()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	ClosePositionDlg();
	CNCMDoc* pDoc = GetDocument();
	pDoc->DeletePartList();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}



void CLeftView::OnPaToolsTurn()
{
	((CMainFrame*)AfxGetMainWnd())->PaToolsCr();
}

void CLeftView::OnPaToolsMt()
{
	((CMainFrame*)AfxGetMainWnd())->PaToolsCr();
}

NProgram * CLeftView::GetSelProg(void)
{
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	int i = GetSelProgNum();
	if(i < 0)
		return NULL;
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg;
	CString key;
	pPrL->GetAt(i,key,(CObject *&)pProg);
	return pProg;
}

void CLeftView::OnChProgrStcontexport()
{
	NProgram *pProg = GetSelProg();
	if(!pProg)
		return;
	CNCMDoc *pDoc = GetDocument();
	ASSERT(pDoc);
	if(pDoc->NGetProgramList()->HaveCompTraj())
		StContOut Export(&pProg->GetCompGeomArray());
	if(pDoc->NGetProgramList()->HaveProgTraj())
		StContOut Export(&pProg->GetGeomArray());
}

void CLeftView::OnPaStockCrProfile()
{
	CreateStockProfile(NULL);
}

bool CLeftView::CreateStockProfile(class NStock *pExStock, const CString &InitFileName, int ReplIndex /* = -1*/)
{
	if(!PaStockCanCreate())
		return false;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	class NStock *pStock = NULL;
	if (pExStock == NULL)
	{
		pDoc->pCreateContDlg->UnSet();
	}
	else
		if(!(pExStock->IsKindOf(RUNTIME_CLASS(NStockCont)) || pExStock->IsKindOf(RUNTIME_CLASS(NStockBSP))))
		{
			pDoc->pCreateStlDlg->UnSet();
			pExStock = NULL;
		}
	pDoc->pCreateContDlg->m_FileName = InitFileName;
	if( pDoc->pCreateContDlg->DoModal(pExStock) == IDOK)
	{
		if((pStock = pDoc->pCreateContDlg->GetNewStock()) != NULL)
		{
			pStock->SetEdgesAttrs(pDoc->NGetStockList()->GetEdgesAttrs());
			if (ReplIndex == -1)
				pDoc->AddStock(pStock);
			else
				pDoc->ReplaceStock(ReplIndex, pStock);
			AfxGetMainWnd()->PostMessage(WM_APP_UPDATE_PANES, N_OGL | N_LEFT);// PostMessage is nesessary for NObjDropTarget::OnDrop works properly
			pDoc->SetModifiedFlag();
			return true;
		}
	}
	else
		delete pStock;
	return false;
}

void CLeftView::OnPaStockCrPipe()
{
	CreateStockPipe(NULL);
}

bool CLeftView::CreateStockPipe(class NStock *pExStock, int ReplIndex /* = -1*/)
{
	if(!PaStockCanCreate())
		return false;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	class NStock *pStock = NULL;

	if (pDoc->pCreatePipDlg == nullptr)
	{
		pDoc->pCreatePipDlg = new NStockCrPipDial(IDD_STOCK_CREATE_PIPE);
		pDoc->pCreatePipDlg->UnSet();
	}
	else if (pDoc->pCreatePipDlg->GetID() != IDD_STOCK_CREATE_PIPE)
	{
		delete pDoc->pCreatePipDlg;
		pDoc->pCreatePipDlg = new NStockCrPipDial(IDD_STOCK_CREATE_PIPE);
		pDoc->pCreatePipDlg->UnSet();
	}
	pDoc->pCreatePipDlg->SetMillDialog(true);
	if (pDoc->pCreatePipDlg->DoModal(pDoc->GetProgDim(), pExStock) == IDOK)
	{
		if ((pStock = pDoc->pCreatePipDlg->GetNewStock()) != NULL)
		{
			pStock->SetEdgesAttrs(pDoc->NGetStockList()->GetEdgesAttrs());
			if (ReplIndex == -1)
				pDoc->AddStock(pStock);
			else
				pDoc->ReplaceStock(ReplIndex, pStock);
			pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_STOCKSTATECHANGE, NULL);
			pDoc->SetModifiedFlag();
			return true;
		}
	}
	else
		delete pStock;
	return false;
}
void CLeftView::OnPaStockCrCone()
{
	CreateStockCone(NULL);
}

bool CLeftView::CreateStockCone(class NStock *pExStock, int ReplIndex /* = -1*/)
{
	if(!PaStockCanCreate())
		return false;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	class NStock *pStock = NULL;

	if (pDoc->pCreatePipDlg == nullptr)
	{
		pDoc->pCreatePipDlg = new NStockCrPipDial(IDD_STOCK_CREATE_CONE);
		pDoc->pCreatePipDlg->UnSet();
	}
	else if (pDoc->pCreatePipDlg->GetID() != IDD_STOCK_CREATE_CONE)
	{
		delete pDoc->pCreatePipDlg;
		pDoc->pCreatePipDlg = new NStockCrPipDial(IDD_STOCK_CREATE_CONE);
		pDoc->pCreatePipDlg->UnSet();
	}
	pDoc->pCreatePipDlg->SetMillDialog(false);
	if (pDoc->pCreatePipDlg->DoModal(pDoc->GetProgDim(), pExStock) == IDOK)
		if((pStock = pDoc->pCreatePipDlg->GetNewStock()) != NULL)
		{
			pStock->SetEdgesAttrs(pDoc->NGetStockList()->GetEdgesAttrs());
			if (ReplIndex == -1)
				pDoc->AddStock(pStock);
			else
				pDoc->ReplaceStock(ReplIndex, pStock);
			pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_STOCKSTATECHANGE, NULL);
			pDoc->SetModifiedFlag();
			return true;
		}	
	delete pStock;
	return false;
}

void CLeftView::OnPaPartCrProfile()
{
	PaPartCrProfile(CString());
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT, NULL);
}

void CLeftView::OnPaStockCrContbox()
{
	CreateStockContbox(NULL);
}

bool CLeftView::CreateStockContbox(class NStock *pExStock, int ReplIndex /* = -1*/)
{
	if(!PaStockCanCreate())
		return false;

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	class NStock *pStock = NULL;
	if(pExStock == NULL)
		pDoc->pCreateContMDlg->UnSet();
	else
		if(!pExStock->IsKindOf(RUNTIME_CLASS(NStockSweep)))
		{
			pDoc->pCreateContMDlg->UnSet();
			pExStock = NULL;
		}
	if( pDoc->pCreateContMDlg->DoModal(pExStock) == IDOK)
	{
		if((pStock = pDoc->pCreateContMDlg->GetNewStock()) != NULL)
		{
			pStock->SetEdgesAttrs(pDoc->NGetStockList()->GetEdgesAttrs());
			if (ReplIndex == -1)
				pDoc->AddStock(pStock);
			else
				pDoc->ReplaceStock(ReplIndex, pStock);
			pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT | N_STOCKSTATECHANGE,NULL);
			pDoc->SetModifiedFlag();
			return true;
		}
	}
	else
		delete pStock;
	return false;
}

void CLeftView::MoveItem(HTREEITEM Source, HTREEITEM Target)
{
	CNCMDoc* pDoc = GetDocument();
	NObjTreeCtrl &tc = GetTreeCtrl();
// move inName program befor curName program
	CString inName = tc.GetItemText(Source);
	CString curName = tc.GetItemText(Target);
	NMapStringToOb *plist = pDoc->NGetProgramList();
	CString key;
	for each(const auto& el in *plist)
	{
		key = el.first;
		if( key.Find( inName ) != -1 )
			break;
	}
	inName = key;
	for each (const auto & el in *plist)
	{
		key = el.first;
		if( key.Find( curName ) != -1 )
			break;
	}
	curName = key;
	plist->MoveBefore(curName,inName);
	pDoc->SetProgrTDT(pDoc->GetProgrTDT()); // To change pDoc->ProgrToDispTextNum
	NUpdate();
	pDoc->RegeneratePrograms();
	pDoc->UpdateAllViews(nullptr, N_PROGRAMCHANGED);
}

void CLeftView::MoveExObj(HTREEITEM Source, HTREEITEM Target)
{
	HTREEITEM SourceParent = GetTreeCtrl().GetParentItem(Source);
	if (SourceParent == NULL)
		return;
	if (GetTreeCtrl().GetItemData(SourceParent) != NExObjList)
		return;
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString key = GetTreeCtrl().GetItemText(Source);
	auto it = pDoc->NGetExObjList()->find(key);
	if (it == pDoc->NGetExObjList()->end())
		return;
	class NStock* pExObj = static_cast<class NStock*>(it->second);

	class NStock* pNewObj = nullptr;
	pExObj->CreateCopyTo(pNewObj);
	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	pDoc->DeleteFixtWU(pExObj);
	switch (GetTreeCtrl().GetItemData(Target))
	{
	case NPartList:
		pDoc->AddPartWU(pNewObj);
		break;
	case NStock:
	case NStocksList:
		auto Type = NCMComData::GetType();
		if (Type == AT_MILL)
			pDoc->AddStock(pNewObj);
		break;
	}
	pDoc->GetUndoHistory().EndGroup(InternalGroup);

	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::UpdatePropPane()
{
	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	if (CurItem == NULL)
		return;
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);
	FillPropPane(CurItem, ParentItem);
}

void CLeftView::ProcClick(CPoint &point)
{
	UINT Flags;
	HTREEITEM CurItem = TreeCtrl.HitTest(point, &Flags);
	TreeCtrl.SelectItem(CurItem);
	
	if(Flags & TVHT_ONITEMSTATEICON)
	{
		CNCMDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		GetTreeCtrl().ProcPosClick(CurItem);
		ProcPosChecks();
		pDoc->UpdateAllViews(NULL, N_OGL);
	}
	else if (Flags & TVHT_ONITEMLABEL)
	{
		TreeCtrl.ShowContextToolbar(point);
	}
}


void CLeftView::OnChExobjReplace()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
		return;

	ClosePositionDlg();
	GetTreeCtrl().SelectItem(NULL);// To clear PropPane
	CNCMDoc* pDoc = GetDocument();
	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	if (FixtureCrStl())
		pDoc->DeleteStockWU(pStock);
	pDoc->GetUndoHistory().EndGroup(InternalGroup);

	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnChExobjDelete()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CString key;
	class NStock *pStock = nullptr; 
	if(!GetSelListElem(key, pStock))
		return;
	ClosePositionDlg();
	GetTreeCtrl().SelectItem(nullptr);// To clear PropPane
	CNCMDoc* pDoc = GetDocument();
	pDoc->DeleteFixtWU(pStock);
	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnChExobjDisassemble()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CString key;
	class NStock *pStock = nullptr;
	if(!GetSelListElem(key, pStock))
		return;
	ClosePositionDlg();
	GetTreeCtrl().SelectItem(nullptr);// To clear PropPane

	NStockStl* pStockStl = dynamic_cast<NStockStl*>(pStock);
	if (pStockStl == nullptr)
		return;
	BStl& Stl = pStockStl->GetSTL();
	if (!Stl.GetDCEL().IsCreated())
		Stl.GetDCEL().MakeFromStl(Stl);
	if (Stl.GetDCEL().HaveTripleEdges())
	{
		if (AfxMessageBox(IDS_MES_STL_HAVE_TRIPLE_EDGES, MB_ICONQUESTION | MB_YESNO) != IDYES)
			return;
	}
	std::vector<CString> FileNames;
	
	FileNames.push_back(pStockStl->GetFileName());
	Stl.WriteSTLmulti(FileNames);// FileNames contains one name before this call and many names after this call
	LoadFixtures(FileNames);

	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnChExobjSendToStock()
{
	if (!((CMainFrame*)AfxGetMainWnd())->CheckCutting())
		return;
	CString key;
	class NStock* pStock = nullptr;
	if (!GetSelListElem(key, pStock))
		return;
	ClosePositionDlg();
	GetTreeCtrl().SelectItem(nullptr);// To clear PropPane
	CNCMDoc* pDoc = GetDocument();
	auto Type = NCMComData::GetType();
	if (Type == AT_MILL)
	{
		class NStock* pNewObj = nullptr;
		pStock->CreateCopyTo(pNewObj);
		const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
		pDoc->DeleteFixtWU(pStock);
		pDoc->AddStock(pNewObj);
		pDoc->GetUndoHistory().EndGroup(InternalGroup);
	}

	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnUpdateChExobjSendToStock(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(NCMComData::GetType() == AT_MILL);
}

void CLeftView::OnChExobjSendToPart()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CString key;
	class NStock *pStock = nullptr;
	if(!GetSelListElem(key, pStock))
		return;
	ClosePositionDlg();
	GetTreeCtrl().SelectItem(nullptr);// To clear PropPane
	CNCMDoc* pDoc = GetDocument();

	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	pDoc->DeleteFixtWU(pStock);
	pDoc->AddPartWU(pStock);
	pDoc->GetUndoHistory().EndGroup(InternalGroup);

	pDoc->UpdateAllViews(nullptr, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnChExobjColor()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	CNCMDoc* pDoc = GetDocument();
	CColorDialog ColorDialog(pStock->GetColor().GetRGB(), 0, this);
	if(ColorDialog.DoModal() != IDOK)
		return;
	pStock->SetColor(ColorDialog.GetColor());
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnChExobjVisible()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pStock->Visible = !pStock->IsVisible();
	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnUpdatePaStockVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	CNCMDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->IsThereAnyStock());
	pCmdUI->SetCheck(pDoc->NGetStockList()->IsVisible());
}

void CLeftView::OnUpdatePaExobjVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	CNCMDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->IsThereAnyExObj());
	pCmdUI->SetCheck(pDoc->NGetExObjList()->IsVisible());
}

void CLeftView::OnUpdatePaProgVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	CNCMDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->IsThereAnyProgr());
	pCmdUI->SetCheck(pDoc->NGetProgramList()->Visible);
}

void CLeftView::OnUpdatePaPartVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	CNCMDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->IsThereAnyPart());
	pCmdUI->SetCheck(pDoc->NGetPartList()->IsVisible());
}

void CLeftView::OnUpdateChExobjVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pCmdUI->SetCheck(pStock->IsVisible());
}

void CLeftView::OnPaExobjAllvis()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetExObjList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = true;
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaExobjAllunv()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetExObjList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = false;
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaExobjInv()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetExObjList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = !pS->IsVisible();
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::SetSelection(bool ExObjListChanged, bool PartListChanged, bool StockListChanged
	, bool ToolListChanged, bool MachineChanged, HTREEITEM NewItem ,HTREEITEM ParentItemNew)
{
	TreeCtrl.CloseContextToolBar();

	// Clear selection
	bool NeedRedraw = false;
	CNCMDoc* pDoc = GetDocument();
	NMapStringToOb*elist = pDoc->NGetExObjList();
	NeedRedraw |= (elist->GetSelected() != -1);
	elist->ClearSelected();
	elist = pDoc->NGetPartList();
	NeedRedraw |= (elist->GetSelected() != -1);
	elist->ClearSelected();
	elist = pDoc->NGetStockList();
	NeedRedraw |= (elist->GetSelected() != -1);
	elist->ClearSelected();
	elist = pDoc->NGetToolList();
	NeedRedraw |= (elist->GetSelected() != -1);
	elist->ClearSelected();
	MTPartArray& PartArray = pDoc->GetMachTool().GetParts();
	NeedRedraw |= (PartArray.GetSelected() != -1);
	PartArray.SelectPart(-1);

	if(ExObjListChanged)
	{
		CMapStringToNStock *elist = pDoc->NGetExObjList();
		if(NewItem && ParentItemNew)
		{
			if (GetTreeCtrl().GetItemData(ParentItemNew) == NExObjList)
			{
				elist->SetSelectedKey(GetTreeCtrl().GetItemText(NewItem));
				NeedRedraw = true;
			}
		}
	}
	if(PartListChanged)
	{
		CMapStringToNStock *elist = pDoc->NGetPartList();
		if(NewItem && ParentItemNew)
		{
			if(GetTreeCtrl().GetItemData(ParentItemNew) == NPartList)
			{
				elist->SetSelectedKey(GetTreeCtrl().GetItemText(NewItem));
				NeedRedraw = true;
			}
		}
	}
	if(StockListChanged)
	{
		CMapStringToNStock *slist = pDoc->NGetStockList();
		if (NewItem && ParentItemNew)
		{
			if (GetTreeCtrl().GetItemData(ParentItemNew) == NStocksList)
			{
				slist->SetSelectedKey(GetTreeCtrl().GetItemText(NewItem));
			}
		}
		if (NewItem && !ParentItemNew)
		{
			if (GetTreeCtrl().GetItemData(NewItem) == NStock)
			{
				slist->SetSelectedKey(GetTreeCtrl().GetItemText(NewItem));
			}
		}
		((CMainFrame *)AfxGetMainWnd())->ApplyBoxDialogs();
		NeedRedraw = true;
	}
	if(ToolListChanged)
	{
		CMapStringToNTool *slist = pDoc->NGetToolList();
		if(NewItem && ParentItemNew)
		{
			if(GetTreeCtrl().GetItemData(ParentItemNew) == NToolList)
			{
				CString text = GetTreeCtrl().GetItemText(NewItem);
				int k = text.Find(" ");
				if(k >= 0)
					slist->SetSelectedKey(text.Left(k));
			}
		}
		((CMainFrame *)AfxGetMainWnd())->ApplyBoxDialogs();
		NeedRedraw = true;
	}
	if(MachineChanged)
	{
		MTPartArray& PartArray = pDoc->GetMachTool().GetParts();
		if(NewItem && ParentItemNew)
		{
			if(GetTreeCtrl().GetItemData(ParentItemNew) == NMachine)
			{
				PartArray.SelectPart(GetTreeCtrl().GetItemText(NewItem));
			}
		}
		((CMainFrame *)AfxGetMainWnd())->ApplyBoxDialogs();
		NeedRedraw = true;
	}
	if(NeedRedraw)
		pDoc->UpdateAllViews(NULL, N_OGL, NULL);
	FillPropPane(NewItem, ParentItemNew);
}

void CLeftView::OnChExobjPos()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	CNCMDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag();
	ClosePositionDlg();
	pDoc->NGetPosChecks()->SetAt(CMapStringToNStock::PrefixFixt + key, pStock);
	MakePosDlg(this, pDoc);
}

void CLeftView::OnUpdatePaExobjAllvis(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetExObjList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaExobjAllunv(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetExObjList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaExobjInv(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetExObjList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaExobjDelete(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetExObjList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnPaExobjDelete()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	ClosePositionDlg();
	CNCMDoc* pDoc = GetDocument();
	pDoc->DeleteExobjList();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}


void CLeftView::OnPaExobjPos()
{
	CNCMDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag();
	ClosePositionDlg();
	GetTreeCtrl().MergeStL2PosChecks(pDoc->NGetExObjList(), CMapStringToNStock::PrefixFixt, true);
	MakePosDlg(this, pDoc);
}

void CLeftView::OnUpdatePaExobjPos(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetExObjList()->empty();
	pCmdUI->Enable(Enable);
}
void CLeftView::OnChExobjExportstl()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	if(!pStock->IsKindOf(RUNTIME_CLASS(NStockStl)))
		return;
	CString filename;
	CString ext;
	ext.LoadString(IDS_STL_EXT);
	CString filter;
	filter = "*." + ext;
	filter = filter + _T("|") + filter + _T("||");
	BFileDialogWPath fd(_T("LastFixtPath"), FALSE, ext, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return;

	CStdioFile f;
	if(!f.Open(filename,
		CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return;
	((NStockStl *)pStock)->WriteSTL(f);
	f.Close();
}

void CLeftView::OnChExobjWf()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pStock->WFrame = !pStock->WFrame;

	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnUpdateChExobjWf(CCmdUI *pCmdUI)
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pCmdUI->SetCheck(pStock->WFrame);
}

void CLeftView::OnPaPartAllvis()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetPartList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = true;
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaPartAllunv()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetPartList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = false;
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaPartInv()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetPartList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = !pS->IsVisible();
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaStockAllvis()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetStockList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = true;
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaStockAllunv()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetStockList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = false;
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaStockInv()
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetStockList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		pS->Visible = !pS->IsVisible();
	}
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnPaStockTemp()
{
	if(!PaStockCanCreate())
		return;
	ClosePositionDlg();
	((CMainFrame *)AfxGetMainWnd())->CreateDimStock();
	//CreateStockBoxTemp(NULL);
	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_STOCKSTATECHANGE);
}

void CLeftView::OnPaStockTempRem()
{
	if(!PaStockCanCreate())
		return;
	ClosePositionDlg();
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	GetTreeCtrl().SelectItem(NULL);// To clear PropPane
	pDoc->RemoveTempStock();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_STOCKSTATECHANGE);
}

void CLeftView::OnPaPartPos()
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	pDoc->SetModifiedFlag();
	ClosePositionDlg();
	GetTreeCtrl().MergeStL2PosChecks(pDoc->NGetPartList(), CMapStringToNStock::PrefixPart, true);
	MakePosDlg(this, pDoc);
}

void CLeftView::OnUpdatePaPartAllvis(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetPartList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaPartAllunv(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetPartList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaPartInv(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetPartList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaStockAllvis(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetStockList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaStockAllunv(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetStockList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaStockInv(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetStockList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaStockTemp(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetStockList()->empty();
	Enable &= !((CMainFrame *)AfxGetMainWnd())->IsCrDimStockVisible();
	Enable &= theApp.GetGlobalState().GetModelMode() != NGlobalState::BSP;
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaStockTempRem(CCmdUI *pCmdUI)
{
	BOOL Enable = GetDocument()->NGetStockList()->HaveTempStock();
	pCmdUI->Enable(Enable);
}

bool CLeftView::ClosePositionDlg()
{
	const bool res = pPosDlg != nullptr;
	CNCMDoc* pDoc = GetDocument();
	if(res)
	{
		pPosDlg->GetWindowRect(LastPosDlgRect);
		delete pPosDlg;
		pPosDlg = nullptr;
	}
	if (pDoc->IsStockPosChecked())
		pDoc->ResetStocks();
	pDoc->NGetPosChecks()->RemoveAll();
	return res;
}
void CLeftView::CloseSectProgDlg()
{
	if(pSectProgDlg)
	{
		delete pSectProgDlg;
		pSectProgDlg = NULL;
	}
}

void CLeftView::OnChPartReplace()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock) || !pStock)
		return;

	ClosePositionDlg();
	GetTreeCtrl().SelectItem(NULL);// To clear PropPane

	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	if(PartCrStl())
		pDoc->DeleteStockWU(pStock);
	pDoc->GetUndoHistory().EndGroup(InternalGroup);

	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnChStockDelete()
{
	if(!PaStockCanCreate())
		return;
	ClosePositionDlg();
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc->IsThereAnyStock())
		return;

	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	if(pStock->IsTemporary())
	{
		AfxMessageBox(IDS_STOCK_IS_TEMP);
		return;
	}
	GetTreeCtrl().SelectItem(NULL);// To clear PropPane

	pDoc->DeleteStockWU(pStock);

	if (!pDoc->IsThereAnyStock())
		theApp.OnDontcutMode();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT | N_STOCKSTATECHANGE);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnUpdateChStockDelete(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockRepBox(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockRepCone(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockRepContbox(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockRepCyl(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockDivide(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockRepPipe(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockRepProfile(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockRepStl(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnUpdateChStockPos(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	if (pDoc)
	{
		BOOL enable = pDoc->IsThereAnyStock();
		pCmdUI->Enable(enable);
	}
}

void CLeftView::OnChPartDelete()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	CString key;
	class NStock *pStock = NULL; 
	if(!GetSelListElem(key, pStock))
		return;
	ClosePositionDlg();
	pDoc->DeletePartWU(pStock);
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnChPartColor()
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	CColorDialog ColorDialog(pStock->GetColor().GetRGB(), 0, this);
	if(ColorDialog.DoModal() != IDOK)
		return;
	pStock->SetColor(ColorDialog.GetColor());
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnPaStockColor()
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	CString key;
	CColorDialog ColorDialog(pDoc->NGetStockList()->GetColor().GetRGB(), 0, this);
	if(ColorDialog.DoModal() != IDOK)
		return;
	pDoc->NGetStockList()->SetColor(ColorDialog.GetColor());
	pDoc->CreateStocksRender();
	UpdatePropPane();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnChPartVisible()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pStock->Visible = !pStock->IsVisible();
	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnChStockVisible()
{
	this->SetStockVisible();
	AfxGetMainWnd()->SendMessage(WM_APP_STOCK_VISIBLE);
	CNCMDoc* pDoc = GetDocument();
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	UpdatePropPane();
}

void CLeftView::OnChStockWFrame()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pStock->WFrame = !pStock->WFrame;

	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnChPartWf()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pStock->WFrame = !pStock->WFrame;

	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
	UpdatePropPane();
}

void CLeftView::OnChPartPos()
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pDoc->SetModifiedFlag();
	ClosePositionDlg();
	pDoc->NGetPosChecks()->SetAt(CMapStringToNStock::PrefixPart + key, pStock);
	MakePosDlg(this, pDoc);
}

void CLeftView::OnPaStockPos()
{
	if(((CNCMApp *)AfxGetApp())->IsInCuttingMode())
	{
		theApp.OnDontcutMode();
	}
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	if(!pDoc->IsThereAnyStock())
		return;

	pDoc->SetModifiedFlag();
	ClosePositionDlg();
	GetTreeCtrl().MergeStL2PosChecks(pDoc->NGetStockList(), CMapStringToNStock::PrefixStock, true);
	if(pDoc->NGetPartList()->GetMooveWithStock())
		GetTreeCtrl().MergeStL2PosChecks(pDoc->NGetPartList(), CMapStringToNStock::PrefixPart, true);
	if(pDoc->NGetExObjList()->GetMooveWithStock())
		GetTreeCtrl().MergeStL2PosChecks(pDoc->NGetExObjList(), CMapStringToNStock::PrefixFixt, true);
	MakePosDlg(this, pDoc);
}

void CLeftView::OnChStockPos()
{
	if(((CNCMApp *)AfxGetApp())->IsInCuttingMode())
	{
		theApp.OnDontcutMode();
	}
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}

	if (!pDoc->IsThereAnyStock())
		return;
	class NStock *pStock = NULL;
	CString key;
	if(!GetSelListElem(key, pStock))
		return;
	
	pDoc->SetModifiedFlag();
	ClosePositionDlg();
	pDoc->NGetPosChecks()->SetAt(CMapStringToNStock::PrefixStock + key, pStock);
	if (pDoc->NGetPartList()->GetMooveWithStock())
		GetTreeCtrl().MergeStL2PosChecks(pDoc->NGetPartList(), CMapStringToNStock::PrefixPart, true);
	if (pDoc->NGetExObjList()->GetMooveWithStock())
		GetTreeCtrl().MergeStL2PosChecks(pDoc->NGetExObjList(), CMapStringToNStock::PrefixFixt, true);
	MakePosDlg(this, pDoc);
}

void CLeftView::OnChPartExportstl()
{
	CString key;
	class NStock* pStock = NULL;
	if (!GetSelListElem(key, pStock))
		return;
	if (!pStock->IsKindOf(RUNTIME_CLASS(NStockStl)))
		return;
	CString filename;
	CString ext;
	ext.LoadString(IDS_STL_EXT);
	CString filter;
	filter = "*." + ext;
	filter = filter + _T("|") + filter + _T("||");
	BFileDialogWPath fd(_T("LastPartPath"), FALSE, ext, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else
		return;

	CStdioFile f;
	if (!f.Open(filename,
		CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return;
	((NStockStl*)pStock)->WriteSTL(f);
	f.Close();
}

void CLeftView::OnChPartExportdxf()
{
	CString key;
	class NStock* pStock = NULL;
	if (!GetSelListElem(key, pStock))
		return;
	NStockStl* pStockStl = dynamic_cast<NStockStl*>(pStock);
	if (pStockStl == nullptr)
		return;
	NRotationContour Dlg(this);
	Dlg.ShowStartMessage(!pStockStl->GetSTL().Is1Connected());
	if (Dlg.DoModal() == IDOK)
	{
		if (!pStockStl->GetSTL().WriteRotCont(!Dlg.GetFindStartEnd(), !Dlg.GetCorrectNormal()))
			AfxMessageBox(IDS_MES_STL_IMPOSSIBLE_BUILD_CONTOUR);
	}
}

void CLeftView::OnUpdateChPartVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pCmdUI->SetCheck(pStock->IsVisible());
}

void CLeftView::OnUpdateChPartWf(CCmdUI *pCmdUI)
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pCmdUI->SetCheck(pStock->WFrame);
}

void CLeftView::OnUpdatePaPartDelete(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetPartList()->empty();
	pCmdUI->Enable(Enable);
}

void CLeftView::OnUpdatePaStockPos(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument()->IsThereAnyStock());
}

void CLeftView::OnUpdatePaStockDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument()->IsThereAnyStock());
}

void CLeftView::OnUpdatePaPartPos(CCmdUI *pCmdUI)
{
	BOOL Enable = !GetDocument()->NGetPartList()->empty();
	pCmdUI->Enable(Enable);
}
bool CLeftView::IsToolbar(const CCmdUI *pCmdUI) const
{
	CToolBar *pTB = dynamic_cast<CToolBar *>(pCmdUI->m_pOther);
	return(pTB != NULL);
}

void CLeftView::PaPartCrProfile(const CString & InitFileName)
{
	if (!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	ClosePositionDlg();

	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	class NStock *pStock = NULL;
	if (!InitFileName.IsEmpty())
		pDoc->pCreateContDlg->m_FileName = InitFileName;
	if (pDoc->pCreateContDlg->DoModal(NULL) == IDOK)
	{
		if ((pStock = pDoc->pCreateContDlg->GetNewStock()) != NULL)
		{
			NColor Col;
			if (pDoc->IsThereAnyPart())
				Col.SetRGB(NToolCombined::Palet.GetRecurColor(0));
			else
			{
				NColor *c;
				if (NCM_PROJECT.Colors.Lookup("ДетальНомер1", c) && (c->r() != 0 || c->g() != 0 || c->b() != 0))
					Col = *c;
				else
					Col.SetRGB(NToolCombined::Palet.GetRecurColor(0));
			}
			pStock->SetColor(Col);
			pDoc->AddPartWU(pStock);
			pDoc->SetModifiedFlag();
		}
	}
	else
		delete pStock;
}

bool CLeftView::TvnBegindrag()
{
	ClosePositionDlg();
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return false;
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return false;
	}
	return true;
}

bool CLeftView::IsPosDlgActive() const
{
	if (pPosDlg == nullptr)
		return false;
	return pPosDlg->IsActive();
}

void CLeftView::OnPick(RPickID ObjID)
{
	auto ObjName = ObjID.GetObjName();
	HTREEITEM ParentItem = NULL;
	HTREEITEM item = NULL;
	switch (ObjID.GetGroup())
	{
	case OG_STOCKS:
		if (GetTreeCtrl().NStockItem != nullptr)
			item = GetTreeCtrl().NStockItem;
		else
			ParentItem = GetTreeCtrl().NStocksItem;
		break;
	case OG_PARTS:
		ParentItem = GetTreeCtrl().NPartItem;
		break;
	case OG_FIXTURES:
		ParentItem = GetTreeCtrl().NExObjItem;
		break;
	case OG_MACHINE:
		ParentItem = GetTreeCtrl().NMachineItem;
		ObjName = (ObjName - 1) / 2;// see MTConfig::GetPartID
		break;
	case OG_TOOLS:
		ParentItem = GetTreeCtrl().NToolItem;
		break;
	}
	if (item == NULL)// all cases exclude one and only stock
	{
		if (ParentItem == NULL)
			return;
		if (ParentItem == GetTreeCtrl().NToolItem)
		{
			const CNCMDoc* pDoc = GetDocument();
			CString key = pDoc->NGetToolList()->GetKey(ObjName);
			item = GetTreeCtrl().GetChildItem(ParentItem);
			for (int i = 0; item != NULL; ++i)
			{
				CString text = GetTreeCtrl().GetItemText(item);
				int k = text.Find(" ");
				if (key == text.Left(k))
					break;
				item = GetTreeCtrl().GetNextSiblingItem(item);
			}
		}
		else
		{
			item = GetTreeCtrl().GetChildItem(ParentItem);
			for (int i = 0; i < ObjName && item != nullptr; ++i)
			{
				item = GetTreeCtrl().GetNextSiblingItem(item);
			}
		}
	}
	GetTreeCtrl().SelectItem(item);
	if (IsPosDlgActive())
	{
		GetTreeCtrl().ProcPosClick(item);
		ProcPosChecks();
	}
	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL, N_OGL);
}

NPositionDlg* CLeftView::MakePosDlg(CWnd* pParent, CNCMDoc* pDoc)
{
	pPosDlg = nullptr;
	if (!((CMainFrame*)AfxGetMainWnd())->CheckCutting())
		return nullptr;
	pPosDlg = new NPositionDlg(pParent, pDoc);
	pPosDlg->Create(NPositionDlg::IDD, this);
	if (!LastPosDlgRect.IsRectEmpty())
		pPosDlg->MoveWindow(LastPosDlgRect);
	pPosDlg->StartPosDlg();// should be called after Create
	pPosDlg->ShowWindow(SW_SHOW);
	return pPosDlg;
}

void CLeftView::OnUpdateChPartPos(CCmdUI *pCmdUI)
{
	if(IsToolbar(pCmdUI))
		return;
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock) || !pStock)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if(!pStock->IsKindOf(RUNTIME_CLASS(NStockStl)))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

void CLeftView::OnUpdateChPartExportstl(CCmdUI *pCmdUI)
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock) || !pStock)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if(!pStock->IsKindOf(RUNTIME_CLASS(NStockStl)))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}

void CLeftView::OnUpdatePaStockPolyhromeTurn(CCmdUI *pCmdUI)
{
}

CNCMDoc * CLeftView::GetDocument() 
{
	CFrameWnd *pMainFrame = (CFrameWnd *)AfxGetMainWnd();
	if(!pMainFrame)
		return NULL;
	return (CNCMDoc *)pMainFrame->GetActiveDocument();
}

NObjTreeCtrl & CLeftView::GetTreeCtrl()
{
	return TreeCtrl;
}

void CLeftView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	TreeCtrl.SetFocus();
}

void CLeftView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	TreeCtrl.CloseContextToolBar();
	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = 0;

	TreeCtrl.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CLeftView::CalcContextMenuID()
{
	CWnd *pMenuOwner = this;
	CNCMDoc* pDoc = GetDocument();

	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	if (CurItem == NULL) 
		return - 1;
	int MenuID = -1;
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);

	if (ParentItem != NULL) // Item has parent
	{
		switch (GetTreeCtrl().GetItemData(ParentItem))
		{
		case NProgramList:
			switch (NCMComData::GetType())
			{
			case AT_WELD:
				MenuID = IDR_CH_PROGR_WELD;
				break;
			default:
				MenuID = IDR_CH_PROGR;
				break;
			}
			break;
		case NProgramListAux:
			MenuID = IDR_CH_PROGR_AUX;
			break;
		case NMachine:
			MenuID = IDR_CH_MACH;
			break;
		case NToolList:
			MenuID = IDR_CH_TOOL;
			break;
		case NExObjList:
			MenuID = IDR_CH_EXOBJ;
			break;
		case NPartList:
			switch (NCMComData::GetType())
			{
			case AT_WELD:
				MenuID = IDR_CH_PART_WELD;
				break;
			default:
				MenuID = IDR_CH_PART;
				break;
			}
			break;
		case NStocksList:
			switch (NCMComData::GetType())
			{
			case AT_MILL:
				MenuID = pDoc->NGetStockList()->HaveTempStock() ? IDR_CH_STOCK_TEMP : IDR_CH_STOCK;
				break;
			case AT_TURN:
				MenuID = IDR_CH_STOCK_TURN;
				break;
			case AT_MILL_TURN:
				MenuID = IDR_CH_STOCK_MILLTURN;
				break;
			default:
				MenuID = IDR_CH_STOCK;
				break;
			}
			break;
		default:
			ParentItem = GetTreeCtrl().GetParentItem(ParentItem);
			if (ParentItem != NULL)
				if (GetTreeCtrl().GetItemData(ParentItem) == NToolList)
					MenuID = IDR_CH_TOOL1;
				else
					MenuID = -1;
		}
	}
	else // Item has no parent
	{
		switch (GetTreeCtrl().GetItemData(CurItem))
		{
		case NCUnit:
			MenuID = IDR_PA_UNIT;
			break;
		case NMachine:
			MenuID = IDR_PA_MACH;
			switch (NCM_PROJECT.GetGlobalState().GetEditMode())
			{
			case NGlobalState::Edit_Mach:
				MenuID = IDR_PA_MACH_EDIT;
				pMenuOwner = AfxGetMainWnd();
				break;
			default:
				MenuID = IDR_PA_MACH;
				break;
			}
			break;
		case NProgramList:
			switch (NCMComData::GetType())
			{
			case AT_WELD:
				MenuID = IDR_PA_PROGR_WELD;
				break;
			default:
				MenuID = IDR_PA_PROGR;
				break;
			}
			break;
		case NProgramListAux:
			MenuID = IDR_PA_PROGR_AUX;
			break;
		case NToolList:
			switch (NCMComData::GetType())
			{
			case AT_MILL:
				MenuID = IDR_PA_TOOLS;
				break;
			case AT_PUNCH:
				MenuID = IDR_PA_TOOLS;
				break;
			case AT_TURN:
				MenuID = IDR_PA_TOOLS_TURN;
				break;
			case AT_MILL_TURN:
				MenuID = IDR_PA_TOOLS_MILLTURN;
				break;
			case AT_WELD:
				MenuID = IDR_PA_TOOLS_WELD;
				break;
			default:
				MenuID = IDR_PA_TOOLS;
				break;
			}
			break;
		case NStock:
			switch (NCMComData::GetType())
			{
			case AT_MILL:
				MenuID = pDoc->NGetStockList()->HaveTempStock() ? IDR_CH_STOCK_TEMP : IDR_CH_STOCK;
				break;
			case AT_TURN:
				MenuID = IDR_CH_STOCK_TURN;
				break;
			case AT_MILL_TURN:
				MenuID = IDR_CH_STOCK_MILLTURN;
				break;
			default:
				MenuID = IDR_CH_STOCK;
				break;
			}
			break;
		case NPartList:
			switch (NCMComData::GetType())
			{
			case AT_MILL:
				MenuID = IDR_PA_PART;
				break;
			case AT_TURN:
				MenuID = IDR_PA_PART_TURN;
				break;
			case AT_MILL_TURN:
				MenuID = IDR_PA_PART_MILLTURN;
				break;
			case AT_WELD:
				MenuID = IDR_PA_PART;
				break;
			default:
				MenuID = IDR_PA_PART;
				break;
			}
			break;
		case NStocksList:
			switch (NCMComData::GetType())
			{
			case AT_MILL:
				MenuID = pDoc->NGetStockList()->HaveTempStock() ? IDR_PA_STOCK_TEMP : IDR_PA_STOCK;
				break;
			case AT_TURN:
				MenuID = IDR_PA_STOCK_TURN;
				break;
			case AT_MILL_TURN:
				MenuID = pDoc->NGetStockList()->HaveTempStock() ? IDR_PA_STOCK_TEMP : IDR_PA_STOCK_MILLTURN;
				break;
			default:
				MenuID = IDR_PA_STOCK;
				break;
			}
			break;
		case NExObjList:
			MenuID = IDR_PA_EXOBJ;
			break;
		default:

			MenuID = -1;
			break;
		}
	}
	return MenuID;
}

void CLeftView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = &TreeCtrl;
	ASSERT_VALID(pWndTree);
	TreeCtrl.CloseContextToolBar();

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}
	TreeCtrl.SetFocus();
	CNCMDoc* pDoc = GetDocument();

	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	if (CurItem == NULL)
		return;
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);

	auto MenuID = CalcContextMenuID();
	if(MenuID < 0) 
		return;
	CMenu menu;
	VERIFY(menu.LoadMenu(MenuID));
	pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	// Set checked and enabled to menu items 

	NToolCombined *pToolCon = NULL;
	NTool *pTool = NULL;
	NToolID ToolID;
	CString ItemText = GetTreeCtrl().GetItemText(CurItem);
	switch(MenuID)
	{
	case IDR_PA_PROGR:
	case IDR_PA_PROGR_AUX:
	case IDR_PA_PROGR_WELD:
		if( ! pDoc->IsThereAnyProgr() )
		{
			pPopup->EnableMenuItem(ID_PA_PROG_ALLVIS,MF_GRAYED);
			pPopup->EnableMenuItem(ID_PA_PROG_ALLUNV,MF_GRAYED);
			pPopup->EnableMenuItem(ID_PA_PROG_INV,MF_GRAYED);
		}
		pPopup->CheckMenuItem(ID_PA_PROG_FULL, ( pDoc->NGetProgramList()->PartialDraw == TrajPart::TP_WHOLE ? MF_CHECKED : MF_UNCHECKED ));
		pPopup->CheckMenuItem(ID_PA_PROG_PART, ( pDoc->NGetProgramList()->PartialDraw == TrajPart::TP_PART ? MF_CHECKED : MF_UNCHECKED ));
		pPopup->CheckMenuItem(ID_PA_PROG_WORM, ( pDoc->NGetProgramList()->PartialDraw == TrajPart::TP_WORM ? MF_CHECKED : MF_UNCHECKED ));

#ifdef NC_DEMO
		pPopup->EnableMenuItem(ID_LOAD,MF_GRAYED);
		pPopup->EnableMenuItem(ID_LOAD_AUX,MF_GRAYED);
		pPopup->EnableMenuItem(ID_PA_PROG_RELO,MF_GRAYED);
		pPopup->EnableMenuItem(ID_PA_PROGR_CR,MF_GRAYED);
#endif
#ifdef NCM_EDU
		{
			int AddEduTestA = 1;
			CString code, ans;
			Security::GetCode(&code);
			code = code + NCMVersion;
			Security::GetAnswer(code, &ans);
			int i = 5;
			if(ans.GetLength() > 2)
				AddEduTestA = int(ans[0] * ans[1] * ans[2]);
			if(((CMainFrame *)AfxGetMainWnd())->AddEduTest != AddEduTestA)
			{
				for(i = 0; i < 5 ; ++i)
				{
					Security::GetAnswer(code + Security::GetDate(i), &ans);
					if(ans.GetLength() > 2)
						AddEduTestA = int(ans[0] * ans[1] * ans[2]);
					if(((CMainFrame *)AfxGetMainWnd())->AddEduTest == AddEduTestA)
						break;
				}
			}
			else
				i = 0;
			if(i == 5)
			{
				pPopup->RemoveMenu(ID_LOAD, MF_BYCOMMAND);
				pPopup->RemoveMenu(ID_LOAD_AUX, MF_BYCOMMAND);
				pPopup->RemoveMenu(ID_PA_PROG_RELO, MF_BYCOMMAND);
				pPopup->RemoveMenu(ID_PA_PROGR_CR, MF_BYCOMMAND);
			}
		}
#endif

		break;
	case IDR_CH_PROGR:
	case IDR_CH_PROGR_AUX:
	case IDR_CH_PROGR_WELD:
#ifdef NC_DEMO
		pPopup->EnableMenuItem(ID_SAVE, MF_GRAYED);
#endif
		break;
	case IDR_PA_STOCK_TURN:
	case IDR_PA_STOCK:
	case IDR_PA_STOCK_MILLTURN:
		//pStock = pDoc->GetStock();
		pPopup->EnableMenuItem(ID_PA_STOCK_LOAD, MF_ENABLED);
		pPopup->EnableMenuItem(ID_PA_STOCK_CR_BOX, MF_ENABLED);
		pPopup->EnableMenuItem(ID_PA_STOCK_CR_CYL, MF_ENABLED);
		pPopup->EnableMenuItem(ID_PA_STOCK_CR_STL, MF_ENABLED);
		break;
	case IDR_CH_STOCK_TURN:
	case IDR_CH_STOCK:
	case IDR_CH_STOCK_MILLTURN:
		break;
	case IDR_PA_MACH:
		break;
	case IDR_PA_PART:
	case IDR_PA_PART_TURN:
	case IDR_PA_PART_MILLTURN:
		break;
	case IDR_PA_TOOLS:
	case IDR_PA_TOOLS_TURN:
	case IDR_PA_TOOLS_MILLTURN:
	case IDR_PA_TOOLS_WELD:
		if( ! pDoc->IsThereAnyTool() )
		{
			pPopup->EnableMenuItem(ID_PA_TOOLS_ALLVIS, MF_GRAYED);
			pPopup->EnableMenuItem(ID_PA_TOOLS_ALLUNV, MF_GRAYED);
			pPopup->EnableMenuItem(ID_PA_TOOLS_INV, MF_GRAYED);
		}

		break;
	case IDR_CH_TOOL:
		ToolID.CrFromPrintStr(ItemText.Left(ItemText.Find(" ")));
				
		pDoc->NGetToolList()->Lookup(ToolID,pToolCon);
		
		ASSERT( pToolCon );
		pPopup->EnableMenuItem(ID_CH_TOOL_PASTE,
			(!pCopyTool) ? MF_GRAYED : MF_ENABLED);
		pPopup->CheckMenuItem(ID_CH_TOOL_VISIBLE, ( pToolCon->IsVisible() ) ? MF_CHECKED : MF_UNCHECKED );
		pPopup->CheckMenuItem(ID_CH_TOOL_WF, ( pToolCon->WFrame ) ? MF_CHECKED : MF_UNCHECKED );
		break;
	case IDR_CH_TOOL1:
		ParentItem = GetTreeCtrl().GetParentItem(CurItem);
		ItemText = GetTreeCtrl().GetItemText(ParentItem);
		ToolID.CrFromPrintStr(ItemText.Left(ItemText.Find(" ")));
		pDoc->NGetToolList()->Lookup(ToolID,pToolCon);
		
		ASSERT( pToolCon );
		int index = int(GetTreeCtrl().GetItemData(CurItem)); 
		pPopup->EnableMenuItem(ID_CH_TOOL_PASTE,
			(!pCopyTool) ? MF_GRAYED : MF_ENABLED);
		pPopup->EnableMenuItem(ID_CH_TOOL_COLOR, 
			( 40 <= pToolCon->GetTool(index)->GetType() &&
				pToolCon->GetTool(index)->GetType() <= 43 ) ? MF_GRAYED : MF_ENABLED);
		pPopup->CheckMenuItem(ID_CH_TOOL_WF, 
			( pToolCon->GetTool(index)->WFrame ) ? MF_CHECKED : MF_UNCHECKED );
		pPopup->CheckMenuItem(ID_CH_TOOL_VISIBLE, 
			( pToolCon->GetTool(index)->Visible ) ? MF_CHECKED : MF_UNCHECKED );
		break;

	}
	// Show the menu
	((CWinAppEx*)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(*pPopup, point.x, point.y, this, TRUE);
	//auto ID = ((CWinAppEx*)AfxGetApp())->GetContextMenuManager()->TrackPopupMenu(*pPopup, point.x, point.y, this);
	//if (ID != 0)
	//	SendMessage(WM_COMMAND, ID);

	menu.DestroyMenu();
}

void CLeftView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}
//void CLeftView::OnPaint()
//{
//	//if (GetTreeCtrl().IsWindowVisible())
//	//	return;
//	// Fill client rect when TreeCtrl is invisible
//	
//	//CRect rect;
//	//GetClientRect(rect);
//
//	//CPaintDC dc(this); // device context for painting
//	//dc.FillSolidRect(rect, GetBkColor(dc));
//}

void CLeftView::OnPaMachToolVisible()
{
	CNCMDoc* pDoc = GetDocument();
	MTMachineTool& MT = pDoc->GetMachTool();
	MT.SetDrawAllTools(!MT.GetDrawAllTools());
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,N_OGL | N_LEFT_ATTR, NULL);
}

void CLeftView::OnUpdatePaMachToolVisible(CCmdUI *pCmdUI)
{
	CNCMDoc* pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->GetMachTool().GetDrawAllTools());
}

void CLeftView::OnPaStockSecView()
{
	((CMainFrame *)AfxGetMainWnd())->OnPaStockSecView();
}

void CLeftView::OnUpdateChProgrDxfexport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.GetConfig() & CM_ExpDXF);
}

void CLeftView::OnUpdateChProgrAptexport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.GetConfig() & CM_ExpAPTCL);
}

void CLeftView::OnUpdateStockExstl(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.GetConfig() & CM_ExpSTL);
}

void CLeftView::OnPaMachEdit()
{
	theApp.SetEditMachMode();
}

void CLeftView::OnPaMachEditEnd()
{
	theApp.StopEditMachMode();
}

void CLeftView::OnUpdatePaMachEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(theApp.GetConfig() & CM_MachineEditor);
}

void CLeftView::OnPaToolsCrWeld()
{
	((CMainFrame*)AfxGetMainWnd())->PaToolsCr();
}

void CLeftView::OnChPartWsect()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	if(!pStock->IsKindOf(RUNTIME_CLASS(NWeldPart)))
		return;
	NWeldPart *pWeldPart = (NWeldPart *)pStock;
	NSectArrSheet Sheet(*pWeldPart);
	Sheet.DoModal();
}

void CLeftView::OnChProgrWeldSect()
{
	NProgram *pProg = GetSelProg();
	if(!pProg)
		return;
	ASSERT(pProg->IsKindOf(RUNTIME_CLASS(NProgramWeld)));
	CNCMDoc* pDoc = GetDocument();
	//NSectProgDlg Dlg(pDoc, (NProgramWeld *)pProg);
	//Dlg.DoModal();
	pSectProgDlg = new NSectProgDlg(pDoc, (NProgramWeld *)pProg);
	((NProgramWeld *)pProg)->SetDialog(pSectProgDlg);
	pSectProgDlg->Create(NSectProgDlg::IDD, this);
	pSectProgDlg->ShowWindow(SW_SHOW);
}

void CLeftView::OnChPsectVisible()
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	((NWeldPart *)pStock)->SetVisibleSect(!((NWeldPart *)pStock)->GetVisibleSect());
	CNCMDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT_ATTR, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::OnUpdateChPsectVisible(CCmdUI *pCmdUI)
{
	CString key;
	class NStock *pStock = NULL;
	if(!GetSelListElem(key, pStock))
		return;
	pCmdUI->SetCheck(((NWeldPart *)pStock)->GetVisibleSect());
}

void CLeftView::OnPropertiesEd()
{
	((CMainFrame *)AfxGetMainWnd())->ShowPropPane();
	UpdatePropPane();
}

void CLeftView::OnUpdateChProgrVisible(CCmdUI *pCmdUI)
{
	if (IsToolbar(pCmdUI))
		return;
	int ip = GetSelProgNum();
	if(ip < 0)
		return;
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg;
	CString key;
	pPrL->GetAt(ip, key, (CObject *&)pProg);
	pCmdUI->SetCheck(pProg->IsVisible());
}

void CLeftView::OnUpdateChProgrActive(CCmdUI *pCmdUI)
{
	int ip = GetSelProgNum();
	if(ip < 0)
		return;
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNProgram *pPrL = pDoc->NGetProgramList();
	NProgram *pProg;
	CString key;
	pPrL->GetAt(ip, key, (CObject *&)pProg);
	pCmdUI->SetCheck(pDoc->GetUnit()->GetState()->GetCurProg() == key);
}


void CLeftView::OnPaPartSubtract()
{
	CNCMDoc* pDoc = GetDocument();
	CMapStringToNStock *elist = pDoc->NGetPartList();
	CMapStringToNStock *slist = pDoc->NGetStockList();
	for each (const auto & el in *elist)
	{
		class NStock* pS = static_cast<class NStock*>(el.second);
		static int count = 0;
		++count;
		if(count % 40 == 0)
			MFOREST.Compress(true);
		if(pS->IsKindOf(RUNTIME_CLASS(NStockStl)))
		{
			for each (const auto & el in *slist)
			{
				class NStock* pStock = static_cast<class NStock*>(el.second);
				if (pStock && pS)
					pStock->SubtractStl(((NStockStl *)pS)->GetSTL());
			}
		}
	}
	pDoc->CreateStocksRender();
	
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT, NULL);
	pDoc->SetModifiedFlag();
}

void CLeftView::FillPropPane(HTREEITEM NewItem, HTREEITEM ParentItemNew)
{
	if(!pPropPane)
		return;
	if(pPropPane->GetSafeHwnd() == NULL)
		return;

	// Send pPObj to the prop editor
	((CMainFrame *)AfxGetMainWnd())->Send2PropPane(GetAbsObj(NewItem, ParentItemNew));
}

PAbstractObject *CLeftView::GetAbsObj(HTREEITEM NewItem, HTREEITEM ParentItemNew)
{
	CNCMDoc* pDoc = GetDocument();
	CString key;
	PAbstractObject *pPObj = NULL;
	if(NewItem != NULL)
	{
		if(ParentItemNew == NULL)
		{
			switch (GetTreeCtrl().GetItemData(NewItem))
			{
			case NProgramList:
				pPObj = pDoc->NGetProgramList();
				break;
			case NProgramListAux:
				pPObj = NULL;
				break;
			case NToolList:
				pPObj = pDoc->NGetToolList();
				break;
			case NExObjList:
				pPObj = pDoc->NGetExObjList();
				break;
			case NPartList:
				pPObj = pDoc->NGetPartList();
				break;
			case NStocksList:
				pPObj = pDoc->NGetStockList();
				break;
			case NStock:
				pPObj = pDoc->NGetStockList();
				break;
			case NMachine:
				pPObj = &pDoc->GetMachTool();
				break;
			case NCUnit:
				pPObj = pDoc;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (GetTreeCtrl().GetItemData(ParentItemNew))
			{
			case NProgramList:
				pPObj = GetSelProg();
				break;
			case NProgramListAux:
				pPObj = GetSelProg();
				break;
			case NToolList:
				{
					pDoc->FillToolsInfo();
					CString ToolText = GetTreeCtrl().GetItemText(NewItem);
					int i = ToolText.Find(" ");
					NToolID ToolID;
					ToolID.CrFromPrintStr(ToolText.Left(i));
					NToolCombined *pToolCon = NULL;
					pDoc->NGetToolList()->Lookup(ToolID, pToolCon);
					pPObj = pToolCon;
				}
				break;
			case NExObjList:
				{
					class NStock *pStock = NULL;
					GetSelListElem(key, pStock);
					pPObj = pStock;
				}
				break;
			case NPartList:
				{
					class NStock *pStock = NULL;
					GetSelListElem(key, pStock);
					pPObj = pStock;
				}
				break;
			case NStocksList:
				{
					class NStock *pStock = NULL;
					GetSelListElem(key, pStock);
					pPObj = pStock;
				}
				break;
			case NMachine:
				{
					HTREEITEM Item = GetTreeCtrl().GetSelectedItem();
					CString AxisName = GetTreeCtrl().GetItemText(Item);
					MTMachineTool &Mach = pDoc->GetMachTool();
					pPObj = Mach.GetPart(AxisName);
					
				}
				break;
			default:
				break;
			}

		}
	}
	return pPObj;
}


void CLeftView::OnChStockDivide()
{
	if(!PaStockCanCreate())
		return;
	((CMainFrame *)AfxGetMainWnd())->OnChStockDivide();
}

void CLeftView::LoadProgramsAux(const std::vector<CString>& names)
{
#ifndef NC_DEMO
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	for (int i = 0; i < int(names.size()); ++i)
	{
		CString filename = names[i];
		//Load program
		NProgram *pP = pDoc->LoadNewProgram(filename);
		if(pP == nullptr) 
		{
			CString str;
			str.Format(IDS_MES_READ,filename);

			if(AfxMessageBox(str,
					MB_ICONQUESTION | 
					MB_YESNO )
				== IDYES)
			
				continue;
			else
				return;
		}
		pDoc->NAddAuxProgram(filename, pP );
	}
	pDoc->UpdateAllViews(NULL, N_LEFT | N_NEW_TEXT | N_SETTINGS | N_PROGRAMCHANGED); // Should be before RegeneratePrograms to ensure correct errors processing
	pDoc->RegeneratePrograms();
	pDoc->UpdateAllViews(NULL, N_LEFT | N_PROGRAMCHANGED | N_TEXT); // Needed to set auto tools; N_TEXT needed to remove error icon
	pDoc->SetModifiedFlag();
#endif
}

void CLeftView::OnLoadAux()
{
	if(!((CMainFrame *)AfxGetMainWnd())->CheckCutting())
		return;
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	
	CString name;
	CString filter;
	CString AddExt("");//Additional file extension defined in the unit definition
	if(!pDoc->GetUnit()->GetFileExt(AddExt))
		AddExt = "*.tap";
	filter.Format(IDS_PROG_FILTER,AddExt,AddExt);
	CString filename;
	BFileDialogWPath fd(_T("LastProgPath"), TRUE, NULL, NULL, NULL, filter);
	fd.m_ofn.Flags = fd.m_ofn.Flags | OFN_ALLOWMULTISELECT;
	CHAR buf[4096] = "";
	fd.m_ofn.lpstrFile = buf;
	fd.m_ofn.nMaxFile = 4096;
	if(fd.DoModal() != IDOK)
		return;
	std::vector<CString> names;
	// For all selected files
	for(POSITION pos = fd.GetStartPosition() ; pos != NULL ;)
	{
		names.push_back(fd.GetNextPathName(pos));
	}
	LoadProgramsAux(names);
}

PAbstractObject * CLeftView::GetAbsObj(void)
{
	HTREEITEM CurItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM ParentItem = GetTreeCtrl().GetParentItem(CurItem);
	return GetAbsObj(CurItem, ParentItem);
}

void CLeftView::GetCheckedToolList(CStringArray &CheckedToolList)
{
	CNCMDoc* pDoc = GetDocument();
	NMapStringToNTool *pToL = pDoc->NGetToolList();
	NObjTreeCtrl& tc = GetTreeCtrl();
	for(HTREEITEM item = tc.GetChildItem(TreeCtrl.NToolItem);
		item != NULL;
		item = tc.GetNextSiblingItem(item))
	{
		if(!TreeCtrl.IsChecked(item))
			continue;
		CString ToolText = tc.GetItemText(item);

		NToolID ToolID;
		int i = ToolText.Find(" ");
		ToolID.CrFromPrintStr(ToolText.Left(i));
		CheckedToolList.Add(ToolID);
	}
}

void CLeftView::OnUpdatePaToolsImpncm(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!((CMainFrame *)AfxGetMainWnd())->IsInImportMode());
}

void CLeftView::OnPaToolsDelunused()
{
	CNCMDoc* pDoc = (CNCMDoc *)GetDocument();
	pDoc->DeleteUnUsedTools();
	pDoc->UpdateAllViews(NULL, N_OGL | N_LEFT);
}

void CLeftView::DeleteItem(HTREEITEM hItem)
{
	NObjTreeCtrl &tc = GetTreeCtrl();
	UINT State = tc.GetItemState(hItem, TVIF_STATE);
	if(State & TVIS_SELECTED)
		tc.SelectItem(NULL);
	tc.DeleteItem(hItem);
}

void CLeftView::LoadStocks(const std::vector<CString>& names)
{
	CNCMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->Comparison.IsActive())
	{
		AfxMessageBox(IDS_END_COMPARE);
		return;
	}
	// For all selected files
	const bool InternalGroup = pDoc->GetUndoHistory().StartGroup();
	for (auto i = 0; i < names.size(); ++i)
	{
		CString NameCopy = names[i];
		const StockFileType StockType = MTEdit::Conv2STL(NameCopy, NStocksList);// NameCopy can be changed here
		if (StockType == SFT_UNKNOWN)
			continue;// ignore file if error while convertion
		switch (StockType)
		{
		case SFT_STL:
		{
			auto Type = NCMComData::GetType();
			if (Type != AT_MILL)
				continue;
			NStockStl *pNewStock = new NStockStl;
			pNewStock->CreateEx(NameCopy);
			if (pNewStock->IsCreated)
				pDoc->AddStock(pNewStock);
			else
				delete pNewStock;
		}
		break;
		case SFT_MFR:
		{
			auto Type = NCMComData::GetType();
			if (Type == AT_MILL)
			{
				NStockBSP *pNewStock = new NStockBSP;
				pNewStock->CreateEx(NameCopy);
				if (pNewStock->IsCreated)
					pDoc->AddStock(pNewStock);
				else
				{
					CString Mes;
					Mes.Format(IDS_MES_FILEO, NameCopy);
					AfxMessageBox(Mes);
					delete pNewStock;
				}
			}
			else
			{
				CreateStockProfile(NULL, NameCopy);
			}
		}
		break;
		case  SFT_CONT:
		{
			auto Type = NCMComData::GetType();
			if (Type != AT_MILL_TURN && Type != AT_TURN)
				continue;
			CreateStockProfile(NULL, NameCopy);
		}
		break;
		default:
			continue;
		}
	}
	pDoc->GetUndoHistory().EndGroup(InternalGroup);
	AfxGetMainWnd()->PostMessage(WM_APP_UPDATE_PANES, N_OGL | N_LEFT);// PostMessage is nesessary for NObjDropTarget::OnDrop works properly
	pDoc->SetModifiedFlag();
}

LRESULT CLeftView::OnLeftDrop(WPARAM wParam, LPARAM lParam)
{
	DocElements ItemData = static_cast<DocElements>(wParam);
	switch (ItemData)
	{
	case NStocksList:
	case DocElements::NStock:
		LoadStocks(names);
		break;
	case NProgramList:
		LoadPrograms(names);
		break;
	case NProgramListAux:
		LoadProgramsAux(names);
		break;
	case NPartList:
		LoadPartsWDial(names);
		break;
	case NExObjList:
		LoadFixtures(names);
		break;
	}
	return 0;
}
