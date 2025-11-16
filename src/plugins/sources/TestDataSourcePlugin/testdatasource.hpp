#pragma once

#include <QObject>
#include <features/sources/source.hpp>
#include "testdatasourcebin.hpp"
#include <sdk/plugins/isourceplugin.hpp>
#include <interfaces/capability/ipreviewablesource.hpp>
#include "testdatasourcepluginutils.hpp"
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>

class TestDataSource : public Source
	, public IPreviewableSource
{
	Q_OBJECT

public:
	struct Config {
		std::string sensorId = "test_sensor_001"; // sensor identifier in the JSON sample
		int minIntervalMs = 200; // minimum interval between samples in milliseconds
		int maxIntervalMs = 1500; // maximum interval between samples in milliseconds
		double minValue = 0.0; // minimum sensor value
		double maxValue = 1.0; // maximum sensor value
	};

	TestDataSource(const std::string& hardwareId, QObject* parent);
	TestDataSource(SourceInfo sourceInfo, QObject* parent);
	~TestDataSource();

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

private slots:
	void onTimerTimeout();

private:
	void scheduleNextTick();
	void createBinIfNeeded();

	std::string m_id;
	std::string m_name;
	std::string m_pluginId = "plugin_test_data";
	Source::Type m_sourceType = Source::Type::DATA;
	quintptr m_windowId = 0;
	std::unique_ptr<TestDataSourceBin> m_bin;

	QTimer m_timer;
	uint64_t m_seq{ 0 };
	Config m_cfg;
};
