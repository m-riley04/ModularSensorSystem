# Set singletons
set_source_files_properties(Docking.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(AppComponentsDockingLib
    URI app.components.docking
    VERSION 1.0
    DESIGNER_SUPPORTED
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
        QtQuick.Window
    DEPENDENCIES
        TARGET AppStylesLib
    QML_FILES
        DockGroup.qml
        DockOverlay.qml
        DockPanel.qml
        DockRoot.qml
        FloatingDock.qml
        DockRow.qml
        Docking.qml
        QML_FILES DockZoneOverlay.qml
        QML_FILES DockRowInsertOverlay.qml
)
