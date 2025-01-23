#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include "../sensor.h"
#include <QThread>
#include <QVideoWidget>
#include <QtConcurrent>
#include <chrono>

#define DEFAULT_FRAME_RATE 30

class Camera : public Sensor
{
	Q_OBJECT

private:
    QCamera mCamera;
    QMediaCaptureSession mSession;
    QVideoSink mSink;
    QMediaRecorder mRecorder;

public:
	Camera(QObject *parent = nullptr);
	~Camera();

	QVariant read() override;
	static bool checkCameraAvailability();

	QCamera& camera() { return mCamera; }
	QMediaCaptureSession& session() { return mSession; }
	QVideoSink& sink() { return mSink; }
	QMediaRecorder& recorder() { return mRecorder; }

public slots:
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;
    
    void setOutput(QVideoWidget* widget);
    void setDevice(QCameraDevice device);
	void setMediaDirectory(QUrl directory);

signals:
    void deviceChanged(QCameraDevice device);
	void mediaDirectoryChanged(QUrl directory);
    void deviceError();
};
