#pragma once

#include <QObject>
#include <QString>
#include "source.h"
#include <QMetaType>

enum class ErrorSeverity {
	INFO,
	WARNING,
	CRITICAL
};

struct SourceError {
	QString msg = "";
	Source* source = nullptr;
	ErrorSeverity severity = ErrorSeverity::INFO;
};

Q_DECLARE_METATYPE(SourceError)