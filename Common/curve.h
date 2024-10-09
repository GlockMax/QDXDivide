#pragma once

#define CU_FAST 0x0001
#define CU_COOL 0x0002
#define CU_SPMI 0x0004 // Mill spindle
#define CU_SUBP 0x0008
#define CU_CYCL 0x0010
#define CU_LCOMP_BEG 0x0020 // Length comp is on at the beginning
#define CU_DCOM 0x0040
#define CU_CONF 0x0080 // Mach config changed
#define CU_ADDC 0x0100
#define CU_GROV 0x0200 // This cadr should be processed as a groove cadr
#define CU_SPLN 0x0400 // Spline
#define CU_PRBE 0x0800 // Probe
#define CU_TOUC 0x1000 // Touch
#define CU_RCP0 0x2000 // RCPActive A axis
#define CU_SPTU 0x4000 // Turn spindle
#define CU_RCP1 0x8000 // RCPActive B axis
#define CU_DEF 0x0000
#define CU_RCP2 0x10000 // RCPActive C axis
#define CU_LCOMP_END 0x20000 // Length comp is on at the end

#include "ResLocal.h"
enum curve
{
	undef = IDS_STATE_VALUE_undef,
	fast = IDS_STATE_VALUE_fast,
	line = IDS_STATE_VALUE_line,
	cwarc = IDS_STATE_VALUE_cwarc,
	ccwarc = IDS_STATE_VALUE_ccwarc,
	rotate = IDS_STATE_VALUE_rotate,
	addcoord = IDS_STATE_VALUE_addcoord
};
enum curveMod
{
	NotDefCM,
	HhArcTang,
	HhChamf,
	HhRound
};
