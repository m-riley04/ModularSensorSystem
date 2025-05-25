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
#include "devices/Device/DevicePropertiesWidget/devicepropertieswidget.h"
#include "VideoDeviceRecordingPropertiesWidget/videodevicerecordingpropertieswidget.h"

class VideoDevice : public Device, public IConfigurableDevice
{
	Q_OBJECT
	Q_INTERFACES(IConfigurableDevice)

public:
	VideoDevice(QByteArray hardwareId, QObject* parent);
    VideoDevice(QCameraDevice qVideoDevice, QObject *parent);
	~VideoDevice();

	static QCameraDevice getCameraDevice(const QByteArray& id);
	static bool checkCameraAvailability();

    QMediaCaptureSession* session() { return &mSession; }
	QCamera* camera() { return &mCamera; }
	QMediaRecorder* recorder() { return &mRecorder; }

	// IConfigurableDevice interface
	QWidget* createConfigWidget(QWidget* parent = nullptr) override;
	void loadSettings(const QJsonObject& obj) override;
	QJsonObject saveSettings() override;

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
