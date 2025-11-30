#include "plugindetailswidget.h"
#include <utils/element_utils.hpp>

PluginDetailsWidget::PluginDetailsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PluginDetailsWidget::~PluginDetailsWidget()
{
}

void PluginDetailsWidget::setPlugin(PluginMetadata* plugin) {
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
		ui.labelPluginDescription->setText("N/A");
		return;
	}

	ui.labelPluginName->setText(QString::fromStdString(m_plugin->name));
	ui.labelAuthor->setText(QString::fromStdString(m_plugin->author));
	ui.labelPluginVersion->setText(QString::number(m_plugin->version));
	ui.labelPluginType->setText(QString::fromStdString(elementTypeToString(m_plugin->type)));
	ui.labelPluginDescription->setText(QString::fromStdString(m_plugin->description));
}