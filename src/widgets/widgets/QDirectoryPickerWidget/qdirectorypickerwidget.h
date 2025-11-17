#pragma once

#include <QWidget>
#include "ui_qdirectorypickerwidget.h"

class QDirectoryPickerWidget : public QWidget
{
	Q_OBJECT

public:
	QDirectoryPickerWidget(QWidget *parent = nullptr);
	~QDirectoryPickerWidget();

private:
	Ui::QDirectoryPickerWidgetClass ui;
};

