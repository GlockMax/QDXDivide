// EditrorView.cpp : implementation file
//

#include "stdafx.h"
#include "NCEnums.h"
#include "ResInv.h"
#include <math.h>
#include "ResLocal.h"
#include "NCMProject.h"
#include "scintilla\include\scintilla.h"
#include "scintilla\include\SciLexer.h"
#include "NRe2Set.h"
#include "NTextFindDlg.h"
#include "EditorView.h"

int EditorView::AutoUpdateTimeout = 1000;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//static int TEST_ERR = 0;
const int SCI_NCMSTYLE_GCODE=0;
const int SCI_NCMSTYLE_MCODE=1;
const int SCI_NCMSTYLE_TEXT=2;
const int USE_STYLE=127;
const int MARKER_BITS_COUNT = 16*16;
/* XPM */
const char* XPIX_ERROR[] = {
	"16 16 14 1",
		" 	c None",
		".	c #800000",
		"+	c #AA0000",
		"@	c #C00000",
		"#	c #FF0000",
		"$	c #DF0000",
		"%	c #FF4040",
		"&	c #A02020",
		"*	c #FFFF00",
		"=	c #FFBFBF",
		"-	c #802B2B",
		";	c #FF0000",
		">	c #804040",
		",	c #806060",
		"                ",
		" ************** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" ************** ",
		"                " };
const char* XPIX_ERRORMIN[] = {
	"16 16 14 1",
		" 	c None",
		".	c #800000",
		"+	c #AA0000",
		"@	c #C00000",
		"#	c #FF0000",
		"$	c #DF0000",
		"%	c #FF4040",
		"&	c #A02020",
		"*	c #FFFF00",
		"=	c #FFBFBF",
		"-	c #802B2B",
		";	c #FF0000",
		">	c #804040",
		",	c #806060",
		"                ",
		" ************** ",
		" ************** ",
		" ************** ",
		" ************** ",
		" ************** ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" ************** ",
		" ************** ",
		" ************** ",
		" ************** ",
		" ************** ",
		"                " };
const char * XPIX_CURRLINE[] = { // if change something check //Arrows
	"16 16 3 1",
		" 	c None",
		".	c #010100",
		"+	c #00007F",
		"                ",
		"         +      ",
		"         ++     ",
		"         +++    ",
		"         ++++   ",
		" +++++++++++++  ",
		" ++++++++++++++ ",
		" +++++++++++++++",
		" +++++++++++++++",
		" ++++++++++++++ ",
		" +++++++++++++  ",
		"         ++++   ",
		"         +++    ",
		"         ++     ",
		"         +      ",
		"                "};

const char * XPIX_BOOKMARK[] = {
	"16 16 16 1",
		" 	c #000000",
		".	c #800000",
		"+	c #008000",
		"@	c #808000",
		"#	c #000080",
		"$	c #800080",
		"%	c #008080",
		"&	c #C0C0C0",
		"*	c #808080",
		"=	c #FF0000",
		"-	c #00FF00",
		";	c #FFFF00",
		">	c #0000FF",
		",	c #FF00FF",
		"'	c #00FFFF",
		")	c #FFFFF",
		"                ",
		"                ",
		"       =        ",
		"     =====      ",
		"   ==))=====    ",
		"  ===))======   ",
		"  ===))======   ",
		"  ===))))====   ",
		"  ==)))))))==   ",
		"  ==)))))))==   ",
		"  ==)))))))==   ",
		"  ===)))))===   ",
		"   ==)))))==    ",
		"     =====      ",
		"       =        ",
		"                ","\0"};


const char * XPIX_BIND[] = {// if change something check //Arrows
		"16 16 3 1",
			" 	c #000000",
			".	c #FF0000",
			"+	c #007F00",
		"                ",
		"                ",
		"          +     ",
		"          ++    ",
		"          +++   ",
		"          ++++  ",
		" ++++++++++++++ ",
		" +++++++++++++++",
		" +++++++++++++++",
		" ++++++++++++++ ",
		"          ++++  ",
		"          +++   ",
		"          ++    ",
		"          +     ",
		"                ",
		"                "};

const char* XPIX_COLLISION[] = {
	"16 16 14 1",
		" 	c None",
		".	c #800000",
		"+	c #AA0000",
		"@	c #C00000",
		"#	c #FF0000",
		"$	c #DF0000",
		"%	c #FF4040",
		"&	c #A02020",
		"*	c #FFFF00",
		"=	c #FFBFBF",
		"-	c #802B2B",
		";	c #FF0000",
		">	c #804040",
		",	c #806060",
		"                ",
		" ************** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" *;;;;;;;;;;;;* ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" *****;;;;***** ",
		" ************** ",
		"                " };
int EditorView::EDITOR_CARET_SLOP = 2;

// EditrorView

//IMPLEMENT_DYNCREATE(EditorView, CDockablePane)

EditorView::EditorView()
{
	status = STATUS_DEAULT;
	StartStatusLine = -1;
	m_bookmarkCount = 0;
	fEnableColorise = true;
	fBind = false;
	maxTextLength = 0;
	pFindDlg = NULL;
	AutoUpdateTimerID = 0;

	NTiParams& Par = NCM_PROJECT.Defaults;
	CString Key = "Defaults/Editor/AutoUpdate/@";
	AutoUpdate = Par.GetBool((Key + "Enable").GetBuffer(), false);
	AutoUpdateTimeout = int(Par.GetDouble((Key + "Timeout").GetBuffer(), 1000.));
}

EditorView::~EditorView()
{
}


BEGIN_MESSAGE_MAP(EditorView, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MODE_PAINTER, OnModeColorise)
	ON_COMMAND(ID_NEXT_BOOKMARK, OnNextBookmark)
	ON_COMMAND(ID_PREV_BOOKMARK, OnPrevBookmark)
	ON_COMMAND(ID_CLEARALL_BOOKMARK, OnClearAllBookmark)
	ON_COMMAND(ID_BUTTON_ADDSPACE,OnAddSpaces)
	ON_COMMAND(ID_BUTTON_REMOVESPACE,OnRemoveSpaces)
	ON_COMMAND(ID_EDIT_COMMENT,OnAddComment)
	ON_COMMAND(ID_EDIT_UNCOMMENT,OnRemoveComment)
	ON_COMMAND(ID_FIND_TEXT,OnFindText)
	ON_COMMAND(ID_BUTTON_LINENUM, OnLineNum)
	ON_COMMAND(SCEN_CHANGE, AutoUpdateReset)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINENUM, OnUpdateLineNum)
	ON_UPDATE_COMMAND_UI(ID_PREV_BOOKMARK, OnUpdatePrevBookmark)
	ON_UPDATE_COMMAND_UI(ID_NEXT_BOOKMARK, OnUpdateNextBookmark)
	ON_UPDATE_COMMAND_UI(ID_CLEARALL_BOOKMARK, OnUpdateClearallBookmark)
	
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
END_MESSAGE_MAP()


// EditrorView diagnostics

#ifdef _DEBUG
void EditorView::AssertValid() const
{
	CDockablePane::AssertValid();
}

void EditorView::Dump(CDumpContext& dc) const
{
	CDockablePane::Dump(dc);
}
#endif //_DEBUG
//////////////////////////////////////////////////////////////////////////



// EditrorView message handlers

void EditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
}

int EditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	

	m_CurrentLine = -1;
	m_wndEditor.Create(_T(""),WS_CHILD | WS_VISIBLE | WS_TABSTOP,CRect(0,0,0,0),this,10000,NULL);
	char path[500];
	int k=GetModuleFileName(AfxGetApp()->m_hInstance,path,500);
	path[k]='\0';
	::SendMessage(m_wndEditor.GetSafeHwnd(), SCI_LOADLEXERLIBRARY, 0, (LPARAM)path);
	::SendMessage(m_wndEditor.GetSafeHwnd(), SCI_SETLEXERLANGUAGE, 0, (LPARAM)_T("NCM"));
	::SendMessage(m_wndEditor.GetSafeHwnd(), SCI_SETLEXER, SCLEX_CONTAINER, 0);
	pSciMsg = (SciFnDirect)::SendMessage(m_wndEditor.GetSafeHwnd(), SCI_GETDIRECTFUNCTION,0,0);
	pSciWndData = (sptr_t)::SendMessage(m_wndEditor.GetSafeHwnd(), SCI_GETDIRECTPOINTER,0,0);
	SendToScentilla(SCI_SETMARGINTYPEN,0,SC_MARGIN_NUMBER);
	SendToScentilla(SCI_MARKERDEFINEPIXMAP,MARKER_BOOKMARK,(LPARAM)XPIX_BOOKMARK);
	SendToScentilla(SCI_MARKERDEFINEPIXMAP, MARKER_ERROR, (LPARAM)XPIX_ERROR);
	SendToScentilla(SCI_MARKERDEFINEPIXMAP, MARKER_ERROR_MIN, (LPARAM)XPIX_ERRORMIN);
	SendToScentilla(SCI_MARKERDEFINEPIXMAP, MARKER_COLLISION, (LPARAM)XPIX_COLLISION);
	//2019(416)	SendToScentilla(SCI_SETSTYLEBITS,7);
	SendToScentilla(SCI_USEPOPUP,0);
	SendToScentilla(SCI_SETWRAPMODE, SC_WRAP_NONE );
	SetDefaultConfig();

//EK Add
	SendToScentilla(SCI_INDICSETSTYLE, 1, INDIC_SQUIGGLE);// indicator 1 is squiggle underline
	SendToScentilla(SCI_INDICSETSTYLE, 2, INDIC_PLAIN);// indicator 2 is plane underline
	SendToScentilla(SCI_INDICSETFORE, 1, 0x0000ff);// (light red)
	SendToScentilla(SCI_INDICSETFORE, 2, 0xff0000);// (light blue)
	SendToScentilla(SCI_SETCARETLINEFRAME, 2);
	// smart hilighting
	SendToScentilla(SCI_INDICSETSTYLE, 3, INDIC_ROUNDBOX);
	SendToScentilla(SCI_INDICSETALPHA, 3, 30);
	SendToScentilla(SCI_INDICSETUNDER, 3, true);

	if(EDITOR_CARET_SLOP > 0)
		SendToScentilla(SCI_SETYCARETPOLICY, CARET_SLOP | CARET_EVEN | CARET_STRICT, EDITOR_CARET_SLOP);
	SendToScentilla(SCI_SETCARETLINEBACK, RGB(230, 230, 230));
	SendToScentilla(SCI_SETCARETLINEVISIBLE, TRUE);
//2019(416)	SendToScentilla(SCI_SETKEYSUNICODE, true ); // Added 21.11.12 to ensure cyrillic input to work
//END:EK Add

	SendToScentilla(SCI_SETWORDCHARS, 0, (LPARAM)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.+-");
	maxPXTextWidth = 1;
	SetPXScreenWidth();
	SetMarginWidth(100);
	SendToScentilla(SCI_SETLAYOUTCACHE, 0);

//TEXTSTATUS	wndStatusBar.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0,0,0,0), this, ID_EDITOR_STATUSBAR);
	SendToScentilla(SCI_SETMOUSEDWELLTIME, SC_HOVER_TIME);
	SendToScentilla(SCI_CALLTIPSETPOSITION, 1);
	SendToScentilla(SCI_CALLTIPSETBACK, RGB(255, 255, 255));
	SendToScentilla(SCI_CALLTIPSETFORE, RGB(0, 0, 0));
	SendToScentilla(SCI_SETMARGINSENSITIVEN, 1, 1); // margin 1 is sensitive for mouse click
	SendToScentilla(SCI_SETSCROLLWIDTHTRACKING, 1); 
	return 0;
}

void EditorView::Init()
{
	//Инициализация стилей
	//////////////////////////////////////////////////////////////////////////
	//Arrows
	CString str;
	str.Format(_T("+	c #%02X%02X%02X"), Config.arr1color.red, Config.arr1color.green, Config.arr1color.blue);
	XPIX_CURRLINE[3] = str;
	SendToScentilla(SCI_MARKERDEFINEPIXMAP, MARKER_ARROW, (LPARAM)XPIX_CURRLINE);

	str.Format("+	c #%02X%02X%02X", Config.arr2color.red, Config.arr2color.green, Config.arr2color.blue);
	XPIX_BIND[3] = str;
	SendToScentilla(SCI_MARKERDEFINEPIXMAP, MARKER_BIND, (LPARAM)XPIX_BIND);
	//Шрифты
	SendToScentilla(SCI_STYLESETFONT,STYLE_DEFAULT,(LPARAM)Config.font.lfFaceName);
	LONG FontSize = -MulDiv(Config.font.lfHeight, 72, GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY));
	SendToScentilla(SCI_STYLESETSIZE,STYLE_DEFAULT, FontSize);
	SendToScentilla(SCI_STYLESETWEIGHT,STYLE_DEFAULT,Config.font.lfWeight);
	SendToScentilla(SCI_STYLESETITALIC, STYLE_DEFAULT, Config.font.lfItalic != 0);
	SendToScentilla(SCI_STYLESETUNDERLINE, STYLE_DEFAULT, Config.font.lfUnderline != 0);
	//////////////////////////////////////////////////////////////////////////
	
	SendToScentilla(SCI_STYLESETBACK, STYLE_DEFAULT,*((DWORD*)&Config.wbkcolor));
	SendToScentilla(STYLE_DEFAULT);
	SendToScentilla(SCI_STYLECLEARALL);

	SendToScentilla(SCI_SETCODEPAGE, 0);
	SendToScentilla(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_RUSSIAN);// Нет необходимости. SCI_SETCODEPAGE, 0 рещает проблему <- неправда

	SendToScentilla(SCI_STYLESETFORE, (LPARAM)STYLE_LINENUMBER, *((DWORD*)&Config.numfrcolor));
	SendToScentilla(SCI_STYLESETBACK, (LPARAM)STYLE_LINENUMBER, *((DWORD*)&Config.numbkcolor));

	SendToScentilla(SCI_STYLESETFORE, (LPARAM)Config.StyleComment.ID, *((DWORD*)&Config.StyleComment.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleGCode.ID,*((DWORD*)&Config.StyleGCode.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleMCode.ID,*((DWORD*)&Config.StyleMCode.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleOther.ID,*((DWORD*)&Config.StyleOther.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleReg.ID,*((DWORD*)&Config.StyleReg.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleText.ID,*((DWORD*)&Config.StyleText.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleDigit.ID,*((DWORD*)&Config.StyleDigit.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleCmdNumber.ID,*((DWORD*)&Config.StyleCmdNumber.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleLineNumber.ID,*((DWORD*)&Config.StyleLineNumber.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleFeedCode.ID,*((DWORD*)&Config.StyleFeedCode.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleSpeedCode.ID,*((DWORD*)&Config.StyleSpeedCode.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleToolCode.ID,*((DWORD*)&Config.StyleToolCode.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleLinear.ID,*((DWORD*)&Config.StyleLinear.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleAngle.ID,*((DWORD*)&Config.StyleAngle.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleArc.ID,*((DWORD*)&Config.StyleArc.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleOffset.ID,*((DWORD*)&Config.StyleOffset.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleProgramSE.ID,*((DWORD*)&Config.StyleProgramSE.color));
	SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleProgramBE.ID,*((DWORD*)&Config.StyleProgramBE.color));
	
	//////////////////////////////////////////////////////////////////////////
	for(int i = 0; i < NUM_CURVETYPE; i++)
	{
//		SendToScentilla(SCI_STYLESETBACK,(LPARAM)Config.StyleCurveType[i].ID,*((DWORD*)&Config.StyleCurveType[i].bkcolor));
		SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleCurveType[i].ID,*((DWORD*)&Config.StyleCurveType[i].fcolor));
	}
	for(int i = 0; i < NUM_TOOL; i++)
	{
//		SendToScentilla(SCI_STYLESETBACK,(LPARAM)Config.StyleTool[i].ID,*((DWORD*)&Config.StyleTool[i].bkcolor));
		SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleTool[i].ID,*((DWORD*)&Config.StyleTool[i].fcolor));
	}
	for(int i = 0; i < NUM_COOL; i++)
	{
//		SendToScentilla(SCI_STYLESETBACK,(LPARAM)Config.StyleCool[i].ID,*((DWORD*)&Config.StyleCool[i].bkcolor));
		SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleCool[i].ID,*((DWORD*)&Config.StyleCool[i].fcolor));
	}
	for(int i = 0; i < NUM_CYCLE; i++)
	{
//		SendToScentilla(SCI_STYLESETBACK,(LPARAM)Config.StyleCycle[i].ID,*((DWORD*)&Config.StyleCycle[i].bkcolor));
		SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleCycle[i].ID,*((DWORD*)&Config.StyleCycle[i].fcolor));
	}
	for(int i = 0; i < NUM_FEED; i++)
	{
//		SendToScentilla(SCI_STYLESETBACK,(LPARAM)Config.StyleFeed[i].ID,*((DWORD*)&Config.StyleFeed[i].bkcolor));
		SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleFeed[i].ID,*((DWORD*)&Config.StyleFeed[i].fcolor));
	}
	for(int i = 0; i < NUM_COMP; i++)
	{
//		SendToScentilla(SCI_STYLESETBACK,(LPARAM)Config.StyleComp[i].ID,*((DWORD*)&Config.StyleComp[i].bkcolor));
		SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleComp[i].ID,*((DWORD*)&Config.StyleComp[i].fcolor));
	}
	for(int i = 0; i < NUM_LR; i++)
	{
//		SendToScentilla(SCI_STYLESETBACK,(LPARAM)Config.StyleLRComp[i].ID,*((DWORD*)&Config.StyleLRComp[i].bkcolor));
		SendToScentilla(SCI_STYLESETFORE,(LPARAM)Config.StyleLRComp[i].ID,*((DWORD*)&Config.StyleLRComp[i].fcolor));
	}
	//////////////////////////////////////////////////////////////////////////
	
}

BOOL EditorView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{

	NMHDR *lpnmhdr;
	lpnmhdr = (LPNMHDR) lParam; 

	SCNotification *sn;
	if(lpnmhdr->hwndFrom == m_wndEditor.GetSafeHwnd())
	{
		sn = (SCNotification *)lParam;
		if(lpnmhdr->code == SCN_STYLENEEDED)
		{
			Sci_Position startPos = (Sci_Position)::SendMessage(m_wndEditor.GetSafeHwnd(), SCI_GETENDSTYLED, 0, 0);
			Sci_Position lineNumber = (Sci_Position)::SendMessage(m_wndEditor.GetSafeHwnd(), SCI_LINEFROMPOSITION, startPos, 0);
			startPos = (Sci_Position)::SendMessage(m_wndEditor.GetSafeHwnd(), SCI_POSITIONFROMLINE, lineNumber, 0);
			if (startPos != sn->position)
				Colorise(startPos, sn->position);
		}
		else if(lpnmhdr->code == SCN_DOUBLECLICK)
		{
			SCI_OnDblClick();
		}
		else if(lpnmhdr->code == SCN_HOTSPOTDOUBLECLICK)
		{
			SCI_OnHotDblClick();
		}
		else if(lpnmhdr->code == SCN_UPDATEUI)
		{
			auto SStart = SendToScentilla(SCI_GETSELECTIONSTART);
			auto SEnd = SendToScentilla(SCI_GETSELECTIONEND);
			if (SEnd > SStart)
			{
				if (SEnd - SStart < 255)
				{
					char dest[256];
					Sci_TextRange tr;
					tr.chrg.cpMin = static_cast<long>(SStart);
					tr.chrg.cpMax = static_cast<long>(SEnd);
					tr.lpstrText = dest;
					SendToScentilla(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
					Word2Highlight = CString(dest);
				}
				else
					Word2Highlight.Empty();
			}
			else
			{
				static LRESULT PrevPos = -1;
				auto CurPos = SendToScentilla(SCI_GETCURRENTPOS);
				if (PrevPos != CurPos)
				{
					Word2Highlight.Empty();
					auto curLine = SendToScentilla(SCI_LINEFROMPOSITION, CurPos);
					auto firstPos = SendToScentilla(SCI_POSITIONFROMLINE, curLine);
					if (CurPos != firstPos)
					{
						auto wordStart = SendToScentilla(SCI_WORDSTARTPOSITION, CurPos, true);
						auto wordEnd = SendToScentilla(SCI_WORDENDPOSITION, wordStart, true);

						if (wordStart != wordEnd && wordEnd - wordStart < 255)
						{
							char dest[256];
							Sci_TextRange tr;
							tr.chrg.cpMin = static_cast<long>(wordStart);
							tr.chrg.cpMax = static_cast<long>(wordEnd);
							tr.lpstrText = dest;
							SendToScentilla(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
							Word2Highlight = CString(dest);
						}
					}

					PrevPos = CurPos;
				}
			}
			HighlightWord(Word2Highlight);
		}
		else if(lpnmhdr->code == SCN_MODIFIED)
		{
			if (sn->linesAdded != 0)
				SetMarginWidth(GetLineCount());
			if(
				!(SC_MOD_CHANGESTYLE & sn->modificationType) && !(SC_PERFORMED_UNDO & sn->modificationType)
				&& !(SC_PERFORMED_REDO & sn->modificationType) && !(SC_MOD_CHANGEFOLD & sn->modificationType)
				&& !(SC_MOD_CHANGEMARKER & sn->modificationType))
			{
				SendToScentilla(SCI_BEGINUNDOACTION);// This two lines prevent undo actions joining up
				SendToScentilla(SCI_ENDUNDOACTION);
				if((GetLineWidth(sn->line) > maxTextLength)&&(abs(sn->linesAdded)<=1))
				{
					maxTextLength = sn->length;
					if(fEnableColorise)
						SetPXScreenWidth();
				}
			}
			if (AutoUpdate && ((SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT) & sn->modificationType))
					AutoUpdateReset();
		}
		else if (lpnmhdr->code == SCN_UPDATEUI && fBind)
		{
			int line = GetCurLine();
			if(m_CurrentLine != line)
			{
				OnChangeLine(line);
				//SetBindMarker(line);
				m_CurrentLine = line;
			}
		}
		else if (lpnmhdr->code == SCN_ZOOM)
		{
			SetMarginWidth(GetLineCount());
		}
		else if (lpnmhdr->code == SCN_DWELLSTART)
		{
			if (sn->position == -1) // margin
			{
				const auto width0 = SendToScentilla(SCI_GETMARGINWIDTHN, 0);
				const auto width1 = SendToScentilla(SCI_GETMARGINWIDTHN, 1);
				if (sn->x >= width0 && sn->x < width0 + width1) // dwell on margin #1
				{
					const auto pos = SendToScentilla(SCI_POSITIONFROMPOINT, sn->x, sn->y);
					const int curline = (int)SendToScentilla(SCI_LINEFROMPOSITION, pos);
					const auto markerMask = SendToScentilla(SCI_MARKERGET, curline);
					if (markerMask & (0x1 << MARKER_ERROR_MIN)) // bit mask for error min marker
					{
						const CString Text = GetErrorMinText(curline);
						if (!Text.IsEmpty())
							SendToScentilla(SCI_CALLTIPSHOW, pos, LPARAM(Text.GetString()));
					}
					else if (markerMask & (0x1 << MARKER_COLLISION)) // bit mask for collision marker
					{
						const CString Text = GetCollisionText(curline);
						if (!Text.IsEmpty())
							SendToScentilla(SCI_CALLTIPSHOW, pos, LPARAM(Text.GetString()));
					}
				}
			}
		}
		else if (lpnmhdr->code == SCN_DWELLEND)
		{
			SendToScentilla(SCI_CALLTIPCANCEL);
		}
		else if (lpnmhdr->code == SCN_MARGINCLICK)
		{
			if (sn->margin == 1)
			{
				SendToScentilla(SCI_GOTOPOS, sn->position);
				OnAddBookmark();
			}
		}
	}
	return CDockablePane::OnNotify(wParam, lParam, pResult);
}

void EditorView::Colorise(ptrdiff_t startPos, ptrdiff_t endPos)
{
	if(!fEnableColorise)
		return;
	int	StartLineNumber = (int)SendToScentilla(SCI_LINEFROMPOSITION,startPos,0);
	CString str=GetText(StartLineNumber);
	int e = Config.BlockCommentEnd.IsEmpty() ? -1 : str.Find(Config.BlockCommentEnd);
	if(e != -1 && STATUS_BLOCKCOMENT != status)
	{
		int s = Config.BlockCommentStart.IsEmpty() ? -1 : str.Find(Config.BlockCommentStart);
		
		if(s == -1 || s >= e)
		{
			while (StartLineNumber>0)
			{
				StartLineNumber --;
				str = GetText(StartLineNumber);
				if(str.Find(Config.BlockCommentStart) != -1)
					break;
			}
		}
	}
	//StartLineNumber = (int)SendToScentilla(SCI_LINEFROMPOSITION,startPos,0);
	startPos = (int)SendToScentilla(SCI_POSITIONFROMLINE,StartLineNumber);
	int lastLineNumber = (int)SendToScentilla(SCI_LINEFROMPOSITION,endPos,0);
	SendToScentilla(SCI_STARTSTYLING,startPos,USE_STYLE);
//	SendToScentilla(SCI_STYLESETBACK,(LPARAM)Config.StyleComment.ID,0);
	for(int lineNumber = StartLineNumber;lineNumber<=lastLineNumber;lineNumber++)
	{
		str=GetText(lineNumber);
		str.MakeUpper();
		int length=0;
		int type=0;
		int slength=0;
		//SendToScentilla(SCI_SETSTYLING,endPos - startPos,Config.StyleGCode.ID);
		if(status == STATUS_STRINGCOMENT)
			status = STATUS_DEAULT;
		if(status == STATUS_BLOCKCOMENT && StartStatusLine == lineNumber)
			status = STATUS_DEAULT;
		while (0 != str.GetLength()) 
		{
			type = GetType(str,length);
			slength = length;
			int style=-1;
			if(status == STATUS_STRINGCOMENT)
			{
				if(StartStatusLine != lineNumber)
					status = STATUS_DEAULT;
				else 
					style = Config.StyleComment.ID;
			}
			else if(status == STATUS_BLOCKCOMENT)
			{
				if(type == editor_cfg::ID_SPECSTYLE_BLOCKCOMENTEND)
					status = STATUS_DEAULT;
				style = Config.StyleComment.ID;
			}
			if(style < 0)
			{
				switch(type)
				{
				case editor_cfg::ID_STYLE_CMDNUMBER:
					if(Config.fCmdNumber)
						style = oldType;
					else
						style = type;
					oldType = type;
					break;
				case editor_cfg::ID_STYLE_COMENT:
					status = STATUS_STRINGCOMENT;
					StartStatusLine = lineNumber;
					style = type;
					break;
				case editor_cfg::ID_SPECSTYLE_BLOCKCOMENTSTART:
					status = STATUS_BLOCKCOMENT;
					StartStatusLine = lineNumber;
					style = Config.StyleComment.ID;
					break;
				default:
					style = type;
					oldType = type;
				}
			}
			SendToScentilla(SCI_SETSTYLING,slength,style);
		}
	}
}



int EditorView::GetType(CString& str,int& length)
{
	CString cmd(GetLexem(str));
	length = cmd.GetLength();
	str.Delete(0, max(length, 1));
	int counter = 0;
	while (cmd.GetLength() > counter)
	{
		if(cmd[counter] == '\n' || cmd[counter] == '\r' || cmd[counter] == ' ' || cmd[counter] == '\t')
			cmd.Delete(counter);
		else
			counter++;
	}
	if(IsDigit(cmd))
	{
		if(oldType == Config.StyleGCode.ID || oldType == Config.StyleMCode.ID)
			return Config.StyleCmdNumber.ID;
		else
			return Config.StyleDigit.ID;
	}
	else if(cmd == Config.General)
		return Config.StyleGCode.ID;
	else if(cmd == Config.Misc)
		return Config.StyleMCode.ID;
	else if(cmd == Config.Register)
		return Config.StyleReg.ID;
	else if(cmd == Config.StringComment)
		return Config.StyleComment.ID;
	//else if(cmd == Config.CommentBlock)
	//	return Config.StyleComment.ID;
	else if(cmd == Config.BlockCommentEnd)
		return editor_cfg::ID_SPECSTYLE_BLOCKCOMENTEND;
	else if(cmd == Config.BlockCommentStart)
		return editor_cfg::ID_SPECSTYLE_BLOCKCOMENTSTART;
	else if(cmd == Config.LineNumber)
		return Config.StyleLineNumber.ID;
	else if(cmd == Config.Feed)
		return Config.StyleFeedCode.ID;
	else if(cmd == Config.Speed)
		return Config.StyleSpeedCode.ID;
	else if(cmd == Config.Tool)
		return Config.StyleToolCode.ID;
	else if(cmd == Config.ProgramSE)
		return Config.StyleProgramSE.ID;
	else if(cmd == Config.ProgramBE)
		return Config.StyleProgramBE.ID;
	else if(cmd == Config.HorizontalAxis || cmd == Config.VerticalAxis 
		|| cmd == Config.SpindleAxis)
		return Config.StyleLinear.ID;
	else if(cmd == Config.HorizontalRotationAxis || cmd == Config.VerticalRotationAxis 
		|| cmd == Config.SpindleRotationAxis)
		return Config.StyleAngle.ID;
	else if(cmd == Config.CircleHorzCenter || cmd == Config.CircleVerticalCenter 
		|| cmd == Config.CircleDepthCenter || cmd == Config.CircleRadius)
		return Config.StyleArc.ID;
	else if(cmd == Config.DiaCompOffset || cmd == Config.LengthCompOffset)
		return Config.StyleOffset.ID;
	else
	{
		if(IsWordDef(cmd))
			return Config.StyleOther.ID;
		return Config.StyleText.ID;
	}

	return 0;
}

LRESULT EditorView::SendToScentilla(UINT message, WPARAM wparam,LPARAM lparam) const
{
	///return ::SendMessage(m_wndEditor.GetSafeHwnd(),message,wparam,lparam);
	return pSciMsg(pSciWndData, message, wparam, lparam);
}

CString EditorView::GetText(int lineNumber) const
{
	int lineLength;
	char buf[1000];
	CString str;
	if (lineNumber >= 0)
	{
		lineLength = GetLineLength(lineNumber);
		if (0 == lineLength)
			return "";
		SendToScentilla(SCI_GETLINE, lineNumber, (LPARAM)buf);
		buf[lineLength] = '\0';
		str = buf;
		return str;
	}
	int lineCount = (int)SendToScentilla(SCI_GETLINECOUNT);
	for (int i = 0; i < lineCount; i++)
	{
		lineLength = GetLineLength(i);
		if (0 == lineLength)
		{
			str += "\n";
			continue;
		}
		SendToScentilla(SCI_GETLINE, i, (LPARAM)buf);
		buf[lineLength] = '\0';
		str += buf;
	}
	return str;
}

int EditorView::GetLineLength(int lineNumber) const
{
	return (int)SendToScentilla( SCI_LINELENGTH,lineNumber);
}

void EditorView::SetConfig(editor_cfg cfg)
{
	Config = cfg;
	Init();
}

editor_cfg EditorView::GetConfig(void)
{
	return Config;
}
void EditorView::SetDefaultConfig(void)
{
	//memset(&Config,0,sizeof(editor_cfg));
	Config.StyleText.short_name="НераспознанныеСимволы";
	Config.StyleText.description="Цвет раскраски простого текста программы";
	Config.StyleText.ID=editor_cfg::ID_STYLE_TEXT;
	Config.StyleGCode.short_name="GКоды";
	Config.StyleGCode.description="Цвет раскраски G кодов";
	Config.StyleGCode.ID=editor_cfg::ID_STYLE_GCODE;
	Config.StyleMCode.short_name="MКоды";
	Config.StyleMCode.description="Цвет раскраски M кодов";
	Config.StyleMCode.ID=editor_cfg::ID_STYLE_MCODE;
	Config.StyleReg.short_name="Регистр";
	Config.StyleReg.description="Цвет раскраски регистров";
	Config.StyleReg.ID=editor_cfg::ID_STYLE_REGISTER;
	Config.StyleOther.short_name="Обозначения";
	Config.StyleOther.description="Цвет раскраски обозначений";
	Config.StyleOther.ID=editor_cfg::ID_STYLE_OTHER;
	Config.StyleDigit.short_name="ПростыеЧисла";
	Config.StyleDigit.description="Цвет раскраски простых чисел";
	Config.StyleDigit.ID=editor_cfg::ID_STYLE_DIGIT;
	Config.StyleCmdNumber.short_name="НомерКоманды";
	Config.StyleCmdNumber.description="Цвет раскраски номера команды";
	Config.StyleCmdNumber.ID=editor_cfg::ID_STYLE_CMDNUMBER;
	Config.StyleComment.short_name="Комментарии";
	Config.StyleComment.description="Цвет раскраски комментариев";
	Config.StyleComment.ID=editor_cfg::ID_STYLE_COMENT;
	Config.General="G";
	Config.Misc="M";
	Config.Register="R";
	Config.StringComment="//";
	Config.BlockDebug=";";
	Config.BlockCommentStart = "(*";
	Config.BlockCommentEnd = "*)";
	Config.ModalCommentStart = "(*";
	Config.ModalCommentEnd = "*)";

	Config.StyleCurveType[0].ID = editor_cfg::ID_STYLE_CT_FAST;
	Config.StyleCurveType[0].short_name = "ХолХод";

	Config.StyleCurveType[1].ID = editor_cfg::ID_STYLE_CT_LINE;
	Config.StyleCurveType[1].short_name = "ДвижениеПоОтрезку";

	Config.StyleCurveType[2].ID = editor_cfg::ID_STYLE_CT_CWARC;
	Config.StyleCurveType[2].short_name = "ДвижениеПЧС";

	Config.StyleCurveType[3].ID = editor_cfg::ID_STYLE_CT_CCWARC;
	Config.StyleCurveType[3].short_name = "ДвижениеПрЧС";

	for (int i = 0;i < NUM_TOOL; i++)
	{
		Config.StyleTool[i].ID = editor_cfg::ID_STYLE_TOOL + i;
	}

	Config.StyleCool[0].ID = editor_cfg::ID_STYLE_COOL_OFF;
	Config.StyleCool[0].short_name = "БезОхлаждения";

	Config.StyleCool[1].ID = editor_cfg::ID_STYLE_COOL_MIST;
	Config.StyleCool[1].short_name = "ОхлаждениеВоздухом";

	Config.StyleCool[2].ID = editor_cfg::ID_STYLE_COOL_FLOOD;
	Config.StyleCool[2].short_name = "ОхлаждениеЖидкостью";

	int counter = 0;
	Config.StyleCycle[0].ID = editor_cfg::ID_STYLE_CYCLE_DRILL;
	Config.StyleCycle[0].short_name = "ЦиклСверления";
	Config.StyleCycle[1].ID = editor_cfg::ID_STYLE_CYCLE_REAM;
	Config.StyleCycle[1].short_name = "ЦиклРезьбы";
	Config.StyleCycle[2].ID = editor_cfg::ID_STYLE_CYCLE_TAPPING;
	Config.StyleCycle[2].short_name = "ЦиклРасточки";

	Config.StyleComp[0].ID = editor_cfg::ID_STYLE_COMP_UP;
	Config.StyleComp[0].short_name = "КоррекцияВверх";
	Config.StyleComp[1].ID = editor_cfg::ID_STYLE_COMP_DOWN;
	Config.StyleComp[1].short_name = "КоррекцияВниз";
	Config.StyleComp[2].ID = editor_cfg::ID_STYLE_COMP_OFF;
	Config.StyleComp[2].short_name = "КоррекцияНаДлинуВыкл";

	Config.StyleLRComp[0].ID = editor_cfg::ID_STYLE_LRCOMP_OFF;
	Config.StyleLRComp[0].short_name = "КонтурнаяКоррекцияВыкл";
	Config.StyleLRComp[1].ID = editor_cfg::ID_STYLE_LRCOMP_LEFT;
	Config.StyleLRComp[1].short_name = "КоррекцияВлево";
	Config.StyleLRComp[2].ID = editor_cfg::ID_STYLE_LRCOMP_RIGHT;
	Config.StyleLRComp[2].short_name = "КоррекцияВправо";
	Config.StyleFeed[0].ID = editor_cfg::ID_STYLE_FEED_MAX;
	Config.StyleFeed[0].short_name = "МаксимальнаяПодача";
	Config.StyleFeed[1].ID = editor_cfg::ID_STYLE_FEED_RAPID;
	Config.StyleFeed[1].short_name = "ХолостойХод";
	Config.StyleFeed[2].ID = editor_cfg::ID_STYLE_FEED_FIRST;
	Config.StyleFeed[2].short_name = "Подача1";
	Config.StyleFeed[3].ID = editor_cfg::ID_STYLE_FEED_SECOND;
	Config.StyleFeed[3].short_name = "Подача2";

	Config.StyleLineNumber.ID = editor_cfg::ID_STYLE_LINENUMBER;
	Config.StyleLineNumber.short_name = "НомерЛинии";
	
	Config.StyleFeedCode.ID = editor_cfg::ID_STYLE_FEEDCODE;
	Config.StyleFeedCode.short_name = "Подача";

	Config.StyleSpeedCode.ID = editor_cfg::ID_STYLE_FEEDCODE;
	Config.StyleSpeedCode.short_name = "Скорость";
	
	Config.StyleToolCode.ID = editor_cfg::ID_STYLE_TOOLCODE;
	Config.StyleToolCode.short_name = "Инструмет";
	
	Config.StyleLinear.ID = editor_cfg::ID_STYLE_LINEAR;
	Config.StyleLinear.short_name = "ЛинейныеКоординаты";
	
	Config.StyleAngle.ID = editor_cfg::ID_STYLE_ANGLE;
	Config.StyleAngle.short_name = "УгловыеКоординаты";
	
	Config.StyleArc.ID = editor_cfg::ID_STYLE_ARC;
	Config.StyleArc.short_name = "КруговаяИнтерполяция";
	
	Config.StyleOffset.ID = editor_cfg::ID_STYLE_OFFSET;
	Config.StyleOffset.short_name = "Коррекция";
	
	Config.StyleProgramSE.ID = editor_cfg::ID_STYLE_PROGRAMSE;
	Config.StyleProgramSE.short_name = "СтартПрограммы";
	
	Config.StyleProgramBE.ID = editor_cfg::ID_STYLE_PROGRAMBE;
	Config.StyleProgramBE.short_name = "НачалоПрограммы";



	//Config.font.font_name[0] = 0;
	//Config.font.font_bold = 1;
	//Config.font.font_italic = 0;
	//Config.font.font_size = 10;

//	Init();
}

bool EditorView::IsDigit(const CString &str)
{
	if(atof(str)!=0)
		return true;
	bool fPoint = false;
	bool fSign = false; 
	for(int i=0;i<str.GetLength();i++)
	{
		if(str[i]!='0')
		{
			if(str[i]=='.' && fPoint)
				return false;
			if('.' == str[i])
			{
				fPoint = true;
				continue;
			}
			if((('+' == str[i])||('-' == str[i]))&&(fSign||fPoint))
				return false;
			if(('+' == str[i])||('-' == str[i]))
			{
				fSign = true;
				continue;
			}
			return false;
		}
	}
	return true;
}

CString EditorView::GetLexem(const CString& str)
{
	if(0 == str.GetLength())
		return "";
	if(IsDigit(str[0]))
	{
		int i = 1;
		for(;i<str.GetLength();i++)
		{
			if(!IsDigit(str[i]))
				break;
		}
		return str.Left(i);
	}
	int i = 0;
	for(;i<str.GetLength();i++)
	{
		if(IsDigit(str[i]))
			break;
	}
	CString temp = str.Left(i);
	if(-1 != temp.Find(Config.StringComment))
	{
		int k = temp.Find(Config.StringComment);
		if(k == 0)
			return temp.Left(Config.StringComment.GetLength());
		temp = temp.Left(k);
	}
	//if(-1 != temp.Find(Config.CommentBlock))
	//{
	//	int k = temp.Find(Config.CommentBlock);
	//	if(k == 0)
	//		return temp.Left(Config.CommentBlock.GetLength());
	//	temp = temp.Left(k);
	//}
	int k = Config.BlockCommentStart.IsEmpty() ? -1 : temp.Find(Config.BlockCommentStart);
	if(-1 != k)
	{
		if(k == 0)
			return temp.Left(Config.BlockCommentStart.GetLength());
		temp = temp.Left(k);
	}
	k = Config.BlockCommentEnd.IsEmpty() ? -1 : temp.Find(Config.BlockCommentEnd);
	if(-1 != k)
	{
		int k = temp.Find(Config.BlockCommentEnd);
		if(k == 0)
			return temp.Left(Config.BlockCommentEnd.GetLength());
		temp = temp.Left(k);
	}
	return temp;
}

void EditorView::SCI_OnDblClick(void)
{
	OnEditorDblClick();
}

void EditorView::SCI_OnHotDblClick()
{
}

int EditorView::AddLine(CString str, int linenum)
{
	int cl = (int)SendToScentilla(SCI_GETLINECOUNT);
	if(str == "")
		str = "\n";
	else if(str[str.GetLength()-1] != '\n')
		str+="\n";
	if(linenum == -1 || cl == linenum)
	{
		//pos = (int)SendToScentilla(SCI_GETLINECOUNT);
		SendToScentilla(SCI_APPENDTEXT,str.GetLength(),LPARAM((LPCTSTR)str));
		linenum = cl;
	}
	else if(cl < linenum)
	{
		for(int i = 0; i<linenum - cl; i++)
			SendToScentilla(SCI_APPENDTEXT,1,(LPARAM)"\n");
	}
	else if(cl > linenum)
	{
		int pos = GetStartLinePos(linenum);
		str +='\0';
		SendToScentilla(SCI_INSERTTEXT, pos, LPARAM((LPCTSTR)str));
	}
	else
		SendToScentilla(SCI_APPENDTEXT, str.GetLength(), LPARAM((LPCTSTR)str));
	Sci_Position cx = GetLineWidth(linenum);
	if(cx > maxTextLength)
	{
		LNWidth = cx;
		SetPXScreenWidth();
	}
	return GetStartLinePos(linenum);
}
void EditorView::ReplaceLine(CString str, int linenum)
{
	int cl = (int)SendToScentilla(SCI_GETLINECOUNT);
	if (linenum < 0 || linenum >= cl)
		return;

	if (str == "")
		str = "\n";
	else if (str[str.GetLength() - 1] != '\n')
		str += "\n";

	int startPos = GetStartLinePos(linenum);
	int endPos = GetStartLinePos(linenum + 1);
	SendToScentilla(SCI_SETTARGETRANGE, startPos, endPos);
	SendToScentilla(SCI_REPLACETARGET, str.GetLength(), LPARAM((LPCTSTR)str));
}
int EditorView::GetCurLine()
{
	int anc = (int)SendToScentilla(SCI_GETANCHOR);
	int pos = (int)SendToScentilla(SCI_GETCURRENTPOS);
	int ps = pos;
	if(anc < pos)
		ps = anc;
	int curline = (int)SendToScentilla(SCI_LINEFROMPOSITION,ps);
	return curline;
}
int EditorView::GetCurLineWOAnchor()
{
	int pos = (int)SendToScentilla(SCI_GETCURRENTPOS);
	int curline = (int)SendToScentilla(SCI_LINEFROMPOSITION, pos - 1);
	return curline;
}

void EditorView::OnAddBookmark()
{
	int line = GetCurLine();
	int mmask = (int)SendToScentilla(SCI_MARKERGET,line);
	if(mmask &(1 << MARKER_BOOKMARK))
	{
		SendToScentilla(SCI_MARKERDELETE,line,MARKER_BOOKMARK);
		m_bookmarkCount--;
		return;
	}
	SendToScentilla(SCI_MARKERADD,line,MARKER_BOOKMARK);
	m_bookmarkCount++;
	CDocument* pDoc = GetDocument();
	pDoc->SetModifiedFlag();
}

void EditorView::OnNextBookmark()
{
	int findmask = (1 << MARKER_BOOKMARK);
	int line = GetCurLine();
	int mmask = (int)SendToScentilla(SCI_MARKERGET, line);
	if(mmask & findmask)
		line++;
	int mLine = (int)SendToScentilla(SCI_MARKERNEXT, line, findmask);
	if(mLine < 0)
		mLine = (int)SendToScentilla(SCI_MARKERNEXT, 0, findmask);
	if(mLine < 0)
		return;
	SendToScentilla(SCI_SETFOCUS, TRUE);// To activate cursor on destination line
	SendToScentilla(SCI_GOTOLINE, mLine);
}
void EditorView::GetBookmarkArr(NArray& arr)
{
	int line = 0;
	int findmask = (1 << MARKER_BOOKMARK);
	int mmask = 0;
	arr.RemoveAll();
	while(line >= 0)
	{
		line = (int)SendToScentilla(SCI_MARKERNEXT,line,findmask);
		if(line < 0)
			return;
		arr.Add(line);
		line++;
	}
}

void EditorView::SetBookmarkArr(const NArray& arr)
{
	SendToScentilla(SCI_MARKERDELETEALL,MARKER_BOOKMARK);
	for(int i = 0; i < arr.GetSize(); i++)
		SendToScentilla(SCI_MARKERADD,arr[i],MARKER_BOOKMARK);
	m_bookmarkCount = int(arr.GetSize());
}

void EditorView::OnPrevBookmark()
{
	int findmask = (1 << MARKER_BOOKMARK);
	int line = GetCurLine();
	int mmask = (int)SendToScentilla(SCI_MARKERGET,line);
	if((mmask & findmask) && line>0)
		line--;
	int mLine = (int)SendToScentilla(SCI_MARKERPREVIOUS,line,findmask);
	if(mLine < 0)
	{
		mLine = GetLineCount();
		mLine = (int)SendToScentilla(SCI_MARKERPREVIOUS,mLine,findmask);
	}
	if(mLine < 0)
		return;
	SendToScentilla(SCI_GOTOLINE,mLine);
}
void EditorView::OnClearAllBookmark()
{
	SendToScentilla(SCI_MARKERDELETEALL,MARKER_BOOKMARK);
	m_bookmarkCount = 0;
	CDocument* pDoc = GetDocument();
	pDoc->SetModifiedFlag();
}
void EditorView::OnUpdatePrevBookmark(CCmdUI* pCmdUI)
{
	if(m_bookmarkCount)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}
void EditorView::OnUpdateNextBookmark(CCmdUI* pCmdUI)
{
	if(m_bookmarkCount)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}
void EditorView::OnUpdateClearallBookmark(CCmdUI* pCmdUI)
{
	if(m_bookmarkCount)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void EditorView::OnAddComment()
{
	CommentSelection(false);
}

void EditorView::OnRemoveComment()
{
	CommentSelection(true);
	return;
}

void EditorView::OnAddSpaces()
{
	SendToScentilla(SCI_BEGINUNDOACTION);
	OnRemoveSpaces();
	CStringArray strArr;
	SaveBookmarks();
	NRe2Set ReplaceSet;
	ReplaceSet.AddReplacePair(0, _T("([A-M,O-Z])"), _T(" \\1"));
	int lc = GetLineCount();
	for(int i = 0;i < lc; ++i)
	{
		CString str = GetLine(i);
		ReplaceSet.Apply(str);
		strArr.Add(str);
	}
	DeleteAllText(false);
	for(int i = 0;i < strArr.GetSize(); i++)
		AddLine(strArr[i]);
	RestoreBookmarks();
	SendToScentilla(SCI_ENDUNDOACTION);
	CDocument* pDoc = GetDocument();
	pDoc->SetModifiedFlag();
}

void EditorView::OnRemoveSpaces()
{
	SendToScentilla(SCI_BEGINUNDOACTION);
	CString str;
	CStringArray strArr;
	SaveBookmarks();
	int lc = GetLineCount();
	for(int i = 0;i < lc;i++)
	{
		str = GetLine(i);
		int pos = 0;
		while (pos<str.GetLength())
		{
			if(str[pos] == ' ')
			{
				str.Delete(pos);
				continue;
			}
			pos++;
		}
		strArr.Add(str);
	}
	DeleteAllText(false);
	for(int i = 0;i < strArr.GetSize(); i++)
		AddLine(strArr[i]);
	RestoreBookmarks();
	SendToScentilla(SCI_ENDUNDOACTION);
	CDocument* pDoc = GetDocument();
	pDoc->SetModifiedFlag();
}
int EditorView::GetLineCount()
{
	int count = (int)SendToScentilla(SCI_GETLINECOUNT);
	return count;
}
CString EditorView::GetLine(int linenum)
{
	if(linenum < 0 && linenum > GetLineCount())
		return "";
	char ch[1000];
	SendToScentilla(SCI_GETLINE,linenum,(LPARAM)ch);
	int linelength  = GetLineLength(linenum);
	ch[linelength] = 0;
	CString str = ch;
	str.Remove('\n');
	str.Remove('\r');
	return str;
}
void EditorView::DeleteAllText(bool fRemoveBookmarks)
{
	auto bAutoUpdate = AutoUpdate;
	AutoUpdate = false;
	m_CurrentLine = -1;
	SendToScentilla(SCI_CLEARALL);

	if(fRemoveBookmarks)
		Bookmarks.RemoveAll();
	maxTextLength = 0;
	AutoUpdate = bAutoUpdate;
}
void EditorView::SaveBookmarks()
{
	int findmask = (1 << MARKER_BOOKMARK);
	int line = 0;
	Bookmarks.RemoveAll();
	while (-1 != line)
	{
		line = (int)SendToScentilla(SCI_MARKERNEXT,line,findmask);
		if(line == -1)
			break;
		Bookmarks.Add(line);
		line++;
	}
}
void EditorView::RestoreBookmarks()
{
	SendToScentilla(SCI_MARKERDELETEALL,MARKER_BOOKMARK);
	for(int i = 0; i < Bookmarks.GetSize();i++)
	{
		SendToScentilla(SCI_MARKERADD,Bookmarks[i],MARKER_BOOKMARK);
	}
}
LRESULT EditorView::OnKeyPressed(WPARAM wparam, LPARAM lparam)
{
	int i;
	i = 0;
	return 0;
}
void EditorView::EnableColorise(bool flag)
{
	fEnableColorise = flag;
}
bool EditorView::IsWordDef(const CString& str)
{
	return false;
}
void EditorView::Undo()
{
	if(!CanUndo())
		return;
	SendToScentilla(SCI_UNDO);
}
void EditorView::Redo()
{
	if(!CanRedo())
		return;
	SendToScentilla(SCI_REDO);
}
bool EditorView::CanUndo()
{
	if(!SendToScentilla(SCI_CANUNDO))
		return false;
	return true;
}
bool EditorView::CanRedo()
{
	if(!SendToScentilla(SCI_CANREDO))
		return false;
	return true;
}
void EditorView::EnableUndo(bool flag)
{
	if(flag)
		SendToScentilla(SCI_SETUNDOCOLLECTION,1);
	else
	{
		SendToScentilla(SCI_EMPTYUNDOBUFFER);
		SendToScentilla(SCI_SETUNDOCOLLECTION,0);
	}
}
void EditorView::Paste()
{
	SendToScentilla(SCI_PASTE);
}
bool EditorView::CanPaste()
{
	if(SendToScentilla(SCI_CANPASTE) == 0)
		return false;
	return true;
}
void EditorView::Cut()
{
	SendToScentilla(SCI_CUT);
}
void EditorView::Copy()
{
	SendToScentilla(SCI_COPY);
}
bool EditorView::CanCutCopy()
{ 
	if(SendToScentilla(SCI_GETSELECTIONEND) - SendToScentilla(SCI_GETSELECTIONSTART))
		return true;
	return false;
}
void EditorView::SelectAll()
{
	SendToScentilla(SCI_SELECTALL);
}
void EditorView::Renum()
{
}

CString EditorView::GetSelText()
{
	int pos= (int)SendToScentilla(SCI_GETCURRENTPOS);
	int anc = (int)SendToScentilla(SCI_GETANCHOR);
	if(pos == anc)
		return "";
	char *ch =new char [abs(pos - anc) + 10];
	SendToScentilla(SCI_GETSELTEXT,0,(LPARAM)ch);
	CString ret = ch;
	delete []ch;
	return ret;
}
int EditorView::FindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr, int startPos /*= -1*/)
{
	// returns position of the text founded or -1
	// selects founded text
	// sets target to founded text (if exists)
	int flag = 0;
	if (bCase)
	{
		flag = SCFIND_MATCHCASE;
	}
	if (bWord)
	{
		flag |= SCFIND_WHOLEWORD;
	}
	if (bExpr)
	{
		flag |= SCFIND_REGEXP | SCFIND_CXX11REGEX;
	}
	int pos = 0;
	const int spos = startPos == -1 ? GetCurPos() : startPos;
	SendToScentilla(SCI_TARGETWHOLEDOCUMENT);
	SendToScentilla(SCI_SETTARGETSTART, spos);
	SendToScentilla(SCI_SETSEARCHFLAGS, flag);

	pos = (int)SendToScentilla(SCI_SEARCHINTARGET, (int)strlen(lpszFind), (LPARAM)((char*)lpszFind));
	int s = GetCurPos();
	if (pos < 0 && spos > 0)
	{ // find from the start
		SendToScentilla(SCI_TARGETWHOLEDOCUMENT);
		pos = (int)SendToScentilla(SCI_SEARCHINTARGET, (int)strlen(lpszFind), (LPARAM)((char*)lpszFind));
	}
	if (pos >= 0)
	{
		const int line = GetLineFromPos(pos);
		GoToLine(line);
		const auto end = SendToScentilla(SCI_GETTARGETEND);
		SendToScentilla(SCI_SETSEL, pos, end);
	}
	int s1 = GetCurPos();
	return pos;
}
int EditorView::GetDocLength()
{
	return (int)SendToScentilla(SCI_GETTEXTLENGTH);
}
void EditorView::PrepareSerch()
{
	SendToScentilla(SCI_SEARCHANCHOR);
}
int EditorView::GetCurPos()
{
	return (int)SendToScentilla(SCI_GETCURRENTPOS);
}
void EditorView::SetSelection(int startPos,int length)
{
	SendToScentilla(SCI_SETSEL,startPos,startPos+length);
}
void EditorView::MarkAll(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr)
{
	SetPos(0);
	int lasts = 0;
	GoToLine(0);
	int pos = FindText(lpszFind, bCase, bWord, bExpr);
	while (pos >= 0)
	{
		if(pos <= lasts)
			break;
		const int fm = (int)SendToScentilla(SCI_MARKERGET, GetCurLine());
		if (!(fm & (1 << MARKER_BOOKMARK)))
		{
			OnAddBookmark();
		}
		lasts = pos;
		pos = FindText(lpszFind,bCase,bWord,bExpr);
	}
	SaveBookmarks(); 
}
void EditorView::SetPos(int pos)
{
	SendToScentilla(SCI_SETCURRENTPOS,pos);
	SendToScentilla(SCI_GOTOPOS);
}
int EditorView::ReplaceText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord, BOOL bExpr, LPCTSTR lpszReplace)
{
	const auto SelStart = SendToScentilla(SCI_GETSELECTIONSTART);
	const auto SelEnd = SendToScentilla(SCI_GETSELECTIONEND);
	SendToScentilla(SCI_SETTARGETRANGE, SelStart, SelEnd);
	int flag = 0;
	if (bCase)
	{
		flag = SCFIND_MATCHCASE;
	}
	if (bWord)
	{
		flag |= SCFIND_WHOLEWORD;
	}
	if (bExpr)
	{
		flag |= SCFIND_REGEXP | SCFIND_CXX11REGEX;
	}
	SendToScentilla(SCI_SETSEARCHFLAGS, flag);
	auto pos = (int)SendToScentilla(SCI_SEARCHINTARGET, (int)strlen(lpszFind), (LPARAM)((char*)lpszFind));
	if (pos < 0 || SelStart != SendToScentilla(SCI_GETSELECTIONSTART) || SelEnd != SendToScentilla(SCI_GETSELECTIONEND))
		pos = FindText(lpszFind, bCase, bWord, bExpr);
	LRESULT res = 0;
	if (pos >= 0)
	{
		auto SciMes = bExpr ? SCI_REPLACETARGETRE : SCI_REPLACETARGET;
		res = SendToScentilla(SciMes, -1, (LPARAM)((char*)lpszReplace));
	}
	FindText(lpszFind, bCase, bWord, bExpr);
	return int(res);
}
void EditorView::ReplaceAll(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr, LPCTSTR lpszReplace)
{
	SendToScentilla(SCI_BEGINUNDOACTION);
	SetPos(0);
	int lasts = 0;
	GoToLine(0);
	int pos = FindText(lpszFind, bCase, bWord, bExpr);
	while (pos >= 0)
	{
		if(pos < lasts)
			break;
		lasts = pos + ReplaceText(lpszFind, bCase, bWord, bExpr, lpszReplace);
		pos = FindText(lpszFind, bCase, bWord, bExpr, lasts);
	}
	SendToScentilla(SCI_ENDUNDOACTION);
}
void EditorView::SetBindMarker(int lineNum)
{
//	if(old >= 0)
	//EnableColorise(false);
	SendToScentilla(SCI_MARKERDELETEALL,MARKER_BIND);
	SendToScentilla(SCI_MARKERADD,lineNum,MARKER_BIND);
	//GoToLine(lineNum);
	//EnableColorise(true);
	SendToScentilla(SCI_SETMARGINWIDTHN,1,0);
	SendToScentilla(SCI_SETMARGINWIDTHN,1,16);
	//OnChangeLine(lineNum);
}
void EditorView::GoToLine(int line)
{
	SendToScentilla(SCI_GOTOLINE,line);
}
void EditorView::StartStyling(int startPos)
{
	SendToScentilla(SCI_STARTSTYLING,startPos,USE_STYLE);
}
void EditorView::SetStyle(int length,UINT SyleID)
{
	SendToScentilla(SCI_SETSTYLING,length,SyleID);
}
int EditorView::GetStartLinePos(int line)
{
	return (int)SendToScentilla(SCI_POSITIONFROMLINE,line);
}
void EditorView::SetMarkerCurrLine(int line)
{
	SendToScentilla(SCI_MARKERDELETEALL, MARKER_ARROW);
	SendToScentilla(SCI_MARKERADD, line, MARKER_ARROW);
}
void EditorView::SetMarkerError(int line)
{
	SendToScentilla(SCI_MARKERADD, line, MARKER_ERROR);
}
void EditorView::ClearErrorMarkers()
{
	SendToScentilla(SCI_MARKERDELETEALL, MARKER_ERROR);
}
void EditorView::SetMarkerErrorMin(int line)
{
	SendToScentilla(SCI_MARKERADD, line, MARKER_ERROR_MIN);
}
void EditorView::SetMarkerCollision(int line)
{
	SendToScentilla(SCI_MARKERADD, line, MARKER_COLLISION);
}
void EditorView::ClearErrorMarkersMin()
{
	SendToScentilla(SCI_MARKERDELETEALL, MARKER_ERROR_MIN);
}
void EditorView::ClearCollisionMarkers()
{
	SendToScentilla(SCI_MARKERDELETEALL, MARKER_COLLISION);
}
void EditorView::Renumeration(const CString& N, int nmb, int max, int step, int width, const CString& SP)
{
	SendToScentilla(SCI_BEGINUNDOACTION);
	DeleteNumbers(N);
	if(step <= 0)
		return;
	int count = GetLineCount();
	CString line,str;
	int number = nmb;
	int spos;
	char ch[500];
	memset(ch,'0',500);
	int an = 0;
	for(int i = 0; i < count; i++)
	{
		if(number > max)
			number = nmb;
		line = GetLine(i);
		if (GetLexem(line) == SP)
		{
			continue;
		}
		str.Format("%d",number);
		an = width - str.GetLength();
		if(an > 0)
		{
			ch[an] = 0;
			str = ch + str;
		}
		spos = GetStartLinePos(i);
		SetSelection(spos,line.GetLength());
		line = N + str + line;
		SendToScentilla(SCI_REPLACESEL,0,(LPARAM)((LPCTSTR)line));
		number += step;
	}
	SendToScentilla(SCI_ENDUNDOACTION);
}
void EditorView::DeleteNumbers(const CString& N)
{
	const int count = GetLineCount();
	for(int i = 0; i < count; i++)
	{
		int DelSize = 0;
		CString line = GetLine(i);
		if (line == "")
			continue;
		CString str = GetLexem(line);
		if(IsDigit(str))
			DelSize = str.GetLength();
		else
		{
			if (str != N)
				continue;
			DelSize = str.GetLength();
			line.Delete(0,DelSize);
			str = GetLexem(line);
			if(IsDigit(str))
				DelSize += str.GetLength();
		}
		int spos = GetStartLinePos(i);
		SetSelection(spos, DelSize);
		SendToScentilla(SCI_REPLACESEL, 0, (LPARAM)"");
	}
}
void EditorView::OnModeColorise()
{
	OnModePainter();
	CurrentFeed = 0;
}
void EditorView::ReColor()
{
	SendToScentilla(SCI_CLEARDOCUMENTSTYLE);
	Colorise(0, GetDocLength());
}
void EditorView::SetPXScreenWidth()
{
	SendToScentilla(SCI_SETSCROLLWIDTH,maxPXTextWidth);
}
int EditorView::GetLineFromPos(int pos)
{
	return (int)SendToScentilla(SCI_LINEFROMPOSITION,pos);
}
void EditorView::OnEditFind()
{
	delete pFindDlg;
	pFindDlg = new NTextFindDlg;
	pFindDlg->SetNTextView(this);
	pFindDlg->Create(NTextFindDlg::IDD, this);
	pFindDlg->ShowWindow(SW_SHOW);
}

void EditorView::HighlightWord(CString& Word2H)
{
	// save target locations for other search functions
	auto originalStartPos = SendToScentilla(SCI_GETTARGETSTART);
	auto originalEndPos = SendToScentilla(SCI_GETTARGETEND);

	// Get the range of text visible and highlight everything in it
	auto firstLine = static_cast<int>(SendToScentilla(SCI_GETFIRSTVISIBLELINE));
	auto nbLineOnScreen = SendToScentilla(SCI_LINESONSCREEN);
	auto nbLines = nbLineOnScreen + 1;
	auto lastLine = firstLine + nbLines;
	auto currentLine = firstLine;
	int prevDocLineChecked = -1;	//invalid start

	// Determine mode for SmartHighlighting
	bool isWordOnly = true;
	bool isCaseSensentive = true;
	SendToScentilla(SCI_SETINDICATORCURRENT, 3);
	auto firstPos = SendToScentilla(SCI_POSITIONFROMLINE, firstLine);
	auto lastPos = SendToScentilla(SCI_POSITIONFROMLINE, lastLine + 1);
	if (lastPos == -1)
		lastPos = int(SendToScentilla(SCI_GETLENGTH)) - 1;
	SendToScentilla(SCI_INDICATORCLEARRANGE, firstPos,  lastPos - firstPos);

	if (Word2H.IsEmpty())
		return;
	bool Stop = false;
	for (; currentLine < lastLine; ++currentLine)
	{
		int docLine = static_cast<int>(SendToScentilla(SCI_DOCLINEFROMVISIBLE, currentLine));
		if (docLine == prevDocLineChecked)
			continue;	//still on same line (wordwrap)
		prevDocLineChecked = docLine;
		int startPos = static_cast<int>(SendToScentilla(SCI_POSITIONFROMLINE, docLine));
		int endPos = static_cast<int>(SendToScentilla(SCI_POSITIONFROMLINE, docLine + 1));
		if (endPos == -1)
		{	//past EOF
			endPos = int(SendToScentilla(SCI_GETLENGTH)) - 1;
			Stop = true;
		}
		int targetStart = 0;
		int targetEnd = 0;
		while (targetStart != -1 && targetStart != -2)
		{
			SendToScentilla(SCI_SETTARGETRANGE, startPos, endPos);
			targetStart = int(SendToScentilla(SCI_SEARCHINTARGET, Word2H.GetLength(), reinterpret_cast<LPARAM>(Word2H.GetBuffer())));
			// If we've not found anything, just break out of the loop
			if (targetStart == -1 || targetStart == -2)
				break;

			targetEnd = int(SendToScentilla(SCI_GETTARGETEND));

			if (targetEnd > endPos) {	//we found a result but outside our range, therefore do not process it
				break;
			}

			int foundTextLen = targetEnd - targetStart;
			int replaceDelta = 0;
			if (foundTextLen > 0)
			{
				SendToScentilla(SCI_INDICATORFILLRANGE, targetStart, foundTextLen);
			}

			// After the processing of the last string occurence the search loop should be stopped
			// This helps to avoid the endless replacement during the EOL ("$") searching
			if (targetStart + foundTextLen == endPos)
				break;

			startPos = targetStart + foundTextLen + replaceDelta;		//search from result onwards
			endPos += replaceDelta;									//adjust end of range in case of replace
		}

	}

	// restore the original targets to avoid conflicts with the search/replace functions
	SendToScentilla(SCI_SETTARGETRANGE, originalStartPos, originalEndPos);
}

void EditorView::NFindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr)
{
	if (FindText(lpszFind, bCase, bWord, bExpr) < 0)
		AfxMessageBox(IDS_EDITOR_TEXTNOTFIND);
}

void EditorView::NMarkAll(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr)
{
	MarkAll(lpszFind,bCase,bWord,bExpr);
}

void EditorView::NReplaceText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr, LPCTSTR lpszReplace)
{
	ReplaceText(lpszFind,bCase,bWord,bExpr,lpszReplace);
}

void EditorView::NReplaceAll(LPCTSTR lpszFind, BOOL bCase, BOOL bWord,BOOL bExpr, LPCTSTR lpszReplace)
{
	ReplaceAll(lpszFind,bCase,bWord,bExpr,lpszReplace);
}

void EditorView::OnFindText()
{
	OnEditFind();
}

void EditorView::SetMarginWidth(int MaxLine)
{
	CString str;
	str.Format(_T("%d"), MaxLine);
	int width = str.GetLength() + 1;
	if(width < 3)
		width = 3;
	char *ch = new char[width + 1];
	memset(ch, '_', width);
	ch[width] = 0;
	LNWidth = (int)SendToScentilla(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)ch);
	delete[] ch;
	if(!Config.fEnableNumString)
		SendToScentilla(SCI_SETMARGINWIDTHN, 0, 0);
	else
		SendToScentilla(SCI_SETMARGINWIDTHN, 0, LNWidth);
}

void EditorView::UpdateStat(const CString& prgName, int CurCh, int WholeChannels)
{
	CString str;
	str.Format(" (%d) channel: %d / %d",GetLineCount(), CurCh, WholeChannels);
	CString Title;
	Title.LoadString(ID_TEXTPANE);
	if (prgName.IsEmpty())
		SetWindowText(Title);
	else
	{
		int index = prgName.ReverseFind('\\');
		SetWindowText(prgName.Right(prgName.GetLength() - index - 1) + str);
	}
}

bool EditorView::SaveCopy(const CString& FilePath) const
{
	CStdioFile f;
	if (!f.Open(FilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		return false;
	CString WholeText = GetText();
	f.Write(WholeText, WholeText.GetLength());
	f.Close();
	return true;
}

void EditorView::SetText(const CString &str)
{
	SendToScentilla(SCI_SETTEXT, 0, (LPARAM)((LPCTSTR)str));
}

void EditorView::AppendText(const CString &str)
{
	SendToScentilla(SCI_APPENDTEXT, str.GetLength(), (LPARAM)((LPCTSTR)str));
}

void EditorView::OnLineNum()
{
	if(Config.fEnableNumString)
		Config.fEnableNumString = false;
	else
		Config.fEnableNumString = true;
	SetMarginWidth(GetLineCount());
		
}

void EditorView::OnUpdateLineNum(CCmdUI* pCmdUI)
{
	if(Config.fEnableNumString)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

ptrdiff_t EditorView::GetLineWidth(ptrdiff_t line)
{
	return (Sci_Position)SendToScentilla(SCI_POINTXFROMPOSITION, 0, line);
}

int EditorView::GetLineEndPosition(int line)
{
	return (int)SendToScentilla(SCI_GETLINEENDPOSITION, line);
}

CDocument* EditorView::GetDocument() const
{
	CFrameWnd *pMainFrame = (CFrameWnd *)AfxGetMainWnd();
	if(!pMainFrame)
		return NULL;
	return pMainFrame->GetActiveDocument();
}

void EditorView::SetSavePoint(void)
{
	SendToScentilla(SCI_SETSAVEPOINT);
}

bool EditorView::GetModify(void)
{
	return (SendToScentilla(SCI_GETMODIFY) > 0);
}
void EditorView::RemoveFindDlg(void)
{
	pFindDlg = NULL;
}

void EditorView::CommentSelection(bool Uncomment)
{
	int SelStart = int(SendToScentilla(SCI_GETSELECTIONSTART));
	int SelEnd = int(SendToScentilla(SCI_GETSELECTIONEND));
	if (SelStart >= SelEnd)
		return;
	int SelStartLine = GetLineFromPos(SelStart);
	int SelEndLine = GetLineFromPos(SelEnd);

	int PosInStartLine = SelStart - int(SendToScentilla(SCI_POSITIONFROMLINE, SelStartLine));
	int PosInEndLine = SelEnd - int(SendToScentilla(SCI_POSITIONFROMLINE, SelEndLine));
	int EndLineLength = GetLineLength(SelEndLine);

	SendToScentilla(SCI_BEGINUNDOACTION);
	if (PosInStartLine == 0 && (PosInEndLine == 0 || PosInEndLine == EndLineLength - 1))
	{// Comment out or uncomment full lines
		CString CommentFullStr(Config.StringComment);
		if (CommentFullStr.IsEmpty())
			return;
		CommentFullStr += '\0'; 
		if (PosInEndLine == EndLineLength - 1)
			++SelEndLine;
		for (int Line = SelStartLine; Line < SelEndLine; ++Line)
		{
			int LineStartPosition = int(SendToScentilla(SCI_POSITIONFROMLINE, Line));
			if (Uncomment)
			{
				char FirstChar = char(SendToScentilla(SCI_GETCHARAT, LineStartPosition));
				if (CommentFullStr[0] == FirstChar)
				{
					SendToScentilla(SCI_SETTARGETSTART, LineStartPosition);
					SendToScentilla(SCI_SETTARGETEND, LineStartPosition + 1);
					SendToScentilla(SCI_REPLACETARGET, 0, LPARAM((LPCTSTR)CommentFullStr));
				}
			}
			else
			{
				SendToScentilla(SCI_INSERTTEXT, LineStartPosition, LPARAM((LPCTSTR)CommentFullStr));
			}
		}
	}
	else 
	{ // Comment a part of a line
		CString CommentPartStrBeg(Config.BlockCommentStart);
		CString CommentPartStrEnd(Config.BlockCommentEnd);
		if (SelStartLine != SelEndLine)
		{
			CommentPartStrBeg = Config.ModalCommentStart;
			CommentPartStrEnd = Config.ModalCommentEnd;
		}
		if (CommentPartStrBeg.IsEmpty() || CommentPartStrEnd.IsEmpty())
			return;
		CommentPartStrBeg += '\0';
		CommentPartStrEnd += '\0';
		if (Uncomment)
		{
			char FirstChar = char(SendToScentilla(SCI_GETCHARAT, SelStart));
			char LastChar = char(SendToScentilla(SCI_GETCHARAT, SelEnd - 1));
			if (CommentPartStrBeg[0] == FirstChar && CommentPartStrEnd[0] == LastChar)
			{
				SendToScentilla(SCI_SETTARGETSTART, SelEnd - 1);
				SendToScentilla(SCI_SETTARGETEND, SelEnd);
				SendToScentilla(SCI_REPLACETARGET, 0, LPARAM((LPCTSTR)CommentPartStrBeg));
				SendToScentilla(SCI_SETTARGETSTART, SelStart);
				SendToScentilla(SCI_SETTARGETEND, SelStart + 1);
				SendToScentilla(SCI_REPLACETARGET, 0, LPARAM((LPCTSTR)CommentPartStrBeg));
			}
		}
		else
		{
			SendToScentilla(SCI_INSERTTEXT, SelEnd, LPARAM((LPCTSTR)CommentPartStrEnd));
			SendToScentilla(SCI_INSERTTEXT, SelStart, LPARAM((LPCTSTR)CommentPartStrBeg));
		}
	}
	SendToScentilla(SCI_ENDUNDOACTION);
}

CString EditorView::GetErrorMinText(int curline)
{
	return CString();
}

CString EditorView::GetCollisionText(int curline) const
{
	return CString();
}

void EditorView::ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate/* = TRUE*/)
{
	if(pFindDlg && !bShow)
	{
		pFindDlg->DestroyWindow();
		delete pFindDlg;
		RemoveFindDlg();
	}
	CDockablePane::ShowPane(bShow, bDelay, bActivate);
}

bool EditorView::IsDigit(char Sym)
{
	return 
		Sym == '0' || Sym == '1' || Sym == '2' || Sym == '3' || Sym == '4' || 
		Sym == '5' || Sym == '6' || Sym == '7' || Sym == '8' || Sym == '9' ;
}

void EditorView::AutoUpdateReset()
{
	KillTimer(AutoUpdateTimerID);
	AutoUpdateTimerID = SetTimer(T_AUTO_UPDATE, AutoUpdateTimeout, &AutoUpdateTimerProc);
}

void EditorView::AutoUpdateTimerProc(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	CWnd* pWnd = CWnd::FromHandle(hWnd);
	pWnd->KillTimer(nIDEvent);
	EditorView* pThisView = dynamic_cast<EditorView*>(pWnd);
	if (pThisView == nullptr)// error
		return;
	pThisView->AutoUpdateTimerID = 0;
	pThisView->AutoUpdateAction();
}

