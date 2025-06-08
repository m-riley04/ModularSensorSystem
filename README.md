# ModularSensorSystem

# Main Features
- Plugin-based architecture
	- Sources
	- Data Processors
- Simultaneous data recording
- Data visualization
- "Clipping" feature
- Real-time data processing
- Efficient and organized UI/UX

# Build Guide
1. Pull the repo
	- The lead developer (@m-riley04) uses [Visual Studio 2022](https://visualstudio.microsoft.com/vs/), so it is recommended to use that IDE for development.
	- Support for other platforms and IDEs is planned, but not yet implemented.
2. Install Qt 6.9.0
	- Use the [official installer](https://www.qt.io/download-qt-installer) and the [official Visual Studio extension](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022) for easy setup.
3. Install [vcpkg](https://vcpkg.io) and use it to install dependencies
	- The project uses a vcpkg manifest file to manage dependencies, so you can run `vcpkg install` to get everything set up.
	- The build process for the dependencies may take a while (anywhere from \~1 hour to \~4 hours), so be patient.
	- After the first build of the dependencies, they should be cached, so subsequent builds will be much faster.
4. Open the [solution file](ModularSensorSystem.sln) in Visual Studio
	- The project is set up to use C++20 and Qt 6.9.0, so make sure your Visual Studio installation supports these versions.

# Documentation and Development Resources
External documentation is found in the [docs](docs/) directory. The following should help you get started:
- [Architecture Overview](docs/architecture_overview.md)
- [Naming Conventions](docs/naming_conventions.md)

# Dependencies
- Qt 6.9.0 (can be installed and linked using the official installer and Visual Studio extension)
- All other dependencies are managed through a VCPKG manifest file