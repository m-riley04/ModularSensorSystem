#include "controllers/loggingcontroller.hpp"

// Define static members
QMutex LoggingController::logMutex;
QFile* LoggingController::logFile = nullptr;

LoggingController::LoggingController(SettingsController& sc, QObject *parent)
	: QObject(parent), m_settingsController(sc)
{
	// Set the message pattern for logging
	qSetMessagePattern("[%{time yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] %{message}");

	// Initialize log file based on settings
	QDir logDir = m_settingsController.advancedSettings().logDirectory;

	if (!logDir.exists()) {
		QDir().mkpath(logDir.absolutePath());
	}

	QString logFilePath = logDir.absolutePath() + "/" + generateLogFileName(m_settingsController.advancedSettings().useUniqueLogFiles);
	logFile = new QFile(logFilePath, this);

	// Install custom message handler
	qInstallMessageHandler(&LoggingController::messageHandler);

	// Add log header
	logMutex.lock();
	if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
		QTextStream stream(logFile);
		stream << "----- Log started at " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " -----" << Qt::endl;
		stream.flush();
	} else {
		std::cerr << "Failed to open log file: " << logFilePath.toLocal8Bit().constData() << std::endl;
	}
	logMutex.unlock();
}

LoggingController::~LoggingController()
{
	if (logFile && logFile->isOpen()) {
		logFile->close();
	}
}

void LoggingController::info(const QString & message)
{
	qInfo() << message;
}

void LoggingController::warning(const QString& message)
{
	qWarning() << message;
}

void LoggingController::critical(const QString& message)
{
	qCritical() << message;
}

void LoggingController::fatal(const QString& message)
{
	qFatal() << message;
}

void LoggingController::debug(const QString& message)
{
	// TODO/CONSIDER: disable debug messages entirely if debug mode is not enabled in settings?
	qDebug() << message;
}

QString LoggingController::generateLogFileName(bool unique)
{
	if (!unique) return QString("log.txt");
	QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
	return QString("log_%1.txt").arg(timestamp);
}

void LoggingController::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	QMutexLocker locker(&logMutex); // Ensure thread-safe logging
	if (!logFile || !logFile->isOpen()) {
		std::cerr << "Log file not open: " << msg.toLocal8Bit().constData() << std::endl;
		return;
	}

	QTextStream stream(logFile);
	QString formattedMsg = qFormatLogMessage(type, context, msg); // Utilize the built-in formatter

	// write the formatted message to the log file
	stream << formattedMsg << Qt::endl;

	// flush the stream to ensure immediate writing
	stream.flush();

	if (type == QtFatalMsg) {
		// For fatal errors, it's crucial to ensure the message is written before aborting (automatically done after returning)
		logFile->close(); // Ensure the file is closed before aborting
	}
}

