/*******************************************************************************************************************************************************
  #@@        *@    *@@%#@@#    &@    #@%@@,         @(        &@   .@.     @@@@@@@%     @         @(        &@     .@@@%&@@&     &@    @@#        %@
  #@/@       *@   *@      @%   &@   %@      @/      @(        &@   .@.     @,     ,@    @         @(        &@    @@        @*   &@    @,&@       %@
  #@  @(     *@   ,@           &@   #@              @(        &@   .@.     @,      @*   @         @(        &@   @&              &@    @, *@      %@
  #@   &@    *@     @@@,       &@    *@@%           @(        &@   .@.     @,     &@    @         @(        &@   @,              &@    @,   @(    %@
  #@    ,@   *@         .@@.   &@         (@@       @(        &@   .@.     @@@@@@%      @         @(        &@   @,     @@@@@&   &@    @,    @@   %@
  #@      @/ *@           *@   &@           &@      @(        @@   .@.     @,           @         @(        @@   @&         &&   &@    @,     (@. %@
  #@       @@,@   @@      (@   &@   @#      &@      (@       (@.   .@.     @,           @         (@       (@.    @@        &&   &@    @,       @%%@
  #@        *@@    (@@%#&@&    &@    %@@#%@@(         @@@%&@@(     .@.     @,           @@@@@@@@    @@@%&@@(        @@@%&@@@     &@    @,        %@@

* Copyright (C) 2018 - 2020, winsoft666, <winsoft666@outlook.com>.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
*
* Expect bugs
*
* Please use and enjoy.
* Please let me know of any bugs/improvements that you have found/implemented and I will fix/incorporate them into this file.
********************************************************************************************************************************************************/


#ifndef _PLUGIN_COMMON_H_
#define _PLUGIN_COMMON_H_

#include <stdio.h>
#include <strsafe.h>
#include <tchar.h>

#define PLUGIN_BUF_LEN 1024

#define NSISAPI extern "C" __declspec(dllexport) void __cdecl


#define EXDLL_INIT()           {  \
        g_stringsize=stringSize; \
        g_stacktop=stacktop;      \
        g_variables=variables; }



typedef struct _stack_t {
    struct _stack_t *next;
    TCHAR text[1];
} stack_t;


static unsigned int g_stringsize;
static stack_t **g_stacktop;
static TCHAR *g_variables;

enum {
    INST_0,         // $0
    INST_1,         // $1
    INST_2,         // $2
    INST_3,         // $3
    INST_4,         // $4
    INST_5,         // $5
    INST_6,         // $6
    INST_7,         // $7
    INST_8,         // $8
    INST_9,         // $9
    INST_R0,        // $R0
    INST_R1,        // $R1
    INST_R2,        // $R2
    INST_R3,        // $R3
    INST_R4,        // $R4
    INST_R5,        // $R5
    INST_R6,        // $R6
    INST_R7,        // $R7
    INST_R8,        // $R8
    INST_R9,        // $R9
    INST_CMDLINE,   // $CMDLINE
    INST_INSTDIR,   // $INSTDIR
    INST_OUTDIR,    // $OUTDIR
    INST_EXEDIR,    // $EXEDIR
    INST_LANG,      // $LANGUAGE
    __INST_LAST
};


static int __stdcall popstring(TCHAR *str) {
    stack_t *th;
    if (!g_stacktop || !*g_stacktop) return 1;
    th = (*g_stacktop);
    _tcscpy(str, th->text);
    *g_stacktop = th->next;
    GlobalFree((HGLOBAL)th);
    return 0;
}


static void __stdcall pushstring(const TCHAR *str) {
    stack_t *th;
    if (!g_stacktop) return;
    th = (stack_t *)GlobalAlloc(GPTR, sizeof(stack_t) + g_stringsize);
    lstrcpyn(th->text, str, g_stringsize);
    th->next = *g_stacktop;
    *g_stacktop = th;
}

static long __stdcall popint() {
    TCHAR buf[512] = {0};
    popstring(buf);
    return _ttol(buf);
}

static void __stdcall pushint(long value) {
    TCHAR buf[512] = {0};
    StringCchPrintf(buf, 512, TEXT("%ld"), value);
    pushstring(buf);
}


static TCHAR *__stdcall getuservariable(int varnum) {
    if (varnum < 0 || varnum >= __INST_LAST) return NULL;
    return g_variables + varnum * g_stringsize;
}

static void __stdcall setuservariable(int varnum, TCHAR *var) {
    if (var != NULL && varnum >= 0 && varnum < __INST_LAST)
        lstrcpy(g_variables + varnum * g_stringsize, var);
}


enum NSPIM {
    NSPIM_UNLOAD,    // This is the last message a plugin gets, do final cleanup
    NSPIM_GUIUNLOAD, // Called after .onGUIEnd
};

typedef UINT_PTR (*NSISPLUGINCALLBACK)(enum NSPIM);

// extra_parameters data structures containing other interesting stuff
// but the stack, variables and HWND passed on to plug-ins.
typedef struct {
    int autoclose;
    int all_user_var;
    int exec_error;
    int abort;
    int exec_reboot; // NSIS_SUPPORT_REBOOT
    int reboot_called; // NSIS_SUPPORT_REBOOT
    int XXX_cur_insttype; // depreacted
    int plugin_api_version; // see NSISPIAPIVER_CURR
    // used to be XXX_insttype_changed
    int silent; // NSIS_CONFIG_SILENT_SUPPORT
    int instdir_error;
    int rtl;
    int errlvl;
    int alter_reg_view;
    int status_update;
} exec_flags_t;

#ifndef NSISCALL
    #define NSISCALL __stdcall
#endif

typedef struct {
    exec_flags_t *exec_flags;
    int (NSISCALL *ExecuteCodeSegment)(int, HWND);
    void (NSISCALL *validate_filename)(TCHAR *);
    int (NSISCALL *RegisterPluginCallback)(HMODULE, NSISPLUGINCALLBACK); // returns 0 on success, 1 if already registered and < 0 on errors
} ExtraParameters;





#endif //_PLUGIN_COMMON_H_
