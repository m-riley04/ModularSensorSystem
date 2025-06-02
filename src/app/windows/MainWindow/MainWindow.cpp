#include "MainWindow.h"
#include <Pages/MainPage/mainpage.h>

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

    // Initialize
	initPages();
    initWidgets();
    initSignals();
}

MainWindow::~MainWindow()
{}

void MainWindow::initPages()
{
	if (!pController) {
		qDebug() << "MainController is not initialized.";
		return;
	}

	MainPage* mainPage = new MainPage(pController.get(), this);
	ui.pagesStack->addWidget(mainPage);
	mainPage->setLayout(ui.pagesStack->layout());
}

void MainWindow::initWidgets()
{
	// Set default page to home
	ui.pagesStack->setCurrentIndex(0);
}

void MainWindow::initSignals() {
	SourceController* pSourceController = pController->sourceController();

    // Error messages
    connect(pController.get(), &MainController::errorOccurred, [this](const SourceError& e) {
        QString deviceInfo = "Source: " + (e.source != nullptr ? e.source->name() : "null");
        QString errorMessage = "Error: " + e.msg + "\n" + deviceInfo;
        QMessageBox::warning(this, "Error", errorMessage);
        });

    // Pages
	connect(ui.buttonHome, &QPushButton::clicked, [this]() {
		ui.pagesStack->setCurrentIndex(0);
		});
    connect(ui.buttonPlayback, &QPushButton::clicked, [this]() {
        ui.pagesStack->setCurrentIndex(1);
        });

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}