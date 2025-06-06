#include "mainpage.h"

MainPage::MainPage(MainController* controller, QWidget *parent)
	: QWidget(parent), pController(controller)
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
}

void MainPage::initSignals() {
	// TODO: think of what to do here
}