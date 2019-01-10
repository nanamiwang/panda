// dllmain.cpp : Defines the entry point for the DLL application.
#include "dllmain.h"
#define _ELPP_STL_LOGGING
#include <iostream>
#include <string>
#include "log.h"

HMODULE thisdll;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	thisdll = hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		log_init("panda_j2534.log");
		logA("panda started, process: %08X", ::GetCurrentProcessId());
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		logA("panda stopped, process: %08X", ::GetCurrentProcessId());
		break;
	}
	return TRUE;
}
