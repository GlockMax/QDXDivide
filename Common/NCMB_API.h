#pragma once
//#include "vld.h"

#ifdef NCMB_EXPORTS
#define NCMB_API __declspec(dllexport)
#else
#define NCMB_API __declspec(dllimport)
#endif
