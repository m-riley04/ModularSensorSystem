#include "arduinopantiltmount.hpp"
#include <controllers/loggingcontroller.hpp>
#include <QUrl>
#include <QUrlQuery>

ArduinoPanTiltMount::ArduinoPanTiltMount(const ElementInfo& element, QObject* parent)
	: Mount(element, parent)
	, m_serialPort(new QSerialPort(QString::fromStdString(element.name), this))
	, m_panTiltInfo(PanTiltInfo())
{
	// Add serial port connections FIRST
	connect(m_serialPort, &QSerialPort::readyRead, this, &ArduinoPanTiltMount::readSerialData);
	connect(m_serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
		if (error != QSerialPort::NoError) {
			setError("Error at the serial port: " + m_serialPort->errorString());
			m_serialPort->clearError();
		}
		});
	connect(m_serialPort, &QSerialPort::bytesWritten, this, [this](qint64 bytes) {
		if (bytes <= 0) {
			LoggingController::warning("No bytes were written to Arduino Pan-Tilt Mount serial port.");
		}
		LoggingController::debug(QString("Wrote %1 bytes to Arduino Pan-Tilt Mount serial port.").arg(bytes));
		});
	connect(m_serialPort, &QSerialPort::baudRateChanged, this, [this](qint32 baudRate, QSerialPort::Directions directions) {
		QString directionsStr;
		switch (directions) {
		case QSerialPort::Input:
			directionsStr = "Input";
			break;
		case QSerialPort::Output:
			directionsStr = "Output";
			break;
		case QSerialPort::AllDirections:
		default:
			directionsStr = "Input/Output";
		}
		LoggingController::info(QString("Baud rate changed to %1 (Directions: %2)").arg(baudRate).arg(directionsStr));
		});
	connect(m_serialPort, &QSerialPort::dataTerminalReadyChanged, this, [this](bool set) {
		LoggingController::info(QString("DTR changed to %1").arg(set));
		});
	connect(m_serialPort, &QSerialPort::aboutToClose, this, [this]() {
		LoggingController::info("Serial port about to close.");
		});
	connect(m_serialPort, &QSerialPort::readChannelFinished, this, [this]() {
		LoggingController::info("Read channel finished.");
		});
	connect(m_serialPort, &QSerialPort::requestToSendChanged, this, [this](bool set) {
		LoggingController::info(QString("RTS changed to %1").arg(set));
		});
	connect(m_serialPort, &QSerialPort::breakEnabledChanged, this, [this](bool set) {
		LoggingController::info(QString("Break enabled changed to %1").arg(set));
		});

	if (!m_serialPort->open(QIODevice::ReadWrite)) {
		setError("Failed to open serial port for Arduino Pan-Tilt Mount: " + m_serialPort->errorString());
		return;
	}

	if (!m_serialPort->setBaudRate(QSerialPort::Baud115200)) {
		setError("Failed to set baud rate for Arduino Pan-Tilt Mount serial port: " + m_serialPort->errorString());
		return;
	}

	// Open DTR (this is required for initialization of device)
	if (!m_serialPort->setDataTerminalReady(true)) {
		setError("Failed to set DTR");
		return;
	}

	// Get initial serial port info
	if (!this->sendInfoCommand()) {
		setError("Failed to get initial pan-tilt mount info from serial port.");
	}

	LoggingController::info("Arduino Pan-Tilt Mount initialized on serial port: " + m_serialPort->portName());
}

bool ArduinoPanTiltMount::moveTo(double panAngle, double tiltAngle)
{
	QString command = QString::number(static_cast<int>(panAngle)) + "," + QString::number(static_cast<int>(tiltAngle)) + "\n";
	return sendCommand(command);
}

double ArduinoPanTiltMount::panAngle() const
{
	return m_panTiltInfo.panAngle;
}

double ArduinoPanTiltMount::panMinAngle() const
{
	return m_panTiltInfo.minPanAngle;
}

double ArduinoPanTiltMount::panMaxAngle() const
{
	return m_panTiltInfo.maxPanAngle;
}

double ArduinoPanTiltMount::tiltAngle() const
{
	return m_panTiltInfo.tiltAngle;
}

double ArduinoPanTiltMount::tiltMinAngle() const
{
	return m_panTiltInfo.minTiltAngle;
}

double ArduinoPanTiltMount::tiltMaxAngle() const
{
	return m_panTiltInfo.maxTiltAngle;
}

PanTiltInfo ArduinoPanTiltMount::info() const
{
	return m_panTiltInfo;
}

bool ArduinoPanTiltMount::recenter()
{
	// TODO: do actual advanced recentering logic here
	double panBoundsMedian = (m_panTiltInfo.minPanAngle + m_panTiltInfo.maxPanAngle) / 2.0;
	double tiltBoundsMedian = (m_panTiltInfo.minTiltAngle + m_panTiltInfo.maxTiltAngle) / 2.0;
	return this->moveTo(panBoundsMedian, tiltBoundsMedian);
}

bool ArduinoPanTiltMount::refreshInfo()
{
	if (!m_serialPort->isOpen()) {
		LoggingController::info("Serial port is not open. Attempting to open...");
		if (!m_serialPort->open(QIODevice::ReadWrite)) {
			setError("Failed to open serial port for Arduino Pan-Tilt Mount: " + m_serialPort->errorString());
			return false;
		}
	}

	return this->sendInfoCommand();
}

PanTiltError ArduinoPanTiltMount::error() const
{
	return m_error;
}

bool ArduinoPanTiltMount::sendInfoCommand()
{
	QString command = "info\n";
	return sendCommand(command);
}

void ArduinoPanTiltMount::readSerialData()
{
	LoggingController::debug("Reading serial data from Arduino Pan-Tilt Mount...");

	// Read available data into read buffer
	m_readBuffer.append(m_serialPort->readAll());
	if (m_readBuffer.isEmpty() || m_readBuffer.isNull()) {
		LoggingController::warning("Received empty or null info response from serial port.");
		return;
	}

	// Check if this is the end of the response
	qsizetype terminatorIndex = m_readBuffer.indexOf('\n');
	if (terminatorIndex != -1) {
		// Extract the complete message
		QByteArray completeMessage = m_readBuffer.left(terminatorIndex);

		// Remove processed data from buffer (including the newline)
		m_readBuffer.remove(0, terminatorIndex + 1);

		// Store for parsing and parse
		QByteArray temp = m_readBuffer;
		m_readBuffer = completeMessage;
		this->parseResponse();
		m_readBuffer = temp;

		LoggingController::debug("Parsed response from Arduino Pan-Tilt Mount.");
	}
}

void ArduinoPanTiltMount::parseResponse()
{
	// Parse response data as query string
	QUrlQuery query;
	//query.setQuery(QUrl::fromPercentEncoding(m_readBuffer));
	query.setQuery(m_readBuffer);
	if (query.hasQueryItem("minYaw")) {
		m_panTiltInfo.minPanAngle = query.queryItemValue("minYaw").toDouble();
	}
	if (query.hasQueryItem("maxYaw")) {
		m_panTiltInfo.maxPanAngle = query.queryItemValue("maxYaw").toDouble();
	}
	if (query.hasQueryItem("minPitch")) {
		m_panTiltInfo.minTiltAngle = query.queryItemValue("minPitch").toDouble();
	}
	if (query.hasQueryItem("maxPitch")) {
		m_panTiltInfo.maxTiltAngle = query.queryItemValue("maxPitch").toDouble();
	}
	if (query.hasQueryItem("yaw")) {
		m_panTiltInfo.panAngle = query.queryItemValue("yaw").toDouble();
	}
	if (query.hasQueryItem("pitch")) {
		// TODO: fix this in a better way
		QString tiltAngleStr = query.queryItemValue("pitch");
		tiltAngleStr = tiltAngleStr.trimmed();
		tiltAngleStr = tiltAngleStr.replace("%0D", "");
		m_panTiltInfo.tiltAngle = tiltAngleStr.toDouble();
	}

	emit dataUpdated();
}

void ArduinoPanTiltMount::setError(const QString& errorMsg)
{
	m_error.msg = errorMsg.toStdString();
	LoggingController::warning("ArduinoPanTilt error: " + errorMsg);

	emit errorOccurred(errorMsg);
}

bool ArduinoPanTiltMount::sendCommand(const QString& command)
{
	LoggingController::info("Sending command to Arduino Pan-Tilt Mount: " + command.trimmed());
	if (m_serialPort->write(command.toUtf8()) == -1) {
		setError("Failed to write command to serial port.");
		return false;
	}

	return true;
}
