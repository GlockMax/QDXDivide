#pragma once
#include "NPassiveObject.h"
#include "NObject.h"
// NMovableObj command target

class NMovableObj : public NPassiveObject
{
	friend class NPositionDlg;
public:
	NMovableObj();
	virtual ~NMovableObj();
protected:
	bool Moving;
	RelPos pos_X;
	RelPos pos_Y;
	RelPos pos_Z;
	double bas_X;
	double bas_Y;
	double bas_Z;
	BMatr Matr;
public:
	void SetMoving(bool val){ Moving = val;}
	bool IsMoving(void){ return Moving;}
	virtual void Transformm(const class BMatr& M){Matr = Matr * M; Changed();}
	void Serialize(CArchive& ar);
	void ResetMatr(void) { Matr.SetE();}
	virtual const BMatr& GetMatr() const { return Matr;}
	BMatr& GetMatrRef() { return Matr; }
};


