#pragma once
#include "BSP_MODEL_API.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "MQuadTree.h"
#include "gbodyrender.h"


class BSP_MODEL_API GQuadRender : public GBodyRender
{
	static const int ELEM_ID_SIZE = sizeof(ElemID)/sizeof(GLfloat);
	friend class GQuadRenderMill;
	friend class GQuadRenderRot;


#pragma endregion LOOP_THROUGH_ALL_BODIES
public:
	int FillAllArraysCh(MQuadBody & Body) override;
	int FillModifArraysCh(MQuadBody & Tree) override;
public:
	GQuadRender(const MQuadBody &Tree);
	GQuadRender(const GQuadRender &inRender);
	virtual ~GQuadRender(void);
	void ClearExtArr(void);	
	class MBSPCell*** GetExtArray() const {return ExtArr;}
    void SetExtArray(class MBSPCell*** EArr){ExtArr=EArr;}
	unsigned int GetRowsNum() const;
	unsigned int GetColsNum() const;

protected:
	int RowsNum;
	int ColsNum;
	class MBSPCell*** ExtArr;
	static double SmoothAngle;
	GLvoid*** Array; 
public:
	virtual void WriteSTL(CStdioFile & f) const override = 0;
	virtual NCMApplicationType GetType() const = 0;
	virtual void FillCell(MQuadBody & Tree, int ix, int iy) = 0;
	virtual void ActivateExtArr(void);
	virtual const BPoint RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const = 0;
	MQuadBody::Window CalcWindow(const BBox & Bounds) const override;
	static double GetSmoothAngle(void);
	static void SetSmoothAngle(double Angle);
	void GetCellIndexes(double x, double y, int & ix, int & iy) const;
	void TransModifArraysCh(GBodyRender& inRender) override;
protected:
	bool Proper(const MQuadBody & Tree);
	static void StoreID(GLfloat *pRet, ElemID ElID);
};
