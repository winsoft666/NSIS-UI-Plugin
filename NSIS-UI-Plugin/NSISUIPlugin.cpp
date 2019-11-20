#include "stdafx.h"
#include "SetupPage-Qt.h"


#define NSMETHOD_INIT() do {\
        PluginContext::Instance()->SetExtraParameters(extra); \
        PluginContext::Instance()->SetParentHwnd(hwndParent); \
        extra->RegisterPluginCallback(PluginContext::Instance()->pluginHandle(), PluginCallback); \
        EXDLL_INIT(); } while(false)

static UINT_PTR PluginCallback(enum NSPIM msg) {
    return 0;
}

NSISAPI ShowSetupUI(HWND hwndParent, int stringSize, TCHAR *variables, stack_t **stacktop, ExtraParameters *extra) {
    NSMETHOD_INIT();
  
    TCHAR szNsisPluginDir[MAX_PATH] = { 0 };
    popstring(szNsisPluginDir);

    // Start show Qt UI
    //
    if (_tcslen(szNsisPluginDir) > 0) {
        QString strQtPluginDir;
#if (defined _UNICODE || defined UNICODE)
        strQtPluginDir = QString::fromStdWString(szNsisPluginDir);
#else
        strQtPluginDir = QString::fromStdString(szNsisPluginDir);
#endif
        QApplication::addLibraryPath(strQtPluginDir);
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    int argc = 1;
    char currentPath[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, currentPath, MAX_PATH);
    char *argv[2] = { {currentPath}, {} };
    QApplication app(argc, argv);
    SetupPage_Qt *mainPage = new SetupPage_Qt();
    PluginContext::Instance()->SetSetupPage(mainPage);
    mainPage->show();
    app.exec();
}

NSISAPI BindInstallEventToNsisFunc(HWND hwndParent, int stringSize, TCHAR *variables, stack_t **stacktop, ExtraParameters *extra) {
    NSMETHOD_INIT();

    TCHAR szEventName[MAX_PATH] = { 0 };
    popstring(szEventName);
    long callbackFuncAddress = popint();

    PluginContext::Instance()->BindInstallEvent(szEventName, callbackFuncAddress);
}

NSISAPI BindButtonClickedEventToNsisFunc(HWND hwndParent, int stringSize, TCHAR *variables, stack_t **stacktop, ExtraParameters *extra) {
    NSMETHOD_INIT();

    TCHAR szControlName[MAX_PATH] = { 0 };
    popstring(szControlName);

    long callbackFuncAddress = popint();
    
    PluginContext::Instance()->BindButtonClickedEvent(szControlName, callbackFuncAddress);
}

NSISAPI GetInstallDirectory(HWND hwndParent, int stringSize, TCHAR *variables, stack_t **stacktop, ExtraParameters *extra) {
    NSMETHOD_INIT();
    tstring strDir;
    if (PluginContext::Instance()->GetSetupPage()) {
        strDir = PluginContext::Instance()->GetSetupPage()->GetInstallDirectory();
    }
    pushstring(strDir.c_str());
}


NSISAPI SetInstallStepDescription(HWND hwndParent, int stringSize, TCHAR *variables, stack_t **stacktop, ExtraParameters *extra) {
    NSMETHOD_INIT();
    TCHAR szDescription[MAX_PATH] = { 0 };
    long percent = -1;

    popstring(szDescription);
    percent = popint();

    if (PluginContext::Instance()->GetSetupPage()) {
        PluginContext::Instance()->GetSetupPage()->SetInstallStepDescription(szDescription, percent);
    }
}

NSISAPI BackgroundRun(HWND hwndParent, int stringSize, TCHAR *variables, stack_t **stacktop, ExtraParameters *extra) {
    NSMETHOD_INIT();
    long nsisFuncAddress = popint();

    std::thread t = std::thread([nsisFuncAddress]() {
        PluginContext::Instance()->ExecuteNsisFunction(nsisFuncAddress - 1);
    });
    t.detach();
}