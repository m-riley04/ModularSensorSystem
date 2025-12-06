#pragma once

#include <QObject>
#include "controllers/settingscontroller.hpp"
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <atomic>

/**
 * @brief A controller class for logging messages at various severity levels.
 * TODO: Utilize multithreading to ensure there is no blocking I/O during logging operations.
 * Currently, it's basically just an abstraction over Qt's logging functions so I can add more complex features later.
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

	static QString generateLogFileName(bool unique = true);
	static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private slots:
	void onEnableLoggingChanged(bool enabled);

private:
	SettingsController& m_settingsController;

	static QMutex logMutex;
	static QFile* logFile;
	static std::atomic<bool> s_loggingEnabled;
	static QtMessageHandler previousHandler;
};

