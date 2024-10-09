#pragma once
#include "T3DInterface.h"

class __declspec( dllexport ) T3DxInit
{
public:
	T3DxInit(void);
	~T3DxInit(void);
	static int Init(void);
	static int UnInit(void);
	static T3DInterface *pInt;
};
