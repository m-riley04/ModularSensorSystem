#include "MainWindow.h"

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
    setWindowFlags(Qt::FramelessWindowHint);

    // Initialize
    /*
        TODO: either remove initStyles() or implement/document properly.
        Currently, we are loading the stylesheet directly in the UI file and UI editor.
    */
	//initStyles();
	initPages();
    initWidgets();
    initSignals();
}

MainWindow::~MainWindow()
{}



void MainWindow::initStyles()
{
    // Configure application properties
	const QString stylePath = ":/styles/styles/styles.qss";
    QFile f(stylePath);

	// Check if the stylesheet file exists
	if (!f.exists()) {
		QMessageBox::warning(this, "Error", "Stylesheet file not found: " + stylePath);
		return;
	}

	// Check if the stylesheet file can be opened
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "Error", "Could not open stylesheet file.");
        return;
    }

	// Read the stylesheet and apply it to the application
    const QString stylesheetString = f.readAll();
    qApp->setStyleSheet(stylesheetString);
}

void MainWindow::initPages()
{
	if (!pController) {
		qDebug() << "MainController is not initialized.";
		return;
	}

    // Init main page
    pMainPage = new MainPage(pController.get(), this);
    pMainPage->setObjectName("mainPage");
	ui.pagesStack->addWidget(pMainPage);
    pMainPage->setLayout(ui.pagesStack->layout());
}

void MainWindow::initWidgets()
{
    // Init menu bar
    ui.menuBar->setParent(this->centralWidget());

    // Remove widgets from layout
    QLayout* layout = this->centralWidget()->layout();
    layout->removeWidget(ui.titleBar);
    layout->removeWidget(ui.frameNav);
	layout->removeWidget(ui.toolBar);
    layout->removeWidget(ui.pagesStack);

    // Init stack
    layout->addWidget(ui.titleBar); // Add menu bar FIRST so it is ABOVE all
    layout->addWidget(ui.menuBar); // Add menu bar SECOND so it is UNDER the title bar
    layout->addWidget(ui.toolBar);
    layout->addWidget(ui.frameNav);
    layout->addWidget(ui.pagesStack);

    // Init title bar
    ui.titleBar->setParentWindow(this);

	// Set default page to home
	ui.pagesStack->setCurrentIndex(0);

    // Init menu bar
    ui.actionViewPresetsList->setChecked(!pMainPage->presetsGroup()->isVisible()); // Not sure why I have to invert this to NOT, but it works.
    ui.actionViewSourcesList->setChecked(!pMainPage->sourcesGroup()->isVisible());
    ui.actionViewProcessorsList->setChecked(!pMainPage->processorsGroup()->isVisible());
    ui.actionViewControls->setChecked(!pMainPage->controlsGroup()->isVisible());
    ui.actionViewEntireRow->setChecked(!pMainPage->controlsFrame()->isVisible());

    ui.actionViewMenuBar->setChecked(!ui.menuBar->isVisible());
    ui.actionViewToolbar->setChecked(!ui.toolBar->isVisible());
    ui.actionViewCustomWindowHandle->setChecked(!ui.titleBar->isVisible());
    

    // Init toolbar
    updateToolbarButtonsState();
}

void MainWindow::initActionSignals()
{
    if (!checkIfControllersAreOk(pController.get())) return;

    // Presets
	connect(ui.actionSavePreset, &QAction::triggered, this, &MainWindow::openSavePresetDialog);
    connect(ui.actionLoadPreset, &QAction::triggered, this, &MainWindow::onLoadPresetClicked);
	connect(ui.actionDeletePreset, &QAction::triggered, this, &MainWindow::openDeletePresetDialog);
    connect(ui.actionToggleProcessing, &QAction::triggered, this, [this](bool checked) {
        // TODO: implement this
		});

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
        pMainPage->presetsGroup()->setVisible(checked);
        });
    connect(ui.actionViewSourcesList, &QAction::triggered, [this](bool checked) {
        pMainPage->sourcesGroup()->setVisible(checked);
        });
    connect(ui.actionViewProcessorsList, &QAction::triggered, [this](bool checked) {
        pMainPage->processorsGroup()->setVisible(checked);
        });
    connect(ui.actionViewControls, &QAction::triggered, [this](bool checked) {
        pMainPage->controlsGroup()->setVisible(checked);
        });
    connect(ui.actionViewEntireRow, &QAction::triggered, [this](bool checked) {
        pMainPage->controlsFrame()->setVisible(checked);
        });

    connect(ui.actionViewMenuBar, &QAction::triggered, [this](bool checked) {
        ui.menuBar->setVisible(checked);
        });
    connect(ui.actionViewToolbar, &QAction::triggered, [this](bool checked) {
        ui.toolBar->setVisible(checked);
        });
    connect(ui.actionViewCustomWindowHandle, &QAction::triggered, [this](bool checked) {
        ui.titleBar->setVisible(checked);

        Qt::WindowType flags = checked ? Qt::FramelessWindowHint : Qt::Window;
        setWindowFlags(flags);

		show(); // Refresh the window to apply the new flags
        });

    // Quitting/restarting
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::initSignals() {
    if (!checkIfControllersAreOk(pController.get())) return;
	SourceController* pSourceController = pController->sourceController();

    // Init error message propagation
    connect(pController.get(), &MainController::errorOccurred, [this](const SourceError& e) {
        QString deviceInfo = "Source: " + (e.source != nullptr ? e.source->name() : "null");
        QString errorMessage = "Error: " + e.msg + "\n" + deviceInfo;
        QMessageBox::warning(this, "Error", errorMessage);
        });

    // Init pages
	connect(ui.buttonHome, &QPushButton::clicked, [this]() {
		ui.pagesStack->setCurrentIndex(0);
		});
    connect(ui.buttonPlayback, &QPushButton::clicked, [this]() {
        ui.pagesStack->setCurrentIndex(1);
        });

    // Connect signals to child widgets
    connect(pMainPage->presetsWidget(), &PresetsWidget::selectedPresetChanged, this, &MainWindow::onSelectedPresetItemChanged);
    connect(pMainPage->sourcesWidget(), &SourcesWidget::selectedSourceChanged, this, &MainWindow::onSelectedSourceItemChanged);
    connect(pMainPage->processorsWidget(), &ProcessorsWidget::selectedProcessorChanged, this, &MainWindow::onSelectedProcessorItemChanged);

    // Init toolbar and actions
    initActionSignals();
}

bool MainWindow::checkIfControllersAreOk(MainController* controller) const
{
    // Check the main controller first and foremost
    if (!controller) {
        qDebug() << "MainController is not initialized.";
        return false;
	}

    // Check if they are all okay just to see if we can exit early
    if (controller->sourceController() && 
        controller->processingController() && 
        controller->presetsController() && 
        controller->pluginController() /*&& 
        controller->settingsController()*/ && 
        controller->clipController() && 
        controller->recordingController()) {
        return true;
	}

	// Otherwise, we can do more work to see which specific subcontroller(s) are not initialized
    for (BackendControllerBase* subcontroller : controller->getAllSubcontrollers()) {
        if (!subcontroller) {
            qDebug() << "A subcontroller is not initialized:" << subcontroller->name();
            // TODO/CONSIDER: Show a message box or some other UI feedback to the user
			// TODO/CONSIDER: return early here, or continue checking all controllers? Probably easier for debugging this way.
		}
    }

    return false;
}

void MainWindow::openSavePresetDialog()
{
    if (!checkIfControllersAreOk(pController.get())) return;

    // Get the preset name from the user
    bool ok;
    QString presetName = QInputDialog::getText(this, "Save Preset", "Preset name:", QLineEdit::Normal, QString(), &ok);
    if (ok && !presetName.isEmpty()) {
        // Check if the preset name already exists
        // If so, ask to overwrite
        auto existingItems = pMainPage->presetsWidget()->listWidget()->findItems(presetName, Qt::MatchExactly);
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
    if (!checkIfControllersAreOk(pController.get())) return;
    
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
    if (!checkIfControllersAreOk(pController.get())) return;

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

}

void MainWindow::onRefreshPresetClicked()
{
    // Check controllers
    if (!checkIfControllersAreOk(pController.get())) return;

    // Scan
    pController->presetsController()->scanForPresets(); // CONSIDER: Pass specific path?
}

void MainWindow::openAddSourceDialog()
{
    // Check controllers
    if (!checkIfControllersAreOk(pController.get())) return;

	// Create and show the AddSourceDialog
    AddSourceDialog* addDeviceDialog = new AddSourceDialog(pController->pluginController(), this);
    addDeviceDialog->setWindowModality(Qt::WindowModal);

    connect(addDeviceDialog, &AddSourceDialog::sourceConfirmed, pController->sourceController(), &SourceController::addSource);

    addDeviceDialog->show();
}

void MainWindow::openRemoveSourceDialog()
{
    // Check controllers
    if (!checkIfControllersAreOk(pController.get())) return;

    // Check selected item
    if (!pSelectedSourceItem) {
        QMessageBox::warning(this, "No Source Selected", "Please select a source to remove.");
        return;
	}

    auto response = QMessageBox::question(this, "Remove Source", "Are you sure you want to remove the selected source?", QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
        Source* source = pSelectedSourceItem->data(Qt::UserRole).value<Source*>();

        // Remove source from the controller
        pController->sourceController()->removeSource(source);
    }
}

void MainWindow::openConfigureSourceDialog()
{
    // Check controllers
    if (!checkIfControllersAreOk(pController.get())) return;

    // Check selected item
    if (!pSelectedSourceItem) {
        QMessageBox::warning(this, "No Source Selected", "Please select a source to configure.");
        return;
    }

    Source* source = pSelectedSourceItem->data(Qt::UserRole).value<Source*>();
    if (auto cfg = qobject_cast<IConfigurableSource*>(source)) {
        QWidget* w = cfg->createConfigWidget(this);
        QDialog dlg(this);
        dlg.setWindowTitle(source->name() + " Properties");
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
    if (!checkIfControllersAreOk(pController.get())) return;

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
    if (!checkIfControllersAreOk(pController.get())) return;

    // Check selected item
    if (!pSelectedProcessorItem) {
        QMessageBox::warning(this, "No Processor Selected", "Please select a processor to remove.");
        return;
    }

    auto response = QMessageBox::question(this, "Remove Processor", "Are you sure you want to remove the selected processor?", QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes) {
        ProcessorBase* processor = pSelectedProcessorItem->data(Qt::UserRole).value<ProcessorBase*>();

        // Remove source from the controller
        pController->processingController()->removeProcessor(processor);
    }
}

void MainWindow::openConfigureProcessorDialog()
{
    QMessageBox::warning(this, "Feature Not Implemted", "This feature has not been implemented yet.");
}

void MainWindow::onSelectedSourceItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
	if (pSelectedSourceItem == current) return; // No change
	pSelectedSourceItem = current;
    updateToolbarButtonsState(); // CONSIDER: move this to it's own connection to this signal
}

void MainWindow::onSelectedPresetItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (pSelectedPresetItem == current) return; // No change
    pSelectedPresetItem = current;
    updateToolbarButtonsState(); // CONSIDER: move this to it's own connection to this signal
}

void MainWindow::onSelectedProcessorItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (pSelectedProcessorItem == current) return; // No change
    pSelectedProcessorItem = current;
    updateToolbarButtonsState(); // CONSIDER: move this to it's own connection to this signal
}

void MainWindow::updateToolbarButtonsState()
{
    if (!checkIfControllersAreOk(pController.get())) return;

    /// PRESETS
    bool hasPresets = !pController->presetsController()->presets().isEmpty();
    ui.actionLoadPreset->setEnabled(hasPresets && pSelectedPresetItem);
    ui.actionDeletePreset->setEnabled(hasPresets && pSelectedPresetItem);

    /// SOURCES
    bool hasSources = !pController->sourceController()->sources().isEmpty();
    ui.actionRemoveSource->setEnabled(hasSources && pSelectedSourceItem);
    ui.actionConfigureSource->setEnabled(hasSources && pSelectedSourceItem); // TODO/CONSIDER: change this action to open a dialog for ALL sources

    /// PROCESSING
    bool hasProcessors = !pController->processingController()->processors().isEmpty();
    ui.actionRemoveProcessor->setEnabled(hasProcessors && pSelectedSourceItem);
	ui.actionConfigureProcessor->setEnabled(hasProcessors && pSelectedSourceItem);
    ui.actionToggleProcessing->setEnabled(hasProcessors);
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
#ifdef Q_OS_WIN
    MSG* msg = static_cast<MSG*>(message);

    if (msg->message == WM_NCHITTEST)
    {
        if (isMaximized())
        {
            return false;
        }

        *result = 0;
        const LONG borderWidth = 8;
        RECT winrect;
        GetWindowRect(reinterpret_cast<HWND>(winId()), &winrect);

        // must be short to correctly work with multiple monitors (negative coordinates)
        short x = msg->lParam & 0x0000FFFF;
        short y = (msg->lParam & 0xFFFF0000) >> 16;

        bool resizeWidth = minimumWidth() != maximumWidth();
        bool resizeHeight = minimumHeight() != maximumHeight();
        if (resizeWidth)
        {
            //left border
            if (x >= winrect.left && x < winrect.left + borderWidth)
            {
                *result = HTLEFT;
            }
            //right border
            if (x < winrect.right && x >= winrect.right - borderWidth)
            {
                *result = HTRIGHT;
            }
        }
        if (resizeHeight)
        {
            //bottom border
            if (y < winrect.bottom && y >= winrect.bottom - borderWidth)
            {
                *result = HTBOTTOM;
            }
            //top border
            if (y >= winrect.top && y < winrect.top + borderWidth)
            {
                *result = HTTOP;
            }
        }
        if (resizeWidth && resizeHeight)
        {
            //bottom left corner
            if (x >= winrect.left && x < winrect.left + borderWidth &&
                y < winrect.bottom && y >= winrect.bottom - borderWidth)
            {
                *result = HTBOTTOMLEFT;
            }
            //bottom right corner
            if (x < winrect.right && x >= winrect.right - borderWidth &&
                y < winrect.bottom && y >= winrect.bottom - borderWidth)
            {
                *result = HTBOTTOMRIGHT;
            }
            //top left corner
            if (x >= winrect.left && x < winrect.left + borderWidth &&
                y >= winrect.top && y < winrect.top + borderWidth)
            {
                *result = HTTOPLEFT;
            }
            //top right corner
            if (x < winrect.right && x >= winrect.right - borderWidth &&
                y >= winrect.top && y < winrect.top + borderWidth)
            {
                *result = HTTOPRIGHT;
            }
        }

        if (*result != 0)
            return true;

        QWidget* action = QApplication::widgetAt(QCursor::pos());
        if (action == this) {
            *result = HTCAPTION;
            return true;
        }
    }
#endif

    return false;
}