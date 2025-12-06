#pragma once

#include <QObject>
#include <pipeline/bins/recorderbin.hpp>
#include <gst/video/videooverlay.h>
#include <pipeline/sinks/recording_defaults.hpp>

class USBVideoSourceRecorderBin : public RecorderBin {
public:
	USBVideoSourceRecorderBin(const boost::uuids::uuid& uuid, const std::string& id);

	virtual ~USBVideoSourceRecorderBin() = default;

	bool setRecordingEnabled(bool enabled) {
		if (!m_valveElement) {
			return false;
		}
		
		g_object_set(m_valveElement, "drop", !enabled, nullptr);
		return true;
	}

	bool setRecordingFilePath(const std::string& filePath) {
		if (!m_filesinkElement) {
			return false;
		}

		if (filePath.empty()) {
			LoggingController::warning("Empty file path provided to setRecordingFilePath");
			return false;
		}

		// Debug: log and assert path length
		LoggingController::debug("Setting recording file path to: " + QString::fromUtf8(filePath.c_str()));
		LoggingController::debug("RecorderBin: path string =" + QString::fromUtf8(filePath.c_str()));

		// Ensure non-empty sanitized path gets applied before any state changes
		g_object_set(m_filesinkElement, "location", filePath.c_str(), nullptr);
		return true;
	}

	bool finalizeRecording() override {
		// Send EOS to the muxer to finalize the file
		if (!m_encoder) {
			return false;
		}
		
		return gst_element_send_event(m_encoder, gst_event_new_eos());
		
	}

protected:
	virtual bool build() override;

private:
	GstElement* m_parse = nullptr;
	GstElement* m_muxer = nullptr;
};