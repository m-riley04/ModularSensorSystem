#include "plugindetailswidget.h"

PluginDetailsWidget::PluginDetailsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PluginDetailsWidget::~PluginDetailsWidget()
{
}

void PluginDetailsWidget::setPlugin(IPlugin* plugin) {
	if (m_plugin == plugin) return;
	m_plugin = plugin;
	emit pluginChanged(m_plugin);
	updateUi();
}

void PluginDetailsWidget::updateUi() {
	if (!m_plugin) {
		ui.labelPluginName->setText("N/A");
		ui.labelPluginVersion->setText("N/A");
		ui.labelPluginType->setText("N/A");
		return;
	}

	ui.labelPluginName->setText(QString::fromStdString(m_plugin->name()));
	ui.labelPluginVersion->setText(QString::number(m_plugin->version()));
	ui.labelPluginType->setText(QString::fromStdString(pluginTypeToString(m_plugin->type())));
}