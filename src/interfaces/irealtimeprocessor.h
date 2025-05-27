#pragma once

#include <QObject>

class IRealTimeProcessor  : public QObject
{
	Q_OBJECT

public:
	IRealTimeProcessor(QObject *parent);
	~IRealTimeProcessor();
};

