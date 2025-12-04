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
	void setPlugin(PluginMetadata* plugin);

signals:
	void pluginChanged(PluginMetadata* plugin);

private:
	Ui::PluginDetailsWidgetClass ui;
	PluginMetadata* m_plugin = nullptr;

	void updateUi();
};

