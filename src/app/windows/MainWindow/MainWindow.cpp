#include "MainWindow.h"
#include <dialogs/AddSourceDialog/addsourcedialog.h>
#include <dialogs/AddProcessorDialog/addprocessordialog.h>
#include <dialogs/AddMountDialog/addmountdialog.h>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	// Initialize the main controller
	pController = std::make_unique<MainController>(this);

    // Initialize core app
    QCoreApplication::setApplicationName("ModularSensorSystem");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("Riley Meyerkorth");
    QCoreApplication::setOrganizationDomain("rileymeyerkorth.com");

    // Init flags
    //setWindowFlags(Qt::FramelessWindowHint);

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

void MainWindow::initWidgets()
{
    // Init menu bar
    //ui.menuBar->setParent(this->centralWidget());

    // Remove widgets from layout
    /*QLayout* layout = this->centralWidget()->layout();
    layout->removeWidget(ui.titleBar);
	layout->removeWidget(ui.toolBar);*/
    //layout->removeWidget(ui.dockWidget);
    //layout->removeWidget(ui.splitter);

    // Init stack
    //layout->addWidget(ui.titleBar); // Add menu bar FIRST so it is ABOVE all
    //layout->addWidget(ui.menuBar); // Add menu bar SECOND so it is UNDER the title bar
    //layout->addWidget(ui.toolBar);
    //layout->addWidget(ui.dockWidget);
    //layout->addWidget(ui.splitter);


    // Init title bar
    //ui.titleBar->setParentWindow(this);

    // Init menu bar
    ui.actionViewPresetsList->setChecked(!ui.groupPresets->isVisible()); // Not sure why I have to invert this to NOT, but it works.
    ui.actionViewControls->setChecked(!ui.groupControls->isVisible());
    ui.actionViewEntireRow->setChecked(!ui.frameControls->isVisible());

    ui.actionViewMenuBar->setChecked(!ui.menuBar->isVisible());
    ui.actionViewToolbar->setChecked(!ui.toolBar->isVisible());
    //ui.actionViewCustomWindowHandle->setChecked(!ui.titleBar->isVisible());

    // Init toolbar
    updateToolbarButtonsState();

    // Init session controls widget
    ui.sessionControls->setController(pController.get());

    // Init preview container widget
    ui.devicePreviewWidget->setController(pController.get());
	// TODO/DEBUG: add test video output
	ui.devicePreviewWidget->setAttribute(Qt::WA_NativeWindow); // Ensure the widget has a native window handle for video rendering
    pController->sessionController()->setVideoSinkWindowId(ui.devicePreviewWidget->winId());

    // Init presets widget
    ui.presetsWidget->setController(pController.get());

    // Init elements tree
    ui.dockWidget->setController(pController.get());
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
        ui.groupControls->setVisible(checked);
        });
    connect(ui.actionViewEntireRow, &QAction::triggered, [this](bool checked) {
        ui.frameControls->setVisible(checked);
        });

    connect(ui.actionViewMenuBar, &QAction::triggered, [this](bool checked) {
        ui.menuBar->setVisible(checked);
        });
    connect(ui.actionViewToolbar, &QAction::triggered, [this](bool checked) {
        ui.toolBar->setVisible(checked);
        });
  //  connect(ui.actionViewCustomWindowHandle, &QAction::triggered, [this](bool checked) {
  //      ui.titleBar->setVisible(checked);

  //      Qt::WindowType flags = checked ? Qt::FramelessWindowHint : Qt::Window;
  //      setWindowFlags(flags);

		//show(); // Refresh the window to apply the new flags
  //      });

    // About
    connect(ui.actionGitHub, &QAction::triggered, this, &MainWindow::openGithubRepository);

    // Quitting/restarting
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::initSignals() {
    if (!pController->checkIfControllersAreOk()) return;
	SourceController* pSourceController = pController->sourceController();

    // Init error message propagation
    connect(pController.get(), &MainController::errorOccurred, [this](const SourceError& e) {
        QString deviceInfo = "Source: " + (e.source != nullptr ? QString::fromStdString(e.source->name()) : "null");
        QString errorMessage = "Error: " + e.msg + "\n" + deviceInfo;
        QMessageBox::warning(this, "Error", errorMessage);
        });

    // Connect signals to child widgets
    connect(ui.presetsWidget, &PresetsWidget::selectedPresetChanged, this, &MainWindow::onSelectedPresetItemChanged);
	connect(ui.dockWidget, &DockableElementsManagerWidget::elementSelected, this, &MainWindow::onSelectedElementChanged);

    // Init toolbar and actions
    initActionSignals();
}

void MainWindow::openSavePresetDialog()
{
    if (!pController->checkIfControllersAreOk()) return;

    // Get the preset name from the user
    bool ok;
    QString presetName = QInputDialog::getText(this, "Save Preset", "Preset name:", QLineEdit::Normal, QString(), &ok);
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
        QMessageBox::warning(this, "No Preset Selected", "Please select a preset to load.");
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
        QMessageBox::warning(this, "No Preset Selected", "Please select a preset to remove.");
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
        QMessageBox::warning(this, "No Mount Selected", "Please select a mount to remove.");
        return;
    }

    auto response = QMessageBox::question(this, "Remove Mount", "Are you sure you want to remove the selected mount?", QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
		// TODO: Check this implementation
        const Mount* mount = pController->mountController()->byId(m_selectedElement->id);

        // Remove mount from the controller
		// TODO: reconsider const casting and const usage
        pController->mountController()->removeMount(const_cast<Mount*>(mount));
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
        QMessageBox::warning(this, "No Source Selected", "Please select a source to remove.");
        return;
	}

    auto response = QMessageBox::question(this, "Remove Source", "Are you sure you want to remove the selected source?", QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
		// TODO: check this implementation
        const Source* source = pController->sourceController()->byId(m_selectedElement->id);

        // Remove source from the controller
        // TODO: reconsider const casting and const usage
        pController->sourceController()->removeSource(const_cast<Source*>(source));
    }
}

void MainWindow::openConfigureSourceDialog()
{
    // Check controllers
    if (!pController->checkIfControllersAreOk()) return;

    // Check selected item
    if (m_selectedElement->kind != ElementTreeNode::Kind::Source) {
        QMessageBox::warning(this, "No Source Selected", "Please select a source to configure.");
        return;
    }

	// Get the source from the selected element
    // TODO: check this implementation
	Source* source = pController->sourceController()->byId(m_selectedElement->id);
    if (auto cfg = qobject_cast<IConfigurableSource*>(source)) {
        QWidget* w = cfg->createConfigWidget(this);
        QDialog dlg(this);
        dlg.setWindowTitle(QString::fromStdString(source->name()) + " Properties");
        QVBoxLayout lay(&dlg);
        lay.addWidget(w);
        dlg.exec();
    }
    else {
        // fallback: show generic property inspector
        //showGenericPropertyDialog(source);
        QMessageBox::information(this, "Properties", "No properties available for this source.");
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
        QMessageBox::warning(this, "No Processor Selected", "Please select a processor to remove.");
        return;
    }

    auto response = QMessageBox::question(this, "Remove Processor", "Are you sure you want to remove the selected processor?", QMessageBox::Yes | QMessageBox::No);
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
    QMessageBox::warning(this, "Feature Not Implemted", "This feature has not been implemented yet.");
}

void MainWindow::openGithubRepository()
{
    QString repoLink = "https://github.com/m-riley04/ModularSensorSystem"; // TODO: Make this configurable or in a top-level file
    if (!QDesktopServices::openUrl(repoLink)) {
        QMessageBox::warning(this, "Link Error", "Could not open the GitHub repository.");
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
	ui.actionOpenCloseSources->setEnabled(hasSources);
	ui.actionStartStopSources->setEnabled(hasSources && ui.actionOpenCloseSources->isChecked());

    /// PROCESSING
    bool hasProcessors = !pController->processingController()->processors().isEmpty();
    ui.actionRemoveProcessor->setEnabled(hasProcessors && m_selectedElement->kind == ElementTreeNode::Kind::Processor);
	ui.actionConfigureProcessor->setEnabled(hasProcessors && m_selectedElement->kind == ElementTreeNode::Kind::Processor);
    ui.actionToggleProcessing->setEnabled(hasProcessors);

	/// MOUNTS
    bool hasMounts = !pController->mountController()->mounts().isEmpty();
	ui.actionRemoveMount->setEnabled(hasMounts && m_selectedElement->kind == ElementTreeNode::Kind::Mount);
	ui.actionEditMount->setEnabled(hasMounts && m_selectedElement->kind == ElementTreeNode::Kind::Mount);
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

/**
 * Primarily for the custom window resizing and dragging.
 * Implement on Windows only for now.
 */
//bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
//{
//#ifdef Q_OS_WIN
//    MSG* msg = static_cast<MSG*>(message);
//
//    if (msg->message == WM_NCHITTEST)
//    {
//        if (isMaximized())
//        {
//            return false;
//        }
//
//        *result = 0;
//        const LONG borderWidth = 8;
//        RECT winrect;
//        GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);
//
//        // must be short to correctly work with multiple monitors (negative coordinates)
//        short x = msg->lParam & 0x0000FFFF;
//        short y = (msg->lParam & 0xFFFF0000) >> 16;
//
//        bool resizeWidth = minimumWidth() != maximumWidth();
//        bool resizeHeight = minimumHeight() != maximumHeight();
//        if (resizeWidth)
//        {
//            //left border
//            if (x >= winrect.left && x < winrect.left + borderWidth)
//            {
//                *result = HTLEFT;
//            }
//            //right border
//            if (x < winrect.right && x >= winrect.right - borderWidth)
//            {
//                *result = HTRIGHT;
//            }
//        }
//        if (resizeHeight)
//        {
//            //bottom border
//            if (y < winrect.bottom && y >= winrect.bottom - borderWidth)
//            {
//                *result = HTBOTTOM;
//            }
//            //top border
//            if (y >= winrect.top && y < winrect.top + borderWidth)
//            {
//                *result = HTTOP;
//            }
//        }
//        if (resizeWidth && resizeHeight)
//        {
//            //bottom left corner
//            if (x >= winrect.left && x < winrect.left + borderWidth &&
//                y < winrect.bottom && y >= winrect.bottom - borderWidth)
//            {
//                *result = HTBOTTOMLEFT;
//            }
//            //bottom right corner
//            if (x < winrect.right && x >= winrect.right - borderWidth &&
//                y < winrect.bottom && y >= winrect.bottom - borderWidth)
//            {
//                *result = HTBOTTOMRIGHT;
//            }
//            //top left corner
//            if (x >= winrect.left && x < winrect.left + borderWidth &&
//                y >= winrect.top && y < winrect.top + borderWidth)
//            {
//                *result = HTTOPLEFT;
//            }
//            //top right corner
//            if (x < winrect.right && x >= winrect.right - borderWidth &&
//                y >= winrect.top && y < winrect.top + borderWidth)
//            {
//                *result = HTTOPRIGHT;
//            }
//        }
//
//        if (*result != 0)
//            return true;
//
//        QWidget* action = QApplication::widgetAt(QCursor::pos());
//        if (action == this) {
//            *result = HTCAPTION;
//            return true;
//        }
//    }
//#endif
//
//    return false;
//}