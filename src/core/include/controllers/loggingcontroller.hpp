#pragma once

#include <QObject>
#include <QThread>
#include "controllers/settingscontroller.hpp"
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <atomic>
#include "logging/logwriterworker.hpp"

/**
 * @brief A controller class for logging messages at various severity levels.
 * Uses a dedicated worker thread for non-blocking file I/O. Log messages are enqueued and written asynchronously, so logging calls never block on disk I/O.
 */
class LoggingController : public QObject
{
	Q_OBJECT

public:
	LoggingController(SettingsController& sc, QObject *parent);
	~LoggingController();

	void clearLogs();

	static void info(const QString& message);
	static void warning(const QString& message);
	static void critical(const QString& message);
	static void fatal(const QString& message);
	static void debug(const QString& message);

private slots:
	void onEnableLoggingChanged(bool enabled);
	void onUseUniqueLogFileNamesChanged(bool useUniqueNames);
	void onWorkerError(const QString& error);

private:
	SettingsController& m_settingsController;

	/**
	 * @brief Starts the LogWriterWorker in a dedicated thread to avoid I/O blocking.
	 * @param logDirectory 
	 */
	void startWorker(const QDir& logDirectory);

	/**
	 * @brief Stops the LogWriterWorker and cleans up the thread.
	 */
	void stopWorker();

	/**
	 * @brief Generates the log file name using current settings. Will always start with "log" prefix.
	 * @return The generated log file name. If unique names are enabled, appends a timestamp.
	 */
	static QString generateLogFileName();

	/**
	 * @brief The custom message handler that forwards log messages to the LogWriterWorker.
	 * @param type
	 * @param context
	 * @param msg
	 */
	static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

	static QMutex s_workerMutex;
	static QThread* s_workerThread;
	static LogWriterWorker* s_worker;
	static std::atomic<bool> s_loggingEnabled;
	static std::atomic<bool> s_useUniqueLogFileNames;
	static QtMessageHandler previousHandler;
};

