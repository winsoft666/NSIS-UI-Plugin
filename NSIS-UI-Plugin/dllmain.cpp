#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
            PluginContext::Instance()->SetPluginHandle(hModule);
            break;
        }
        case DLL_THREAD_ATTACH: {
            break;
        }
        case DLL_THREAD_DETACH: {
            break;
        }
        case DLL_PROCESS_DETACH: {
            PluginContext::Instance()->SetPluginHandle(NULL);
            break;
        }
    }
    return TRUE;
}

