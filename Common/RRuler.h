#pragma once
#include "OPENGLRENDER_API.h"

class OPENGLRENDER_API RRuler
{
public:
	RRuler(void);
	~RRuler(void);
public:
	static void Draw(bool Active, const class Text& fontList);
};
