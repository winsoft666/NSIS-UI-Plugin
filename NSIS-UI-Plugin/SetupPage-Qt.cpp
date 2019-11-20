#include "stdafx.h"
#include "SetupPage-Qt.h"


SetupPage_Qt::SetupPage_Qt(QWidget *parent)
    : FramelessMainWindow(true, parent) {
    ui.setupUi(this);
    ui.tabWidget->setCurrentIndex(0);
    ui.progressBarInstall->setMinimum(0);
    ui.progressBarInstall->setMaximum(100);

    FramelessMainWindow::setAllWidgetMouseTracking(this);
    setResizeable(false);
    setTitlebar({ ui.widgetTitle});

    FramelessMainWindow::loadStyleSheetFile(":/DefaultTheme/main.css", this);

    ui.tabWidget->tabBar()->hide();

    connect(this, &SetupPage_Qt::updateInstallStepDescrition, this, &SetupPage_Qt::onInstallStepDescriptionUpdated);

    connect(ui.pushButtonMin, &QPushButton::clicked, [this]() {
        
    });

    connect(ui.pushButtonClose, &QPushButton::clicked, [this]() {
        PluginContext::Instance()->ExecuteInstallEventFunction(INSTALL_EVENT_USER_CANCEL);

        this->close();
    });

    connect(ui.pushButtonSelectInstallDir, &QPushButton::clicked, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        dir = QDir::toNativeSeparators(dir);
        ui.lineEditInstallDir->setText(dir);
    });
    
    connect(ui.pushButtonStartInstall, &QPushButton::clicked, this, [this]() {
        PluginContext::Instance()->ExecuteInstallEventFunction(INSTALL_EVENT_START_EXTRACT_FILES);

        ui.tabWidget->setCurrentIndex(1);
    });
}

tstring SetupPage_Qt::GetInstallDirectory() {
    QString strDir = ui.lineEditInstallDir->text();
#if (defined _UNICODE || defined UNICODE)
    return strDir.toStdWString();
#else
    return strDir.toStdString();
#endif
}


void SetupPage_Qt::SetInstallStepDescription(const tstring &description, int progressValue /* = -1 */) {
    QString strDescription;
#if (defined _UNICODE || defined UNICODE)
    strDescription = QString::fromStdWString(description);
#else
    strDescription = QString::fromStdString(description);
#endif
    emit updateInstallStepDescrition(strDescription, progressValue);
}

void SetupPage_Qt::onInstallStepDescriptionUpdated(QString description, int progressValue) {
    if (progressValue >= 0 && progressValue <= 100) {
        ui.progressBarInstall->setValue(progressValue);
    }
    ui.labelInstallDescription->setText(description);
}


