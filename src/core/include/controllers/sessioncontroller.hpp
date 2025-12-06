#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QCoreApplication>

#include "controllers/sourcecontroller.hpp"
#include "controllers/processingcontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "features/sources/source.hpp"
#include "interfaces/capability/ianalogsource.hpp"
#include "pipeline/sinks/preview_defaults.hpp"
#include <interfaces/capability/ipreviewablesource.hpp>
#include "models/settings_models.hpp"
#include <utils/session_utils.hpp>
#include "pipeline/sessionpipeline.hpp"
#include "settingscontroller.hpp"

using OneToManyIdMap = QHash<QUuid, std::vector<QUuid>>;

class SessionController : public QObject
{
	Q_OBJECT

public:
	SessionController(SettingsController& settingsController, SourceController& sourceController, ProcessingController& processingController, 
		MountController& mountController, QObject* parent);
	~SessionController();

	const SessionPipeline& pipeline() { return m_pipeline; }
	const QList<const Source*> getSourcesByMount(QUuid mountId) const;
	const QList<const Processor*> getProcessorsBySource(QUuid sourceId) const;

public slots:
	void startSession();
	void stopSession();
	void restartSession();

	void startRecording();
	void stopRecording();

	void clearRecordings();

private:
	SessionPipeline m_pipeline;
	ns m_lastSessionTimestamp = 0;

	SourceController& m_sourceController;
	ProcessingController& m_processingController;
	MountController& m_mountController;
	SettingsController& m_settingsController;

	OneToManyIdMap m_mountToSources;
	OneToManyIdMap m_sourceToProcessors;

signals:
	void sessionStarted();
	void sessionStopped();
	void sessionRestarted();

	void recordingStarted();
	void recordingStopped();

	void errorOccurred(QString errorMessage); // TODO: should I use a const ref?

};