#pragma once

#include <QObject>
#include <pipeline/branches/recorderbranch.hpp>
#include <pipeline/sinks/recording_defaults.hpp>

class ArduinoPanTiltMountRecorderBin : public RecorderBranch {
protected:
	virtual bool buildBodyBin() override;

public:
	ArduinoPanTiltMountRecorderBin(Element* element);
	virtual ~ArduinoPanTiltMountRecorderBin() = default;

	bool finalizeRecording() override {
		// Send EOS to the muxer to finalize the file
		if (!m_filesinkElement) {
			return false;
		}
		
		return gst_element_send_event(m_filesinkElement, gst_event_new_eos());
		
	}
};