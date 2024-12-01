#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include "sensor.h"
#include <QThread>
#include <QVideoWidget>

#define DEFAULT_FRAME_RATE 30

class Camera : public Sensor
{
	Q_OBJECT

private:
    QCamera mCamera;
    QMediaCaptureSession mSession;
    QVideoSink mSink;
    QVideoWidget *pOutput;
    QCameraDevice *pDevice;

public:
	Camera(QObject *parent = nullptr);
	~Camera();

	QVariant read() override;
	static bool checkCameraAvailability();

    QVideoWidget* output();
    QCameraDevice* device();

public slots:
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;

    
    void setOutput(QVideoWidget* widget);
    void setDevice(QCameraDevice device);

    void setActive(bool active);
    void setAutoExposureTime(bool enable);
    void setAutoIsoSensitivity(bool enable);
    void setColorTemperature(int temp);
    void setExposureCompensation(float ev);
    void setExposureMode(QCamera::ExposureMode mode);
    void setFlashMode(QCamera::FlashMode mode);
    void setManualExposureTime(float seconds);
    void setManualIsoSensitivity(int iso);
    void setTorchMode(QCamera::TorchMode mode);
    void setWhiteBalanceMode(QCamera::WhiteBalanceMode mode);
    void zoomTo(float factor, float rate);

signals:
    void deviceChanged();
    void deviceError();
};
