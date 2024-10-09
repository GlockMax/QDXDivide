#pragma once

#ifdef MACHINE_EXPORTS
#define MACHINE_API __declspec(dllexport)
#else
#define MACHINE_API __declspec(dllimport)
#endif
