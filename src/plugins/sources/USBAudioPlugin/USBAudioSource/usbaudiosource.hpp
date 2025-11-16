#pragma once

#include <QObject>
#include <chrono>
#include "features/sources/source.hpp"
#include "interfaces/capability/ipreviewablesource.hpp"
#include <sdk/plugins/isourceplugin.hpp>
#include <utils/boost_qt_conversions.hpp>
#include "utils.hpp"
#include "usbaudiosourcebin.hpp"

class USBAudioSource : public Source
	, public IPreviewableSource
{
	Q_OBJECT

public:
	USBAudioSource(const std::string& hardwareId, QObject* parent);
	USBAudioSource(SourceInfo sourceInfo, QObject *parent);
	~USBAudioSource();

	SourceInfo getSourceInfo(const std::string& id) const;

	std::string id() const override { return m_id; }
	std::string name() const override { return m_name; }
	void setName(const std::string& newName) override { m_name = newName; }
	std::string pluginId() const override { return m_pluginId; }
	Source::Type type() const override { return m_sourceType; }

	GstElement* srcBin() override;

	/// IPreviewableSource interface
	quintptr windowId() const override { return m_windowId; }
	void setWindowId(quintptr newWindowId) override { m_windowId = newWindowId; }
	GstElement* previewSinkBin() override { return nullptr; } // Use default sink

public slots:
	void onSessionStart() override;
	void onSessionStop() override;

private:
	void createBinIfNeeded();

	std::string m_id;
	std::string m_name;
	std::string m_pluginId = "plugin_usb_audio";
	Source::Type m_sourceType = Source::Type::AUDIO;
	quintptr m_windowId = 0;
	std::unique_ptr<USBAudioSourceBin> m_bin;
};
