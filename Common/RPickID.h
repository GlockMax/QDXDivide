#pragma once
#include "OPENGLRENDER_API.h"
#include "GLee.h"
#include "NObjEnums.h"

constexpr int MAX_NAMES = 4;

class OPENGLRENDER_API RPickID
{
public:
	RPickID(void);
	~RPickID(void);
	void Set(int Num, unsigned int *Data);
	PObjectGroup GetGroup(void) const;
	int GetObjName(void) const;
	int GetObjSubName0(void) const;
	int GetObjSubName1(void) const;
	int GetSize() const { return Size; }
	int GetStPos() const { return StPos; }
	void SetStPos(int Val) { StPos = Val; }
	void Set0();
	void Set1(GLuint Group);
	void Set2(GLuint Group, GLuint Name);
	void Set3(GLuint Group, GLuint Name, GLuint Sub0);
	void Set4(GLuint Group, GLuint Name, GLuint Sub0, GLuint Sub1);
protected:
	int Size;
	GLuint Names[MAX_NAMES];
	int StPos;
};
