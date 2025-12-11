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

	bool isDynamic() const override { return true; }
	bool isPosable() const override { return true; }
	bool isAutomaticallyPosed() const override { return false; }

	std::optional<Pose> pose() const override { return std::nullopt; }
	bool setPose(const Pose& newPose) override { return false; }

	// IPanTiltMount implementations
	bool moveTo(double panAngle, double tiltAngle) override final;
	double panAngle() const override final;
	double panMinAngle() const override final;
	double panMaxAngle() const override final;
	double tiltAngle() const override final;
	double tiltMinAngle() const override final;
	double tiltMaxAngle() const override final;
	PanTiltInfo info() const override final;
	bool recenter() override final;
	bool refreshInfo() override final;
	PanTiltError error() const override final;

private slots:
	bool sendCommand(const QString& command);
	bool sendInfoCommand();
	void readSerialData();

private:
	void parseResponse();
	void setError(const QString& errorMsg);

private:
	QSerialPort* m_serialPort = nullptr;
	PanTiltInfo	m_panTiltInfo;
	QByteArray m_readBuffer;
	PanTiltError m_error;
};

