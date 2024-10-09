#pragma once
#include "BPoint.h"
#include "NColor.h"
#include <vector>

struct SPropChange
{
	SPropChange();
	CString Name;
	bool NameChanged;
	
	CString FileName;
	bool FileNameChanged;
	
	int Turret;
	bool TurretChanged;

	int Attachment;
	bool AttachmentChanged;

	int Position;
	bool PositionChanged;
	
	CString EnemyAxisName;
	bool EnemyAxisNameChanged;

	CString Expr;
	bool ExprChanged;

	BPoint Vector;
	bool VectorChanged;
	
	BPoint BasePoint;
	bool BasePointChanged;
	
	bool EnemyAxisChildren;
	bool EnemyAxisChildrenChanged;
	
	bool MyChildren;
	bool MyChildrenChanged;
	
	BPoint VectorY;
	bool VectorYChanged;

	bool Compensation;
	bool CompensationChanged;

	BPoint CompAxisX;
	bool CompAxisXChanged;

	BPoint CompAxisY;
	bool CompAxisYChanged;
	
	BPoint CompAxisZ;
	bool CompAxisZChanged;

	BPoint LocalDV;
	bool LocalDVChanged;

	double LimitMin;
	bool LimitMinChanged;

	double LimitMax;
	bool LimitMaxChanged;

	NColor Color;
	bool ColorChanged;
	
	bool Visibility;
	bool VisibilityChanged;

	bool IsCS;
	bool IsCSChanged;

	double Length;
	bool LengthChanged;

	double CycleVal;
	bool CycleValChanged;

	double CenterDist;
	bool CenterDistChanged;

	NColor MiscColor;
	bool MiscColorChanged;

	double Margin;
	double MarginChanged;

	std::vector<double> SpecialPts;
};//struct SPropChange