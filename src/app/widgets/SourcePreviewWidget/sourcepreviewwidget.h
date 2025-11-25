#pragma once

#include <QWidget>
#include "ui_sourcepreviewwidget.h"
#include "features/sources/source.hpp"
#include <QPainter>

class SourcePreviewWidget : public QWidget
{
	Q_OBJECT

public:
	SourcePreviewWidget(Source* source, QWidget *parent = nullptr);
	~SourcePreviewWidget();

	Source* source() const { return m_source; }

private:
	Ui::SourcePreviewWidgetClass ui;
	Source* m_source;
	
};
