#pragma once

#include <QWidget>
#include "ui_previewcontainerwidget.h"
#include "Widgets/CustomSinkWidget/customsinkwidget.h"
#include <QList>
#include <Devices/CameraDevice/cameradevice.h>

class PreviewContainerWidget : public QWidget
{
	Q_OBJECT

public:
	PreviewContainerWidget(QWidget *parent = nullptr);
	~PreviewContainerWidget();

private:
	Ui::PreviewContainerWidgetClass ui;
	QList<CustomSinkWidget*> mVideoWidgets;

public slots:
	void addVideoWidget(CameraDevice* camera);

signals:
	void videoWidgetAdded(CustomSinkWidget* widget);

};
