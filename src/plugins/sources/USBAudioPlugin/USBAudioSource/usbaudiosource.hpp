#pragma once

#include <QObject>
#include <chrono>
#include "features/sources/source.hpp"
#include "interfaces/capability/general/ipreviewable.hpp"
#include "interfaces/capability/general/ipipelineelement.hpp"
#include <sdk/plugins/isourceplugin.hpp>
#include <utils/boost_qt_conversions.hpp>
#include "utils.hpp"
#include "usbaudiosourcebin.hpp"
#include "usbaudiosourcepreviewbranch.hpp"

class USBAudioSource : public Source
	, public IPreviewable
	, public IPipelineElement
{
	Q_OBJECT

public:
	USBAudioSource(const std::string& hardwareId, QObject* parent);
	USBAudioSource(SourceInfo sourceInfo, QObject *parent);
	~USBAudioSource();

	SourceInfo getSourceInfo(const std::string& id) const;
	Source::Type type() const override { return m_sourceType; }

	/// IPipelineElement interface
	GstElement* gstSrcBin() override;
	GstElement* gstFilterBin() override final { return nullptr; }
	GstElement* gstSinkBin() override final { return nullptr; }

	/// IPreviewableSource interface
	quintptr windowId() const override { return m_windowId; }
	void setWindowId(quintptr newWindowId) override { m_windowId = newWindowId; }
	PreviewBranch* previewBranch() override;
	GstElement* previewSinkBin() override;
	std::string previewSinkElementName() const override { return "preview_" + boost::uuids::to_string(uuid()); }

public slots:
	void onSessionStart() override;
	void onSessionStop() override;

private:
	void createBinIfNeeded();
	void createPreviewBranchIfNeeded();
	Source::Type m_sourceType = Source::Type::AUDIO;
	quintptr m_windowId = 0;
	std::unique_ptr<USBAudioSourceBin> m_bin;
	std::unique_ptr<USBAudioSourcePreviewBranch> m_previewBranch;
};
