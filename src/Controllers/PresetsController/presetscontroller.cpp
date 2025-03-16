#include "presetscontroller.h"

const string KEY_AUTO_STOP = "auto_stop";
const string KEY_QUALITY = "quality";
const string KEY_ENCODING_MODE = "encoding_mode";

const string KEY_VIDEO_CODEC = "codec";
const string KEY_VIDEO_BITRATE = "bitrate";
const string KEY_FRAME_RATE = "frame_rate";
const string KEY_RESOLUTION_WIDTH = "resolution_width";
const string KEY_RESOLUTION_HEIGHT = "resolution_height";

const string KEY_AUDIO_CODEC = "codec";
const string KEY_AUDIO_BITRATE = "bitrate";
const string KEY_SAMPLE_RATE = "sample_rate";
const string KEY_CHANNELS = "channels";

PresetsController::PresetsController(QObject *parent)
	: QObject(parent)
{}

PresetsController::~PresetsController()
{}

void PresetsController::writeGeneralRecordingPreset(QString presetPath, bool autoStop, Quality quality, EncodingMode encodingMode)
{
	QSettings preset(presetPath, QSettings::Format::IniFormat);

	preset.beginGroup("General Recording");
	preset.setValue(KEY_AUTO_STOP, QVariant(autoStop));
	preset.setValue(KEY_QUALITY, QVariant(autoStop));
	preset.setValue(KEY_ENCODING_MODE, QVariant(autoStop));
	preset.endGroup();
}
