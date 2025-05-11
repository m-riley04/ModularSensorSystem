#pragma once

#include <QWidget>
#include "ui_sensorscontaineritemwidget.h"
#include <qevent.h>
#include <QDrag>
#include <QMimeData>
#include "Widgets/CustomSinkWidget/customsinkwidget.h"

class SensorsContainerItemWidget : public QWidget
{
	Q_OBJECT

public:
	SensorsContainerItemWidget(QWidget* parent = nullptr);
	SensorsContainerItemWidget(QWidget *parent = nullptr, CustomSinkWidget* cameraSinkWidget = nullptr);
	~SensorsContainerItemWidget();

	CustomSinkWidget* cameraSinkWidget() { return pCameraSinkWidget; }

private:
	Ui::SensorsContainerItemWidgetClass ui;

	QPoint dragStartPosition;
	CustomSinkWidget* pCameraSinkWidget = nullptr;

	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

public slots:
	void setWidget(CustomSinkWidget* widget); // This is for cameras
	//void setWidget(SensorSinkWidget* widget); // This is for sensors

signals:
	void widgetChanged(CustomSinkWidget* widget);
	//void widgetChanged(SensorSinkWidget* widget);
};
