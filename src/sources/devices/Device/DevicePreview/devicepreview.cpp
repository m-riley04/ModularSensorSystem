#include "devicepreview.h"

DevicePreview::DevicePreview(QObject* parent)
	: QObject(parent), pDevice(static_cast<Device*>(parent)) // TODO: MAKE THIS SAFER/ERROR HANDLE
{}

DevicePreview::~DevicePreview()
{}
