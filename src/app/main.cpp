#include "Windows/MainWindow/MainWindow.h"
#include <QtWidgets/QApplication>
#include "controllers/maincontroller.hpp"
#include <QSettings>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <cstdio>

/**
 * Sets up the console for debug output on Windows.
 * Qt applications on Windows do not have a console by default, and
 * g_print was being ignored in the normal debug console.
 */
void setupConsole()
{
	AllocConsole();

	FILE* fpOut = nullptr;
	FILE* fpErr = nullptr;

	freopen_s(&fpOut, "CONOUT$", "w", stdout);
	freopen_s(&fpErr, "CONOUT$", "w", stderr);
}
#endif

int main(int argc, char *argv[])
{
	#ifdef Q_OS_WINDOWS
	// Removed this because I implemented a better logging system that writes to files.
	// Need to re-evaluate if I want this behavior again later.
	//if (IsDebuggerPresent()) setupConsole();
	#endif

	// Create application
    QApplication a(argc, argv);

	// Initialize core app
	QCoreApplication::setApplicationName("ModularSensorSystem");
	QCoreApplication::setApplicationVersion("1.0.0");
	QCoreApplication::setOrganizationName("Riley Meyerkorth");
	QCoreApplication::setOrganizationDomain("rileymeyerkorth.com");

	// Initialize settings and controllers
	QSettings settings = QSettings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat, nullptr);
	MainController mainController(settings, nullptr);
	UiSettingsController uiSettingsController(settings, nullptr);

	// Crete and show main window
	MainWindow w(mainController, uiSettingsController, nullptr);
    w.show();

	// Execute the application and handle exceptions
    int ret;
    try {
        ret = a.exec();
	}
	catch (const std::bad_alloc&) {
		// TODO: clean up here, saving, closing files, etc.

		ret = EXIT_FAILURE;
	}
	catch (const std::exception& e) {
		qDebug() << "Exception caught in main:" << e.what();
		ret = EXIT_FAILURE;
	}
	catch (...) {
		qDebug() << "Unknown error caught in main.";
		ret = EXIT_FAILURE;
	}

	// Clean up and exit
	settings.deleteLater();
	mainController.deleteLater();
	uiSettingsController.deleteLater();
	w.deleteLater();

    return ret;
}
