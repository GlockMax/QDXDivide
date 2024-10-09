// BStlB.h: interface for the BStlB class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "NCMB_API.h"
#include "GLee.h"
#include <vector>
#include "GL\GL.h"
#include "GL\GLu.h"
#include "BPoint.h"	
#include "BBox.h"


class NCMB_API BStlB
{
public:
	const CString & GetFileName()const;
	void SetFileName(const CString & Name);
	bool CreateEx(const CString &name);
	virtual void Serialize(CArchive &ar, double InFileVersion);
	const BPoint &GetOrigZero(void) const { return OrigZero; };
	BStlB();
	virtual ~BStlB();
	void MakeCopy(BStlB &object) const;
	virtual void Clear();
	bool CreateFrMem(int TriNum, const GLfloat* data);

protected:
	BPoint Base;
	BPoint OrigZero;
	CString FileName;
	GLfloat *Triangles;
	bool Create(const CString &ProjFileName = CString());
	BBox Gabar;
	CTime FileTime;
//	DECLARE_SERIAL(BStlB)
public:
	const GLfloat * GetTriangles(void) const;
	GLfloat *MakeOneSection(const BPoint &P, const BPoint &V, bool Trimed = false, const BPoint &AddV = BPoint(0., 0., 1., 0.)) const;
	virtual void Transform(const BMatr &M);
	void WriteSTL(CStdioFile & f) const;
	double GetVolume(void);
protected:
	bool FindInt(const BPoint &p0, const BPoint &p1, double d0, double d1, CArray<BPoint, const BPoint &> &Lines) const;
public:
	const BPoint RayCasting(const BPoint& P, const BPoint& V, double MinW, BPoint& N) const;
	size_t RayShreddingTri(const BPoint& P, const BPoint& V, double MinW, std::vector<BPoint>& Pts) const;
	const BBox& GetGabar() const;
	const BPoint &GetBase(void) const {return Base;}
	void MoveBase(double x, double y, double z);
	void RecalcNormals(void);
	const CTime &GetFileTime(void) { return FileTime;}
	double FindMaxZDist(void);

};
