cmake_minimum_required(VERSION 3.10)
project(lesson-02-app VERSION 1.0)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GST_INCLUDE_DIRS}
)

# Link directories
link_directories(
    ${GST_LIBRARY_DIRS}
)

# Create the application
add_executable(framemonitor_test
    src/main.cpp
)

# Link against GStreamer libraries
target_link_libraries(framemonitor_test
    ${GST_LIBRARIES}
)

# Add debugging symbols in Debug build
target_compile_options(framemonitor_test PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3>
)

# Installation (optional)
install(TARGETS framemonitor_test
    RUNTIME DESTINATION bin
)

# Print application configuration
message(STATUS "Lesson 02 application configuration:")
message(STATUS "  Installation prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Debug symbols: $<CONFIG:Debug>")