#pragma once

#include <QString>
#include "sdk/plugins/ielementplugin.h"
#include "features/sources/source.h"

struct SourceInfo {
	std::string id;
	std::string displayName;
	Source::Type type;
};

class ISourcePlugin : public IElementPlugin
{

public:
	/// Enumerate hardware this plugin can drive
	virtual std::vector<SourceInfo> availableSources() const = 0;
	virtual Source::Type sourceType() const = 0;
	virtual Source* createSource(const std::string& id, QObject* parent) = 0;
};

#define ISourcePlugin_iid "com.modularsensorsystem.ISourcePlugin/1.0"
Q_DECLARE_INTERFACE(ISourcePlugin, ISourcePlugin_iid)