#pragma once
#ifdef MACRO_EXPORTS
#define Macro_API __declspec(dllexport)
#else
#define Macro_API __declspec(dllimport)
#endif
