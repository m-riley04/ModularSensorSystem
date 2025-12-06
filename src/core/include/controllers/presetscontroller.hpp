#pragma once

#include <QObject>
#include <qjsonarray.h>
#include <QDir>
#include <qdiriterator.h>
#include <QJsonDocument>
#include "features/sources/source.hpp"
#include "controllers/sourcecontroller.hpp"
#include "controllers/plugincontroller.hpp"
#include "presets/preset.hpp"
#include "interfaces/capability/iconfigurablesource.hpp"

#include "controllers/settingscontroller.hpp"

class PresetsController : public QObject
{
	Q_OBJECT

public:
	PresetsController(SettingsController& sc, QObject *parent);
	~PresetsController();

	QList<Preset> presets() const { return mPresets; }

	void savePreset(const QString& name, const QList<Source*>& activeSources = QList<Source*>(), const QString& dirPath = QString());
	void loadPreset(const QString& path, SourceController& sourceController, PluginController& pluginController);
	void removePreset(QString filePath);

	void scanForPresets(QString presetDir = QString());

private:
	SettingsController& m_settingsController;
	QDir& m_presetDirectory;
	QList<Preset> mPresets;

	QJsonArray sourcePresetsToJson(const QList<SourcePreset>& sourcePresets);
	QList<SourcePreset> jsonToSourcePresets(const QJsonArray& jsonArray);

signals:
	void presetSaved(const QString& path);
	void presetLoaded(const QString& path);
	void presetRemoved(const QString& path);
	void presetsScanned(const QList<Preset>& presets);
};
