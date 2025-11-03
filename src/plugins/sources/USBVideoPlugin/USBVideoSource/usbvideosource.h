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
#include <sdk/plugins/isourceplugin.h>
#include <utils/boost_qt_conversions.h>
#include <usbvideoplugin.h>

class USBVideoSource : public Source, 
	public IVideoSource
{
	Q_OBJECT

public:
	USBVideoSource(const std::string& hardwareId, QObject* parent);
    USBVideoSource(SourceInfo sourceInfo, QObject *parent);
	~USBVideoSource();

	SourceInfo getSourceInfo(const std::string& id) const;

	// IVideoSource interface
	QObject* asQObject() override { return this; }

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
