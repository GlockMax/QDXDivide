#pragma once
#include "BMatr.h"
#include "BSP_MODEL_API.h"
#include "MQuadTree.h"

class BSP_MODEL_API QDivideBase
{
public:
	QDivideBase();
	virtual ~QDivideBase(void);
protected:
	enum DIVIDE_MODE
	{
		REMOVE_NONE = NULL,
		REMOVE_OTHERS = NULL + 1,
		REMOVE_CURRENT = NULL + 2
	};

	bool IsActive;
	DIVIDE_MODE mode;
	BMatr CorrMatr; // Used to compensate X rotation in turn and millturn projects
public:
	bool IsActivated(void) const { return IsActive; }
	void ChangeCuttingMode(bool flag);
	bool IsModeSelected() const { return mode != REMOVE_NONE; }
	void SetCorrMatr(const BMatr& M) { CorrMatr = M; };
	virtual void Recreate() {};
	virtual void SetPoint(BPoint point, BPoint view) = 0;
	virtual void Revert() = 0;
};

