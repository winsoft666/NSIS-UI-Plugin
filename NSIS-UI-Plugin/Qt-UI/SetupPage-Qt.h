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
#include <QtWidgets>
#include <QtGlobal>
#include <QtWidgets/QApplication>
#include "ui_MainPage.h"
#include "Qt-FramelessWindow/FramelessMainWindow.h"
#include <QMap>

inline QString tstringToQString(const tstring &str) {
    QString strResult;
#if (defined _UNICODE || defined UNICODE)
    strResult = QString::fromStdWString(str);
#else
    strResult = QString::fromStdString(str);
#endif
    return strResult;
}

inline tstring QStringTotstring(const QString &str) {
#if (defined _UNICODE || defined UNICODE)
    return str.toStdWString();
#else
    return str.toStdString();
#endif
}

class SetupPage_Qt :
    public FramelessMainWindow
    , public SetupPageInterface {
    Q_OBJECT

  public:
    SetupPage_Qt(QWidget *parent = Q_NULLPTR);

    void SetTitle(const tstring &title) override;
    void SetRequiredSpaceKb(long kb) override;
    void SetInstallDirectory(const tstring &dir) override;
    tstring GetInstallDirectory() override;
    void SetInstallStepDescription(const tstring &description, int progressValue = -1) override;
    void NsisExtractFilesFinished() override;
    bool IsCreateDesktopShortcutEnabled() override;
    bool IsAutoStartupOnBootEnabled() override;
  private:
    void updateDriverInfo();
    void exitSetup();
  private:
    Ui::MainPage ui;
    long m_requiredSpaceKb;
    std::future<void> m_addListItemAsync;
    QMutex m_waitingAddItemsMutex;
    QStringList m_waitingAddItems;
};

