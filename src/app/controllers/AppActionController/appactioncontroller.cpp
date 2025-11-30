#include "appactioncontroller.h"

AppActionController::AppActionController(AppActions* actions, MainController& c, QWidget* parentWidget, QObject* parent)
    : QObject(parent), m_controller(c), m_parentWidget(parentWidget)
{
	// Load actions
    m_actions = *actions;
	m_presetActions = *actions->presetActions;
    m_mountActions = *actions->mountActions;
    m_sourceActions = *actions->sourceActions;
    m_processorActions = *actions->processorActions;
    m_sessionActions = *actions->sessionActions;
	m_viewActions = *actions->viewActions;
    m_miscActions = *actions->miscActions;

    initActionSignals();
}

AppActionController::~AppActionController()
{}

void AppActionController::initActionSignals()
{
    // Presets
    connect(m_presetActions.savePreset, &QAction::triggered, this, &AppActionController::onOpenSavePresetDialog);
    connect(m_presetActions.loadPreset, &QAction::triggered, this, &AppActionController::onLoadPresetClicked);
    connect(m_presetActions.deletePreset, &QAction::triggered, this, &AppActionController::onOpenDeletePresetDialog);

    // Session
    connect(m_sessionActions.toggleSession, &QAction::triggered, this, &AppActionController::onToggleSession);
    connect(m_sessionActions.restartSession, &QAction::triggered, &m_controller.sessionController(), &SessionController::restartSession);
    connect(m_sessionActions.toggleRecording, &QAction::triggered, this, &AppActionController::onToggleRecording);
    connect(m_sessionActions.openSessionProperties, &QAction::triggered, this, &AppActionController::onOpenSessionProperties);
    connect(m_sessionActions.clipSession, &QAction::triggered, this, &AppActionController::onClipSession);

    // Mounts
    connect(m_mountActions.openAddMount, &QAction::triggered, this, &AppActionController::onOpenAddMountDialog);
    connect(m_mountActions.openRemoveMount, &QAction::triggered, this, &AppActionController::onOpenRemoveMountDialog);
    connect(m_mountActions.openEditMount, &QAction::triggered, this, &AppActionController::onOpenEditMountDialog);

    // Sources
    connect(m_sourceActions.openAddSource, &QAction::triggered, this, &AppActionController::onOpenAddSourceDialog);
    connect(m_sourceActions.openRemoveSource, &QAction::triggered, this, &AppActionController::onOpenRemoveSourceDialog);
    connect(m_sourceActions.openEditSource, &QAction::triggered, this, &AppActionController::onOpenEditSourceDialog);

    // Processors
    connect(m_processorActions.openAddProcessor, &QAction::triggered, this, &AppActionController::onOpenAddProcessorDialog);
    connect(m_processorActions.openRemoveProcessor, &QAction::triggered, this, &AppActionController::onOpenRemoveProcessorDialog);
    connect(m_processorActions.openEditProcessor, &QAction::triggered, this, &AppActionController::onOpenEditProcessorDialog);
    connect(m_processorActions.toggleProcessing, &QAction::triggered, this, &AppActionController::onToggleProcessing);

    // View
    connect(m_viewActions.viewPresetsList, &QAction::triggered, [this](bool checked) {
        //ui.groupPresets->setVisible(checked);
        });
    connect(m_viewActions.viewControls, &QAction::triggered, [this](bool checked) {
        //ui.sessionControls->setVisible(checked);
        });

    connect(m_viewActions.viewMenuBar, &QAction::triggered, [this](bool checked) {
        //ui.menuBar->setVisible(checked);
        });
    connect(m_viewActions.viewToolbar, &QAction::triggered, [this](bool checked) {
        //ui.toolBar->setVisible(checked);
        });    

    // Plugins
    connect(m_miscActions.openPluginDialog, &QAction::triggered, this, &AppActionController::onOpenPluginDialog);

    // About
    connect(m_miscActions.openGithubRepository, &QAction::triggered, this, &AppActionController::onOpenGithubRepository);

    // Debug
    connect(m_miscActions.generatePipelineDiagram, &QAction::triggered, this, &AppActionController::onPrintPipelineDebugClicked);

    // File menu
    connect(m_miscActions.openAppProperties, &QAction::triggered, this, &AppActionController::onOpenAppPropertiesDialog);
    connect(m_miscActions.quit, &QAction::triggered, this, &AppActionController::quit);
    connect(m_miscActions.restart, &QAction::triggered, this, &AppActionController::restart);
}

void AppActionController::onOpenAppPropertiesDialog() {
    AppPropertiesDialog* appPropertiesDialog = new AppPropertiesDialog(m_parentWidget);
    appPropertiesDialog->setWindowModality(Qt::WindowModal);
    appPropertiesDialog->show();

}

void AppActionController::onOpenPluginDialog() {
    // Create and show the PluginDialog
    PluginsDialog* pluginsDialog = new PluginsDialog(m_controller.pluginController(), m_parentWidget);
    pluginsDialog->setWindowModality(Qt::WindowModal);
    pluginsDialog->show();
}

void AppActionController::onOpenSavePresetDialog()
{
    //// Get the preset name from the user
    //bool ok;
    //QString presetName = QInputDialog::getText(this, tr("Save Preset"), tr("Preset name:"), QLineEdit::Normal, QString(), &ok);
    //if (ok && !presetName.isEmpty()) {
    //    // Check if the preset name already exists
    //    // If so, ask to overwrite
    //    auto existingItems = ui.presetsWidget->listWidget()->findItems(presetName, Qt::MatchExactly);
    //    if (!existingItems.isEmpty()) {
    //        QMessageBox::StandardButton reply;
    //        reply = QMessageBox::question(this, tr("Overwrite Preset"),
    //            tr("Preset already exists. Do you want to overwrite it?"),
    //            QMessageBox::Yes | QMessageBox::No);
    //        if (reply == QMessageBox::No) {
    //            return;
    //        }
    //    }

    //    m_controller.presetsController().savePreset(presetName, m_controller.sourceController().sources());
    //}
    //else if (ok) {
    //    QMessageBox::warning(this, tr("Invalid Name"), tr("Preset name cannot be empty."));
    //    return;
    //}
}

void AppActionController::onLoadPresetClicked()
{
    //// Check selected item
    //if (!pSelectedPresetItem) {
    //    QMessageBox::warning(this, tr("No Preset Selected"), tr("Please select a preset to load."));
    //    return;
    //}

    //// Get the preset name from the item
    //QString presetName = pSelectedPresetItem->text();

    //// Find the preset in the controller
    //PresetsController& m_presetsController = m_controller.presetsController();
    //auto presets = m_presetsController.presets();
    //auto it = std::find_if(presets.begin(), presets.end(), [&presetName](const Preset& preset) {
    //    return preset.name == presetName;
    //    });
    //if (it != presets.end()) {
    //    // Load the preset
    //    m_presetsController.loadPreset(it->path, m_controller.sourceController(), m_controller.pluginController());
    //}
}

void AppActionController::onOpenDeletePresetDialog()
{
    //// Check selected item
    //if (!pSelectedPresetItem) {
    //    QMessageBox::warning(this, tr("No Preset Selected"), tr("Please select a preset to remove."));
    //    return;
    //}

    //// Get the preset name from the item
    //QString presetName = pSelectedPresetItem->text();

    //// Confirm removal
    //QMessageBox::StandardButton reply;
    //reply = QMessageBox::question(this, tr("Remove Preset"),
    //    tr("Are you sure you want to remove the preset: %1?").arg(presetName),
    //    QMessageBox::Yes | QMessageBox::No);
    //if (reply == QMessageBox::No) return;

    //// Find the preset in the controller
    //PresetsController& m_presetsController = m_controller.presetsController();
    //auto presets = m_presetsController.presets();
    //auto it = std::find_if(presets.begin(), presets.end(), [&presetName](const Preset& preset) {
    //    return preset.name == presetName;
    //    });
    //if (it != presets.end()) {
    //    // Remove the preset
    //    m_presetsController.removePreset(it->path);
    //}
    ////ui.listPresets->removeItemWidget(pSelectedItem); // TODO: check if I *need* to "take" the item from the list
    //delete pSelectedPresetItem;
    //pSelectedPresetItem = nullptr;
}

void AppActionController::onOpenEditPresetDialog()
{
    // TODO: Implement
}

void AppActionController::onRefreshPresetClicked()
{
    // Scan
    m_controller.presetsController().scanForPresets(); // CONSIDER: Pass specific path?
}

void AppActionController::onOpenAddMountDialog()
{
    // Create and show the AddMountDialog
    AddMountDialog* addDeviceDialog = new AddMountDialog(&m_controller.pluginController(), m_parentWidget);
    addDeviceDialog->setWindowModality(Qt::WindowModal);

    connect(addDeviceDialog, &AddMountDialog::mountConfirmed, &m_controller.mountController(), &MountController::addMount);

    addDeviceDialog->show();
}

void AppActionController::onOpenRemoveMountDialog()
{
    //auto selectedElement = ui.dockWidget->selectedNode();
    //if (!selectedElement) {
    //    QMessageBox::warning(m_parentWidget, tr("No Mount Selected"), tr("Please select a mount to remove."));
    //    return;
    //}

    //// Check selected item
    //if (selectedElement->kind != IElement::Type::Mount) {
    //    QMessageBox::warning(m_parentWidget, tr("No Mount Selected"), tr("Please select a mount to remove."));
    //    return;
    //}

    //auto response = QMessageBox::question(m_parentWidget, tr("Remove Mount"), tr("Are you sure you want to remove the selected mount?"), QMessageBox::Yes | QMessageBox::No);
    //if (response == QMessageBox::Yes) {
    //    m_controller.mountController().removeMount(selectedElement->uuid);
    //}
}

void AppActionController::onOpenEditMountDialog()
{
    QMessageBox::warning(m_parentWidget, tr("Feature Not Implemented"), tr("This feature has not been implemented yet."));
}

void AppActionController::onOpenAddSourceDialog()
{
    // Create and show the AddSourceDialog
    AddSourceDialog* addDeviceDialog = new AddSourceDialog(&m_controller.pluginController(), m_parentWidget);
    addDeviceDialog->setWindowModality(Qt::WindowModal);

    connect(addDeviceDialog, &AddSourceDialog::sourceConfirmed, [this](ISourcePlugin* plugin, SourceInfo deviceInfo) {
        m_controller.sourceController().addSource(plugin, deviceInfo);
        });

    addDeviceDialog->show();
}

void AppActionController::onOpenRemoveSourceDialog()
{
    //auto selectedElement = ui.dockWidget->selectedNode();
    //if (!selectedElement) {
    //    QMessageBox::warning(this, tr("No Source Selected"), tr("Please select a source to remove."));
    //    return;
    //}

    //// Check selected item
    //if (selectedElement->kind != IElement::Type::Source) {
    //    QMessageBox::warning(this, tr("No Source Selected"), tr("Please select a source to remove."));
    //    return;
    //}

    //auto response = QMessageBox::question(this, tr("Remove Source"), tr("Are you sure you want to remove the selected source?"), QMessageBox::Yes | QMessageBox::No);
    //if (response == QMessageBox::Yes) {
    //    m_controller.sourceController().removeSource(selectedElement->uuid);
    //}
}

void AppActionController::onOpenEditSourceDialog()
{
    //auto selectedElement = ui.dockWidget->selectedNode();
    //if (!selectedElement) {
    //    QMessageBox::warning(m_parentWidget, tr("No Source Selected"), tr("Please select a source to configure."));
    //    return;
    //}

    //// Check selected item
    //if (selectedElement->kind != IElement::Type::Source) {
    //    QMessageBox::warning(m_parentWidget, tr("No Source Selected"), tr("Please select a source to configure."));
    //    return;
    //}

    //// Get the source from the selected element
    //// TODO: check this implementation
    //Source* source = m_controller.sourceController().byId(selectedElement->uuid);
    //if (auto cfg = qobject_cast<IConfigurableSource*>(source)) {
    //    /* QWidget* w = cfg->createConfigWidget(this);
    //     QDialog dlg(this);
    //     dlg.setWindowTitle(QString::fromStdString(source->name()) + " Properties");
    //     QVBoxLayout lay(&dlg);
    //     lay.addWidget(w);
    //     dlg.exec();*/
    //}
    //else {
    //    // fallback: show generic property inspector
    //    //showGenericPropertyDialog(source);
    //    QMessageBox::information(m_parentWidget, tr("Properties"), tr("No properties available for this source."));
    //}
}

void AppActionController::onOpenAddProcessorDialog()
{
    AddProcessorDialog* addProcessorDialog = new AddProcessorDialog(&m_controller.pluginController(), m_parentWidget);
    addProcessorDialog->setWindowModality(Qt::WindowModal);

    connect(addProcessorDialog, &AddProcessorDialog::processorConfirmed, &m_controller.processingController(), &ProcessingController::addProcessor);

    addProcessorDialog->show();
}

void AppActionController::onOpenRemoveProcessorDialog()
{
    //auto selectedElement = ui.dockWidget->selectedNode();
    //if (!selectedElement) {
    //    QMessageBox::warning(m_parentWidget, tr("No Processor Selected"), tr("Please select a processor to remove."));
    //    return;
    //}

    //// Check selected item
    //if (selectedElement->kind != IElement::Type::Processor) {
    //    QMessageBox::warning(m_parentWidget, tr("No Processor Selected"), tr("Please select a processor to remove."));
    //    return;
    //}

    //auto response = QMessageBox::question(m_parentWidget, tr("Remove Processor"), tr("Are you sure you want to remove the selected processor?"), QMessageBox::Yes | QMessageBox::No);
    //if (response == QMessageBox::Yes) {
    //    // Get the processor from the selected element
    //    // TODO: implement this
    //    //ProcessorBase* processor = selectedElement.data(Qt::UserRole).value<ProcessorBase*>();

    //    // Remove source from the controller
    //    //m_controller->processingController()->removeProcessor(processor);
    //}
}

void AppActionController::onOpenEditProcessorDialog()
{
    // TODO: implement
    QMessageBox::information(m_parentWidget, tr("Feature Not Implemented"), tr("This feature has not been implemented yet."));
}

void AppActionController::onToggleProcessing()
{
	// TODO: implement
	QMessageBox::information(m_parentWidget, tr("Feature Not Implemented"), tr("This feature has not been implemented yet."));
}

void AppActionController::onToggleSession(bool checked)
{
    if (checked) m_controller.sessionController().startSession();
	else m_controller.sessionController().stopSession();
}

void AppActionController::onToggleRecording(bool checked)
{
    if (checked) m_controller.sessionController().startRecording();
    else m_controller.sessionController().stopRecording();
}

void AppActionController::onOpenSessionProperties()
{
    // Create new session properties dialog
    SessionPropertiesDialog* dialog = new SessionPropertiesDialog(&m_controller, &m_controller.sessionController().sessionProperties(), m_parentWidget);
    connect(dialog, &SessionPropertiesDialog::settingsChanged,
        this, [this](SessionProperties data) {
            m_controller.sessionController().setSessionProperties(data);
        });
    dialog->show();
}

void AppActionController::onClipSession()
{
    // TODO: implement
    QMessageBox::information(m_parentWidget, tr("Feature Not Implemented"), tr("This feature has not been implemented yet."));
}

void AppActionController::onOpenGithubRepository()
{
    QString repoLink = "https://github.com/m-riley04/ModularSensorSystem"; // TODO: Make this configurable or in a top-level file
    if (!QDesktopServices::openUrl(repoLink)) {
        QMessageBox::warning(m_parentWidget, tr("Link Error"), tr("Could not open the GitHub repository."));
    }
}

void AppActionController::onPrintPipelineDebugClicked()
{
    const GstElement* pipeline = m_controller.sessionController().pipeline().bin();
    if (!pipeline) {
        QMessageBox::warning(m_parentWidget, tr("Pipeline Not Built"), tr("The GStreamer pipeline is not built yet."));
        return;
    }

    QString output = debugDisplayGstBin(pipeline, true);

    if (!output.isEmpty()) {
        QMessageBox::warning(m_parentWidget, tr("Error"), output);
        return;
    }
}

void AppActionController::quit() {
    qApp->quit();
}

void AppActionController::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}