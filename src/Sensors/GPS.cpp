#include "GPS.h"
#include <QNmeaPositionInfoSource>

GPS::GPS(QObject *parent)
	: Sensor(parent)
{
    /// TODO: Move this all somewhere better (Sensor?)
    // Create and configure the serial port
    QSerialPort* serialPort = new QSerialPort();
    serialPort->setPortName("COM6"); // Replace with actual port name
    serialPort->setBaudRate(QSerialPort::Baud9600); // Adjust if necessary
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    // Open the serial port
    if (!serialPort->open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open serial port";
        return;
    }

    // Create a NMEA position info source
    QNmeaPositionInfoSource* nmeaSource = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode);
    nmeaSource->setDevice(serialPort);
    nmeaSource->setUpdateInterval(1000); // Update every 1000 milliseconds

    // Connect to the positionUpdated signal
    QObject::connect(nmeaSource, &QNmeaPositionInfoSource::positionUpdated,
        [](const QGeoPositionInfo& info) {
            if (info.isValid()) {
                qDebug() << "Timestamp:" << info.timestamp();
                qDebug() << "Latitude:" << info.coordinate().latitude();
                qDebug() << "Longitude:" << info.coordinate().longitude();
                qDebug() << "Speed:" << info.attribute(QGeoPositionInfo::GroundSpeed);
            }
            else {
                qDebug() << "Invalid position info received.";
            }
        });

    QObject::connect(serialPort, &QSerialPort::readyRead, [&]() {
        QByteArray data = serialPort->readAll();
        qDebug() << "Data read from serial port:" << data;
        });

    QObject::connect(nmeaSource, &QNmeaPositionInfoSource::errorOccurred,
        [](QGeoPositionInfoSource::Error positionError) {
            qWarning() << "Position error occurred:" << positionError;
        });

    QObject::connect(nmeaSource, &QNmeaPositionInfoSource::error, []() {
        qWarning() << "NMEA error";
        });

    // Start updates
    nmeaSource->startUpdates();
}

GPS::~GPS()
{}
