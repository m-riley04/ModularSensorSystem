#pragma once

#include <qobject.h>
#include "features/mounts/mount.hpp"
#include "interfaces/capability/mounts/ipantiltmount.hpp"
#include <QSerialPort>

class ArduinoPanTiltMount : public Mount, public IPanTiltMount
{
	Q_OBJECT

public:
	ArduinoPanTiltMount(const ElementInfo& element, QObject* parent);
	~ArduinoPanTiltMount() = default;

	// IPanTiltMount implementations
	bool moveTo(double panAngle, double tiltAngle) override final;
	Pose pose() const override final;
	bool recenter() override final;
	bool refreshInfo() override final;
	MountError error() const override final;

private slots:
	bool sendCommand(const QString& command);
	bool sendInfoCommand();
	void readSerialData();

private:
	void parseResponse();
	void setError(const QString& errorMsg);

private:
	QSerialPort* m_serialPort = nullptr;
	Pose m_panTiltInfo;
	QByteArray m_readBuffer;
	MountError m_error;
};

