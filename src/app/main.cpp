#include "Windows/MainWindow/MainWindow.h"
#include <QtWidgets/QApplication>

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
	if (IsDebuggerPresent()) setupConsole();
	#endif

	// Create application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

	// Initialize core app
	QCoreApplication::setApplicationName("ModularSensorSystem");
	QCoreApplication::setApplicationVersion("1.0.0");
	QCoreApplication::setOrganizationName("Riley Meyerkorth");
	QCoreApplication::setOrganizationDomain("rileymeyerkorth.com");

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
    return ret;
}
