#pragma once

#include <QObject>
#include "sources/Source/source.h"
#include <controllers/SourceController/sourcecontroller.h>
#include <presets/Preset/preset.h>
#include <sources/IConfigurableSource/iconfigurablesource.h>
#include <qjsonarray.h>
#include <QDir>
#include <qdiriterator.h>

class PresetsController : public QObject
{
	Q_OBJECT

public:
	PresetsController(const QString& dir, QObject *parent);
	~PresetsController();

	QList<Preset> presets() const { return mPresets; }
	QString presetsDir() const { return mPresetsDir; }

	void savePreset(QString name, QList<Source*> activeSources = QList<Source*>(), QString dirPath = QString());
	void loadPreset(const QString& path, SourceController* sourceController, PluginController* pluginController);
	void removePreset(QString filePath);

	void scanForPresets(QString presetDir = QString());

private:
	QString mPresetsDir;
	QList<Preset> mPresets;

	QJsonArray sourcePresetsToJson(const QList<SourcePreset>& sourcePresets);
	QList<SourcePreset> jsonToSourcePresets(const QJsonArray& jsonArray);

signals:
	void presetSaved(const QString& path);
	void presetLoaded(const QString& path);
	void presetRemoved(const QString& path);

};
