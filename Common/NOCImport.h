#pragma once
#include "OCImport_API.h"

enum StockFileType
{
	SFT_STL,
	SFT_CONT,
	SFT_MFR,
	SFT_STEP,
	SFT_IGES,
	SFT_UNKNOWN
};

class OCIMPORT_API NOCImport
{
public:
	NOCImport();
	static bool ConvExt2STL(StockFileType StockType, const char* SourceFile, const char* DestFile, bool ASCII, double Tol, double Angle);
protected:
	static bool ConvExt2STLb(class XSControl_Reader& Reader, const char* SourceFile, const char* DestFile, bool ASCII, double Tol, double Angle);
};

