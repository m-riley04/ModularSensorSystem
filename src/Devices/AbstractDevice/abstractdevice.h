#pragma once

#include <QObject>

class AbstractDevice : public QObject
{
	Q_OBJECT

public:
	AbstractDevice(QObject *parent);
	~AbstractDevice();

	virtual void open() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void close() = 0;
	virtual void restart() = 0;

signals:
	void opened();
	void started();
	void stopped();
	void closed();
};
