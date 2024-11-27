#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>
#include "sensor.h"
#include <QThread>

#define DEFAULT_FRAME_RATE 30

class Camera : public Sensor
{
	Q_OBJECT

private:
    cv::VideoCapture camera;
    QTimer* frameTimer;
    int _fps = DEFAULT_FRAME_RATE;
    int _videoDeviceIndex = 0;

public:
	Camera(QObject *parent=nullptr);
	~Camera();

    bool isOpened();

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
};
