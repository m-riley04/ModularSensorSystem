set(SRC
    usbvideoplugin.cpp
    usbvideoplugin.h
    USBVideoClipBuffer/usbvideoclipbuffer.cpp
    USBVideoClipBuffer/usbvideoclipbuffer.h
    USBVideoClipEncoder/usbvideoclipencoder.cpp
    USBVideoClipEncoder/usbvideoclipencoder.h
    USBVideoPreview/usbvideopreview.cpp
    USBVideoPreview/usbvideopreview.h
    USBVideoPropertiesWidget/usbvideopropertieswidget.cpp
    USBVideoPropertiesWidget/usbvideopropertieswidget.h
    USBVideoPropertiesWidget/QtCameraFormatTableWidgetItem/qtcameraformatablewidgetitem.cpp
    USBVideoPropertiesWidget/QtCameraFormatTableWidgetItem/qtcameraformatablewidgetitem.h
    USBVideoPropertiesWidget/usbvideopropertieswidget.ui
    USBVideoRecordingPropertiesWidget/usbvideorecordingpropertieswidget.cpp
    USBVideoRecordingPropertiesWidget/usbvideorecordingpropertieswidget.h
    USBVideoRecordingPropertiesWidget/usbvideorecordingpropertieswidget.ui
    USBVideoSource/usbvideosource.cpp
    USBVideoSource/usbvideosource.h
)

add_library(USBVideoPlugin MODULE ${SRC})

set_target_properties(USBVideoPlugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/sources"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/sources"
)

target_link_libraries(USBVideoPlugin
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::Concurrent
        ${TARGET_CORE}
)
