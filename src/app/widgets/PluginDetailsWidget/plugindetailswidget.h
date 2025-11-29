#pragma once

#include <QWidget>
#include "ui_plugindetailswidget.h"
#include <sdk/plugins/iplugin.hpp>
#include <utils/plugins_utils.hpp>
#include <controllers/maincontroller.hpp>

class PluginDetailsWidget : public QWidget
{
	Q_OBJECT

public:
	PluginDetailsWidget(QWidget *parent = nullptr);
	~PluginDetailsWidget();

public slots:
	void setPlugin(IPlugin* plugin);

signals:
	void pluginChanged(IPlugin* plugin);

private:
	Ui::PluginDetailsWidgetClass ui;
	IPlugin* m_plugin = nullptr;

	void updateUi();
};

