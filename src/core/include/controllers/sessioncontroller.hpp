#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QCoreApplication>

#include "controllers/elementscontroller.hpp"
#include "pipeline/sinks/preview_defaults.hpp"
#include <interfaces/capability/general/ipreviewable.hpp>
#include "models/settings_models.hpp"
#include <utils/session_utils.hpp>
#include "pipeline/sessionpipeline.hpp"
#include "settingscontroller.hpp"
#include "core_export.hpp"

class MSS_CORE_API SessionController : public QObject
{
	Q_OBJECT

public:
	SessionController(SettingsController& settingsController, ElementsController& ec, QObject* parent);
	~SessionController();

	const SessionPipeline& pipeline() { return m_pipeline; }

public slots:
	void startSession();
	void stopSession();
	void restartSession();

	void startRecording();
	void stopRecording();

	void startProcessing();
	void stopProcessing();

	void clearRecordings();

private:
	ElementsController& m_elementsController;
	SettingsController& m_settingsController;
	SessionPipeline m_pipeline;
	ns m_lastSessionTimestamp = 0;

signals:
	void sessionStarted();
	void sessionStopped();
	void sessionRestarted();

	void recordingStarted();
	void recordingStopped();

	void errorOccurred(QString errorMessage); // TODO: should I use a const ref?

};