#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>
#include "sensor.h"

#define DEFAULT_VIEWFINDER_FRAME_RATE 60
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
    int _viewfinderFrameRate = DEFAULT_VIEWFINDER_FRAME_RATE;

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
    
    int viewfinderFrameRate();
    
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
    void setFPS(int fps);
    void setFrameWidth(int width);
    void setFrameHeight(int height);
    void setHue(int value);
    void setExposure(int value);
    void setSharpness(int value);
    void setGamma(int value);
    void setBitrate(int value);
    void setViewfinderFrameRate(int frameRate);

signals:
    void deviceChanged();
    void deviceError();
	
	void brightnessChanged(int value);
	void contrastChanged(int value);
	void saturationChanged(int value);
	void gainChanged(int value);
	void exposureChanged(int value);
	void backlightChanged(bool value);
	void autoExposureChanged(bool value);
	void frameWidthChanged(int value);
	void frameHeightChanged(int value);
    void fpsChanged(int value);
	void hueChanged(int value);
	void sharpnessChanged(int value);
	void gammaChanged(int value);
	void bitrateChanged(int value);
    void viewfinderFrameRateChanged(int frameRate);
};
