#pragma once

#include "NMileStones.h"
class NCUnit;

class NMileStone
{
protected:
	const NMileStones *pStones;
	enum StoneNames Name;
public:
	NMileStone(enum StoneNames Name);
	virtual ~NMileStone(void);
	virtual bool IsToolChange(void) { return false;}
	virtual bool IsReached(const NMileStones * pStones) = 0;
	enum StoneNames GetName() const { return Name; }
protected:
	const NCUnitStatePrev *GetPrevState() const;
	NCUnitState *GetCurState() const;
	NCUnit *GetUnit() const;

public:
	virtual int GetCode(void) const;
};

#define OneStoneClass(Name) class N##Name : public NMileStone\
{public:	N##Name(enum StoneNames Name) : NMileStone(Name){};	virtual ~N##Name(void){};\
	virtual bool IsReached(const NMileStones * pStones);

		OneStoneClass(MiToolChange)
			bool IsToolChange(void) override { return true;}
		};
		OneStoneClass(MiProgStartEnd)};
		OneStoneClass(MiZOnly)};
		OneStoneClass(MiXOnly)};
		OneStoneClass(MiYOnly)};
		OneStoneClass(MiBookmark)};
		OneStoneClass(MiCollision)};
		OneStoneClass(MiError)};
		OneStoneClass(MiProgOptStop)};
		OneStoneClass(MiSpindleSpeed)};
		OneStoneClass(MiSpindleForwardReverse)};
		OneStoneClass(MiFeed)};
		OneStoneClass(MiCoolant)};
		OneStoneClass(MiPositioning)};
		OneStoneClass(MiCircularInterpolation)};
		OneStoneClass(MiLinearInterpolation)};
		OneStoneClass(MiCycle)};
		OneStoneClass(MiSubprogramCall)};
		OneStoneClass(MiSubprogramStartEnd)};
		OneStoneClass(MiToolLengthCompensationActive)};
		OneStoneClass(MiToolLengthCompensationCancel)};
		OneStoneClass(MiToolDiameterCompensationActive)};
		OneStoneClass(MiToolDiameterCompensationCancel)};
		OneStoneClass(MiLifeTime)};
		OneStoneClass(MiCheckZeroFeed)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckLengthComp)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckLengthOffsetNum)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckDiamOffsetNum)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckIJKRDef)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckXYChamf)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckDiamCompOff)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckFeedTurnSpindleTurnOff)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckFeedTurnSpindleMillOn)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckFeedMillSpindleMillOff)
			virtual int GetCode(void) const;
		};
		OneStoneClass(MiCheckFeedMillSpindleTurnOn)
			virtual int GetCode(void) const;
		};
