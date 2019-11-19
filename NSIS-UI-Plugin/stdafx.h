#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <string>
#include <map>

// Support Chinese
# pragma execution_character_set("utf-8")

#if (defined _UNICODE || defined UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#include "PluginCommon.h"
#include "PluginContext.h"
#include "InstallEvent.h"

