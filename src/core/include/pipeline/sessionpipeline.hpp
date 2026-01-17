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
#include <controllers/elementscontroller.hpp>
#include <pipeline/branches/previewbranch.hpp>
#include <pipeline/branches/recorderbranch.hpp>
#include <pipeline/branches/processingbranch.hpp>
#include <pipeline/branches/intermediaries/previewcompositor.hpp>
#include <vector>
#include <map>

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
	explicit SessionPipeline(SessionSettings& settings, ElementsController& ec, QObject* parent = nullptr);
	virtual ~SessionPipeline() = default;

	const GstElement* bin() const { return GST_ELEMENT(m_pipeline.get()); }

	State state() const { return m_state; }
	bool isStarted() const { return m_state == State::STARTED; }
	bool isStopped() const { return m_state == State::STOPPED; }
	bool isRecording() const { return m_state == State::RECORDING; }
	bool isBuilt() const { return m_pipeline != nullptr; }
	bool isProcessingEnabled() const { return m_isProcessingEnabled; }

	void setSessionTimestamp(ns timestamp) { m_lastSessionTimestamp = timestamp; }

public slots:
	void setState(State newState);
	void startRecording();
	void stopRecording();
	void startProcessing();
	void stopProcessing();
	bool build(const QList<Element*>&, const QList<IRecordable*>&);
	bool close();

	void onPipelineError(const QString& errorMessage);
	void onPipelineEos();

private:
	bool start();
	bool stop();
	bool cleanup();

	bool createSourceElements(Element*);
	bool createSourceBranches(Element*, GstElement*);
	bool createPreviewBranch(Element*, GstElement*, ProcessingBranch* processorBranch = nullptr);
	bool createRecorderBranch(Element*, GstElement*);
	ProcessingBranch* createProcessorBranch(Element* sourceElement, Processor* processor, GstElement* tee);
	bool createAndLinkPreviewBin(Element*, GstElement*);
	bool createAndLinkRecordBin(Element*, GstElement*);

	bool openRecordingValves(QList<IRecordable*>&);
	bool closeRecordingValves(QList<IRecordable*>&);
	bool openRecordingValveForElement(IRecordable*);
	bool closeRecordingValveForElement(IRecordable*);

	bool openProcessingValveForElement(Processor*);
	bool closeProcessingValveForElement(Processor*);

	std::unique_ptr<GstPipeline, decltype(&gst_object_unref)> m_pipeline;
	State m_state = State::STOPPED;
	bool m_isProcessingEnabled = true;
	ns m_lastSessionTimestamp = 0;
	ns m_lastRecordingTimestamp = 0;
	guint m_pipelineBusWatchId = 0;

	// Borrowed ptrs
	QList<GstElement*> m_sourceBins;
	QList<PreviewBranch*> m_previewBranches;
	QList<RecorderBranch*> m_recordBranches;
	QList<ProcessingBranch*> m_processingBranches;

	// Owned ptrs
	std::map<QUuid, std::unique_ptr<PreviewCompositor>> m_processorCompositors;

	ElementsController& m_elementsController;
	SessionSettings& m_sessionSettings;

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