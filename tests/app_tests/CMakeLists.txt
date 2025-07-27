cmake_minimum_required(VERSION 3.22)
project(
  app_test
  VERSION 1.0
  LANGUAGES CXX)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS QuickTest Qml)

# no need to copy around qml test files for shadow builds - just set the respective define
add_definitions(-DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE QML_FILES *.qml)
add_custom_target(qmlfiles SOURCES ${QML_FILES})

add_executable(app_test main.cpp
               setup.cpp setup.h)

# Add tests
add_test(NAME app_test COMMAND app_test)
add_test(NAME CustomTitleBar COMMAND CustomTitleBar)

target_link_libraries(app_test
  PRIVATE
    Qt6::QuickTest
    Qt6::Qml
    AppComponentsLib
)

