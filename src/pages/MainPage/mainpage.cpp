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
	// Init device list widget
	ui.deviceListWidget->setDeviceController(pController->deviceController());

	// Init session controls widget
	ui.sessionControls->setController(pController);

	// Init preview container widget
	ui.devicePreviewWidget->setController(pController);

	// Init presets widget
	ui.presetsWidget->setController(pController);
}

void MainPage::initSignals() {
	// TODO: think of what to do here
}