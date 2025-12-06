#include "controllers/loggingcontroller.hpp"
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QMutexLocker>
#include <utils/safer_io_utils.hpp>

// Define static members
QMutex LoggingController::logMutex;
QFile* LoggingController::logFile = nullptr;
std::atomic<bool> LoggingController::s_loggingEnabled{ false };
QtMessageHandler LoggingController::previousHandler = nullptr;

LoggingController::LoggingController(SettingsController& sc, QObject *parent)
	: QObject(parent), m_settingsController(sc)
{
	// Set the message pattern for logging
	qSetMessagePattern("[%{time yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] %{message}");

	// Install custom message handler and keep previous to allow chaining
	previousHandler = qInstallMessageHandler(&LoggingController::messageHandler);

	// Mirror current setting and prepare file if enabled
	s_loggingEnabled = m_settingsController.advancedSettings().enableLogging;
	connect(&m_settingsController, &SettingsController::enableLoggingChanged,
			this, &LoggingController::onEnableLoggingChanged);

	onEnableLoggingChanged(s_loggingEnabled.load());
}

LoggingController::~LoggingController()
{
	if (logFile) {
		if (logFile->isOpen()) {
			logFile->close();
		}
		logFile = nullptr;
	}
}

void LoggingController::clearLogs()
{
	QDir logDir = m_settingsController.advancedSettings().logDirectory;
	if (!logDir.exists()) {
		return; // Nothing to clear
	}

	safeDeleteDirectoryContents(
		logDir
		, QStringList() << "log*.txt"
		, QDir::Files);
}

void LoggingController::info(const QString & message)
{
	if (!s_loggingEnabled.load()) return;
	qInfo() << message;
}

void LoggingController::warning(const QString& message)
{
	if (!s_loggingEnabled.load()) return;
	qWarning() << message;
}

void LoggingController::critical(const QString& message)
{
	if (!s_loggingEnabled.load()) return;
	qCritical() << message;
}

void LoggingController::fatal(const QString& message)
{
	// Always forward fatal, even if logging is disabled
	qFatal() << message;
}

void LoggingController::debug(const QString& message)
{
	if (!s_loggingEnabled.load()) return;
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
	if (!s_loggingEnabled.load()) {
		// If logging is disabled, chain to previous handler (console, etc.)
		if (previousHandler) {
			previousHandler(type, context, msg);
		}
		return;
	}

	QMutexLocker locker(&logMutex); // Ensure thread-safe logging
	if (!logFile || !logFile->isOpen()) {
		// Fallback to previous handler if file not ready
		if (previousHandler) {
			previousHandler(type, context, msg);
		}
		return;
	}

	QTextStream stream(logFile);
	QString formattedMsg = qFormatLogMessage(type, context, msg); // Utilize the built-in formatter

	// write the formatted message to the log file
	stream << formattedMsg << Qt::endl;

	// flush the stream to ensure immediate writing
	stream.flush();

	if (type == QtFatalMsg) {
		// For fatal errors, ensure the message is written before aborting
		logFile->close(); // Ensure the file is closed before aborting
	}
}

void LoggingController::onEnableLoggingChanged(bool enabled)
{
	s_loggingEnabled.store(enabled);

	QMutexLocker locker(&logMutex);
	if (enabled) {
		if (!logFile) { // Create log file if it doesn't exist
			QDir logDir = m_settingsController.advancedSettings().logDirectory;
			if (!logDir.exists()) {
				QDir().mkpath(logDir.absolutePath());
			}

			QString logFilePath = logDir.absolutePath() + "/" + generateLogFileName(m_settingsController.advancedSettings().useUniqueLogFiles);
			logFile = new QFile(logFilePath, this);
		}

		if (!logFile->isOpen()) { // Open log file
			if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
				QTextStream stream(logFile);
				stream << "----- Log started at " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " -----" << Qt::endl;
				stream.flush();
			}
		}
	} else {
		if (logFile && logFile->isOpen()) {
			logFile->close();
		}
	}
}

