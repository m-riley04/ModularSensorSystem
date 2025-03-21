#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QVideoFrame>
#include <QVideoWidget>
#include "Windows/AddCameraDialog/AddCameraDialog.h"
#include "Widgets/CustomSinkWidget/customsinkwidget.h";
#include "Windows/QtCameraControlsWindow/qtcameracontrolsdialog.h"
#include "Widgets/SimultaneousMediaPlayer/simultaneousmediaplayer.h"
#include "Controllers/MainController/maincontroller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    std::unique_ptr<MainController> pController;
    std::vector<CustomSinkWidget*> mSinkWidgets;

    void initWidgets();
    void initSignals();

public slots:
    void addVideoWidget(Camera *camera);
    void openAddCameraDialog();
    void removeCamera();

    void quit();
    void restart();

signals:
	void cameraChanged(Camera* camera);
};
