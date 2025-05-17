#pragma once

#include <QObject>
#include "Controllers/RecordingSession/recordingsession.h"
#include <quuid.h>
#include "DevicePreview/devicepreview.h"

class RecordingSession;
class DevicePreview;

class Device : public QObject
{
	Q_OBJECT

public:
	enum Type {
		CAMERA,
		MICROPHONE,
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
		case CAMERA: return "Camera";
		case MICROPHONE: return "Microphone";
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
	QUuid mId;
	QString mName = "New Device";
	Device::Type mDeviceType = Device::Type::OTHER;
	Device::State mState = Device::State::CLOSED;
	qint64 mStartTime = 0;
	Device::ErrorState mErrorState = ErrorState::NO_ERROR;

	RecordingSession* pRecordingSession = nullptr;
	DevicePreview* pDevicePreview = nullptr;

public:
	Device(RecordingSession* recordingSession, QObject *parent);
	~Device();

	virtual void open() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void close() = 0;
	virtual void restart() = 0;

	QUuid id() const { return mId; }
	QString name() const { return mName; }
	Device::Type deviceType() const { return mDeviceType; }
	Device::State state() const { return mState; }
	DevicePreview* preview() const { return pDevicePreview; }

signals:
	void opened();
	void started();
	void stopped();
	void closed();

	void previewAvailable(Device* device, DevicePreview* preview);
};
