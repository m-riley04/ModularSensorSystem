#include "MainWindow.h"
#include <dialogs/AddSourceDialog/addsourcedialog.h>
#include <dialogs/AddProcessorDialog/addprocessordialog.h>
#include <dialogs/AddMountDialog/addmountdialog.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include <utils/debug.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), pController(new MainController(this))
{
    ui.setupUi(this);

    // Initialize
    initWidgets();
    initSignals();
}

MainWindow::~MainWindow()
{}

ElementTreeActions MainWindow::getElementTreeActions() const
{
    ElementTreeActions actions{};
    actions.addSource = ui.actionAddSource;
    actions.removeSource = ui.actionRemoveSource;
    actions.editSource = ui.actionConfigureSource;
    actions.addMount = ui.actionAddMount;
    actions.removeMount = ui.actionRemoveMount;
    actions.editMount = ui.actionEditMount;
    actions.addProcessor = ui.actionAddProcessor;
    actions.removeProcessor = ui.actionRemoveProcessor;
    actions.editProcessor = ui.actionConfigureProcessor;
    return actions;
}

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

    // Init toolbar
    updateToolbarButtonsState();

    // Init session controls widget
    SessionControlsActions sessionActions{
		.startStopSession = ui.actionStartStopSession,
		.recordSession = ui.actionRecord,
		.restartSession = ui.actionRestartSession,
		.clipSession = ui.actionClipSession,
		.openSessionProperties = ui.actionSessionProperties
	};
    ui.sessionControls->setSessionControlActions(sessionActions);

    // Init preview container widget
    ui.devicePreviewWidget->setController(pController);

    // Init presets widget
    ui.presetsWidget->setController(pController);

    // Init elements tree
    ui.dockWidget->setController(pController);
    ui.dockWidget->setActions(this->getElementTreeActions());
}

void MainWindow::initActionSignals()
{
    if (!pController->checkIfControllersAreOk()) return;

    // Presets
	connect(ui.actionSavePreset, &QAction::triggered, this, &MainWindow::openSavePresetDialog);
    connect(ui.actionLoadPreset, &QAction::triggered, this, &MainWindow::onLoadPresetClicked);
	connect(ui.actionDeletePreset, &QAction::triggered, this, &MainWindow::openDeletePresetDialog);
    connect(ui.actionToggleProcessing, &QAction::triggered, this, [this](bool checked) {
        // TODO: implement this
		});

    // Session
    connect(ui.actionStartStopSession, &QAction::triggered, [this](bool checked) {
		if (checked) pController->sessionController()->startSession();
		else pController->sessionController()->stopSession();
        });
    connect(ui.actionRestartSession, &QAction::triggered, pController->sessionController(), &SessionController::restartSession);
    connect(ui.actionRecord, &QAction::triggered, [this](bool checked) {
        if (checked) pController->sessionController()->startRecording();
		else pController->sessionController()->stopRecording(); // use request to allow graceful stopping
        });
    connect(ui.actionSessionProperties, &QAction::triggered, [this]() {
        // Create new session properties dialog
        SessionPropertiesDialog* dialog = new SessionPropertiesDialog(pController, &pController->sessionController()->sessionProperties(), this);
        connect(dialog, &SessionPropertiesDialog::settingsChanged,
            this, [this](SessionProperties data) {
                pController->sessionController()->setSessionProperties(data);
			});
        dialog->show();
        });
    connect(ui.actionClipSession, &QAction::triggered, [this]() {
		// TODO: implement this
        });

    // Mounts
	connect(ui.actionAddMount, &QAction::triggered, this, &MainWindow::openAddMountDialog);
	connect(ui.actionRemoveMount, &QAction::triggered, this, &MainWindow::openRemoveMountDialog);
	connect(ui.actionEditMount, &QAction::triggered, this, &MainWindow::openEditMountDialog);

    // Sources
    connect(ui.actionAddSource, &QAction::triggered, this, &MainWindow::openAddSourceDialog);
    connect(ui.actionRemoveSource, &QAction::triggered, this, &MainWindow::openRemoveSourceDialog);
    connect(ui.actionConfigureSource, &QAction::triggered, this, &MainWindow::openConfigureSourceDialog);

    // Processors
	connect(ui.actionAddProcessor, &QAction::triggered, this, &MainWindow::openAddProcessorDialog);
	connect(ui.actionRemoveProcessor, &QAction::triggered, this, &MainWindow::openRemoveProcessorDialog);
    connect(ui.actionConfigureProcessor, &QAction::triggered, this, &MainWindow::openConfigureProcessorDialog);

    // View
    connect(ui.actionViewPresetsList, &QAction::triggered, [this](bool checked) {
        ui.groupPresets->setVisible(checked);
        });
    connect(ui.actionViewControls, &QAction::triggered, [this](bool checked) {
        ui.sessionControls->setVisible(checked);
        });

    connect(ui.actionViewMenuBar, &QAction::triggered, [this](bool checked) {
        ui.menuBar->setVisible(checked);
        });
    connect(ui.actionViewToolbar, &QAction::triggered, [this](bool checked) {
        ui.toolBar->setVisible(checked);
        });

    // About
    connect(ui.actionGitHub, &QAction::triggered, this, &MainWindow::openGithubRepository);

    // Debug
	connect(ui.actionDebugPipelineDiagram, &QAction::triggered, this, &MainWindow::onPrintPipelineDebugClicked);

    // Quitting/restarting
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::initSignals() {
    if (!pController->checkIfControllersAreOk()) return;
	SourceController* pSourceController = pController->sourceController();
	SessionController* pSessionController = pController->sessionController();

    // Error message propagation
    connect(pSessionController, &SessionController::errorOccurred,
        this, [this](const QString& errorMessage) {
            QMessageBox::critical(this, tr("Session Error"), tr("An error occurred in the session:\n%1").arg(errorMessage));
		});

    // Connect preset widget signals
    connect(ui.presetsWidget, &PresetsWidget::selectedPresetChanged, this, &MainWindow::onSelectedPresetItemChanged);

	// Connect dock widget signals
    connect(ui.dockWidget, &DockableElementsManagerWidget::elementSelected, this, &MainWindow::onSelectedElementChanged);
    connect(pController->sourceController(), &SourceController::sourceRemoved, ui.dockWidget, &DockableElementsManagerWidget::handleRebuildClicked);
    connect(pController->mountController(), &MountController::mountRemoved, ui.dockWidget, &DockableElementsManagerWidget::handleRebuildClicked);
    connect(pController->processingController(), &ProcessingController::processorRemoved, ui.dockWidget, &DockableElementsManagerWidget::handleRebuildClicked);

    // Init toolbar and actions
    initActionSignals();
}

void MainWindow::openSavePresetDialog()
{
    if (!pController->checkIfControllersAreOk()) return;

    // Get the preset name from the user
    bool ok;
    QString presetName = QInputDialog::getText(this, tr("Save Preset"), tr("Preset name:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !presetName.isEmpty()) {
        // Check if the preset name already exists
        // If so, ask to overwrite
        auto existingItems = ui.presetsWidget->listWidget()->findItems(presetName, Qt::MatchExactly);
        if (!existingItems.isEmpty()) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("Overwrite Preset"),
                tr("Preset already exists. Do you want to overwrite it?"),
                QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::No) {
                return;
            }
        }

        PresetsController* pPresetsController = pController->presetsController();
        pPresetsController->savePreset(presetName, pController->sourceController()->sources());
    }
    else if (ok) {
        QMessageBox::warning(this, tr("Invalid Name"), tr("Preset name cannot be empty."));
        return;
    }
}

void MainWindow::onLoadPresetClicked()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;
    
	// Check selected item
    if (!pSelectedPresetItem) {
        QMessageBox::warning(this, tr("No Preset Selected"), tr("Please select a preset to load."));
        return;
    }

    // Get the preset name from the item
    QString presetName = pSelectedPresetItem->text();

    // Find the preset in the controller
    PresetsController* pPresetsController = pController->presetsController();
    auto presets = pPresetsController->presets();
    auto it = std::find_if(presets.begin(), presets.end(), [&presetName](const Preset& preset) {
        return preset.name == presetName;
        });
    if (it != presets.end()) {
        // Load the preset
        pPresetsController->loadPreset(it->path, pController->sourceController(), pController->pluginController());
    }
}

void MainWindow::openDeletePresetDialog()
{
	// Check controllers
    if (!pController->checkIfControllersAreOk()) return;

	// Check selected item
    if (!pSelectedPresetItem) {
        QMessageBox::warning(this, tr("No Preset Selected"), tr("Please select a preset to remove."));
        return;
	}

    // Get the preset name from the item
    QString presetName = pSelectedPresetItem->text();

    // Confirm removal
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Remove Preset"),
        tr("Are you sure you want to remove the preset: %1?").arg(presetName),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;

    // Find the preset in the controller
    PresetsController* pPresetsController = pController->presetsController();
    auto presets = pPresetsController->presets();
    auto it = std::find_if(presets.begin(), presets.end(), [&presetName](const Preset& preset) {
        return preset.name == presetName;
        });
    if (it != presets.end()) {
        // Remove the preset
        pPresetsController->removePreset(it->path);
    }
	//ui.listPresets->removeItemWidget(pSelectedItem); // TODO: check if I *need* to "take" the item from the list
    delete pSelectedPresetItem;
    pSelectedPresetItem = nullptr;
}

void MainWindow::openConfigurePresetDialog()
{
    // TODO: Implement
}

void MainWindow::onRefreshPresetClicked()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;

    // Scan
    pController->presetsController()->scanForPresets(); // CONSIDER: Pass specific path?
}

void MainWindow::openAddMountDialog()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;

    // Create and show the AddMountDialog
    AddMountDialog* addDeviceDialog = new AddMountDialog(pController->pluginController(), this);
    addDeviceDialog->setWindowModality(Qt::WindowModal);

    connect(addDeviceDialog, &AddMountDialog::mountConfirmed, pController->mountController(), &MountController::addMount);

    addDeviceDialog->show();
}

void MainWindow::openRemoveMountDialog()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;

    // Check selected item
    if (m_selectedElement->kind != ElementTreeNode::Kind::Mount) {
        QMessageBox::warning(this, tr("No Mount Selected"), tr("Please select a mount to remove."));
        return;
    }

    auto response = QMessageBox::question(this, tr("Remove Mount"), tr("Are you sure you want to remove the selected mount?"), QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
        pController->mountController()->removeMount(m_selectedElement->id);
    }
}

void MainWindow::openEditMountDialog()
{
    // TODO: implement
}

void MainWindow::openAddSourceDialog()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;

	// Create and show the AddSourceDialog
    AddSourceDialog* addDeviceDialog = new AddSourceDialog(pController->pluginController(), this);
    addDeviceDialog->setWindowModality(Qt::WindowModal);

    connect(addDeviceDialog, &AddSourceDialog::sourceConfirmed, pController->sourceController(), &SourceController::addSource);

    addDeviceDialog->show();
}

void MainWindow::openRemoveSourceDialog()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;

    // Check selected item
    if (m_selectedElement->kind != ElementTreeNode::Kind::Source) {
        QMessageBox::warning(this, tr("No Source Selected"), tr("Please select a source to remove."));
        return;
	}

    auto response = QMessageBox::question(this, tr("Remove Source"), tr("Are you sure you want to remove the selected source?"), QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
        pController->sourceController()->removeSource(m_selectedElement->id);
    }
}

void MainWindow::openConfigureSourceDialog()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;

    // Check selected item
    if (m_selectedElement->kind != ElementTreeNode::Kind::Source) {
        QMessageBox::warning(this, tr("No Source Selected"), tr("Please select a source to configure."));
        return;
    }

	// Get the source from the selected element
    // TODO: check this implementation
	Source* source = pController->sourceController()->byId(m_selectedElement->id);
    if (auto cfg = qobject_cast<IConfigurableSource*>(source)) {
       /* QWidget* w = cfg->createConfigWidget(this);
        QDialog dlg(this);
        dlg.setWindowTitle(QString::fromStdString(source->name()) + " Properties");
        QVBoxLayout lay(&dlg);
        lay.addWidget(w);
        dlg.exec();*/
    }
    else {
        // fallback: show generic property inspector
        //showGenericPropertyDialog(source);
        QMessageBox::information(this, tr("Properties"), tr("No properties available for this source."));
    }
}

void MainWindow::openAddProcessorDialog()
{
	// Check controllers
    if (!pController->checkIfControllersAreOk()) return;

    ProcessingController* pProcessingController = pController->processingController();
    PluginController* pPluginController = pController->pluginController();
    if (!pProcessingController || !pPluginController) return;

    AddProcessorDialog* addProcessorDialog = new AddProcessorDialog(pPluginController, this);
    addProcessorDialog->setWindowModality(Qt::WindowModal);

    connect(addProcessorDialog, &AddProcessorDialog::processorConfirmed, pProcessingController, &ProcessingController::addProcessor);

    addProcessorDialog->show();
}

void MainWindow::openRemoveProcessorDialog()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;

    // Check selected item
    if (m_selectedElement->kind != ElementTreeNode::Kind::Processor) {
        QMessageBox::warning(this, tr("No Processor Selected"), tr("Please select a processor to remove."));
        return;
    }

    auto response = QMessageBox::question(this, tr("Remove Processor"), tr("Are you sure you want to remove the selected processor?"), QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
		// Get the processor from the selected element
        // TODO: implement this
        //ProcessorBase* processor = m_selectedElement.data(Qt::UserRole).value<ProcessorBase*>();

        // Remove source from the controller
        //pController->processingController()->removeProcessor(processor);
    }
}

void MainWindow::openConfigureProcessorDialog()
{
    QMessageBox::warning(this, tr("Feature Not Implemted"), tr("This feature has not been implemented yet."));
}

void MainWindow::openGithubRepository()
{
    QString repoLink = "https://github.com/m-riley04/ModularSensorSystem"; // TODO: Make this configurable or in a top-level file
    if (!QDesktopServices::openUrl(repoLink)) {
        QMessageBox::warning(this, tr("Link Error"), tr("Could not open the GitHub repository."));
    }
}

void MainWindow::onSelectedPresetItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (pSelectedPresetItem == current) return; // No change
    pSelectedPresetItem = current;
    updateToolbarButtonsState(); // CONSIDER: move this to it's own connection to this signal
}

void MainWindow::onSelectedElementChanged(ElementTreeNode* node)
{
	m_selectedElement = node; // TODO: check copying performance impact
    updateToolbarButtonsState();
}

void MainWindow::onSelectedElementRemoved()
{
    m_selectedElement = nullptr;
    // Update the elements tree
    ui.dockWidget->update();
    updateToolbarButtonsState();
}

void MainWindow::updateToolbarButtonsState()
{
    if (!pController->checkIfControllersAreOk()) return;

    /// PRESETS
    bool hasPresets = !pController->presetsController()->presets().isEmpty();
    ui.actionLoadPreset->setEnabled(hasPresets && pSelectedPresetItem);
    ui.actionDeletePreset->setEnabled(hasPresets && pSelectedPresetItem);

    // Check selected element
    if (!m_selectedElement) {
		// TODO: maybe disable all element-related actions here?
        return;
    }

    /// SOURCES
    bool hasSources = !pController->sourceController()->sources().isEmpty();
    ui.actionRemoveSource->setEnabled(hasSources && m_selectedElement->kind == ElementTreeNode::Kind::Source);
    ui.actionConfigureSource->setEnabled(hasSources && m_selectedElement->kind == ElementTreeNode::Kind::Source); // TODO/CONSIDER: change this action to open a dialog for ALL sources

    /// PROCESSING
    bool hasProcessors = !pController->processingController()->processors().isEmpty();
    ui.actionRemoveProcessor->setEnabled(hasProcessors && m_selectedElement->kind == ElementTreeNode::Kind::Processor);
	ui.actionConfigureProcessor->setEnabled(hasProcessors && m_selectedElement->kind == ElementTreeNode::Kind::Processor);
    ui.actionToggleProcessing->setEnabled(hasProcessors);

	/// MOUNTS
    bool hasMounts = !pController->mountController()->mounts().isEmpty();
	ui.actionRemoveMount->setEnabled(hasMounts && m_selectedElement->kind == ElementTreeNode::Kind::Mount);
	ui.actionEditMount->setEnabled(hasMounts && m_selectedElement->kind == ElementTreeNode::Kind::Mount);

    /// SESSION
    ui.actionStartStopSession->setEnabled(hasSources);
    ui.actionRecord->setEnabled(pController->sessionController()->pipeline()->isBuilt());
    ui.actionClipSession->setEnabled(pController->sessionController()->pipeline()->isBuilt());

    /// DEBUG
	ui.actionDebugPipelineDiagram->setEnabled(pController->sessionController()->pipeline()->isBuilt());
}

void MainWindow::onPrintPipelineDebugClicked()
{
	GstElement* pipeline = pController->sessionController()->pipeline()->bin();
    if (!pipeline) {
        QMessageBox::warning(this, tr("Pipeline Not Built"), tr("The GStreamer pipeline is not built yet."));
        return;
	}

    QString output = debugDisplayGstBin(pipeline);

    if (!output.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), output);
        return;
    }
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
