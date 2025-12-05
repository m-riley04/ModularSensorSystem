#pragma once

#include <QObject>
#include "controllers/settingscontroller.hpp"

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

	void info(const QString& message);
	void warning(const QString& message);
	void critical(const QString& message);
	void fatal(const QString& message);
	void debug(const QString& message);

	static QString generateLogFileName(bool unique = true);
	static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private:
	SettingsController& m_settingsController;
	QFile* m_logFile = nullptr;
	QTextStream* m_logStream = nullptr;
};

