#pragma once

#include <QWidget>
#include "ui_devicepreviewwidget.h"
#include "sources/Source/source.h"
#include <sources/Source/SourcePreview/sourcepreview.h>
#include <QPainter>

class DevicePreviewWidget : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent* event);

public:
	DevicePreviewWidget(SourcePreview* preview, QWidget *parent = nullptr);
	~DevicePreviewWidget();

	const SourcePreview* preview() const { return pPreview; }

private:
	Ui::DevicePreviewWidgetClass ui;
	QImage mFrame;

	const SourcePreview* pPreview = nullptr;

private slots:
	void setFrame(const QImage&);
	
};
