#include "MainWindow.h"
#include <Pages/MainPage/mainpage.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.titleBar->setParentWindow(this);

	// Initialize the main controller
	pController = std::make_unique<MainController>(this);

    // Initialize core app
    QCoreApplication::setApplicationName("ModularSensorSystem");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("Riley Meyerkorth");
    QCoreApplication::setOrganizationDomain("rileymeyerkorth.com");

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    // Initialize
    initStyles();
	initPages();
    initWidgets();
    initSignals();
}

MainWindow::~MainWindow()
{}

void MainWindow::initStyles()
{
    // Configure application properties
	const QString stylePath = QDir::currentPath() + "/styles/styles.qss";
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