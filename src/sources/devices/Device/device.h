#pragma once

#include <QObject>
#include "controllers/RecordingSession/recordingsession.h"
#include <quuid.h>
#include "DevicePreview/devicepreview.h"
#include <QPointer>
#include "sources/devices/IClippableDevice/ClipBufferBase/clipbufferbase.h"
#include "DeviceError/deviceerror.h"

class RecordingSession;
class DevicePreview;

class Device : public QObject
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
		DEVICE_NOT_FOUND,
		DEVICE_BUSY,
		DEVICE_DISCONNECTED,
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
		case DEVICE_NOT_FOUND: return "Device Not Found";
		case DEVICE_BUSY: return "Device Busy";
		case DEVICE_DISCONNECTED: return "Device Disconnected";
		case UNKNOWN_ERROR: return "Unknown Error";
		default: return "Unknown";
		}
	}

protected:
	/// <summary>
	/// The unique hardware ID if possible.
	/// Should be set in each child's initializer.
	/// </summary>
	QByteArray mId;

	QString mPluginId = "Unknown Plugin";

	QString mName = "New Device";
	Device::Type mDeviceType = Device::Type::OTHER;
	Device::State mState = Device::State::CLOSED;
	qint64 mStartTime = 0;
	Device::ErrorState mErrorState = ErrorState::NO_ERROR;

	QPointer<RecordingSession> pRecordingSession;
	std::unique_ptr<DevicePreview> pPreview = nullptr;
	std::unique_ptr<ClipBufferBase> pClipBuffer = nullptr;

public:
	Device(QByteArray hardwareId, QObject *parent);
	Device(QObject *parent);
	~Device();

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
	QByteArray id() const { return mId; }

	QString pluginId() const { return mPluginId; }

	/// <summary>
	/// The user-facing name of the device.
	/// </summary>
	QString name() const { return mName; }

	/// <summary>
	/// The type of device
	/// </summary>
	Device::Type deviceType() const { return mDeviceType; }

	/// <summary>
	/// The current state of the device
	/// </summary>
	Device::State state() const { return mState; }

	/// <summary>
	/// The DevicePreview object for the device.
	/// Used for previewing the device's output.
	/// </summary>
	DevicePreview* preview() const { return pPreview.get(); }

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
	/// Emitted when the device's preview is available.
	/// </summary>
	void previewAvailable(Device* device, DevicePreview* preview);

	void errorOccurred(const DeviceError& e);
};
