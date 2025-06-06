#pragma once

#include <QWidget>
#include "ui_sourcepreviewwidget.h"
#include "sources/Source/source.h"
#include <sources/Source/SourcePreview/sourcepreview.h>
#include <QPainter>

class SourcePreviewWidget : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent* event);

public:
	SourcePreviewWidget(SourcePreview* preview, QWidget *parent = nullptr);
	~SourcePreviewWidget();

	const SourcePreview* preview() const { return pPreview; }

private:
	Ui::SourcePreviewWidgetClass ui;
	QImage mFrame;

	const SourcePreview* pPreview = nullptr;

private slots:
	void setFrame(const QImage&);
	
};
