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
#include "SetupPage-Qt.h"
#include "../DriverInfo.h"

SetupPage_Qt::SetupPage_Qt(QWidget *parent)
    : FramelessMainWindow(true, parent)
    , m_requiredSpaceKb(0) {
    ui.setupUi(this);

    ui.tabWidget->setCurrentIndex(0);
    ui.progressBarInstall->setMinimum(0);
    ui.progressBarInstall->setMaximum(100);
    ui.pushButtonToFinishedPage->setEnabled(false);

    FramelessMainWindow::setAllWidgetMouseTracking(this);
    setResizeable(false);
    setTitlebar({ ui.widgetTitle});

    FramelessMainWindow::loadStyleSheetFile(":/DefaultTheme/main.css", this);

    ui.tabWidget->tabBar()->hide();

    connect(ui.pushButtonMin, &QPushButton::clicked, [this]() {
        this->showMinimized();
    });

    connect(ui.pushButtonClose, &QPushButton::clicked, [this]() {
        PluginContext::Instance()->ExecuteInstallEventFunction(INSTALL_EVENT_USER_CANCEL);

        exitSetup();
    });

    connect(ui.pushButtonSelectInstallDir, &QPushButton::clicked, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        dir = QDir::toNativeSeparators(dir);
        ui.lineEditInstallDir->setText(dir);
        updateDriverInfo();
    });

    connect(ui.pushButtonStartInstall, &QPushButton::clicked, this, [this]() {
        QString strDir = ui.lineEditInstallDir->text();
        if (strDir.length() == 0)
            return;

        QDir dir(strDir);
        if (!dir.exists()) {
            if (!dir.mkdir(strDir)) {
                return;
            }
        }

        PluginContext::Instance()->ExecuteInstallEventFunction(INSTALL_EVENT_START_EXTRACT_FILES);

        ui.tabWidget->setCurrentIndex(1);
    });

    connect(ui.pushButtonToFinishedPage, &QPushButton::clicked, [this]() {
        ui.tabWidget->setCurrentIndex(2);
    });

    connect(ui.pushButtonFinish, &QPushButton::clicked, [this]() {
        PluginContext::Instance()->ExecuteInstallEventFunction(INSTALL_EVENT_BEFORE_FINISHED);

        exitSetup();
    });

    m_addListItemAsync = std::async(std::launch::async, [this]() {
        HANDLE exitEvent = PluginContext::Instance()->GetExitEvent();
        while (WaitForSingleObject(exitEvent, 100) != WAIT_OBJECT_0) {
            m_waitingAddItemsMutex.lock();
            if (m_waitingAddItems.size() > 0) {
                QMetaObject::invokeMethod(this, [this]() {
                    ui.listWidgetInstallDetails->addItems(m_waitingAddItems);
                    ui.listWidgetInstallDetails->scrollToBottom();
                }, Qt::BlockingQueuedConnection);
                m_waitingAddItems.clear();
            }
            m_waitingAddItemsMutex.unlock();
        }
    });

    PluginContext::Instance()->ExecuteInstallEventFunction(INSTALL_EVENT_UI_PREPARED);
}


void SetupPage_Qt::SetTitle(const tstring &title) {
    this->setWindowTitle(tstringToQString(title));
}


void SetupPage_Qt::SetRequiredSpaceKb(long kb) {
    m_requiredSpaceKb = kb;
    updateDriverInfo();
}

void SetupPage_Qt::SetInstallDirectory(const tstring &dir) {
    ui.lineEditInstallDir->setText(tstringToQString(dir));
    updateDriverInfo();
}

tstring SetupPage_Qt::GetInstallDirectory() {
    QString strDir = ui.lineEditInstallDir->text();
    return QStringTotstring(strDir);
}

void SetupPage_Qt::SetInstallStepDescription(const tstring &description, int progressValue /* = -1 */) {
    QMetaObject::invokeMethod(this, [this, progressValue]() {
        if (progressValue >= 0 && progressValue <= 100) {
            ui.progressBarInstall->setValue(progressValue);
        }
    }, Qt::QueuedConnection);

    m_waitingAddItemsMutex.lock();
    m_waitingAddItems.push_back(tstringToQString(description));
    m_waitingAddItemsMutex.unlock();
}


void SetupPage_Qt::NsisExtractFilesFinished() {
    QMetaObject::invokeMethod(this, [this]() {
        ui.pushButtonToFinishedPage->setEnabled(true);
    }, Qt::QueuedConnection);
}

bool SetupPage_Qt::IsCreateDesktopShortcutEnabled() {
    return ui.checkBoxDesktopShortcut->isChecked();
}


bool SetupPage_Qt::IsAutoStartupOnBootEnabled() {
    return ui.checkBoxAutoStartupOnBoot->isChecked();
}

void SetupPage_Qt::updateDriverInfo() {
    int driver = DriveInfo::GetDrive(ui.lineEditInstallDir->text().toStdWString().c_str());
    if (driver > 0) {
        float driverTotalMb = DriveInfo::GetTotalMB(driver);
        float driverFreeMb = DriveInfo::GetFreeMB(driver);

        QString strDiskInfo = QString("Required: %1MB Free: %2MB  Total: %3MB").arg(m_requiredSpaceKb / 1024).arg(driverFreeMb).arg(driverTotalMb);
        ui.labelDiskInfo->setText(strDiskInfo);
    }
}

void SetupPage_Qt::exitSetup() {
    HANDLE exitEvent = PluginContext::Instance()->GetExitEvent();
    if (exitEvent)
        SetEvent(exitEvent);
    if (m_addListItemAsync.valid())
        m_addListItemAsync.wait();
    this->close();
}


