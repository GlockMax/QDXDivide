#pragma once

#ifdef OCIMPORT_EXPORTS
#define OCIMPORT_API __declspec(dllexport)
#else
#define OCIMPORT_API __declspec(dllimport)
#endif
