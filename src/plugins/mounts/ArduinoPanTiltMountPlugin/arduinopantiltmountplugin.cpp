#include "arduinopantiltmountplugin.hpp"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <controllers/loggingcontroller.hpp>

ArduinoPanTiltMountPlugin::ArduinoPanTiltMountPlugin()
{

}

ArduinoPanTiltMount* ArduinoPanTiltMountPlugin::createMount(const std::string& id, QObject* parent)
{
	for (auto& mountInfo : discover()) {
		if (mountInfo.id == id) {
			return new ArduinoPanTiltMount(mountInfo, parent);
		}
	}
    return nullptr;
}

const std::vector<ElementInfo> ArduinoPanTiltMountPlugin::discover() const
{
	// Get list of available serial ports
	QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

    /*
	// Debug: print all available serial ports
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& portInfo : serialPortInfos) {
        qDebug() << "\n"
            << "Port:" << portInfo.portName() << "\n"
            << "Location:" << portInfo.systemLocation() << "\n"
            << "Description:" << portInfo.description() << "\n"
            << "Manufacturer:" << portInfo.manufacturer() << "\n"
            << "Serial number:" << portInfo.serialNumber() << "\n"
            << "Vendor Identifier:"
            << (portInfo.hasVendorIdentifier()
                ? QByteArray::number(portInfo.vendorIdentifier(), 16)
                : QByteArray()) << "\n"
            << "Product Identifier:"
            << (portInfo.hasProductIdentifier()
                ? QByteArray::number(portInfo.productIdentifier(), 16)
                : QByteArray());
    }*/

    std::vector<ElementInfo> list;
    for (const QSerialPortInfo& portInfo : serialPorts) {
        if (portInfo.manufacturer() != "Arduino") {
			LoggingController::info("Skipping non-Arduino device on port: " + portInfo.portName());
            continue;
        }

        ElementInfo element{
			.id = portInfo.serialNumber().toStdString(), // unique ID based on serial number
            .name = portInfo.portName().toStdString(),
            .displayName = "Arduino Pan Tilt (" + portInfo.portName().toStdString() + ")",
            .pluginId = "arduinopantilt_mount_plugin",
		};
		list.push_back(element);

		LoggingController::info("Found Arduino Pan Tilt device on port: " + portInfo.portName());
	}

    return list;
}
