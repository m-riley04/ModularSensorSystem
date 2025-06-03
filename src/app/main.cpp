#include "Windows/MainWindow/MainWindow.h"
#include <QtWidgets/QApplication>
#include <sources/Source/SourceError/sourceerror.h>

int main(int argc, char *argv[])
{
	// Register metatypes
	qRegisterMetaType<SourceError>("SourceError");

	// Create application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
	

	// Get the stylesheet from the styles file
	


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
