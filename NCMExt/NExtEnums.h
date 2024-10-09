#pragma once

enum class TrajPart
{
	TP_WHOLE = 0, // whole toolpath
	TP_PART = 1, // worked-out toolpath
	TP_WORM = 2 // fragment of toolpath
	// Concrete numbers needed for backward compatibility
};

enum CollLogElemTypeE
{
	LEE_NOTDEFINED,	// Reserved
	LEE_COLLISION,	// Reserved
	LEE_OUTOFRANGE,	// Reserved
	LEE_COLLTOOL,	// Tool collision
	LEE_COLLHOLD,	// Holder collision
	LEE_COLLSHANK,	// Shank collision
	LEE_COLLVERT,
	LEE_EMPTYTOOL
};

struct CollisionData
{
	CollLogElemTypeE CollisionType;
	int ProgramNum;
	int LineNum;
	int StepNum;
};
