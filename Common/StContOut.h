// StContOut.h: interface for the StContOut class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <list>
#include "NCadrGeomArray.h"

class AFX_EXT_CLASS StContOut  
{
public:
	StContOut(const NCadrGeomArray * InArray);
	StContOut();
	~StContOut();
	int GetErrorCode() const { return ErrorCode; }

	int Load(NCadrGeom *& OutArray, CString &filename);
	size_t ReadFile(CString &Text, const CString &filename);
	size_t ReadFromFileStC(std::list<NCadrGeom>& ResList, const CString &filename);
	size_t ReadFromFileF2C(std::list<NCadrGeom>& ResList, const CString &filename);
	int ReadFromFileDXFPoly(NCadrGeom*& OutArray, const CString& filename);
	size_t ReadFromFileDXF(std::list<NCadrGeom>& ResList, const CString& filename);
	size_t ReadFromStrStC(std::list<NCadrGeom>& ResList, size_t Size, CString &Text);
	size_t ReadFromStrF2C(std::list<NCadrGeom>& ResList, int Size, CString &Text);
	size_t ReadFromStrDXF(std::list<NCadrGeom>& ResList, int Size, CString &Text);
	size_t ReadFromFile(NCadrGeom*& OutArray, const CString& filename);
	size_t ReadFromStr(NCadrGeom*& OutArray, size_t Size, CString& text);
protected:
	int ErrorCode;
};
