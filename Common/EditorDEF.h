#ifndef EDITORDEF_H
#define EDITORDEF_H
#include "AfxTempl.h"
const int MAX_STYLES=127;
const int FONT_NAME_SIZE = 31;

typedef struct _style_color
{
	_style_color()
	{
		red = 0;
		green = 0;
		blue = 0;
		res=0;
	}
	DWORD red	: 8;
	DWORD green	: 8;
	DWORD blue	: 8;
	DWORD res   : 8;//резерв

} style_color;
typedef struct _editor_style
{
	style_color color;
	UINT ID;
	CString short_name;
	CString description;
} editor_style;
typedef struct _editor_bkstyle
{
	style_color bkcolor;
	style_color fcolor;
	UINT ID;
	CString short_name;
} editor_bkstyle;
const int NUM_CURVETYPE = 4;
const int NUM_TOOL = 40;
const int NUM_COOL = 3;
const int NUM_CYCLE = 3;
const int NUM_COMP = 3;
const int NUM_LR = 3;
const int NUM_FEED = 4;
typedef struct _editor_cfg
{
	enum {ID_STYLE_TEXT=0,ID_STYLE_GCODE,ID_STYLE_MCODE,ID_STYLE_REGISTER,ID_STYLE_OTHER,
		ID_STYLE_DIGIT,ID_STYLE_CMDNUMBER,ID_STYLE_COMENT,ID_SPECSTYLE_BLOCKCOMENTSTART,
	ID_SPECSTYLE_BLOCKCOMENTEND,ID_STYLE_CT_FAST,ID_STYLE_CT_LINE,ID_STYLE_CT_CWARC,/*13*/
	ID_STYLE_CT_CCWARC,ID_STYLE_COOL_OFF,ID_STYLE_COOL_MIST,ID_STYLE_COOL_FLOOD,//17
	ID_STYLE_CYCLE_DRILL,ID_STYLE_CYCLE_TAPPING,ID_STYLE_CYCLE_REAM,//20
	ID_STYLE_COMP_UP,ID_STYLE_COMP_DOWN,ID_STYLE_COMP_OFF,//23
	ID_STYLE_LRCOMP_OFF,ID_STYLE_LRCOMP_LEFT,ID_STYLE_LRCOMP_RIGHT,//26
	ID_STYLE_FEED_MAX,ID_STYLE_FEED_RAPID,ID_STYLE_FEED_FIRST,ID_STYLE_FEED_SECOND,//30
	ID_STYLE_LINENUMBER,/*32 - 39 стили сцентиллы*/ID_STYLE_TOOL = 40,/*40 - 80*/
	ID_STYLE_FEEDCODE = 81,ID_STYLE_SPEEDCODE,ID_STYLE_TOOLCODE,//83
	ID_STYLE_PROGRAMSE,ID_STYLE_PROGRAMBE,ID_STYLE_LINEAR,ID_STYLE_LINEAR2,//87
	ID_STYLE_ANGLE,ID_STYLE_ARC,ID_STYLE_OFFSET//90
	};
	enum {ID_BKSTATUS_DEFAULT=0};
	_editor_cfg()
	{
		fCmdNumber = false;
		fEnableNumString = true;
	}
	style_color defBkColor;
	editor_style StyleGCode;
	editor_style StyleMCode;
	editor_style StyleReg;
	editor_style StyleComment;
	editor_style StyleText;
	editor_style StyleOther;
	editor_style StyleDigit;
	editor_style StyleCmdNumber;
	editor_style StyleLineNumber;
	editor_style StyleFeedCode;
	editor_style StyleSpeedCode;
	editor_style StyleToolCode;
	editor_style StyleProgramSE;
	editor_style StyleProgramBE;
	editor_style StyleLinear;
	editor_style StyleAngle;
	editor_style StyleArc;
	editor_style StyleOffset;

	//////////////////////////////////////////////////////////////////////////
	
	editor_bkstyle StyleCurveType[NUM_CURVETYPE];
	editor_bkstyle StyleTool[NUM_TOOL];
	editor_bkstyle StyleCool[NUM_COOL];
	editor_bkstyle StyleFeed[NUM_FEED];
	editor_bkstyle StyleCycle[NUM_CYCLE];
	editor_bkstyle StyleComp[NUM_COMP];
	editor_bkstyle StyleLRComp[NUM_LR];
	LOGFONT font;
	style_color wbkcolor;
	style_color numbkcolor;
	style_color numfrcolor;
	style_color arr1color;
	style_color arr2color;
	CString General;
	CString Misc;
	CString Register;
	CString StringComment;
	CString BlockDebug;
	CString BlockCommentStart;
	CString BlockCommentEnd;
	CString ModalCommentStart;
	CString ModalCommentEnd;
	//
	CString LineNumber;
	CString Feed;
	CString Speed;
	CString Tool; 
	CString ProgramSE;
	CString ProgramBE;
	CString HorizontalAxis;
	CString VerticalAxis;
	CString SpindleAxis;
	CString HorizontalRotationAxis;
	CString VerticalRotationAxis;
	CString SpindleRotationAxis;
	CString CircleHorzCenter;
	CString CircleVerticalCenter;
	CString CircleDepthCenter;
	CString CircleRadius;
	CString DiaCompOffset;
	CString LengthCompOffset;
	bool fCmdNumber;//Номера команд выделяются цветом самих команд
	bool fEnableNumString;//Показываются номера строк
	//CString Misc
} editor_cfg;


#endif