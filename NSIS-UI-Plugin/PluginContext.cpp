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

#include "stdafx.h"
#include "PluginContext.h"

PluginContext::PluginContext() :
    m_parentHwnd(NULL)
    , m_setupPage(NULL)
    , m_pluginParms(NULL)
    , m_pluginHandle(NULL) {
    m_exitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

PluginContext *PluginContext::Instance() {
    static PluginContext *context = NULL;
    if (!context) {
        context = new PluginContext();
    }
    return context;
}


PluginContext::~PluginContext() {
    if (m_exitEvent) {
        CloseHandle(m_exitEvent);
        m_exitEvent = NULL;
    }
}

void PluginContext::SetExtraParameters(ExtraParameters *param) {
    m_pluginParms = param;
}

ExtraParameters *PluginContext::GetExtraParameters() {
    return m_pluginParms;
}

void PluginContext::SetPluginHandle(HMODULE h) {
    m_pluginHandle = h;
}

HMODULE PluginContext::pluginHandle() const {
    return m_pluginHandle;
}

void PluginContext::SetParentHwnd(HWND h) {
    m_parentHwnd = h;
}

HWND PluginContext::parentHwnd() const {
    return m_parentHwnd;
}

HANDLE PluginContext::GetExitEvent() {
    return m_exitEvent;
}

void PluginContext::SetSetupPage(SetupPageInterface* page) {
    m_setupPage = page;
}

SetupPageInterface* PluginContext::GetSetupPage() {
    return m_setupPage;
}

void PluginContext::BindInstallEvent(const tstring &eventName, long nsisFuncAddress) {
    m_installEventBindMap[eventName] = nsisFuncAddress;
}

void PluginContext::BindButtonClickedEvent(const tstring &eventName, long nsisFuncAddress) {
    m_buttonClickedEventBindMap[eventName] = nsisFuncAddress;
}

bool PluginContext::ExecuteNsisFunction(long funcAddress) {
    if (m_pluginParms) {
        m_pluginParms->ExecuteCodeSegment(funcAddress, NULL);
        return true;
    }
    return false;
}

bool PluginContext::ExecuteInstallEventFunction(const tstring &installEvent) {
    if (m_pluginParms && m_installEventBindMap.find(installEvent) != m_installEventBindMap.end()) {
        m_pluginParms->ExecuteCodeSegment(m_installEventBindMap[installEvent] - 1, NULL);
        return true;
    }
    return false;
}

bool PluginContext::ExecuteButtonClickedEventFunction(const tstring &buttonName) {
    if (m_pluginParms && m_buttonClickedEventBindMap.find(buttonName) != m_buttonClickedEventBindMap.end()) {
        m_pluginParms->ExecuteCodeSegment(m_buttonClickedEventBindMap[buttonName] - 1, NULL);
        return true;
    }
    return false;
}
