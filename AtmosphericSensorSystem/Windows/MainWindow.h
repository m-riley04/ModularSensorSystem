#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "../Controllers/MainController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    MainController* controller;

    void initWidgets();
    void initSignals();

public slots:
    void displayFrame(QVariant data);
    void openCameraProperties();

    void quit();
    void restart();
};
