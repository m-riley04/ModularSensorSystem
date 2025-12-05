#pragma once

#include <QObject>

class LoggingController : public QObject
{
	Q_OBJECT

public:
	LoggingController(QObject *parent);
	~LoggingController();
};

