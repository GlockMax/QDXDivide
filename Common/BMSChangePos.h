#pragma once

class AFX_EXT_CLASS BMSChangePos
{
public:
	BMSChangePos(void);
	BMSChangePos(double iNewValue, int iIndex, int iHistPosAbs, bool Fix) { Fixed = Fix;  NewValue = iNewValue; Index = iIndex; HistPosAbs = iHistPosAbs; };
	~BMSChangePos(void);
	int GetIndex(void) const { return Index;}
	int GetPos(void) const { return HistPosAbs;}
	double GetValue(void) const { return NewValue;}
	void SetValue(double Val, bool Fix) { NewValue = Val; Fixed = Fix; }
	bool IsFixed() const { return Fixed; }
protected:
	double NewValue;
	int Index;
	int HistPosAbs;
	bool Fixed;// non incremental and non start value
};
