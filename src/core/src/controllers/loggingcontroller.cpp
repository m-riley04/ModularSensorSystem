#include "controllers/loggingcontroller.hpp"
#include <QDir>
#include <QDateTime>
#include <QMutexLocker>
#include <utils/safer_io_utils.hpp>

// Define static members
QMutex LoggingController::s_workerMutex;
QThread* LoggingController::s_workerThread = nullptr;
LogWriterWorker* LoggingController::s_worker = nullptr;
std::atomic<bool> LoggingController::s_loggingEnabled{ false };
std::atomic<bool> LoggingController::s_useUniqueLogFileNames{ false };
QtMessageHandler LoggingController::previousHandler = nullptr;

LoggingController::LoggingController(SettingsController& sc, QObject *parent)
	: QObject(parent), m_settingsController(sc)
{
	// Set the message pattern for logging
	qSetMessagePattern("[%{time yyyy-MM-dd HH:mm:ss.zzz}] [%{type}] %{message}");

	// Install custom message handler and keep previous to allow chaining
	previousHandler = qInstallMessageHandler(&LoggingController::messageHandler);

	// Mirror current settings (order matters - set flags before starting worker)
	s_useUniqueLogFileNames.store(m_settingsController.advancedSettings().useUniqueLogFiles);
	s_loggingEnabled.store(m_settingsController.advancedSettings().enableLogging);
	
	// Connect settings change signals
	connect(&m_settingsController, &SettingsController::enableLoggingChanged,
		this, &LoggingController::onEnableLoggingChanged);
	connect(&m_settingsController, &SettingsController::useUniqueLogFilesChanged,
		this, &LoggingController::onUseUniqueLogFileNamesChanged);

	// Start worker if logging is enabled (only once, after all flags are set)
	if (s_loggingEnabled.load()) {
		startWorker(m_settingsController.advancedSettings().logDirectory);
	}
}

LoggingController::~LoggingController()
{
	// Stop worker thread
	stopWorker();

	// Restore previous message handler
	qInstallMessageHandler(previousHandler);
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

void LoggingController::info(const QString& message)
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
	qDebug() << message;
}

void LoggingController::onUseUniqueLogFileNamesChanged(bool useUniqueNames)
{
	s_useUniqueLogFileNames.store(useUniqueNames);

	// Restart worker with new file name if logging is enabled
	if (s_loggingEnabled.load()) {
		stopWorker();
		startWorker(m_settingsController.advancedSettings().logDirectory);
	}
}

void LoggingController::onEnableLoggingChanged(bool enabled)
{
	s_loggingEnabled.store(enabled);

	if (enabled) {
		startWorker(m_settingsController.advancedSettings().logDirectory);
	} else {
		stopWorker();
	}
}

void LoggingController::onWorkerError(const QString& error)
{
	// Log to stderr since file logging failed
	fprintf(stderr, "LoggingController: %s\n", qPrintable(error));
}

QString LoggingController::generateLogFileName()
{
	if (!s_useUniqueLogFileNames.load()) return QString("log.txt");
	QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
	return QString("log_%1.txt").arg(timestamp);
}

void LoggingController::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	// Always chain to previous handler first (console output, etc.)
	if (previousHandler) {
		previousHandler(type, context, msg);
	}

	if (!s_loggingEnabled.load()) {
		return;
	}

	// Enqueue message for async writing (non-blocking)
	QMutexLocker locker(&s_workerMutex);
	if (s_worker && s_worker->isRunning()) {
		s_worker->enqueueMessage(qFormatLogMessage(type, context, msg));
	}
}

void LoggingController::startWorker(const QDir& logDirectory)
{
	QMutexLocker locker(&s_workerMutex);
	
	// Don't start if already running
	if (s_workerThread && s_workerThread->isRunning()) return;

	// Ensure log directory exists
	if (!logDirectory.exists()) {
		QDir().mkpath(logDirectory.absolutePath());
	}

	QString logFilePath = logDirectory.absolutePath() + "/" + generateLogFileName();

	// Create worker and thread
	s_worker = new LogWriterWorker();
	s_workerThread = new QThread();

	// Connect worker start - capture logFilePath by value, use local pointer
	LogWriterWorker* worker = s_worker;
	connect(s_workerThread, &QThread::started, worker, [worker, logFilePath]() {
		worker->start(logFilePath);
	});
	
	// Connect error signal with queued connection to avoid cross-thread issues
	connect(s_worker, &LogWriterWorker::errorOccurred, this, &LoggingController::onWorkerError, Qt::QueuedConnection);

	// Move worker to thread and start
	s_worker->moveToThread(s_workerThread);
	s_workerThread->start();
}

void LoggingController::stopWorker()
{
	QThread* threadToStop = nullptr;
	LogWriterWorker* workerToStop = nullptr;

	{
		QMutexLocker locker(&s_workerMutex);

		if (!s_workerThread || !s_worker) {
			return;
		}

		// Take ownership of pointers
		threadToStop = s_workerThread;
		workerToStop = s_worker;
		
		// Clear static pointers immediately so no new messages are enqueued
		s_worker = nullptr;
		s_workerThread = nullptr;
	}

	// Disconnect all signals from the worker to prevent any callbacks after we start cleanup
	workerToStop->disconnect();

	// Call stop directly using BlockingQueuedConnection
	// This safely executes stop() on the worker thread and waits for it to complete
	if (threadToStop->isRunning()) {
		bool invoked = QMetaObject::invokeMethod(workerToStop, "stop", Qt::BlockingQueuedConnection, Q_ARG(bool, true));
		if (!invoked) {
			// If invoke failed, the thread may not be running an event loop yet
			// Just proceed to quit
		}
	}

	// Now quit the thread's event loop
	threadToStop->quit();
	
	// Wait for thread to finish with timeout
	if (!threadToStop->wait(3000)) {
		// Force terminate if it doesn't stop gracefully
		threadToStop->terminate();
		threadToStop->wait();
	}

	// Clean up - safe to delete now since thread is stopped
	delete workerToStop;
	delete threadToStop;
}
