#pragma once

enum PObjectGroup
{
	OG_GR_EMPTY = 0,
	OG_NO_GROUP = 0x01,
	OG_STOCKS = 0x02,
	OG_PARTS = 0x04,
	OG_FIXTURES = 0x08,
	OG_TOOLS = 0x10,
	OG_MACHINE = 0x20,
	OG_PROGRAMS = 0x40,
	OG_DIMENS = 0x80,
	OG_OCTREE = 0x100,
	OG_HOLD = 0x200,
	OG_UCS = 0x400,
	OG_SKELETON = 0x800
};
enum PObjectStatus
{
	ST_EMPTY = 0,
	USED = 1,
	CHANGED = 2,
	DELETED = 3,
	NEW = 4
};
