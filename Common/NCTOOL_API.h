#pragma once

#ifdef NCTOOL_EXPORTS
#define NCTOOL_API __declspec(dllexport)
#else
#define NCTOOL_API __declspec(dllimport)
#endif
