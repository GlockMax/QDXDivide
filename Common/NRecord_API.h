#pragma once
//#include "vld.h"

#ifdef NRECORD_EXPORTS
#define NRECORD_API __declspec(dllexport)
#else
#define NRECORD_API __declspec(dllimport)
#endif
