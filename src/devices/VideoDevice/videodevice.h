#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QThread>
#include <QVideoWidget>
#include <QtConcurrent>
#include <chrono>
#include "devices/Device/device.h"
#include "Widgets/CustomSinkWidget/customsinkwidget.h"
#include "Clipping/Buffers/VideoClipBuffer/videoclipbuffer.h"

#define DEFAULT_FRAME_RATE 30

class VideoDevice : public Device
{
	Q_OBJECT

private:
    QMediaCaptureSession mSession;
    QCamera mCamera;
    QMediaRecorder mRecorder;
    QAudioInput mAudioInput;
    CustomSinkWidget* pSinkWidget = nullptr;
	std::unique_ptr<VideoClipBuffer> pVideoBuffer = nullptr;

public:
    VideoDevice(RecordingSession* recordingSession, QObject* parent);
    VideoDevice(QCameraDevice qCameraDevice, RecordingSession* recordingSession, QObject *parent);
	~VideoDevice();

	static bool checkCameraAvailability();

    CustomSinkWidget* sinkView() const { return pSinkWidget; }
    QMediaCaptureSession* session() { return &mSession; }
	QCamera* camera() { return &mCamera; }
	QMediaRecorder* recorder() { return &mRecorder; }
    QAudioInput* audioInput() { return &mAudioInput; }
	VideoClipBuffer* videoBuffer() { return pVideoBuffer.get(); }

public slots:
    void open() override;
    void start() override;
    void stop() override;
	void close() override;
    void restart() override;
    
    void setVideoOutput(CustomSinkWidget* widget);
    void setDevice(QCameraDevice device);
	void setMediaDirectory(QUrl directory);

private slots:
	void onNewFrame(const QVideoFrame& frame);

signals:
	void videoOutputChanged(CustomSinkWidget* widget);
    void cameraDeviceChanged(QCameraDevice device);
    void audioDeviceChanged(QAudioDevice audioDevice);
	void mediaDirectoryChanged(QUrl directory);
};
