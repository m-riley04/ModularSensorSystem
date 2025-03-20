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
#include "Generators/OpenCVGenerator/opencvcamera.h"
#include <Widgets/SinkView/sinkview.h>

#define DEFAULT_FRAME_RATE 30

/// <summary>
/// Represents a type of Sensor specifically targeting audio/video
/// </summary>
class Camera : public Sensor
{
	Q_OBJECT

private:
    QMediaCaptureSession mSession;
    QCamera mCamera;
    QMediaRecorder mRecorder;
    QAudioInput mAudioInput;
    SinkView* pSinkView = nullptr;

public:
	Camera(QObject *parent = nullptr);
	~Camera();

	QVariant read() override;
	static bool checkCameraAvailability();

	SinkView* sinkView() const { return pSinkView; }
    QMediaCaptureSession* session() { return &mSession; }
	QCamera* camera() { return &mCamera; }
	QMediaRecorder* recorder() { return &mRecorder; }
    QAudioInput* audioInput() { return &mAudioInput; }

public slots:
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;
    
    void setVideoOutput(SinkView* widget);
    void setDevice(QCameraDevice device);
	void setMediaDirectory(QUrl directory);

signals:
	void videoOutputChanged(SinkView* widget);
    void cameraDeviceChanged(QCameraDevice device);
    void audioDeviceChanged(QAudioDevice audioDevice);
	void mediaDirectoryChanged(QUrl directory);
    void deviceError();
};
