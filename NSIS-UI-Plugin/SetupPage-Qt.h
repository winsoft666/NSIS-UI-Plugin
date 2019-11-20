#pragma once
#include <QtWidgets>
#include <QtGlobal>
#include <QtWidgets/QApplication>
#include "ui_MainPage.h"
#include "Qt-FramelessWindow/FramelessMainWindow.h"
#include <QMap>

class SetupPage_Qt : 
    public FramelessMainWindow
    , public SetupPageInterface
{
    Q_OBJECT

public:
    SetupPage_Qt(QWidget *parent = Q_NULLPTR);
	
    tstring GetInstallDirectory() override;
    void SetInstallStepDescription(const tstring &description, int progressValue = -1) override;
signals:
    void updateInstallStepDescrition(QString description, int progressValue);
private slots:
    void onInstallStepDescriptionUpdated(QString description, int progressValue);
private:
    Ui::MainPage ui;
};

