#pragma once

#include <QObject>
#include <QtMultimedia>
#include <QThread>
#include <QtConcurrent>
#include <chrono>
#include "features/sources/source.hpp"
#include <sdk/plugins/isourceplugin.hpp>
#include "interfaces/capability/general/irecordable.hpp"
#include "interfaces/capability/general/ipipelineelement.hpp"
#include <utils/boost_qt_conversions.hpp>
#include "utils.hpp"
#include "usbvideosourcebin.hpp"
#include "usbvideosourcerecorderbranch.hpp"

class USBVideoSource : public Source
	, public IPreviewable
	, public IRecordable
	, public IPipelineElement
{
	Q_OBJECT

public:
	USBVideoSource(const std::string& hardwareId, QObject* parent);
    USBVideoSource(SourceInfo sourceInfo, QObject *parent);
	~USBVideoSource();

	SourceInfo getSourceInfo(const std::string& id) const;
	Source::Type type() const override { return m_sourceType; }

	/// IPipelineElement interface
	GstElement* gstSrcBin() override;
	GstElement* gstFilterBin() override final { return nullptr; }
	GstElement* gstSinkBin() override final { return nullptr; }

	/// IPreviewableSource interface
	quintptr windowId() const override { return m_windowId; }
	void setWindowId(quintptr newWindowId) override { m_windowId = newWindowId; }
	GstElement* previewSinkBin() override { return nullptr; } // Use default sink
	std::string previewSinkElementName() const override { return "preview_" + boost::uuids::to_string(uuid()); }

	/// IRecordableSource interface
	GstElement* recorderSinkBin() override;
	std::string recorderFileExtension() const override;
	bool setRecordingFilePath(const std::string& filePath) override;
	bool startRecording() override;
	bool stopRecording() override;


public slots:
	void onSessionStart() override;
	void onSessionStop() override;

private:
	void createBinIfNeeded();
	void createRecorderBinIfNeeded();

	Source::Type m_sourceType = Source::Type::VIDEO;
	quintptr m_windowId = 0;
	std::unique_ptr<USBVideoSourceBin> m_bin;
	std::unique_ptr<USBVideoSourceRecorderBranch> m_recorderBin;

	std::string m_recordingFilePath;
};
