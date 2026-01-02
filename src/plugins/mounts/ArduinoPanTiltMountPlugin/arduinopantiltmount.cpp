#include "arduinopantiltmount.hpp"
#include <controllers/loggingcontroller.hpp>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>

ArduinoPanTiltMount::ArduinoPanTiltMount(const ElementInfo& element, QObject* parent)
	: Mount(element, parent)
	, m_serialPort(new QSerialPort(QString::fromStdString(element.name), this))
	, m_panTiltInfo(Pose())
	, m_bin(std::make_unique<ArduinoPanTiltMountBin>(this))

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

Pose ArduinoPanTiltMount::pose() const
{
	return m_panTiltInfo;
}

bool ArduinoPanTiltMount::recenter()
{
	// TODO: do actual advanced recentering logic here
	double yawBoundsMedian = (m_panTiltInfo.bounds.yaw.min + m_panTiltInfo.bounds.yaw.max) / 2.0;
	double pitchBoundsMedian = (m_panTiltInfo.bounds.pitch.min + m_panTiltInfo.bounds.pitch.max) / 2.0;
	return this->moveTo(yawBoundsMedian, pitchBoundsMedian);
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

MountError ArduinoPanTiltMount::error() const
{
	return m_error;
}

GstElement* ArduinoPanTiltMount::gstSrcBin()
{
	// lazy creation; note m_bin must be mutable
	createBinIfNeeded();
	return m_bin->bin();
}

GstElement* ArduinoPanTiltMount::recorderSinkBin()
{
	createRecorderBinIfNeeded();
	return m_recorderBin->bin();
}

std::string ArduinoPanTiltMount::recorderFileExtension() const
{
	return "json";
}

bool ArduinoPanTiltMount::setRecordingFilePath(const std::string& filePath)
{
	if (!m_recorderBin) return false;

	return m_recorderBin->setRecordingFilePath(filePath);
}

bool ArduinoPanTiltMount::startRecording()
{
	if (!m_recorderBin) {
		createRecorderBinIfNeeded();
	}
	return m_recorderBin->setRecordingEnabled(true);
}

bool ArduinoPanTiltMount::stopRecording()
{
	if (!m_recorderBin) {
		createRecorderBinIfNeeded();
	}
	return m_recorderBin->setRecordingEnabled(false) && m_recorderBin->finalizeRecording();;
}

void ArduinoPanTiltMount::createBinIfNeeded()
{
	if (!m_bin) {
		m_bin = std::make_unique<ArduinoPanTiltMountBin>(this);
	}
}

void ArduinoPanTiltMount::createRecorderBinIfNeeded()
{
	if (!m_recorderBin) {
		m_recorderBin = std::make_unique<ArduinoPanTiltMountRecorderBin>(this);
	}
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
		m_panTiltInfo.bounds.yaw.min = query.queryItemValue("minYaw").toDouble();
	}
	if (query.hasQueryItem("maxYaw")) {
		m_panTiltInfo.bounds.yaw.max = query.queryItemValue("maxYaw").toDouble();
	}
	if (query.hasQueryItem("minPitch")) {
		m_panTiltInfo.bounds.pitch.min = query.queryItemValue("minPitch").toDouble();
	}
	if (query.hasQueryItem("maxPitch")) {
		m_panTiltInfo.bounds.pitch.max = query.queryItemValue("maxPitch").toDouble();
	}
	if (query.hasQueryItem("yaw")) {
		m_panTiltInfo.yaw = cleanUriField(query.queryItemValue("yaw")).toDouble();
	}
	if (query.hasQueryItem("pitch")) {
		m_panTiltInfo.pitch = cleanUriField(query.queryItemValue("pitch")).toDouble();
	}

	// Emit dataUpdated signal with the created JSON document
	QByteArray jsonData = createNdjsonPayload(m_panTiltInfo);
	emit dataUpdated(jsonData);
}

void ArduinoPanTiltMount::setError(const QString& errorMsg)
{
	m_error.msg = errorMsg.toStdString();
	LoggingController::warning("ArduinoPanTilt error: " + errorMsg);

	emit errorOccurred(errorMsg);
}

QByteArray ArduinoPanTiltMount::createNdjsonPayload(Pose& pose)
{
	QJsonObject jsonObj;
	jsonObj.insert("minYaw", pose.bounds.yaw.min);
	jsonObj.insert("maxYaw", pose.bounds.yaw.max);
	jsonObj.insert("minPitch", pose.bounds.pitch.min);
	jsonObj.insert("maxPitch", pose.bounds.pitch.max);
	jsonObj.insert("yaw", pose.yaw);
	jsonObj.insert("pitch", pose.pitch);
	QJsonDocument jsonDoc(jsonObj);

	return jsonDoc.toJson(QJsonDocument::Compact).append('\n'); // newline for separation
}

QString ArduinoPanTiltMount::cleanUriField(const QString& field)
{
	QString cleaned = field;
	cleaned = cleaned.trimmed();
	cleaned = cleaned.replace("%0D", ""); // TODO: add more robust cleaning if needed

	return cleaned;
}

bool ArduinoPanTiltMount::sendCommand(const QString& command)
{
	LoggingController::info("Sending command to Arduino Pan-Tilt Mount: " + command.trimmed());
	if (m_serialPort->write(command.toUtf8()) == -1) {
		setError("Failed to write command to serial port.");
		return false;
	}

	// Push NDJSON payload to source bin for recording/streaming
	QByteArray ndjsonPayload = createNdjsonPayload(m_panTiltInfo);
	m_bin->pushSample(ndjsonPayload);

	return true;
}
