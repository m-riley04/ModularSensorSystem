#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>

class SensorWriter : public QObject
{
	Q_OBJECT

public:
	SensorWriter(QObject* parent = nullptr);
	~SensorWriter();

public slots:
	void write(const QVariant& data, const qint64 timestamp);

signals:
	void writeFinished(const qint64 timestamp);
};
