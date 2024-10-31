#pragma once

#include <QtWidgets/QMainWindow>
#include <QtMultimedia>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    QCamera* camera;
    QMediaCaptureSession captureSession;
    QMediaRecorder* mediaRecorder;

    void init();

};
