// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#ifdef _WIN64
extern "C" { int __afxForceUSRDLL; } 
#else
extern "C" { int _afxForceUSRDLL; } 
#endif // added for DEBUG_NEW works.

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

