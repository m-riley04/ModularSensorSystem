#include "MainWindow.h"
#include <dialogs/AddSourceDialog/addsourcedialog.h>
#include <dialogs/AddProcessorDialog/addprocessordialog.h>
#include <dialogs/AddMountDialog/addmountdialog.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include <utils/debug.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_controller(new MainController(this))
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
    ui.devicePreviewWidget->setController(&m_controller);

    // Init presets widget
    ui.presetsWidget->setController(&m_controller);

    // Init elements tree
    ui.dockWidget->setController(&m_controller);
    ui.dockWidget->setActions(this->getElementTreeActions());
}

void MainWindow::initActionSignals()
{
    // Plugins
	connect(ui.actionOpenPluginsManager, &QAction::triggered, this, &MainWindow::onOpenPluginDialog);

    // Presets
	connect(ui.actionSavePreset, &QAction::triggered, this, &MainWindow::openSavePresetDialog);
    connect(ui.actionLoadPreset, &QAction::triggered, this, &MainWindow::onLoadPresetClicked);
	connect(ui.actionDeletePreset, &QAction::triggered, this, &MainWindow::openDeletePresetDialog);
    connect(ui.actionToggleProcessing, &QAction::triggered, this, [this](bool checked) {
        // TODO: implement this
		});

    // Session
    connect(ui.actionStartStopSession, &QAction::triggered, [this](bool checked) {
		if (checked) m_controller.sessionController().startSession();
		else m_controller.sessionController().stopSession();
        });
    connect(ui.actionRestartSession, &QAction::triggered, &m_controller.sessionController(), &SessionController::restartSession);
    connect(ui.actionRecord, &QAction::triggered, [this](bool checked) {
        if (checked) m_controller.sessionController().startRecording();
		else m_controller.sessionController().stopRecording(); // use request to allow graceful stopping
        });
    connect(ui.actionSessionProperties, &QAction::triggered, [this]() {
        // Create new session properties dialog
        SessionPropertiesDialog* dialog = new SessionPropertiesDialog(&m_controller, &m_controller.sessionController().sessionProperties(), this);
        connect(dialog, &SessionPropertiesDialog::settingsChanged,
            this, [this](SessionProperties data) {
                m_controller.sessionController().setSessionProperties(data);
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
	SourceController& m_sourceController = m_controller.sourceController();
	SessionController& m_sessionController = m_controller.sessionController();

    // Error message propagation
    connect(&m_sessionController, &SessionController::errorOccurred,
        this, [this](const QString& errorMessage) {
            QMessageBox::critical(this, tr("Session Error"), tr("An error occurred in the session:\n%1").arg(errorMessage));
		});

    // Connect preset widget signals
    connect(ui.presetsWidget, &PresetsWidget::selectedPresetChanged, this, &MainWindow::onSelectedPresetItemChanged);

	// Connect dock widget signals
    connect(ui.dockWidget, &DockableElementsManagerWidget::elementSelected, this, &MainWindow::updateToolbarButtonsState);
    connect(ui.dockWidget, &DockableElementsManagerWidget::elementRemoved, this, &MainWindow::updateToolbarButtonsState);

    // Init toolbar and actions
    initActionSignals();
}

void MainWindow::onOpenPluginDialog() {
    // Create and show the PluginDialog
    PluginsDialog* pluginsDialog = new PluginsDialog(m_controller.pluginController(), this);
    pluginsDialog->setWindowModality(Qt::WindowModal);
	pluginsDialog->show();
}

void MainWindow::openSavePresetDialog()
{
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

        m_controller.presetsController().savePreset(presetName, m_controller.sourceController().sources());
    }
    else if (ok) {
        QMessageBox::warning(this, tr("Invalid Name"), tr("Preset name cannot be empty."));
        return;
    }
}

void MainWindow::onLoadPresetClicked()
{
	// Check selected item
    if (!pSelectedPresetItem) {
        QMessageBox::warning(this, tr("No Preset Selected"), tr("Please select a preset to load."));
        return;
    }

    // Get the preset name from the item
    QString presetName = pSelectedPresetItem->text();

    // Find the preset in the controller
    PresetsController& m_presetsController = m_controller.presetsController();
    auto presets = m_presetsController.presets();
    auto it = std::find_if(presets.begin(), presets.end(), [&presetName](const Preset& preset) {
        return preset.name == presetName;
        });
    if (it != presets.end()) {
        // Load the preset
        m_presetsController.loadPreset(it->path, m_controller.sourceController(), m_controller.pluginController());
    }
}

void MainWindow::openDeletePresetDialog()
{
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
    PresetsController& m_presetsController = m_controller.presetsController();
    auto presets = m_presetsController.presets();
    auto it = std::find_if(presets.begin(), presets.end(), [&presetName](const Preset& preset) {
        return preset.name == presetName;
        });
    if (it != presets.end()) {
        // Remove the preset
        m_presetsController.removePreset(it->path);
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
    // Scan
    m_controller.presetsController().scanForPresets(); // CONSIDER: Pass specific path?
}

void MainWindow::openAddMountDialog()
{
    // Create and show the AddMountDialog
    AddMountDialog* addDeviceDialog = new AddMountDialog(&m_controller.pluginController(), this);
    addDeviceDialog->setWindowModality(Qt::WindowModal);

    connect(addDeviceDialog, &AddMountDialog::mountConfirmed, &m_controller.mountController(), &MountController::addMount);

    addDeviceDialog->show();
}

void MainWindow::openRemoveMountDialog()
{
    auto selectedElement = ui.dockWidget->selectedNode();
    if (!selectedElement) {
        QMessageBox::warning(this, tr("No Mount Selected"), tr("Please select a mount to remove."));
        return;
	}

    // Check selected item
    if (selectedElement->kind != IElement::Type::Mount) {
        QMessageBox::warning(this, tr("No Mount Selected"), tr("Please select a mount to remove."));
        return;
    }

    auto response = QMessageBox::question(this, tr("Remove Mount"), tr("Are you sure you want to remove the selected mount?"), QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
        m_controller.mountController().removeMount(selectedElement->uuid);
    }
}

void MainWindow::openEditMountDialog()
{
    QMessageBox::warning(this, tr("Feature Not Implemted"), tr("This feature has not been implemented yet."));
}

void MainWindow::openAddSourceDialog()
{
	// Create and show the AddSourceDialog
    AddSourceDialog* addDeviceDialog = new AddSourceDialog(&m_controller.pluginController(), this);
    addDeviceDialog->setWindowModality(Qt::WindowModal);

    connect(addDeviceDialog, &AddSourceDialog::sourceConfirmed, [this](ISourcePlugin* plugin, SourceInfo deviceInfo) {
		m_controller.sourceController().addSource(plugin, deviceInfo);
        });

    addDeviceDialog->show();
}

void MainWindow::openRemoveSourceDialog()
{
    auto selectedElement = ui.dockWidget->selectedNode();
    if (!selectedElement) {
        QMessageBox::warning(this, tr("No Source Selected"), tr("Please select a source to remove."));
        return;
    }

    // Check selected item
    if (selectedElement->kind != IElement::Type::Source) {
        QMessageBox::warning(this, tr("No Source Selected"), tr("Please select a source to remove."));
        return;
	}

    auto response = QMessageBox::question(this, tr("Remove Source"), tr("Are you sure you want to remove the selected source?"), QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
        m_controller.sourceController().removeSource(selectedElement->uuid);
    }
}

void MainWindow::openConfigureSourceDialog()
{
    auto selectedElement = ui.dockWidget->selectedNode();
    if (!selectedElement) {
        QMessageBox::warning(this, tr("No Source Selected"), tr("Please select a source to configure."));
        return;
    }

    // Check selected item
    if (selectedElement->kind != IElement::Type::Source) {
        QMessageBox::warning(this, tr("No Source Selected"), tr("Please select a source to configure."));
        return;
    }

	// Get the source from the selected element
    // TODO: check this implementation
	Source* source = m_controller.sourceController().byId(selectedElement->uuid);
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
    AddProcessorDialog* addProcessorDialog = new AddProcessorDialog(&m_controller.pluginController(), this);
    addProcessorDialog->setWindowModality(Qt::WindowModal);

    connect(addProcessorDialog, &AddProcessorDialog::processorConfirmed, &m_controller.processingController(), &ProcessingController::addProcessor);

    addProcessorDialog->show();
}

void MainWindow::openRemoveProcessorDialog()
{
    auto selectedElement = ui.dockWidget->selectedNode();
    if (!selectedElement) {
        QMessageBox::warning(this, tr("No Processor Selected"), tr("Please select a processor to remove."));
        return;
    }

    // Check selected item
    if (selectedElement->kind != IElement::Type::Processor) {
        QMessageBox::warning(this, tr("No Processor Selected"), tr("Please select a processor to remove."));
        return;
    }

    auto response = QMessageBox::question(this, tr("Remove Processor"), tr("Are you sure you want to remove the selected processor?"), QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
		// Get the processor from the selected element
        // TODO: implement this
        //ProcessorBase* processor = selectedElement.data(Qt::UserRole).value<ProcessorBase*>();

        // Remove source from the controller
        //m_controller->processingController()->removeProcessor(processor);
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

void MainWindow::updateToolbarButtonsState()
{
    /// PRESETS
    bool hasPresets = !m_controller.presetsController().presets().isEmpty();
    ui.actionLoadPreset->setEnabled(hasPresets && pSelectedPresetItem);
    ui.actionDeletePreset->setEnabled(hasPresets && pSelectedPresetItem);

    // Check selected element
    auto selectedElement = ui.dockWidget->selectedNode();
    if (!selectedElement) {
		// TODO: maybe disable all element-related actions here?
        return;
    }

    /// SOURCES
    bool hasSources = !m_controller.sourceController().sources().isEmpty();
    ui.actionRemoveSource->setEnabled(hasSources && selectedElement->kind == IElement::Type::Source);
    ui.actionConfigureSource->setEnabled(hasSources && selectedElement->kind == IElement::Type::Source); // TODO/CONSIDER: change this action to open a dialog for ALL sources

    /// PROCESSING
    bool hasProcessors = !m_controller.processingController().processors().isEmpty();
    ui.actionRemoveProcessor->setEnabled(hasProcessors && selectedElement->kind == IElement::Type::Processor);
	ui.actionConfigureProcessor->setEnabled(hasProcessors && selectedElement->kind == IElement::Type::Processor);
    ui.actionToggleProcessing->setEnabled(hasProcessors);

	/// MOUNTS
    bool hasMounts = !m_controller.mountController().mounts().isEmpty();
	ui.actionRemoveMount->setEnabled(hasMounts && selectedElement->kind == IElement::Type::Mount);
	ui.actionEditMount->setEnabled(hasMounts && selectedElement->kind == IElement::Type::Mount);

    /// SESSION
    ui.actionStartStopSession->setEnabled(hasSources);
    ui.actionRecord->setEnabled(m_controller.sessionController().pipeline().isBuilt());
    ui.actionClipSession->setEnabled(m_controller.sessionController().pipeline().isBuilt());

    /// DEBUG
	ui.actionDebugPipelineDiagram->setEnabled(m_controller.sessionController().pipeline().isBuilt());
}

void MainWindow::onPrintPipelineDebugClicked()
{
	const GstElement* pipeline = m_controller.sessionController().pipeline().bin();
    if (!pipeline) {
        QMessageBox::warning(this, tr("Pipeline Not Built"), tr("The GStreamer pipeline is not built yet."));
        return;
	}

    QString output = debugDisplayGstBin(pipeline, true);

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
