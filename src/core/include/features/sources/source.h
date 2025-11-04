#pragma once

#include <QObject>
#include <quuid.h>
#include <QPointer>
#include "sourceerror.h"
#include "features/ielement.h"
#include <QUuid>
#include <gst/gst.h>

struct SourceError;

/**
 * Represents a data source (e.g., video, audio, data).
 * TODO: rename this to ISource (since I've been working on making this a more pure interface)
 */
class Source : public IElement
{
	Q_OBJECT

public:
	enum Type {
		VIDEO,
		AUDIO,
		DATA,
		OTHER
	};

	enum State {
		OPENED,
		STARTED,
		STOPPED,
		CLOSED
	};

	enum ErrorState {
		NO_ERROR,
		SOURCE_NOT_FOUND,
		SOURCE_BUSY,
		SOURCE_DISCONNECTED,
		UNKNOWN_ERROR
	};

	static bool isVideo(Source* src) {
		return src->type() == Source::Type::VIDEO;
	}

	static bool isAudio(Source* src) {
		return src->type() == Source::Type::AUDIO;
	}

	static bool isData(Source* src) {
		return src->type() == Source::Type::DATA;
	}

	/**
	 * Converts a State enum value to its corresponding string representation.
	 * @param state The state to convert.
	 * @return The string representation of the state.
	 */
	static std::string stateToString(State state)
	{
		switch (state) {
		case OPENED: return "Opened";
		case STARTED: return "Started";
		case STOPPED: return "Stopped";
		case CLOSED: return "Closed";
		default: return "Unknown";
		}
	}

	/**
	 * Converts a Type enum value to its corresponding string representation.
	 * @param type The type to convert.
	 * @return The string representation of the type.
	 */
	static std::string typeToString(Type type)
	{
		switch (type) {
		case VIDEO: return "Video";
		case AUDIO: return "Audio";
		case DATA: return "Data";
		case OTHER: return "Other";
		default: return "Unknown";
		}
	}

	/**
	 * Converts an ErrorState enum value to its corresponding string representation.
	 * @param errorState The error state to convert.
	 * @return The string representation of the error state.
	 */
	static std::string errorStateToString(ErrorState errorState)
	{
		switch (errorState) {
		case NO_ERROR: return "No Error";
		case SOURCE_NOT_FOUND: return "Device Not Found";
		case SOURCE_BUSY: return "Device Busy";
		case SOURCE_DISCONNECTED: return "Device Disconnected";
		case UNKNOWN_ERROR: return "Unknown Error";
		default: return "Unknown";
		}
	}

public:
	Source(QObject* parent = nullptr) : IElement(parent) {}
	virtual ~Source() = default;

	virtual void open() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void close() = 0;
	virtual void restart() = 0;

	/**
	 * The hardware ID for the device
	 * @return hardware ID string
	 */
	virtual std::string id() const override { return "src_unknown"; }

	/**
	 * The name of the device from the system.
	 * Sometimes this is called "description", other times "device-id", and even "device-path".
	 * @return user-friendly name of the device from the hardware.
	 */
	virtual std::string name() const override { return "New Source"; }
	virtual void setName(const std::string& newName) override = 0;
	virtual std::string pluginId() const override { return "plugin_unknown"; }

	/**
	 * The type of device.
	 * @return the type of a device as Source::Type enum.
	 */
	virtual Source::Type type() const { return Source::Type::OTHER; }

	/**
	 * The current state of the device.
	 * @return the state of the device as Source::State enum.
	 */
	virtual Source::State state() const { return Source::State::CLOSED; }

	/**
	 * @brief The window ID where the source's video output should be rendered.
	 * @return The window ID.
	 */
	virtual quintptr windowId() const { return 0; }
	virtual void setWindowId(quintptr newId) = 0;


	virtual GstElement* gstBin() const = 0;

signals:
	void opened();
	void started();
	void stopped();
	void closed();

	void errorOccurred(const SourceError& e);
};
