cmake_minimum_required(VERSION 3.16)
project(ModularSensorSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows Specific Settings
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Directory Structure
add_subdirectory(src/core)
add_subdirectory(src/app)