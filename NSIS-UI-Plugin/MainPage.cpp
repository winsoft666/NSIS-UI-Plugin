#include "stdafx.h"
#include "MainPage.h"


MainPage::MainPage(QWidget *parent)
    : FramelessMainWindow(true, parent) {
    ui.setupUi(this);

    FramelessMainWindow::setAllWidgetMouseTracking(this);
    setResizeable(false);
    setTitlebar({ ui.widgetTitle});

    FramelessMainWindow::loadStyleSheetFile(":/DefaultTheme/main.css", this);

    ui.tabWidget->tabBar()->hide();

    connect(ui.pushButtonMin, &QPushButton::clicked, [this]() {
        
    });

    connect(ui.pushButtonClose, &QPushButton::clicked, [this]() {
        PluginContext::Instance()->ExecuteInstallEventFunction(INSTALL_EVENT_USER_CANCEL);

        this->close();
    });

    connect(ui.pushButtonSelectInstallForder, &QPushButton::clicked, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        ui.lineEditInstallForder->setText(dir);
    });
    
    connect(ui.pushButtonStartInstall, &QPushButton::clicked, this, &MainPage::onStartInstallPushButtonClicked);
}



void MainPage::onStartInstallPushButtonClicked() {
    pushstring(ui.lineEditInstallForder->text().toStdWString().c_str());
    PluginContext::Instance()->ExecuteInstallEventFunction(INSTALL_EVENT_START_EXTRACT_FILES);

    ui.tabWidget->setCurrentIndex(1);
}

