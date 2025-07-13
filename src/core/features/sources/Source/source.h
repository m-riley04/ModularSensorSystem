#pragma once

#include <QObject>
#include "controllers/RecordingSession/recordingsession.h"
#include <quuid.h>
#include "SourcePreview/sourcepreview.h"
#include <QPointer>
#include "features/sources/IClippableSource/ClipBufferBase/clipbufferbase.h"
#include <QUuid>

class RecordingSession;
class SourcePreview;

class Source : public QObject
{
	Q_OBJECT

public:
	enum Type {
		VIDEO,
		AUDIO,
		GPS,
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

	static QString typeToString(Type type)
	{
		switch (type) {
		case VIDEO: return "Camera";
		case AUDIO: return "Microphone";
		case GPS: return "GPS";
		case OTHER: return "Other";
		default: return "Unknown";
		}
	}

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

protected:
	/// <summary>
	/// The unique hardware ID if possible.
	/// Should be set in each child's initializer.
	/// </summary>
	QUuid mId;

	QString mPluginId = "Unknown Plugin";

	QString mName = "New Source";
	Source::Type mSourceType = Source::Type::OTHER;
	Source::State mState = Source::State::CLOSED;
	qint64 mStartTime = 0;
	Source::ErrorState mErrorState = ErrorState::NO_ERROR;

	QPointer<RecordingSession> pRecordingSession;
	std::unique_ptr<SourcePreview> pPreview = nullptr;
	std::unique_ptr<ClipBufferBase> pClipBuffer = nullptr;

public:
	Source(QByteArray hardwareId, QObject* parent);
	Source(QObject* parent);
	~Source();

	virtual void open() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void close() = 0;
	virtual void restart() = 0;

	virtual void beginRecording(RecordingSession*) = 0;
	virtual void endRecording() = 0;

	/// <summary>
	/// The unique ID for the device
	/// </summary>
	QUuid id() const { return mId; }

	QString pluginId() const { return mPluginId; }

	/// <summary>
	/// The user-facing name of the device.
	/// </summary>
	QString name() const { return mName; }

	/// <summary>
	/// The type of device
	/// </summary>
	Source::Type type() const { return mSourceType; }

	/// <summary>
	/// The current state of the device
	/// </summary>
	Source::State state() const { return mState; }

	/// <summary>
	/// The SourcePreview object for the device.
	/// Used for previewing the device's output.
	/// </summary>
	SourcePreview* preview() const { return pPreview.get(); }

	void setSession(RecordingSession* session)
	{
		pRecordingSession = session;
	}

signals:
	void opened();
	void started();
	void stopped();
	void closed();

	/// <summary>
	/// Emitted when the source's preview is available.
	/// </summary>
	void previewAvailable(Source* src, SourcePreview* preview);
};
