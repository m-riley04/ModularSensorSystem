#pragma once

#include <QWidget>
#include "ui_previewcontainerwidget.h"
#include "Widgets/CustomSinkWidget/customsinkwidget.h"
#include <QList>
#include <Devices/CameraDevice/cameradevice.h>
#include <Controllers/MainController/maincontroller.h>

class PreviewContainerWidget : public QWidget
{
	Q_OBJECT

public:
	PreviewContainerWidget(QWidget *parent = nullptr);
	~PreviewContainerWidget();

	void setController(MainController* controller) { 
		if (pController == controller) return;
		pController = controller;
		initSignals();
	}

private:
	Ui::PreviewContainerWidgetClass ui;
	QList<CustomSinkWidget*> mVideoWidgets;

	MainController* pController = nullptr;

	void initSignals();
	CustomSinkWidget* addVideoWidget(CameraDevice* camera);

public slots:
	void addDeviceWidget(Device* device);
	void removeDeviceWidget(Device* device);

signals:
	void deviceWidgetAdded(CustomSinkWidget* widget);
	void deviceWidgetRemoved(CustomSinkWidget* widget);

};
