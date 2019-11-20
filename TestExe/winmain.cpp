#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include "../NSIS-UI-Plugin/PluginCommon.h"

typedef void (* PFN_CreateSetupUI)(HWND hwndParent, int stringSize, TCHAR *variables, stack_t **stacktop, ExtraParameters *extra);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    HMODULE hPlugin = LoadLibrary(TEXT("nsQtPlugin.dll"));
    DWORD dwGLE = GetLastError();
    if (!hPlugin) {
        return 0;
    }

    PFN_CreateSetupUI pfnCreateSetupUI = (PFN_CreateSetupUI)GetProcAddress(hPlugin, "ShowSetupUI");
    assert(pfnCreateSetupUI);

    pfnCreateSetupUI(NULL, 0, nullptr, nullptr, nullptr);

    system("pause");
    return 0;
}