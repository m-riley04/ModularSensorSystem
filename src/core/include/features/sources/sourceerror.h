#pragma once

#include <QObject>
#include <QString>
#include <QMetaType>

// Forward declaration
class Source;

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