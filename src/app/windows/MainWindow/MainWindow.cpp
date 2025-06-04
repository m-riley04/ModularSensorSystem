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
    initStyles();
	initPages();
    initWidgets();
    initSignals();
}

MainWindow::~MainWindow()
{}

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
    // Init title bar
    ui.titleBar->setParentWindow(this);

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