#pragma once

#include <pipeline/branches/teebranch.hpp>
#include <features/sources/source.hpp>
#include <controllers/loggingcontroller.hpp>
#include <core_export.hpp>

class MSS_CORE_API RecorderBranch : public TeeBranch {
protected:
	virtual bool buildBodyBin() = 0;
	GstElement* m_filesinkElement = nullptr;

public:
	RecorderBranch(Element* element, Source::Type sourceType);
	virtual ~RecorderBranch();

	/**
	 * @brief Enables or disables recording by controlling the valve state.
	 * @param enabled True to enable recording, false to disable it.
	 * @return True if the valve state was successfully set, false otherwise.
	 */
	virtual bool setRecordingEnabled(bool enabled) {
		return m_prefix.setValveClosed(!enabled);
	}

	/**
	 * @brief Sets the file path where the recording will be saved.
	 * @param filePath The file system path where the recording output will be written.
	 * @return Returns true if the file path was successfully set; false if the filesink element is null or the path is empty.
	 */
	virtual bool setRecordingFilePath(const std::string& filePath) {
		if (!m_filesinkElement) {
			LoggingController::critical("Cannot set recording path: filesink is null");
			return false;
		}

		if (filePath.empty()) {
			LoggingController::critical("Cannot set recording path: empty file path provided");
			return false;
		}

		LoggingController::debug("Setting recording file path to: " + QString::fromUtf8(filePath.c_str()));

		// Ensure non-empty sanitized path gets applied before any state changes
		g_object_set(m_filesinkElement, "location", filePath.c_str(), nullptr);
		return true;
	}

	/**
	 * @brief Finalizes recording by sending EOS to the appropriate element(s).
	 * @return True if the EOS event was successfully sent; false otherwise.
	 */
	virtual bool finalizeRecording() = 0;
};