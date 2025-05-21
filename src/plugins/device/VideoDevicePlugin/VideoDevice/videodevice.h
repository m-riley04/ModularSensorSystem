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
#include "devices/IConfigurableDevice/iconfigurabledevice.h"
#include "VideoDevicePropertiesWidget/videodevicepropertieswidget.h"

class VideoDevice : public Device, public IConfigurableDevice
{
	Q_OBJECT
	Q_INTERFACES(IConfigurableDevice)

public:
    VideoDevice(QCameraDevice qVideoDevice, QObject *parent);
	~VideoDevice();

	static bool checkCameraAvailability();

    QMediaCaptureSession* session() { return &mSession; }
	QCamera* camera() { return &mCamera; }
	QMediaRecorder* recorder() { return &mRecorder; }

	// IConfigurableDevice interface
	QWidget* createConfigWidget(QWidget* parent = nullptr) override;
	void loadSettings(const QJsonObject& obj) override;
	void saveSettings(QJsonObject& obj) const override;

private:
	QMediaCaptureSession mSession;
	QCamera mCamera;
	QMediaRecorder mRecorder;

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
