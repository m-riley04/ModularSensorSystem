#pragma once

#include <QObject>
#include <qjsonarray.h>
#include <QDir>
#include <qdiriterator.h>
#include <QJsonDocument>
#include "features/sources/source.hpp"
#include "controllers/sourcecontroller.hpp"
#include "presets/preset.hpp"
#include "features/sources/iconfigurablesource.hpp"
#include "controllers/backendcontrollerbase.hpp"

class PresetsController : public BackendControllerBase
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
	void presetsScanned(const QList<Preset>& presets);
};
