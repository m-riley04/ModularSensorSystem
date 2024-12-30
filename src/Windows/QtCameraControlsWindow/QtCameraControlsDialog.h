#pragma once

#include <QDialog>
#include "ui_QtCameraControlsDialog.h"
#include "../../Sensors/Camera.h"
#include <QtConcurrent>
#include <QtConcurrent/qtconcurrentfilter.h>
#include "../../Widgets/QtCameraFormatTableWidgetItem/QtCameraFormatTableWidgetItem.h"
#include <algorithm>

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
	
	void populateFilterDropdowns();

	void initializeFormatGroup(Camera* camera);
	void initializeSettingsGroup(Camera* camera);

	void connectPropertyControls(Camera* camera);
	void connectFormatControls();

	void resetFilters();

public slots:
	void updateFormatTable();
};
