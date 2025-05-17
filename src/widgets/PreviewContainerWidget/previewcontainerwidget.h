#pragma once

#include <QWidget>
#include "ui_previewcontainerwidget.h"
#include "Widgets/CustomSinkWidget/customsinkwidget.h"
#include <QList>
#include "devices/VideoDevice/videodevice.h"
#include <Controllers/MainController/maincontroller.h>
#include "devices/Device/DevicePreview/devicepreview.h"
#include "widgets/DevicePreviewWidget/devicepreviewwidget.h"

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
	QList<DevicePreviewWidget*> mDevicePreviewWidgets;

	MainController* pController = nullptr;

	void initSignals();

public slots:
	void addDeviceWidget(Device* device);
	void removeDeviceWidget(Device* device);

signals:
	void deviceWidgetAdded(DevicePreviewWidget* widget);
	void deviceWidgetRemoved(DevicePreviewWidget* widget);

};
