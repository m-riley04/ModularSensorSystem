#pragma once

#include <QDialog>
#include <QtConcurrent>
#include "Sensors/Camera.h"
#include "ui_CameraPropertiesWindow.h"

#define DEFAULT_BRIGHTNESS 0
#define DEFAULT_CONTRAST 10
#define DEFAULT_SATURATION 10
#define DEFAULT_GAIN 100
#define DEFAULT_EXPOSURE 0
#define DEFAULT_BACKLIGHT 0
#define DEFAULT_WHITE_BALANCE 4500
#define DEFAULT_AUTO_EXPOSURE 1

class CameraPropertiesWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::CameraPropertiesWindowClass ui;
	Camera* camera;

	int brightnessInitial;
	int contrastInitial;
	int saturationInitial;
	int gainInitial;
	int exposureInitial;
	bool backlightInitial;
	bool autoExposureInitial;

	void initWidgets();
	void initSignals();
	void updateDeviceDropdown();

public:
	CameraPropertiesWindow(QWidget *parent = nullptr, Camera *camera = nullptr);
	~CameraPropertiesWindow();

public slots:
	void restoreDefaults();
	void restoreInitialChanges();

	void dialogButtonClicked(QAbstractButton* button);
	
signals:
	void brightnessChanged(int value);
	void contrastChanged(int value);
	void saturationChanged(int value);
	void gainChanged(int value);
	void exposureChanged(int value);
	void backlightChanged(bool value);
	void autoExposureChanged(bool value);

};
