#pragma once
#include "BSP_MODEL_API.h"
#include "intsafe.h"
#include "CadrID.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "GL\GLu.h"
#include "ClrPoint.h"

#define ElemID unsigned int
typedef union DDX
{
	DDX() {}
	DDX(ElemID e, cadrID c) { Elem = e; Cadr = c; }
	__int64 Val;
	struct
	{
		ElemID Elem;
		cadrID Cadr;
	};
} DX_ID;

constexpr int STOCK_COLOR_NUM = 0;
constexpr int FAST_COLOR_NUM = 1;
constexpr int RESERVED_PCS = 2; // Whole number of above variables
constexpr ElemID ID_MASK_PCNUM = 0xF0000000;
constexpr ElemID ID_MASK_SURNUM = 0x07FFFFFF;
constexpr int ID_NORM_X = 13;
constexpr int ID_NORM_Y = 13;// ID_NORM_X + ID_NORM_X + 1 must be equal to ID_MASK_SURNUM bits number
constexpr ElemID ID_MASK_NORMAL = 0x08000000;
constexpr unsigned int ID_SHIFT_PCNUM = 28;
constexpr int PC_PAL_SIZE = 16;
constexpr int SURF_NUM_SHIFT = 3;
constexpr ElemID ID_EMPTY = ID_MASK_SURNUM - SURF_NUM_SHIFT;
constexpr int IGNORE_A_MASK = 0x00FFFFFF;

class NColor;

class BSP_MODEL_API MElemIDProc
{
protected:
	int HighlightedSurfID;
	static PackedColor PCPal[PC_PAL_SIZE];// Palette for used colors
	static bool AllowPCPalUse;
public:
	MElemIDProc(void);
	virtual ~MElemIDProc(void);
	virtual PackedColor GetColor(ElemID id);
	virtual bool GetStockEdgesS(void);
	virtual bool GetStockEdgesDX(void);
	static int GetSurfID(ElemID id);
	static void GetNormalFromSurfID(int SurfID, float N[3]);
	static void SetNormalToSurfID(int &SurfID, float N[3]);
	static ElemID SetSurfID(ElemID *id, int SurfID);
	static ElemID SetPCn(ElemID *id, int PCNum);
	static int GetPCn(ElemID id) { return (id & ID_MASK_PCNUM) >> ID_SHIFT_PCNUM;}
	int GetHighlightedSurfID(){ return HighlightedSurfID;}
	void SetHighlightedSurfID(int InID){ HighlightedSurfID = InID;}
	static int AddPC(PackedColor Col);
	static int ReplacePC(PackedColor Old, PackedColor New);
	static int GetInd(PackedColor Color);
	static int ReplacePC(const NColor &Old, const NColor &New);
	static PackedColor MakePC(const NColor &Col);
	static void RemovePC(int Num);
	static void ClearPCP(const NColor &Col0);
	static PackedColor GetPC(int Num);
	static void SetAllowPCPalUse(bool val);
	static bool GetAllowPCPalUse(void);
	static bool IsSurfIndEmpty(int index) { return (index & ID_MASK_SURNUM) == ID_EMPTY; }
public:
	static const int ID_NULL = 0x00000000;
	static const int ID_DEFAULT = 0x20000000;
	static const int ID_ORIENT = 0x00000001;
	static const int ID_DUMMY = 0x00000002;
	static const int ID_REMOVE = INT_MAX - 1;
};
