// AptCLFile.h: interface for the AptCLFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APTCLFILE_H__ECE7C050_5E2C_478D_8F8A_4E1E5756902E__INCLUDED_)
#define AFX_APTCLFILE_H__ECE7C050_5E2C_478D_8F8A_4E1E5756902E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>

class AptCLFile
{
public:
	void Coolant(int type);
	void CoolantOff();
	void CycleOn(int type,double depth, int feed,int fm,double clear,double rapdepth);
	void CycleOff();
	void Remark(const CString  &str);
	void LoadTl(int num);
	void SpindlOn();
	void Spindl(int speed);
	void Spindl(BOOL dir, int speed);
	void SpindlOff();
	void CutComOf();
	void CutComOn(BOOL side, int num);
	void Feed(int value, int fm = 0);
	void Arc(int type,double xb,double yb,double zb,double xe,double ye,double ze,double i,double j,double k,double nx,double ny,double nz);
	void Line(double x, double y, double z);
	void Fast(double x, double y, double z);
	BOOL Close();
	BOOL Create(const CString &FileName);
	AptCLFile();
	virtual ~AptCLFile();

protected:
	CString GetSubstr(int StrID, int n = 0);
	std::ofstream file;
};

#endif // !defined(AFX_APTCLFILE_H__ECE7C050_5E2C_478D_8F8A_4E1E5756902E__INCLUDED_)
