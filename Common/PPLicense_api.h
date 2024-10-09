#pragma once
//#include "vld.h"

#ifdef PPLicense_EXPORTS
#define PPLicense_API __declspec(dllexport)
#else
#define PPLicense_API __declspec(dllimport)
#endif
