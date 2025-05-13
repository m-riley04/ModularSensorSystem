#pragma once

#include <QObject>



class Device : public QObject
{
	Q_OBJECT

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

private:
	QString mId;
	Device::Type mDeviceType = Device::Type::OTHER;
	Device::State mState = Device::State::CLOSED;
	qint64 mStartTime = 0;
	Device::ErrorState mErrorState;

public:
	Device(QObject *parent);
	~Device();

	virtual void open() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void close() = 0;
	virtual void restart() = 0;

signals:
	void opened();
	void started();
	void stopped();
	void closed();
};
