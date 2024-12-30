#pragma once

#include <QDialog>
#include "ui_QtCameraControlsDialog.h"
#include "../../Sensors/Camera.h"
#include <QtConcurrent>
#include <QtConcurrent/qtconcurrentfilter.h>

enum FormatFilter
{
	FORMAT_FILTER_FPS,
	FORMAT_FILTER_RESOLUTION,
	FORMAT_FILTER_PIXEL_FORMAT
};

class QtCameraControlsDialog : public QDialog
{
	Q_OBJECT

public:
	QtCameraControlsDialog(Camera* camera, QWidget *parent = nullptr);
	~QtCameraControlsDialog();

	static QString sizeToString(QSize size);
	static QSize stringToSize(const QString& str);

private:
	Ui::QtCameraControlsDialogClass ui;
	Camera* pCamera;
	QList<QCameraFormat> mFormats;
	QCameraFormat defaultFormat;
	float selectedFps;
	QString selectedResolution;
	QVideoFrameFormat::PixelFormat selectedPixelFormat;
	
	int populateFpsDropdown(QString resolution, QVideoFrameFormat::PixelFormat pixelFormat);
	QString populateResolutionDropdown(float fps, QVideoFrameFormat::PixelFormat pixelFormat);
	QVideoFrameFormat::PixelFormat populatePixelFormatDropdown(float fps, QString resolution);

	void initializeFormatGroup(Camera* camera);
	void initializeSettingsGroup(Camera* camera);

	void connectPropertyControls(Camera* camera);
	void connectFormatControls();

	void resetFormatDropdowns();

public slots:
	void onFpsChanged(int index);
	void onResolutionChanged(int index);
	void onPixelFormatChanged(int index);
	void onFilterChanged(int id);
};
