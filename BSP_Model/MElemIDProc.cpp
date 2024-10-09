#include "stdafx.h"
#include "math.h"
#include "NColor.h"
#include "melemidproc.h"

PackedColor MElemIDProc::PCPal[PC_PAL_SIZE] = {GLfloat(0.)};// Palette for used colors
bool MElemIDProc::AllowPCPalUse = false;

MElemIDProc::MElemIDProc(void)
{
	HighlightedSurfID = -4;
}

MElemIDProc::~MElemIDProc(void)
{
}

bool MElemIDProc::GetStockEdgesS(void)
{
	return false;
}

bool MElemIDProc::GetStockEdgesDX(void)
{
	return false;
}
PackedColor MElemIDProc::GetColor(ElemID id)
{
	int PCNum = GetPCn(id);
	PackedColor col;
	if(PCNum < RESERVED_PCS)
	{
		col.fr = 0;
		col.fg = 255;
		col.fb = 0;
		col.fa = 255;
	}
	else 
	{
		col = GetPC(PCNum - RESERVED_PCS);
		if(col.ival == 0)
		{
			col.fr = 0;
			col.fg = 255;
			col.fb = 0;
			col.fa = 255;
		}
	}


	if(GetSurfID(id) == GetHighlightedSurfID())
	{
		col.fr = 255 - col.fr;
		col.fg = 255 - col.fg;
		col.fb = 255 - col.fb;
	}
	return col;
}
int MElemIDProc::GetSurfID(ElemID id)
{
	return (id & ID_MASK_NORMAL) ? (-int(id & ID_MASK_SURNUM) - 1 + SURF_NUM_SHIFT) : (int(id & ID_MASK_SURNUM) - SURF_NUM_SHIFT);
}
ElemID MElemIDProc::SetSurfID(ElemID *id, int SurfID)
{
	if(SurfID < 0)
	{
		SurfID = - SurfID - 1;
		*id |= ID_MASK_NORMAL;
	}
	*id = (*id & (ID_MASK_PCNUM | ID_MASK_NORMAL)) | ((SurfID + SURF_NUM_SHIFT) & ID_MASK_SURNUM);
	return *id;
}
ElemID MElemIDProc::SetPCn(ElemID *id, int PCNum)
{
	*id = ((PCNum << ID_SHIFT_PCNUM) & ID_MASK_PCNUM) | (*id & (ID_MASK_SURNUM | ID_MASK_NORMAL));
	return *id;
}
int MElemIDProc::AddPC(PackedColor Col)
{
	for(int k = 0; k < PC_PAL_SIZE; ++k)
		if((PCPal[k].ival & IGNORE_A_MASK) == (Col.ival & IGNORE_A_MASK))
			return k;
	for(int i = 0; i < PC_PAL_SIZE; ++i)
		if(PCPal[i].ival == 0)
		{
			PCPal[i] = Col;
			return i;
		}
	return 0;
}
int MElemIDProc::ReplacePC(PackedColor Old, PackedColor New)
{
	int k = GetInd(Old);
	if (k == -1 || k >= PC_PAL_SIZE)
		return -1;
	PCPal[k].ival = New.ival;
	return k;
}

int MElemIDProc::GetInd(PackedColor Color)
{
	int k = 0;
	for (; k < PC_PAL_SIZE; ++k)
		if (PCPal[k].ival == Color.ival)
			break;
	if (k == PC_PAL_SIZE)
		return -1;
	return k;
}

void MElemIDProc::RemovePC(int Num)
{
	if(Num >= 0 && Num < PC_PAL_SIZE)
		PCPal[Num].ival = 0;
}

void MElemIDProc::ClearPCP(const NColor &Col0)
{
	PCPal[0].fr = Col0.r(); 
	PCPal[0].fg = Col0.g();
	PCPal[0].fb = Col0.b();
	PCPal[0].fa = Col0.a();

	for(int i = 1; i < PC_PAL_SIZE; ++i)
		PCPal[i].ival = 0;
}

PackedColor MElemIDProc::GetPC(int Num)
{
	return AllowPCPalUse ? PCPal[Num] : PCPal[0];
}
void MElemIDProc::SetAllowPCPalUse(bool val)
{
	AllowPCPalUse = val;
}
bool MElemIDProc::GetAllowPCPalUse(void)
{
	return AllowPCPalUse;
}
int MElemIDProc::ReplacePC(const NColor &Old, const NColor &New)
{
	return ReplacePC(MakePC(Old), MakePC(New));
}
PackedColor MElemIDProc::MakePC(const NColor &Color)
{
	PackedColor PCol;
	PCol.fa = 255;
	PCol.fr = Color.r();
	PCol.fg = Color.g();
	PCol.fb = Color.b();
	return PCol;
}

void MElemIDProc::GetNormalFromSurfID(int SurfID, float N[3])
{
	int Num = ((-SurfID) & ID_MASK_SURNUM) - 1;
	unsigned int MaskX = (0x01 << ID_NORM_X) - 1;
	N[0] = float((Num & MaskX) * 2. / MaskX - 1.);
	Num = Num >> ID_NORM_X;
	unsigned int MaskY = ((0x01 << ID_NORM_Y) - 1);
	N[1] = float((Num & MaskY) * 2. / MaskY - 1.);
	Num = Num >> ID_NORM_Y;
	float Zsign = float(1 - (0x01 & Num) * 2);
	if (fabs(N[0]) == 1.)
		N[1] = 0.;
	else if (fabs(N[1]) == 1.)
		N[0] = 0.;
	N[2] = float(sqrt(fmax(0., 1. - N[0] * N[0] - N[1] * N[1])));
	if (N[2] == 1.)
	{
		N[0] = 0.;
		N[1] = 0.;
	}
	N[2] *= Zsign;
}

void MElemIDProc::SetNormalToSurfID(int &SurfID, float N[3])
{
	unsigned int MaskX = (0x01 << ID_NORM_X) - 1;
	unsigned int NumX = unsigned int((N[0] + 1.) * 0.5 * MaskX);
	unsigned int MaskY = (0x01 << ID_NORM_Y) - 1;
	unsigned int NumY = unsigned int((N[1] + 1.) * 0.5 * MaskY);
	NumY = NumY << ID_NORM_X;
	unsigned int NumZ = (N[2] > 0) ? 0 : (0x01 << (ID_NORM_Y + ID_NORM_X));
	SurfID = - int(NumY | NumX | NumZ) - 1;
}
