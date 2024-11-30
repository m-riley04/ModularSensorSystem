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
    QCamera m_camera;
    QMediaCaptureSession m_session;
    QVideoSink m_sink;
    int m_fps = DEFAULT_FRAME_RATE;
    int m_videoDeviceIndex = 0;

public:
	Camera(QObject *parent = nullptr);
	~Camera();

	QVariant read() override;
	static bool checkCameraAvailability();

public slots:
    void setVideoDevice(int deviceIndex);
    void setVideoDevice(QCameraDevice device);
    
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;

    void setVideoWidget(QVideoWidget* widget);

signals:
    void deviceChanged();
    void deviceError();
};
