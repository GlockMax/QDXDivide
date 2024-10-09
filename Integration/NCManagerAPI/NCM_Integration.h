#pragma once
#include "StdAfx.h"

const int MAX_TOOLS = 256;

class AFX_EXT_CLASS NCM_Integration
{
public:
	NCM_Integration(void);
	~NCM_Integration(void);
	bool setStock(const double min[3],const double max[3],double resolution,const UCHAR rgb[3]);
	bool  setStock(const char* stlFileName,double resolution,const UCHAR rgb[3]);
	bool  renderStock();
	void  resetStock();
	bool removeToolTrace(int ToolID
		, const double Bp[3]
		, const double Bn[3]
		, const double Ep[3]
		, const double En[3]
		, const double Ca[3]
		, bool Arc
		, bool CCW
		, bool checkCollison);
	int newCustomMillingTool(const char* contour,const UCHAR rgb[3]= 0);
	bool  setToolColor(int toolId, const unsigned char rgb[3]);
protected:
	bool Cut();
	void ClearTools(void);
	double BufferLength(void);

	double BufferHeight(void);
	void MoveBuffer(void);
	double BufferTol(void);


protected:
	class NStock *pStock;
	class NToolCombined *ComTools[MAX_TOOLS];
	int ToolsNum;
	class NCadrGeom *Geom;
	int GeomSize;
	double LastNormal[3];
	double LastPoint[3];
	int LastTool;
	class NCadrGeom *Buffer;
	int BufferSize;
};
