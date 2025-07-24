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

# target_link_libraries(YoloObjectDetectionProcessorPlugin PRIVATE core)

# add_custom_command(TARGET YoloObjectDetectionProcessorPlugin POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:YoloObjectDetectionProcessorPlugin> "${CMAKE_BINARY_DIR}/plugins/processors"
# )
