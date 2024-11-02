#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>

enum CameraState {
    CAMERA_IDLE,
    CAMERA_RECORDING,
    CAMERA_PAUSED,
};

class Camera : public QObject
{
	Q_OBJECT

private:
    cv::VideoCapture camera;
    cv::VideoWriter videoWriter;
    QUrl outputDir; /// TODO: Maybe change this to QDir 
    QTimer* frameTimer;
    CameraState state = CAMERA_IDLE;

public:
	Camera(QObject *parent);
	~Camera();

    void open(int deviceIndex);
    void release();

    bool isOpened();

signals:
    void cameraStarted();
    void cameraClosed();
    void deviceChanged();
    void deviceError();
    void recordingStarted();
    void recordingStopped();
    void recordingError();

public slots:
    void setVideoDevice();

    void restart();
    void start();
    void stop();

    void startRecording();
    void pauseRecording();
    void stopRecording();

    void setBrightness(int value);
    void setContrast(int value);
    void setSaturation(int value);
    void setGain(int value);
    void setBacklight(bool value);
    void setAutoExposure(bool value);
    /*void setFPS();
    void setFrameWidth();
    void setFrameHeight();
    void setHue();
    void setExposure();
    void setSharpness();
    void setGamma();
    void setBitrate();*/

    void setOutputDirectory(QUrl directory);
};
