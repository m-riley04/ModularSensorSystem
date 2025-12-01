#pragma once

#include <QObject>
#include <QWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <qinputdialog.h>
#include <controllers/maincontroller.hpp>
#include <dialogs/SessionPropertiesDialog/sessionpropertiesdialog.h>
#include <dialogs/AppPropertiesDialog/apppropertiesdialog.h>
#include <dialogs/PluginsDialog/pluginsdialog.h>
#include <dialogs/AddMountDialog/addmountdialog.h>
#include <dialogs/AddSourceDialog/addsourcedialog.h>
#include <dialogs/AddProcessorDialog/addprocessordialog.h>
#include "action_containers.h"

// Forward declarations
struct ElementTreeNode;

class AppActionController : public QObject
{
	Q_OBJECT

public:
    AppActionController(AppActions* actions, MainController& c, QWidget* parentWidget, QObject *parent);
	~AppActionController();

	AppActions& actions() { return m_actions; }
	PresetActions& presetActions() { return m_presetActions; }
	MountActions& mountActions() { return m_mountActions; }
	SourceActions& sourceActions() { return m_sourceActions; }
	ProcessorActions& processorActions() { return m_processorActions; }
	SessionActions& sessionActions() { return m_sessionActions; }
	ViewActions& viewActions() { return m_viewActions; }

public slots:
    void onElementSelected(ElementTreeNode* node);
	void onElementRemoved();
	void onPresetElementSelected(QListWidgetItem* current, QListWidgetItem* previous);

private slots:
    void onOpenSavePresetDialog();
    void onLoadPresetClicked();
    void onOpenDeletePresetDialog();
    void onOpenEditPresetDialog();
    void onRefreshPresetClicked();

    void onOpenAddMountDialog();
    void onOpenRemoveMountDialog();
    void onOpenEditMountDialog();

    void onOpenAddSourceDialog();
    void onOpenRemoveSourceDialog();
    void onOpenEditSourceDialog();

    void onOpenAddProcessorDialog();
    void onOpenRemoveProcessorDialog();
    void onOpenEditProcessorDialog();
	void onToggleProcessing();

    void onToggleSession(bool);
    void onToggleRecording(bool);
	void onOpenSessionProperties();
    void onClipSession();

    void onOpenGithubRepository();
    void onPrintPipelineDebugClicked();
    void onOpenAppPropertiesDialog();
    void onOpenPluginDialog();

	void quit();
	void restart();

private:
    void initActionSignals();

    void refreshActionStates();
	void refreshPresetActionStates();
    void refreshSourceActionStates();
    void refreshMountActionStates();
    void refreshProcessorActionStates();
    void refreshSessionActionStates();

private:
    QWidget* m_parentWidget;
	MainController& m_controller;
    AppActions m_actions;
	PresetActions m_presetActions;
    MountActions m_mountActions;
    SourceActions m_sourceActions;
	ProcessorActions m_processorActions;
	SessionActions m_sessionActions;
	ViewActions m_viewActions;
    MiscActions m_miscActions;
    
	ElementTreeNode* m_currentSelectedElementNode = nullptr; // TODO: should these even exist?
	QListWidgetItem* m_currentSelectedPresetItem = nullptr;
};

