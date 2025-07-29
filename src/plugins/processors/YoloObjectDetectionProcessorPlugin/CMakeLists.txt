set(SRC
    yoloobjectdetectionprocessorplugin.cpp
    yoloobjectdetectionprocessorplugin.h
    yoloobjectdetectionprocessorplugin.json
    YoloObjectDetectionProcessor/yoloobjectdetectionprocessor.cpp
    YoloObjectDetectionProcessor/yoloobjectdetectionprocessor.h
)

add_library(YoloObjectDetectionProcessorPlugin MODULE ${SRC})

set_target_properties(YoloObjectDetectionProcessorPlugin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/processors"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/processors"
)

target_link_libraries(YoloObjectDetectionProcessorPlugin
    PRIVATE
        Qt6::Core
        Qt6::Multimedia
        ${TARGET_CORE}
)
