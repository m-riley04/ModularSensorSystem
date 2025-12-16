#pragma once

#include <QObject>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/app/gstappsink.h>
#include "features/sources/source.hpp"
#include "interfaces/capability/sources/ianalogsource.hpp"
#include "pipeline/sinks/preview_defaults.hpp"
#include <interfaces/capability/general/ipreviewable.hpp>
#include <utils/debug.hpp>
#include <utils/utils.hpp>
#include <utils/session_utils.hpp>
#include <features/processors/processor.hpp>

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
	bool build(const QList<Element*>&, const QList<IRecordable*>&);
	bool close();

	void onPipelineError(const QString& errorMessage);
	void onPipelineEos();

private:
	bool start();
	bool stop();
	bool cleanup();

	bool createSourceElements(Element*);
	bool createAndLinkPreviewBin(Element*, GstElement*);
	bool createAndLinkRecordBin(Element*, GstElement*);

	
	/**
	 * Inserts the processor bins into the pipeline for the given element.
	 * // TODO/CONSIDER: move this into the main source elements creation somehow?
	 */
	bool insertProcessorBins(Processor*);

	bool openRecordingValves(QList<IRecordable*>&);
	bool closeRecordingValves(QList<IRecordable*>&);
	bool openRecordingValveForElement(IRecordable*);
	bool closeRecordingValveForElement(IRecordable*);

	std::unique_ptr<GstPipeline, decltype(&gst_object_unref)> m_pipeline;
	State m_state = State::STOPPED;
	ns m_lastSessionTimestamp = 0;
	ns m_lastRecordingTimestamp = 0;

	// Volatile refs/ptrs
	QList<GstElement*> m_sourceBins;
	QList<GstElement*> m_previewBins;
	QList<GstElement*> m_recordBins;
	QList<GstElement*> m_processorBins;
	QList<GstElement*> m_recordableElementBins;
	QList<IRecordable*> m_recordableElements;
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