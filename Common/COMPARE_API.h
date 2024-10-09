#pragma once
#ifdef COMPARE_EXPORTS
#define COMPARE_API __declspec(dllexport)
#else
#define COMPARE_API __declspec(dllimport)
#endif
