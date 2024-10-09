#pragma once
//#include "vld.h"

#ifdef OPENGLRENDER_EXPORTS
#define OPENGLRENDER_API __declspec(dllexport)
#else
#ifndef SHARED_HANDLERS
#define OPENGLRENDER_API __declspec(dllimport)
#else
#define OPENGLRENDER_API 
#endif
#endif
