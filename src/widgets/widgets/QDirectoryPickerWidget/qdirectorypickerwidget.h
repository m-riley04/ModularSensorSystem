#pragma once

#include <QWidget>
#include "ui_qdirectorypickerwidget.h"
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>
#include <qmessagebox.h>

class QDirectoryPickerWidget : public QWidget
{
	Q_OBJECT

public:
	QDirectoryPickerWidget(QWidget *parent = nullptr);
	~QDirectoryPickerWidget();

private slots:
	void onOpenDirectoryClicked();
	void onSelectDirectoryClicked();
	void setSelectedDirectory(const QDir& directory);

private:
	Ui::QDirectoryPickerWidgetClass ui;

	QDir m_selectedDirectory;

signals:
	void directoryChanged(const QDir& newDirectory);
};

