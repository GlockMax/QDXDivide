#pragma once

#ifdef NCMB_EXPORTS
#define NCMB_API __declspec(dllexport)
#else
#define NCMB_API __declspec(dllimport)
#endif

//#include "stdafx.h"

class NCMB_API BCadrGeom
{
public:
	BCadrGeom(void);
	~BCadrGeom(void);
	virtual BCadrGeom * GetCopy(void) const = 0;
};
