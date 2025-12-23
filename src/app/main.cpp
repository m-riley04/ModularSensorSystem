#include "Windows/MainWindow/MainWindow.h"
#include <QtWidgets/QApplication>
#include "controllers/maincontroller.hpp"
#include <QSettings>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <vector>

static std::wstring Utf8ToWide(const std::string& s)
{
    if (s.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (n <= 0) throw std::runtime_error("MultiByteToWideChar size failed");
    std::wstring w(n - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, w.data(), n);
    return w;
}

static std::string GetEnvA(const char* name)
{
    char* buf = nullptr;
    size_t len = 0;
    if (_dupenv_s(&buf, &len, name) != 0 || !buf) return {};
    std::string v(buf);
    free(buf);
    return v;
}

static void ConfigureDllSearch()
{
    // Establish a safe, explicit search policy for this process.
    // This changes how dependent DLLs are resolved for modules loaded by name.
    if (!SetDefaultDllDirectories(
		LOAD_LIBRARY_SEARCH_DEFAULT_DIRS
		| LOAD_LIBRARY_SEARCH_USER_DIRS
		| LOAD_LIBRARY_SEARCH_APPLICATION_DIR))
        throw std::runtime_error("SetDefaultDllDirectories failed");

    std::vector<std::wstring> dirs;

    // ORT
    std::string ortRoot = GetEnvA("ORT_ROOT");
    if (!ortRoot.empty())
        dirs.push_back(Utf8ToWide(ortRoot + "\\bin"));

	// Plugins and their deps
	char path[MAX_PATH];
	// Get the full path to the executable
	GetModuleFileNameA(NULL, path, MAX_PATH);
	std::filesystem::path exe_path(path);
	std::string pluginsDirPath = exe_path.parent_path().string() + "\\plugins";
	std::string sourcePluginsPath = pluginsDirPath + "\\sources";
	std::string mountsPluginsPath = pluginsDirPath + "\\mounts";
	std::string procsPluginsPath = pluginsDirPath + "\\processors";
    
    dirs.push_back(Utf8ToWide(sourcePluginsPath));
    dirs.push_back(Utf8ToWide(mountsPluginsPath));
    dirs.push_back(Utf8ToWide(procsPluginsPath));

    for (const auto& d : dirs)
    {
        if (!d.empty())
        {
            DLL_DIRECTORY_COOKIE cookie = AddDllDirectory(d.c_str());
            if (!cookie)
                throw std::runtime_error("AddDllDirectory failed for one of the directories");
            // Optionally store cookies if you ever want to RemoveDllDirectory(cookie).
        }
    }
}

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

	// Configure DLL search paths for Windows (specifically for ONNX Runtime)
	ConfigureDllSearch();


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

    return ret;
}
