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

public slots:
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;

    QVideoWidget* output();
    void setOutput(QVideoWidget* widget);

    QCameraDevice* device();
    void setDevice(QCameraDevice device);

signals:
    void deviceChanged();
    void deviceError();
};
