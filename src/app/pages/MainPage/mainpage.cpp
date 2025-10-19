#include "mainpage.h"
#include "windows/MainWindow/MainWindow.h"

MainPage::MainPage(MainController* controller, MainWindow* mainWindow, QWidget *parent)
	: QMainWindow(parent), pController(controller), m_mainWindow(mainWindow)
{
	ui.setupUi(this);

	initWidgets();
	initSignals();
}

MainPage::~MainPage()
{}

void MainPage::initWidgets()
{
	// Init session controls widget
	ui.sessionControls->setController(pController);

	// Init preview container widget
	ui.devicePreviewWidget->setController(pController);

	// Init source list widget
	ui.sourcesWidget->setSourceController(pController->sourceController());

	// Init processors widget
	ui.processorsWidget->setController(pController);

	// Init presets widget
	ui.presetsWidget->setController(pController);

	// Init elements tree
	ui.dockWidget->setController(pController);
	ui.dockWidget->setActions(m_mainWindow->getElementTreeActions());
}

void MainPage::initSignals() {
	// TODO: think of what to do here
}