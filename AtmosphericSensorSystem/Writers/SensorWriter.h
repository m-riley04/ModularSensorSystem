#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QDir>
#include <QUrl>
#include <QtMultimedia>

class SensorWriter : public QObject
{
	Q_OBJECT

private:
	QString filename;
	QUrl outputPath;
	QVideoFrameInput* frameInput;
	QMediaRecorder recorder;
	QMediaCaptureSession session;
	QMediaFormat format;

	bool isRecording = false;

public:
	SensorWriter(QObject* parent = nullptr);
	~SensorWriter();

public slots:
	void startRecording();
	void stopRecording();
	void write(const QVariant& data, const qint64 timestamp);

signals:
	void writeFinished(const qint64 timestamp);
};
