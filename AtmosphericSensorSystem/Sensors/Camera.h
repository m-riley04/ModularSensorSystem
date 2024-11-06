#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>
#include "sensor.h"

#define DEFAULT_VIEWFINDER_FRAME_RATE 60

class Camera : public Sensor
{
	Q_OBJECT

private:
    cv::VideoCapture camera;
    cv::VideoWriter videoWriter;
    cv::Mat frame;
    QTimer* _frameTimer;
    int _viewfinderFrameRate = DEFAULT_VIEWFINDER_FRAME_RATE;
    int _videoDeviceIndex = 0;

public:
	Camera(QObject *parent);
	~Camera();

    bool isOpened();

    QTimer* frameTimer();

    int brightness();
    int contrast();
    int saturation();
    int gain();
    bool backlight();
    bool autoExposure();
    int fps();
	int frameWidth();
	int frameHeight();
	int hue();
    int exposure();
	int sharpness();
	int gamma();
	int bitrate();

    int viewfinderFrameRate();
    
    Camera& operator >> (cv::Mat& image);

    double calculateFrameRate();

	QVariant read() override;

	static bool checkCameraAvailability();

public slots:
    void setVideoDevice(int deviceIndex);
    
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;

    void captureFrame();

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
