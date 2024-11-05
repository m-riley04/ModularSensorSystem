#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>
#include "sensor.h"

enum CameraState {
    CAMERA_IDLE,
    CAMERA_RECORDING,
    CAMERA_PAUSED,
};

class Camera : public Sensor
{
	Q_OBJECT

private:
    cv::VideoCapture camera;
    cv::VideoWriter videoWriter;
    cv::Mat frame;
    QTimer* frameTimer;
    CameraState _state = CAMERA_IDLE;

public:
	Camera(QObject *parent);
	~Camera();

    void open(int deviceIndex);
    void release();
    bool isOpened();

    QUrl& getOutputDirectory();
    QTimer* getFrameTimer();
    CameraState& getState();

    int brightness();
    int contrast();
    int saturation();
    int gain();
    int exposure();
    bool backlight();
    bool autoExposure();
    
    Camera& operator >> (cv::Mat& image);

	static bool checkCameraAvailability();

public slots:
    void setVideoDevice(int deviceIndex);

    void restart() override;
    void start() override;
    void pause() override;
    void stop() override;

    void startRecording() override;
    void pauseRecording() override;
    void stopRecording() override;

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

signals:
    void deviceChanged();
    void deviceError();
};
