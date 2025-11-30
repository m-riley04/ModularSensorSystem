#pragma once

#include <QDialog>
#include "ui_pluginsdialog.h"
#include <QMenu>
#include <QTreeView>
#include <models/PluginListModel/pluginlistmodel.h>
#include <sdk/plugins/iplugin.hpp>

class PluginsDialog : public QDialog
{
	Q_OBJECT

public:
	PluginsDialog(PluginController& c, QWidget *parent = nullptr);
	~PluginsDialog();

private slots:
	void onPluginSelected(const QModelIndex& current, const QModelIndex& newIdx);
	void onCustomContextMenuRequested(const QPoint& pos);
	void onRescanPlugins();
	void onToggleSelectedPlugin();

private:
	Ui::PluginsDialogClass ui;
	PluginController& m_pluginController;
	PluginListModel* m_model = nullptr;
	PluginMetadata* m_selectedPlugin = nullptr;
	bool m_isSelectedPluginLoaded = true;

	QMenu* m_contextMenu = nullptr;
	QAction* m_actionTogglePlugin = nullptr;

	void initModel();
	void initWidgets();
	void initSignals();
	void initContextMenu();
	
};

