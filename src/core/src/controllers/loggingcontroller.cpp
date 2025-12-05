#include "controllers/loggingcontroller.hpp"


LoggingController::LoggingController(SettingsController& sc, QObject *parent)
	: QObject(parent), m_settingsController(sc)
{
	// Set the message pattern for logging
	qSetMessagePattern("[%{time yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] %{message}");

	// Set enabled state based on settings
	bool loggingEnabled = m_settingsController.advancedSettings().enableLogging;
	bool logDebugMessages = m_settingsController.advancedSettings().enableDebugMode;
	bool logToFile = m_settingsController.advancedSettings().logToFile;

	// Set output to file if enabled
	if (loggingEnabled && logToFile) {
		// Ensure log directory exists
		if (!QDir(m_settingsController.advancedSettings().logDirectory).exists()) {
			QDir().mkpath(m_settingsController.advancedSettings().logDirectory.absolutePath());
		}

		bool useUniqueLogFiles = m_settingsController.advancedSettings().useUniqueLogFiles;
		QString logFilePath = m_settingsController.advancedSettings().logDirectory.absolutePath() + "/" + generateLogFileName(useUniqueLogFiles);
		m_logFile = new QFile(logFilePath, this);
		if (m_logFile->open(QIODevice::Append | QIODevice::Text)) {
			m_logStream = new QTextStream(m_logFile);
			qInstallMessageHandler(&LoggingController::messageHandler);
		} else {
			qWarning() << "Failed to open log file for writing:" << logFilePath;
			delete m_logFile;
		}
	}
}

LoggingController::~LoggingController()
{}

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
	if (!m_settingsController.advancedSettings().enableDebugMode) return;

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
	// TODO
}

