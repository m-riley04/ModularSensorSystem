#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QThread>
#include <QVideoWidget>
#include <QtConcurrent>
#include <chrono>
#include "features/sources/source.h"
#include <sdk/plugins/isourceplugin.h>
#include <utils/boost_qt_conversions.h>
#include "utils.h"
#include "USBVideoSourceBin.h"

class USBVideoSource : public Source
{
	Q_OBJECT

public:
	USBVideoSource(const std::string& hardwareId, QObject* parent);
    USBVideoSource(SourceInfo sourceInfo, QObject *parent);
	~USBVideoSource();

	SourceInfo getSourceInfo(const std::string& id) const;

public slots:
    void open() override;
    void start() override;
    void stop() override;
	void close() override;
    void restart() override;

	std::string id() const override { return m_id; }
	std::string name() const override { return m_name; }
	void setName(const std::string& newName) override { m_name = newName; }
	std::string pluginId() const override { return m_pluginId; }
	Source::Type type() const override { return m_sourceType; }
	Source::State state() const override { return m_state; }
	quintptr windowId() const override { return m_windowId; }
	void setWindowId(quintptr newWindowId) override { m_windowId = newWindowId; }

	GstElement* gstBin() const override { return m_sourceBin->bin(); }


private:
	std::string m_id;
	std::string m_name;
	std::string m_pluginId = "plugin_usb_video";
	Source::Type m_sourceType = Source::Type::VIDEO;
	Source::State m_state = Source::State::CLOSED;
	quintptr m_windowId = 0;
	std::unique_ptr<USBVideoSourceBin> m_sourceBin;
};
