#pragma once

#include <QObject>
#include <QString>
#include <devices/Device/device.h>
#include <QMetaType>

enum class ErrorSeverity {
	INFO,
	WARNING,
	CRITICAL
};

struct DeviceError {
	QString msg;
	Device* device = nullptr;
	ErrorSeverity severity = ErrorSeverity::INFO;
};

Q_DECLARE_METATYPE(DeviceError);