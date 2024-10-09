#pragma once

#include "BPoint.h"
#include "BMatr.h"
#include "MBSPForest.h"
#include "MBSPNode.h"
#include "MPlane.h"
#include "MRay.h"

class QMeasureDlg;
struct SufrInformation 
{
	CString LineID;
	CString Type;
	BPoint BeginPoint;
	BPoint Normal;
	BPoint Axis;
	double Angle;
	double ExtR;
	double IntR;
};

enum QMeasureType
{
	FIND_POINT,	
	AUTO_M,		
	MANUAL_M,
	NONE_M
};

enum PointPosition
{
	POINT_IN_BODY,
	POINT_OUT_BODY,
	POINT_ON_BODY,
	POINT_FULL_OUT_V,
	POINT_FULL_OUT_BV,
	POINT_OUT_NDEF
};

class QMeasure
{
protected:
//parametrs only for internal use 
	QMeasureDlg* MeasureDlg;				//Dialog for in/out information
	QMeasureType Type_Measure;				//Тип вычисления
	QMeasureType Draw_Type;				//Тип рисования
	BPoint bp, ep, outp, vn;
	BMatrPair CMatr;// Correction matrix for mill turn and BodyRot 
	NGlobalState::ModelModes ModelMode;
	class NStock* pStock;
	//function Only for internal use 
	void SetPoint(CPoint point, QMeasureType MesType);
	bool RayShooting(const BPoint& p, const BPoint& v, bool INOUT, BPoint& FoundP, BPoint& FoundN, ElemID& FoundID, cadrID& FoundCadrID) const;
	void DialogBuild(CWnd* p);
	bool Find_Node_Point_Vector(const BPoint& point, const BPoint& vector, bool IN_OUT, BPoint& outpoint, BPoint& outvector, ElemID &FoundID, cadrID& FoundCadrID);
	void CalcAuto(void);
	bool Contains(const BPoint& bp) const;
	BOOL IsActiv;							//Indicator of activity measure stat
	SufrInformation SOrSurfInfo;			//Information about surface under cursor	
	class CNCMDoc& Doc;


public:
	QMeasure(class CNCMDoc& Doc);
	~QMeasure(void);
	
	void SetSurfInformation(BPoint SurfPoint, ElemID ElID, cadrID CaID, const BPoint& N = BPoint(0., 0., 0., 0.));
	BOOL IsActivated(void) const;
	void Activate(NGlobalState::ModelModes Mode, CWnd * parent, class NStock* pSt);
	void DeActivate(void);
	void SetPoint(const BPoint& Ps, const BPoint& View, QMeasureType MesType);
	void Draw(void);
	void CalcManual(void);
	void CalcManual(const BPoint& P, const BPoint& V);
	SufrInformation GetSufrInfo(void);
	void SetDefault(void);
	void GotoCurrentCadr() const;
};