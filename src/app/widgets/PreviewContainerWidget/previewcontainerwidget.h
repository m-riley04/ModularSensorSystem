#pragma once

#include <QWidget>
#include "ui_previewcontainerwidget.h"
#include <QList>
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
	QList<SourcePreviewWidget*> mDevicePreviewWidgets;

	MainController* pController = nullptr;

	void initSignals();

public slots:
	void addDeviceWidget(Device* device);
	void removeDeviceWidget(Device* device);

signals:
	void deviceWidgetAdded(SourcePreviewWidget* widget);
	void deviceWidgetRemoved(SourcePreviewWidget* widget);

};
