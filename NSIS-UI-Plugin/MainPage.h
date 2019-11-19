#pragma once
#include <QtWidgets>
#include <QtGlobal>
#include <QtWidgets/QApplication>
#include "ui_MainPage.h"
#include "Qt-FramelessWindow/FramelessMainWindow.h"
#include <QMap>

class MainPage : public FramelessMainWindow
{
    Q_OBJECT

public:
    MainPage(QWidget *parent = Q_NULLPTR);
	

public slots:
    void onStartInstallPushButtonClicked();
private:

private:
    Ui::MainPage ui;
};

