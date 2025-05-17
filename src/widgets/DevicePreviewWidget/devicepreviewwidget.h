#pragma once

#include <QWidget>
#include "ui_devicepreviewwidget.h"
#include <devices/Device/device.h>
#include <devices/Device/DevicePreview/devicepreview.h>
#include <QPainter>

class DevicePreviewWidget : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent* event);

public:
	DevicePreviewWidget(DevicePreview* preview, QWidget *parent = nullptr);
	~DevicePreviewWidget();

	const DevicePreview* preview() const { return pPreview; }

private:
	Ui::DevicePreviewWidgetClass ui;
	QImage mFrame;

	const DevicePreview* pPreview = nullptr;

private slots:
	void setFrame(const QImage&);
	
};
