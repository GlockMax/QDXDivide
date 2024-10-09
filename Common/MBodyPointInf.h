#pragma once
#include "BSP_MODEL_API.h"
#include "MelemIDProc.h"
#include "BPoint.h"
#include "CadrID.h"

class BSP_MODEL_API MBodyPointInf
{
public:
	MBodyPointInf();
public:
	ElemID ElemID_;
	BPoint Pt_;
	BPoint Nr_;
	cadrID HistCadrID_;
};

