// NViewState.h: interface for the NViewState class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum VisualMode
{
	WireFrame,
	Shade,
	Outline
//	ZlistEnable
};

class NViewState  
{
public:
	void SetBase(double x,double y,double z);
	double BaseX,BaseY,BaseZ;
	VisualMode VMode;
	NViewState();
	virtual ~NViewState();
};

