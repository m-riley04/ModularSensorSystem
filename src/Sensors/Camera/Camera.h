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

/// <summary>
/// Represents a type of Sensor specifically targeting audio/video. Encapsulates a QCamera, QMediaCaptureSession, QVideoSink, QMediaRecorder, and QAudioInput.
/// </summary>
class Camera : public Sensor
{
	Q_OBJECT

private:
    QMediaCaptureSession mSession;
    QCamera mCamera;
    QVideoSink mSink;
    QMediaRecorder mRecorder;
    QAudioInput mAudioInput;

public:
	Camera(QObject *parent = nullptr);
	~Camera();

	QVariant read() override;
	static bool checkCameraAvailability();

    QMediaCaptureSession* session() { return &mSession; }
	QCamera* camera() { return &mCamera; }
	QVideoSink* sink() { return &mSink; }
	QMediaRecorder* recorder() { return &mRecorder; }
    QAudioInput* audioInput() { return &mAudioInput; }

public slots:
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;
    
    void setOutput(QVideoWidget* widget);
    void setDevice(QCameraDevice device);
	void setMediaDirectory(QUrl directory);

signals:
    void cameraDeviceChanged(QCameraDevice device);
    void audioDeviceChanged(QAudioDevice audioDevice);
	void mediaDirectoryChanged(QUrl directory);
    void deviceError();
};
