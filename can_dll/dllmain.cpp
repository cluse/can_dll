// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

extern void Create_Thread();

extern void Destroy_Thread();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		Create_Thread();
		break;
	case DLL_PROCESS_DETACH:
		Destroy_Thread();
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
		break;
    }
    return TRUE;
}

