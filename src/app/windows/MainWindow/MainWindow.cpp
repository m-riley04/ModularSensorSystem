#include "MainWindow.h"
#include <dialogs/AddSourceDialog/addsourcedialog.h>
#include <dialogs/AddProcessorDialog/addprocessordialog.h>
#include <dialogs/AddMountDialog/addmountdialog.h>
#include "dialogs/PluginsDialog/pluginsdialog.h"
#include "widgets/DockedMountControls/dockedmountcontrols.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <utils/debug.hpp>

AppActions MainWindow::createActions() {
    PresetActions* presetActions = new PresetActions{
        .savePreset = ui.actionSavePreset,
        .loadPreset = ui.actionLoadPreset,
        .deletePreset = ui.actionDeletePreset,
        .refreshPreset = ui.actionRefreshPresets
    };

    MountActions* mountActions = new MountActions{
        .openAddMount = ui.actionAddMount,
        .openRemoveMount = ui.actionRemoveMount,
        .openEditMount = ui.actionEditMount
    };

    SourceActions* sourceActions = new SourceActions{
        .openAddSource = ui.actionAddSource,
        .openRemoveSource = ui.actionRemoveSource,
        .openEditSource = ui.actionConfigureSource
    };

    ProcessorActions* processorActions = new ProcessorActions{
        .openAddProcessor = ui.actionAddProcessor,
        .openRemoveProcessor = ui.actionRemoveProcessor,
        .openEditProcessor = ui.actionConfigureProcessor,
        .toggleProcessing = ui.actionToggleProcessing
    };

    SessionActions* sessionActions = new SessionActions{
        .toggleSession = ui.actionStartStopSession,
        .restartSession = ui.actionRestartSession,
        .toggleRecording = ui.actionRecord,
        .clipSession = ui.actionClipSession
    };

    ViewActions* viewActions = new ViewActions{
        .viewPresetsList = ui.actionViewPresetsList,
        .viewControls = ui.actionViewControls,
        .viewMenuBar = ui.actionViewMenuBar,
        .viewToolbar = ui.actionViewToolbar
    };

    MiscActions* miscActions = new MiscActions{
		.openPresetsDirectory = ui.actionOpenPresetsDirectory,
		.openPluginsDirectory = ui.actionOpenPluginsDirectory,
        .openLogsDirectory = ui.actionOpenLogsDirectory,
		.openAppDirectory = ui.actionOpenAppDirectory,
		.openOutputDirectory = ui.actionOpenOutputDirectory,
        .openGithubRepository = ui.actionGitHub, // adjust to existing action name if available
        .generatePipelineDiagram = ui.actionDebugPipelineDiagram,
        .openAppProperties = ui.actionProperties,
        .openPluginDialog = ui.actionOpenPluginsManager,
		.clearLogs = ui.actionClearLogs,
		.clearRecordings = ui.actionClearRecordings,
        .quit = ui.actionQuit,
        .restart = ui.actionRestart
    };

    return {
        .presetActions = presetActions,
        .mountActions = mountActions,
        .sourceActions = sourceActions,
        .processorActions = processorActions,
        .sessionActions = sessionActions,
        .viewActions = viewActions,
        .miscActions = miscActions
    };
}

MainWindow::MainWindow(MainController& mc, UiSettingsController& uisc, QWidget *parent)
    : QMainWindow(parent)
    , m_controller(mc)
	, m_uiSettingsController(uisc)
{
    ui.setupUi(this);

    // Load actions into controller
    AppActions actions = createActions();
    m_actionController = new AppActionController(&actions, m_uiSettingsController, m_controller, this, this);

	// TODO: this logic needs to be re-done and moved elsewhere. This is just a quick and dirty way to get mount controls showing up for now.
	connect(m_actionController, &AppActionController::elementSelectionChanged, this, [this](ElementTreeNode* node) {
		// Get mount from node and set in controls widget
        const Mount* mount = this->m_controller.mountController().byId(node->uuid);
        if (mount == nullptr) return;

        connect(mount, &Mount::errorOccurred, this, [this](const QString& msg) {
			QMessageBox::critical(this, tr("Mount Error"), tr("An error occurred in the mount:\n%1").arg(msg));
            });
        
		// Create new widget for mount controls
		ui.dockWidgetMountController->setMount(const_cast<Mount*>(mount)); // TODO: avoid const cast
    });

    connect(&m_controller.mountController(), &MountController::mountRemoved, this, [this](QUuid mountId) {
        ui.dockWidgetMountController->setMount(nullptr);
		});

    // Load settings
    loadAppSettings();

    // Initialize
    initWidgets();
    initSignals();
}

MainWindow::~MainWindow()
{}

void MainWindow::syncViewActionChecks()
{
    // Initialize checkable actions to reflect current widget visibility
    ui.actionViewPresetsList->setChecked(ui.groupPresets->isVisible());
    ui.actionViewControls->setChecked(ui.sessionControls->isVisible());

    ui.actionViewMenuBar->setChecked(ui.menuBar->isVisible());
    ui.actionViewToolbar->setChecked(ui.toolBar->isVisible());
}

void MainWindow::initWidgets()
{
    // Init menu bar
    syncViewActionChecks();

    // Init session controls widget
    ui.sessionControls->setSessionControlActions(&this->m_actionController->sessionActions());

    // Init preview container widget
    ui.devicePreviewWidget->setController(&m_controller);

    // Init presets widget
    ui.presetsWidget->setController(&m_controller);

    // Init elements tree
    ui.dockWidgetElementManager->setController(&m_controller);
    ui.dockWidgetElementManager->setActions(&this->m_actionController->actions());

	// Init mount controls dock widget
	ui.dockWidgetMountController->setMount(nullptr);
}

void MainWindow::initSignals() {
	SourceController& m_sourceController = m_controller.sourceController();
	SessionController& m_sessionController = m_controller.sessionController();

    // Error message propagation
    connect(&m_sessionController, &SessionController::errorOccurred, this, &MainWindow::handleSessionError);

    // Connect preset widget signals
    connect(ui.presetsWidget, &PresetsWidget::selectedPresetChanged, m_actionController, &AppActionController::onPresetElementSelected);

	// Connect dock widget signals
    connect(ui.dockWidgetElementManager, &DockableElementsManagerWidget::elementSelected, m_actionController, &AppActionController::onElementSelected);
    connect(ui.dockWidgetElementManager, &DockableElementsManagerWidget::elementRemoved, m_actionController, &AppActionController::onElementRemoved);

}

void MainWindow::loadAppSettings()
{
    m_uiSettingsController.loadSettings();
    m_controller.settingsController().loadSettings();

	restoreGeometry(m_uiSettingsController.windowSettings().geometry);
	restoreState(m_uiSettingsController.windowSettings().windowState);
}

void MainWindow::handleSessionError(const QString& errorMessage) {
    QMessageBox::critical(this, tr("Session Error"), tr("An error occurred in the session:\n%1").arg(errorMessage));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    m_uiSettingsController.setWindowSettings({
        .geometry = saveGeometry(),
        .windowState = saveState()
		});

	m_uiSettingsController.saveSettings();
    m_controller.settingsController().saveSettings();
    QMainWindow::closeEvent(event);
}