#pragma once
#ifdef BSPMODEL_EXPORTS
#define BSP_MODEL_API __declspec(dllexport)
#else
#define BSP_MODEL_API __declspec(dllimport)
#endif
