#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include "sensor.h"
#include <QThread>

#define DEFAULT_FRAME_RATE 30

class Camera : public Sensor
{
	Q_OBJECT

private:
    QCamera camera;
    int _fps = DEFAULT_FRAME_RATE;
    int _videoDeviceIndex = 0;

public:
	Camera(QObject *parent=nullptr);
	~Camera();

	QVariant read() override;

	static bool checkCameraAvailability();

public slots:
    void setVideoDevice(int deviceIndex);
    
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;

    void captureFrame();

signals:
    void deviceChanged();
    void deviceError();
};
