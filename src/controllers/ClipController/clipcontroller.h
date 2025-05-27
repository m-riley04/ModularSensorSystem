#pragma once

#include <QObject>
#include "devices/IClippableDevice/ClipBufferBase/clipbufferbase.h"
#include <QDebug>
#include <devices/Device/DevicePropertiesWidget/devicepropertieswidget.h>
#include <devices/IClippableDevice/IClippableDevice.h>
#include <devices/Device/DeviceError/deviceerror.h>

class ClipController : public QObject
{
	Q_OBJECT

public:
	enum State {
		IDLE,
		CLIPPING
	};

public:
	ClipController(QString clippingDir, QObject *parent = nullptr);
	~ClipController();

	State state() const { return mState; }

public slots:
	/// <summary>
	/// Clips MULTIPLE devices and saves the clips to the specified directory.
	/// </summary>
	/// <param name="devices"></param>
	/// <param name="dirPath"></param>
	void clip(QList<Device*> devices, QString dirPath = QString());

	/// <summary>
	/// Clips SINGLE device and saves the clip to the specified directory.
	/// </summary>
	/// <param name="device"></param>
	/// <param name="dirPath"></param>
	void clip(Device* device, QString dirPath = QString());

	/// <summary>
	/// Flushes the clip buffers of ALL passed devices
	/// </summary>
	void flush(QList<Device*> devices);

	/// <summary>
	/// Flushes the clip buffer of the SINGLE device
	/// </summary>
	void flush(Device* device);

	void addClipBuffer(ClipBufferBase* buffer);
	void removeClipBuffer(ClipBufferBase* buffer);

private:
	State mState = State::IDLE;
	QString mClippingDir;
	QList<ClipBufferBase*> mClipBuffers;

signals:
	void clipBufferAdded(ClipBufferBase* buffer);
	void clipBufferRemoved(ClipBufferBase* buffer);
	void flushed(ClipBufferBase* buffer);
	void clipSaved(const QString& fileName);

	void errorOccurred(const DeviceError& e);
};
