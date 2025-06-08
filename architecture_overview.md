# Architecture Overview
For my own sanity and anyone else who might want to contribute, I am going to give a brief overview of the architecture of this project.

- The frontend built with Qt 6.9.0 and C++20, providing a modern and responsive user interface.
- The backend is implemented in C++20, ensuring high performance and efficiency.
	- I am attempting to use the Qt framework as much as possible (ex: `QString` instead of `std::string`), but there are some areas where I have to use standard C/C++ libraries.
- Dependencies are managed with a vcpkg manifest.
	- This allows for easy installation and management of libraries across different platforms; specifically, Windows and Linux.
- The system is built around a plugin-based architecture, allowing for easy addition of new data sources and processors.
	- Plugins are dynamically loaded at runtime, enabling users to extend the system's capabilities without modifying the core codebase.
	- Currently, 2 features are "pluggable": sources and processors (more on them in a bit)
	- To create a new plugin, you can follow the [Plugin Development Guide](docs/plugin_development.md) (unfinished).
- A "Source" is a class that provides data to the system.
	- A "Source" could be a physical device (video, audio, GPS, etc.), a software component, an API, or any other data provider.
	- All sources implement a common interface, allowing the system to handle them uniformly.
- A "Processor" is a class that processes data from sources.
	- This could include filtering, transformation, or any other data manipulation.
	- Primarily (for my use case), processors will be used for real-time object detection, motion detection, and tracking.

## Supported Source Types
- Analog Sensors
- Video Input
- Audio Input
- GPS NMEA