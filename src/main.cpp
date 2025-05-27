#include "Windows/MainWindow/MainWindow.h"
#include <QtWidgets/QApplication>
#include <devices/Device/DeviceError/deviceerror.h>

int main(int argc, char *argv[])
{
	// Register metatypes
	qRegisterMetaType<DeviceError>("DeviceError");

	// Create application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

	// Execute the application and handle exceptions
    int ret;
    try {
        ret = a.exec();
	}
	catch (const std::exception& e) {
		qDebug() << "Exception caught in main:" << e.what();
		ret = EXIT_FAILURE;
	}
	catch (const std::bad_alloc&) {
		// TODO: clean up here, saving, closing files, etc.

		ret = EXIT_FAILURE;
	}
	catch (...) {
		qDebug() << "Unknown exception caught in main.";
		ret = EXIT_FAILURE;
	}
    return ret;
}
