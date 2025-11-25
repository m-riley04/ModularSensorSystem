#pragma once

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "controllers/backendcontrollerbase.hpp"
#include "controllers/sourcecontroller.hpp"
#include "controllers/processingcontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "features/sources/source.hpp"
#include "interfaces/capability/ianalogsource.hpp"
#include "pipeline/sinks/preview_defaults.hpp"
#include <interfaces/capability/ipreviewablesource.hpp>
#include "models/session_properties.hpp"
#include <utils/session_utils.hpp>
#include <QThread>
#include "pipeline/sessionpipeline.hpp"

#define DEFAULT_SESSION_PREFIX "session_"
#define DEFAULT_SESSIONS_DIRECTORY "/sessions"

using OneToManyIdMap = QHash<QUuid, std::vector<QUuid>>;

class SessionController : public BackendControllerBase
{
	Q_OBJECT

public:
	SessionController(SourceController* sourceController, ProcessingController* processingController, 
		MountController* mountController, QObject* parent);
	~SessionController();

	SessionPipeline* pipeline() const { return m_pipeline.get(); }
	SessionProperties& sessionProperties() const { return *m_sessionProperties; }
	QList<const Source*> getSourcesByMount(QUuid mountId) const;
	QList<const Processor*> getProcessorsBySource(QUuid sourceId) const;

public slots:
	void startSession();
	void stopSession();
	void restartSession();

	void startRecording();
	void stopRecording();
	
	void setSessionProperties(SessionProperties properties);

private:
	std::unique_ptr<SessionPipeline> m_pipeline;
	ns m_lastSessionTimestamp = 0;

	QThread* m_pipelineThread;

	std::unique_ptr<SessionProperties> m_sessionProperties = nullptr;

	SourceController* m_sourceController = nullptr;
	ProcessingController* m_processingController = nullptr;
	MountController* m_mountController = nullptr;

	OneToManyIdMap m_mountToSources;
	OneToManyIdMap m_sourceToProcessors;

signals:
	void sessionStarted();
	void sessionStopped();
	void sessionRestarted();

	void recordingStarted();
	void recordingStopped();

	void sessionPropertiesChanged(SessionProperties properties);
	void errorOccurred(QString errorMessage);

};