#pragma once

#include <QObject>

class ProcessingController  : public QObject
{
	Q_OBJECT

public:
	ProcessingController(QObject *parent);
	~ProcessingController();
};

