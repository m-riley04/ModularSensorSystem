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
#include "VideoClipBuffer/videoclipbuffer.h"
#include "VideoPreview/videopreview.h"

#define DEFAULT_FRAME_RATE 30

class VideoDevice : public Device
{
	Q_OBJECT

private:
    QMediaCaptureSession mSession;
    QCamera mCamera;
    QMediaRecorder mRecorder;
	std::unique_ptr<VideoClipBuffer> pVideoBuffer = nullptr;

public:
    VideoDevice(QCameraDevice qVideoDevice, QObject *parent);
	~VideoDevice();

	static bool checkCameraAvailability();

    QMediaCaptureSession* session() { return &mSession; }
	QCamera* camera() { return &mCamera; }
	QMediaRecorder* recorder() { return &mRecorder; }
	VideoClipBuffer* videoBuffer() { return pVideoBuffer.get(); }

public slots:
    void open() override;
    void start() override;
    void stop() override;
	void close() override;
    void restart() override;

	void beginRecording(RecordingSession*) override;
	void endRecording() override;
    
	void setMediaDirectory(QUrl directory);

private slots:
	void onNewFrame(const QVideoFrame& frame);

signals:
	void mediaDirectoryChanged(QUrl directory);
};
