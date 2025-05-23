#include "device.h"

Device::Device(QByteArray hardwareId, QObject* parent)
{
	mId = hardwareId;
}

Device::Device(QObject *parent)
	: QObject(parent)
{}

Device::~Device()
{}
