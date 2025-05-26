#pragma once

#include <QObject>
#include "Clipping/Buffers/ClipBufferBase/clipbufferbase.h"
#include <QDebug>
#include <devices/Device/DevicePropertiesWidget/devicepropertieswidget.h>

class ClipController : public QObject
{
	Q_OBJECT

public:
	ClipController(QString clippingDir, QObject *parent = nullptr);
	~ClipController();

	void clip(QList<Device*> devices, QString dirPath = QString());
	void clip(Device* device, QString dirPath = QString());
	void addClipBuffer(ClipBufferBase* buffer);
	//void removeClipBuffer(ClipBufferBase* buffer) { mClipBuffers.append(buffer); }

private:
	QList<ClipBufferBase*> mClipBuffers;

	QString mClippingDir;

signals:
	void clipBufferAdded(ClipBufferBase* buffer);
	void clipBufferRemoved(ClipBufferBase* buffer);
	void buffersCleared();
	void clipSaved(const QString& fileName);
};
