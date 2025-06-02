#pragma once

#include <QObject>
#include "sources/Source/source.h"

struct SourceInfo {
	QByteArray id;
	QString displayName;
	Source::Type type;
};

class ISourcePlugin
{

public:
	virtual ~ISourcePlugin() = default;

	/// Enumerate hardware this plugin can drive
	virtual QList<SourceInfo> availableSources() const = 0;
	virtual QString pluginName() const = 0;
	virtual Source::Type sourceType() const = 0;
	virtual Source* createSource(const QByteArray& id, QObject* parent) = 0;
};

#define ISourcePlugin_iid "com.modularsensorsystem.ISourcePlugin/1.0"
Q_DECLARE_INTERFACE(ISourcePlugin, ISourcePlugin_iid)