#pragma once

#include <QWidget>
#include "ui_presetswidget.h"
#include <controllers/MainController/maincontroller.h>
#include <QtWidgets>

class PresetsWidget : public QWidget
{
	Q_OBJECT

public:
	PresetsWidget(QWidget *parent = nullptr);
	~PresetsWidget();

	void setController(MainController* controller) {
		if (pController == controller) return;
		pController = controller;
		initSignals();
	}

private slots:
	void onRemoveClicked();
	void onSaveClicked();
	void onLoadClicked();
	void onSelected(QListWidgetItem* item);
	void onDoubleClicked(QListWidgetItem* item);

private:
	Ui::PresetsWidgetClass ui;
	MainController* pController = nullptr;
	QListWidgetItem* pSelectedItem = nullptr;

	void initSignals();
};
