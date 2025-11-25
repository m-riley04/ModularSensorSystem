# ModularSensorSystem

## Main Features

- Plugin-based architecture
	- Sources
	- Data Processors
	- Physical Mounts
- Simultaneous data recording
- Data visualization
- "Clipping" feature
- Real-time data processing
- Efficient and organized UI/UX

## Build Guide

1. Pull the repo
	- The lead developer (@m-riley04) uses [Visual Studio 2022](https://visualstudio.microsoft.com/vs/), so it is recommended to use that IDE for development.
	- Support for other platforms and IDEs is planned, but not yet implemented.
2. Install Qt 6.8.3
	- Use the [official installer](https://www.qt.io/download-qt-installer) and the [official Visual Studio extension](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022) for easy setup.
3. Install [vcpkg](https://vcpkg.io) and use it to install dependencies
	- The project uses a vcpkg manifest file to manage dependencies, so you can run `vcpkg install` to get everything set up.
	- The build process for the dependencies may take a while (anywhere from \~1 hour to \~4 hours), so be patient.
	- After the first build of the dependencies, they should be cached, so subsequent builds will be much faster.
4. Open the [solution file](ModularSensorSystem.sln) in Visual Studio
	- The project is set up to use C++20 and Qt 6.8.3, so make sure your Visual Studio installation supports these versions.

### Environment Variables
The project uses the following (manually-set) environment variables to locate plugins:

- `GST_DEBUG_DUMP_DOT_DIR` - This is the directory where GStreamer will dump debug dot files. This is mainly useful for development and debugging of GStreamer pipelines.
- `GST_PLUGIN_PATH` - The directory where GStreamer plugins are located. This will be the `plugins/gstreamer` folder in the vcpkg installation directory.

To make this easier, a property sheet named `MSSEnvVars.props` has been created. This is mainly only for the frontend/runtime environment.

### Known Issues

#### GStreamer Include Paths on Windows

For whatever reason since 2021 (from what I've found), there has been a bug with vcpkg and specifically with the `gstreamer` library on Windows. I've found a few issues on it ([2021](https://github.com/microsoft/vcpkg/issues/20483), [2022](https://github.com/microsoft/vcpkg/issues/26417), [2023](https://github.com/microsoft/vcpkg/issues/32239)).

Because of this bug, the following paths need to be added to the `Additional Include Paths` of the project:

- `$(_ZVcpkgCurrentInstalledDir)/include/glib-2.0`
- `$(_ZVcpkgCurrentInstalledDir)/lib/glib-2.0/include`
- `$(_ZVcpkgCurrentInstalledDir)/include/gstreamer-1.0`
- `$(_ZVcpkgCurrentInstalledDir)/lib/gstreamer-1.0/include`

This has been simplified for you by adding these paths to a property sheet named `VcpkgIncludeDirs.props`. Add this property sheet to any project that uses vcpkg dependencies to resolve the issue.

#### GStreamer `pango` Plugin Hash Error

When building the dependencies, you may see an error related to the hash for `pango` having a mismatch.

This is a [known issue](https://github.com/microsoft/vcpkg/issues/47984) with vcpkg and can be resolved by doing the following steps:
1. Download the tarball for `pango` [here](https://gitlab.gnome.org//GNOME/pango/-/archive/1.54.0/pango-1.54.0.tar.gz).
2. Append "GNOME-" to the beginning of the downloaded file's name, so it becomes `GNOME-pango-1.54.0.tar.gz`.
3. Move the file to the `downloads` folder in your vcpkg installation directory.
4. Re-run `vcpkg install`/build.

## Documentation and Development Resources

External documentation is found in the [docs](docs/) directory. The following should help you get started:
- [Architecture Overview](docs/architecture_overview.md)
- [Naming Conventions](docs/naming_conventions.md)
- GStreamer Docs
- [Qt Documentation](https://doc.qt.io/qt-6/index.html)]

## Dependencies

- Qt 6.8.3 (can be installed and linked using the official installer and Visual Studio extension)
- All other dependencies are managed through a VCPKG manifest file
- GStreamer (installed through vcpkg, but reccomended to install locally for global debug tools)


## Development Journal

Development journal entries can be found in the [docs/development_journal.md](docs/development_journal.md) file. These entries are just neat/interesting things I've found over the course of development that might help someone else in the future. My headaches are your gain!