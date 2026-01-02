#pragma once

#include <QObject>
#include <pipeline/branches/recorderbranch.hpp>
#include <gst/video/videooverlay.h>
#include <pipeline/sinks/recording_defaults.hpp>

class USBVideoSourceRecorderBin : public RecorderBranch {
private:
	GstElement* m_parse = nullptr;
	GstElement* m_encoder = nullptr;
	GstElement* m_muxer = nullptr;

protected:
	bool buildBodyBin() override;

public:
	USBVideoSourceRecorderBin(Element* element);

	virtual ~USBVideoSourceRecorderBin() = default;

	bool finalizeRecording() override {
		// Send EOS to the muxer to finalize the file
		if (!m_encoder) {
			LoggingController::critical("Cannot finalize recording: encoder element is null");
			return false;
		}
		
		return gst_element_send_event(m_encoder, gst_event_new_eos());
	}
};