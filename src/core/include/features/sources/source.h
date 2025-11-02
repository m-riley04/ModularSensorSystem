#pragma once

#include <QObject>
#include <quuid.h>
#include "sourcepreview.h"
#include <QPointer>
#include "sourceerror.h"
#include "features/ielement.h"
#include <QUuid>

class SourcePreview;
struct SourceError;

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
	static QString stateToString(State state)
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
	static QString typeToString(Type type)
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
	static QString errorStateToString(ErrorState errorState)
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
	Source(QByteArray hardwareId, QObject* parent = nullptr) : IElement(parent) {}; // TOOD: must initialize m_id
	Source(QObject* parent = nullptr) : IElement(parent) {};
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
	std::string id() const override { return m_id.toStdString(); }

	/**
	 * The name of the device from the system.
	 * Sometimes this is called "description".
	 * @return user-friendly name of the device from the hardware.
	 */
	std::string name() const { return m_name.toStdString(); }
	void setName(const std::string& newName) override { m_name = QString::fromStdString(newName); }
	std::string pluginId() const override { return m_pluginId.toStdString(); }

	/**
	 * The type of device.
	 * @return the type of a device as Source::Type enum.
	 */
	Source::Type type() const { return mSourceType; }

	/**
	 * The current state of the device.
	 * @return the state of the device as Source::State enum.
	 */
	Source::State state() const { return mState; }

	/**
	 * SourcePreview object for the device.
	 * Used for previewing the device's output.
	 * @return a weak pointer to the SourcePreview object.
	 */
	SourcePreview* preview() const { return pPreview.get(); }

protected:
	/**
	 * The unique hardware ID if possible.
	 * Should be set in each child's initializer.
	 */
	QString m_id = "Unknown_ID";
	QString m_pluginId = "Unknown Plugin";
	QString m_name = "New Source";

	Source::Type mSourceType = Source::Type::OTHER;
	Source::State mState = Source::State::CLOSED;
	Source::ErrorState mErrorState = ErrorState::NO_ERROR;

	qint64 mStartTime = 0;

	std::unique_ptr<SourcePreview> pPreview = nullptr;

signals:
	void opened();
	void started();
	void stopped();
	void closed();

	/// <summary>
	/// Emitted when the source's preview is available.
	/// </summary>
	void previewAvailable(Source* src, SourcePreview* preview);

	void errorOccurred(const SourceError& e);
};
