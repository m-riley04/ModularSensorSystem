#pragma once

#include <QObject>
#include "sdk/plugins/iplugin.hpp"
#include "features/sources/source.hpp"

struct SourceInfo {
	ElementInfo elementInfo;
	Source::Type type;
};

class ISourcePlugin : public IPlugin
{

public:
	/// Enumerate hardware this plugin can drive
	virtual const std::vector<SourceInfo> discover() const = 0;
	virtual Source::Type sourceType() const = 0;
	virtual Source* createSource(const std::string& id, QObject* parent) = 0;
	virtual const PluginType type() const override final { return PluginType::SOURCE; }
};
