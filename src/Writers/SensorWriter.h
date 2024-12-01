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
	QString m_filename;
	QUrl m_outputPath;
	QVideoFrameInput* m_frameInput;
	QMediaRecorder m_recorder;
	QMediaCaptureSession mSession;
	QMediaFormat m_format;
	qint64 m_previousTimestamp = 0;
	qint64 m_startTimestamp = 0;

	bool m_isRecording = false;

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
