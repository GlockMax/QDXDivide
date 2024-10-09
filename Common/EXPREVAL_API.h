#pragma once

#ifdef EXPREVAL_EXPORTS
#define EXPREVAL_API __declspec(dllexport)
#else
#define EXPREVAL_API __declspec(dllimport)
#endif
