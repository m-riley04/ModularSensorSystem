#pragma once

#include <QObject>
#include <features/sources/source.h>
#include "testdatasourcebin.h"
#include <sdk/plugins/isourceplugin.h>
#include "testdatasourcepluginutils.h"

class TestDataSource : public Source
{
	Q_OBJECT

public:
	TestDataSource(const std::string& hardwareId, QObject* parent);
	TestDataSource(SourceInfo sourceInfo, QObject* parent);
	~TestDataSource();

	SourceInfo getSourceInfo(const std::string& id) const;

	std::string id() const override { return m_id; }
	std::string name() const override { return m_name; }
	void setName(const std::string& newName) override { m_name = newName; }
	std::string pluginId() const override { return m_pluginId; }
	Source::Type type() const override { return m_sourceType; }
	quintptr windowId() const override { return m_windowId; }
	void setWindowId(quintptr newWindowId) override { m_windowId = newWindowId; }

	GstElement* gstBin() const override { return m_sourceBin->bin(); }

private:
	std::string m_id;
	std::string m_name;
	std::string m_pluginId = "plugin_test_data";
	Source::Type m_sourceType = Source::Type::DATA;
	quintptr m_windowId = 0;
	std::unique_ptr<TestDataSourceBin> m_sourceBin;
};
