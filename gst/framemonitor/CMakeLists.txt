# Plugin CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(gst-framemonitor VERSION 1.0)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GST_INCLUDE_DIRS}
)

# Link directories
link_directories(
    ${GST_LIBRARY_DIRS}
)

# Create the plugin library
add_library(${PROJECT_NAME} SHARED
    src/gstframemonitor.cpp
    src/plugin.cpp
)

# Set the output name with the correct prefix
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX "libgst"
    OUTPUT_NAME "framemonitor"
)

# Link against GStreamer libraries
target_link_libraries(${PROJECT_NAME}
    ${GST_LIBRARIES}
)

# Installation
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib/gstreamer-1.0
)

# Install headers (useful if other plugins need to use this one)
install(FILES
    include/gstframemonitor.h
    DESTINATION include/gstreamer-1.0/gst/framemonitor
)

# Add debugging symbols in Debug build
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3>
)

# Print plugin configuration
message(STATUS "Framemonitor plugin configuration:")
message(STATUS "  Installation prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Debug symbols: $<CONFIG:Debug>")