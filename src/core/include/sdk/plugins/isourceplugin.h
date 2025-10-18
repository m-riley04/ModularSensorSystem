#pragma once

#include <QObject>
#include "sdk/plugins/iplugin.h"
#include "features/sources/source.h"

struct SourceInfo {
	std::string id;
	std::string displayName;
	Source::Type type;
};

class ISourcePlugin : public IPlugin
{

public:
	/// Enumerate hardware this plugin can drive
	virtual std::vector<SourceInfo> availableSources() const = 0;
	virtual Source::Type sourceType() const = 0;
	virtual Source* createSource(const std::string& id, QObject* parent) = 0;
	PluginType type() const override { return PluginType::Source; }
};
