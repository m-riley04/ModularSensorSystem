#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QThread>
#include <QVideoWidget>
#include <QtConcurrent>
#include <chrono>
#include "features/sources/source.h"
#include "features/sources/iconfigurablesource.h"
#include "interfaces/capability/ivideosource.h"
#include "USBVideoPreview/usbvideopreview.h"
#include "USBVideoPropertiesWidget/usbvideopropertieswidget.h"
#include "USBVideoRecordingPropertiesWidget/usbvideorecordingpropertieswidget.h"

class USBVideoSource : public Source, 
	public IConfigurableSource, 
	public IVideoSource
{
	Q_OBJECT
	//Q_INTERFACES(IConfigurableSource IClippableSource IVideoSource)

public:
	USBVideoSource(QByteArray hardwareId, QObject* parent);
    USBVideoSource(QCameraDevice qVideoDevice, QObject *parent);
	~USBVideoSource();

	static QCameraDevice getCameraDevice(const QByteArray& id);

	QCamera* camera() { return &mCamera; }

	// IConfigurableDevice interface
	QWidget* createConfigWidget(QWidget* parent = nullptr) override;
	void loadSettings(const QJsonObject& obj) override;
	QJsonObject saveSettings() override;

	// IVideoSource interface
	QObject* asQObject() override { return this; }

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
    
	void setMediaDirectory(QUrl directory);

private slots:
	void onNewFrame(const QVideoFrame& frame);

signals:
	void mediaDirectoryChanged(QUrl directory);

	// IVideoSource interface
	void frameReady(const QVideoFrame&);
};
