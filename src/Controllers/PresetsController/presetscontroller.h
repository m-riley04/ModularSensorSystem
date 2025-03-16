#pragma once

#include <QObject>
#include <QSettings>
#include <QtMultimedia>

using Quality = QMediaRecorder::Quality;
using EncodingMode = QMediaRecorder::EncodingMode;
//using FileFormat = QMediaRecorder::FileFormat;
using string = std::string;

class PresetsController  : public QObject
{
	Q_OBJECT

public:
	PresetsController(QObject *parent);
	~PresetsController();

	static void writeGeneralRecordingPreset(QString presetPath, bool autoStop, Quality quality, EncodingMode encodingMode);
};
