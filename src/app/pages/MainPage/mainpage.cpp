#include "mainpage.h"

MainPage::MainPage(MainController* controller, ElementTreeActions actions, QWidget *parent)
	: QMainWindow(parent), pController(controller), m_actions(actions)
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

	// Init presets widget
	ui.presetsWidget->setController(pController);

	// Init elements tree
	ui.dockWidget->setController(pController);
	ui.dockWidget->setActions(m_actions);
}

void MainPage::initSignals() {
	// TODO: implement something
}