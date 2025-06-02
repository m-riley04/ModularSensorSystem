#pragma once

#include <QDialog>
#include "ui_videodevicerecordingpropertieswidget.h"
#include <QtMultimedia>
#include <qfiledialog.h>
#include <QMessageBox>

enum RecordingControlsTab {
	GENERAL,
	VIDEO,
	AUDIO
};

class VideoDeviceRecordingPropertiesWidget : public QDialog
{
	Q_OBJECT

public:
	VideoDeviceRecordingPropertiesWidget(QWidget* parent = nullptr, QMediaRecorder* recorder = nullptr);
	~VideoDeviceRecordingPropertiesWidget();

	static QString recorderStateDescription(QMediaRecorder::RecorderState state);

private:
	Ui::VideoDeviceRecordingPropertiesWidgetClass ui;
	QMediaRecorder* pRecorder = nullptr;
	QMediaCaptureSession* pCaptureSession = nullptr;
	QAudioInput* pAudioInput = nullptr;
	QCamera* pCamera = nullptr;

	void initWidgets();
	void initSignals();

	template <typename Enum>
	void populateEnumDropdown(QComboBox* dropdown, QList<Enum> enumList,
		std::function<QString(Enum)> enumTextFunction, bool clearDropdown = false, QList<Enum> allowedEnums = {});

	void initGeneralWidgets();
	void initVideoWidgets();
	void initAudioWidgets();

	void initGeneralSignals();
	void initVideoSignals();
	void initAudioSignals();

	void initDefaultValues();
};

template<typename Enum>
inline void VideoDeviceRecordingPropertiesWidget::populateEnumDropdown(QComboBox* dropdown, QList<Enum> enumList,
	std::function<QString(Enum)> enumTextFunction, bool clearDropdown, QList<Enum> allowedEnums)
{
	if (clearDropdown) dropdown->clear();

	for (Enum codec : enumList) {
		if (allowedEnums.contains(codec)) dropdown->addItem(enumTextFunction(codec), QVariant(static_cast<int>(codec)));
	}
}
