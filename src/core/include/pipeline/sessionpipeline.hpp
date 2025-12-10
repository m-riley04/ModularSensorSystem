#pragma once

#include <QObject>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/app/gstappsink.h>
#include "features/sources/source.hpp"
#include "interfaces/capability/sources/ianalogsource.hpp"
#include "pipeline/sinks/preview_defaults.hpp"
#include <interfaces/capability/sources/ipreviewablesource.hpp>
#include <utils/debug.hpp>
#include <utils/utils.hpp>
#include <utils/session_utils.hpp>

constexpr const char* MAIN_PIPELINE_NAME = "main_pipeline";

class SessionPipeline : public QObject
{
	Q_OBJECT
public:
	enum class State {
		STARTED,
		STOPPED,
		RECORDING,
		ERROR
	};

public:
	explicit SessionPipeline(SessionSettings& settings, QObject* parent = nullptr);
	virtual ~SessionPipeline() = default;

	const GstElement* bin() const { return GST_ELEMENT(m_pipeline.get()); }

	State state() const { return m_state; }
	bool isStarted() const { return m_state == State::STARTED; }
	bool isStopped() const { return m_state == State::STOPPED; }
	bool isRecording() const { return m_state == State::RECORDING; }
	bool isBuilt() const { return m_pipeline != nullptr; }

	void setSessionTimestamp(ns timestamp) { m_lastSessionTimestamp = timestamp; }

public slots:
	void setState(State newState);
	void startRecording();
	void stopRecording();
	bool build(const QList<Source*>& sources, const QList<IRecordableSource*>& recSources);
	bool close();

	void onPipelineError(const QString& errorMessage);
	void onPipelineEos();

private:
	bool start();
	bool stop();
	bool cleanup();

	bool createSourceElements(Source* source);
	bool createAndLinkPreviewBin(Source* src, GstElement* srcBin);
	bool createAndLinkRecordBin(Source* src, GstElement* srcBin);

	bool openRecordingValves(QList<IRecordableSource*>&);
	bool closeRecordingValves(QList<IRecordableSource*>&);
	bool openRecordingValveForSource(IRecordableSource* source);
	bool closeRecordingValveForSource(IRecordableSource* source);

	std::unique_ptr<GstPipeline, decltype(&gst_object_unref)> m_pipeline;
	State m_state = State::STOPPED;
	ns m_lastSessionTimestamp = 0;
	ns m_lastRecordingTimestamp = 0;

	// Volatile refs/ptrs
	QList<GstElement*> m_sourceBins;
	QList<GstElement*> m_previewBins;
	QList<GstElement*> m_recordBins;
	QList<GstElement*> m_recordableSourceBins;
	QList<IRecordableSource*> m_recordableSources;
	SessionSettings& m_sessionSettings; // ptr to session settings owned by settings controller
	guint m_pipelineBusWatchId = 0;

signals:
	void started();
	void stopped();
	void recordingStarted();
	void recordingStopped();

	void eosReached();
	void buildCompleted(bool success);
	void stateChanged(State newState);
	void errorOccurred(QString errorMessage);
};