#include "StdAfx.h"
#include "RPickID.h"

RPickID::RPickID(void)
{
	Size = 0;
	StPos = 0;
}

RPickID::~RPickID(void)
{
}

void RPickID::Set(int Num, unsigned int *Data)
{
	Size = Num;
	memcpy(Names, Data, sizeof(Names[0]) * Size);
}

PObjectGroup RPickID::GetGroup(void) const
{
	return (Size <= 0) ? OG_GR_EMPTY : PObjectGroup(Names[0]);
}

int RPickID::GetObjName(void) const
{
	return (Size < 2) ? 0 : Names[1];
}

int RPickID::GetObjSubName0(void) const
{
	return (Size < 3) ? 0 : Names[2];
}

int RPickID::GetObjSubName1(void) const
{
	return (Size < 4) ? 0 : Names[3];
}

void RPickID::Set0()
{
	Size = 0;
}

void RPickID::Set1(GLuint Group)
{
	Size = 1;
	Names[0] = Group;
}

void RPickID::Set2(GLuint Group, GLuint Name)
{
	Size = 2;
	Names[0] = Group;
	Names[1] = Name;
}

void RPickID::Set3(GLuint Group, GLuint Name, GLuint Sub0)
{
	Size = 3;
	Names[0] = Group;
	Names[1] = Name;
	Names[2] = Sub0;
}

void RPickID::Set4(GLuint Group, GLuint Name, GLuint Sub0, GLuint Sub1)
{
	Size = 4;
	Names[0] = Group;
	Names[1] = Name;
	Names[2] = Sub0;
	Names[3] = Sub1;
}
