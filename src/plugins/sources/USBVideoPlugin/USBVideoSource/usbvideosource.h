#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QThread>
#include <QVideoWidget>
#include <QtConcurrent>
#include "core/features/sources/Source/source.h"
#include "core/features/sources/IConfigurableSource/iconfigurablesource.h"
#include "core/features/sources/Source/SourcePropertiesWidget/sourcepropertieswidget.h"
#include "core/features/sources/IClippableSource/iclippablesource.h"
#include "core/interfaces/capability/ivideosource.h"
#include "USBVideoPropertiesWidget/usbvideopropertieswidget.h"
#include <USBVideoClipEncoder/usbvideoclipencoder.h>

class USBVideoSource : public Source, 
	public IConfigurableSource, 
	public IClippableSource, 
	public IVideoSource
{
	Q_OBJECT
	Q_INTERFACES(IConfigurableSource IClippableSource IVideoSource)

public:
	USBVideoSource(QByteArray hardwareId, QObject* parent);
    USBVideoSource(QCameraDevice qVideoDevice, QObject *parent);
	~USBVideoSource();

	static QCameraDevice getCameraDevice(const QByteArray& id);

    QMediaCaptureSession* session() { return &mSession; }
	QCamera* camera() { return &mCamera; }
	QMediaRecorder* recorder() { return &mRecorder; }

	// IConfigurableDevice interface
	QWidget* createConfigWidget(QWidget* parent = nullptr) override;
	void loadSettings(const QJsonObject& obj) override;
	QJsonObject saveSettings() override;

	// IClippableSource interface
	void clip(const QDir& dir) override;
	ClipBufferBase* clipBuffer() override { return pClipBuffer.get(); }

	// IVideoSource interface
    QObject* asQObject()  { return this; }

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

	// IVideoSource interface
	void frameReady(const QVideoFrame&);
};
