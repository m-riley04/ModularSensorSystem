#pragma once

#include <QObject>
#include <QFileDialog>

class Sensor : public QObject
{
	Q_OBJECT

private:
	QUrl outputDir; /// TODO: Maybe change this to QDir 

public:
	Sensor(QObject *parent);
	~Sensor();

public slots:
	void setOutputDirectory(QUrl dir);
	QUrl& getOutputDirectory();
};
