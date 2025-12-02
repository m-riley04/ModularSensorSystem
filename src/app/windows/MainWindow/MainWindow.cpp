#include "MainWindow.h"
#include <dialogs/AddSourceDialog/addsourcedialog.h>
#include <dialogs/AddProcessorDialog/addprocessordialog.h>
#include <dialogs/AddMountDialog/addmountdialog.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <utils/debug.hpp>
#include <dialogs/AppPropertiesDialog/apppropertiesdialog.h>

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
        .openGithubRepository = ui.actionGitHub, // adjust to existing action name if available
        .generatePipelineDiagram = ui.actionDebugPipelineDiagram,
        .openAppProperties = ui.actionProperties,
        .openPluginDialog = ui.actionOpenPluginsManager,
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_appSettings(QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName()))
    , m_controller(MainController(m_appSettings, this))
	, m_uiSettingsController(UiSettingsController(m_appSettings, this))
{
    ui.setupUi(this);

    // Load actions into controller
    AppActions actions = createActions();
    m_actionController = new AppActionController(&actions, m_uiSettingsController, m_controller, this, this);

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
    ui.dockWidget->setController(&m_controller);
    ui.dockWidget->setActions(&this->m_actionController->actions());
}

void MainWindow::initSignals() {
	SourceController& m_sourceController = m_controller.sourceController();
	SessionController& m_sessionController = m_controller.sessionController();

    // Error message propagation
    connect(&m_sessionController, &SessionController::errorOccurred, this, &MainWindow::handleSessionError);

    // Connect preset widget signals
    connect(ui.presetsWidget, &PresetsWidget::selectedPresetChanged, m_actionController, &AppActionController::onPresetElementSelected);

	// Connect dock widget signals
    connect(ui.dockWidget, &DockableElementsManagerWidget::elementSelected, m_actionController, &AppActionController::onElementSelected);
    connect(ui.dockWidget, &DockableElementsManagerWidget::elementRemoved, m_actionController, &AppActionController::onElementRemoved);

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