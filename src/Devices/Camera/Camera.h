#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QThread>
#include <QVideoWidget>
#include <QtConcurrent>
#include <chrono>
#include "Devices/Device/device.h"
#include "Generators/OpenCVGenerator/opencvcamera.h"
#include "Widgets/CustomSinkWidget/customsinkwidget.h"
#include "VideoBuffer/videobuffer.h"

#define DEFAULT_FRAME_RATE 30

/// <summary>
/// Represents a type of Sensor specifically targeting audio/video
/// </summary>
class Camera : public Device
{
	Q_OBJECT

private:
    QMediaCaptureSession mSession;
    QCamera mCamera;
    QMediaRecorder mRecorder;
    QAudioInput mAudioInput;
    CustomSinkWidget* pSinkWidget = nullptr;
	VideoBuffer* pVideoBuffer = nullptr;

public:
	Camera(QObject *parent = nullptr);
	~Camera();

	QVariant read() override;
	static bool checkCameraAvailability();

    CustomSinkWidget* sinkView() const { return pSinkWidget; }
    QMediaCaptureSession* session() { return &mSession; }
	QCamera* camera() { return &mCamera; }
	QMediaRecorder* recorder() { return &mRecorder; }
    QAudioInput* audioInput() { return &mAudioInput; }
	VideoBuffer* videoBuffer() { return pVideoBuffer; }

public slots:
    void initialize() override;
    void start() override;
    void stop() override;
    void restart() override;
    
    void setVideoOutput(CustomSinkWidget* widget);
    void setDevice(QCameraDevice device);
	void setMediaDirectory(QUrl directory);

signals:
	void videoOutputChanged(CustomSinkWidget* widget);
    void cameraDeviceChanged(QCameraDevice device);
    void audioDeviceChanged(QAudioDevice audioDevice);
	void mediaDirectoryChanged(QUrl directory);
    void deviceError();
};
