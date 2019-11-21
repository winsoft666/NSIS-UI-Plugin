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

#pragma once
#include "SetupPageInterface.h"

class PluginContext {
  public:
    static PluginContext *Instance();
    ~PluginContext();
    void SetExtraParameters(ExtraParameters *param);
    ExtraParameters* GetExtraParameters();

    void SetPluginHandle(HMODULE h);
    HMODULE pluginHandle() const;

    void SetParentHwnd(HWND h);
    HWND parentHwnd() const;

    HANDLE GetExitEvent();

    void SetSetupPage(SetupPageInterface* page);
    SetupPageInterface* GetSetupPage();

    void BindInstallEvent(const tstring &eventName, long nsisFuncAddress);
    void BindButtonClickedEvent(const tstring &buttonName, long nsisFuncAddress);

    bool ExecuteNsisFunction(long funcAddress);

    bool ExecuteInstallEventFunction(const tstring &installEvent);
    bool ExecuteButtonClickedEventFunction(const tstring &buttonName);

  private:
    PluginContext();

    HANDLE m_exitEvent;

    HMODULE m_pluginHandle;
    HWND m_parentHwnd;
    SetupPageInterface* m_setupPage;
    ExtraParameters *m_pluginParms;
    std::map<tstring, long> m_installEventBindMap;
    std::map<tstring, long> m_buttonClickedEventBindMap;
};
