#pragma once

#include <QWidget>
#include "ui_previewcontainerwidget.h"

class PreviewContainerWidget : public QWidget
{
	Q_OBJECT

public:
	PreviewContainerWidget(QWidget *parent = nullptr);
	~PreviewContainerWidget();

private:
	Ui::PreviewContainerWidgetClass ui;
};
