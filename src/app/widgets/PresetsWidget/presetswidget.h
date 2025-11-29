#pragma once

#include <QWidget>
#include "ui_presetswidget.h"
#include "controllers/maincontroller.hpp"
#include <QtWidgets>

class PresetsWidget : public QWidget
{
	Q_OBJECT

public:
	PresetsWidget(QWidget *parent = nullptr);
	~PresetsWidget();

	QListWidget* listWidget() const { return ui.listPresets; }

	void setController(MainController* controller) {
		if (m_controller == controller) return;
		m_controller = controller;
		initWidgets();
		initSignals();
	}

private slots:
	void onRemoveClicked();
	void onSaveClicked();
	void onLoadClicked();
	void onSelected(QListWidgetItem* item);
	void onDoubleClicked(QListWidgetItem* item);
	void onRefreshClicked();
	void repopulateList();

private:
	Ui::PresetsWidgetClass ui;
	MainController* m_controller = nullptr;
	QListWidgetItem* pSelectedItem = nullptr;
	bool mControlsVisible = false;

	void initWidgets();
	void initSignals();

signals:
	void selectedPresetChanged(QListWidgetItem* current, QListWidgetItem* previous);
};
