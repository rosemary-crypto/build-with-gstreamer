cmake_minimum_required(VERSION 3.10)
project(build-with-gstreamer VERSION 1.0)

# Set C++ standard for all targets
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_PLUGINS "Build GStreamer plugins" ON)
option(BUILD_APPS "Build example applications" ON)

# Find required packages for the entire project
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED 
    gstreamer-1.0
    gstreamer-base-1.0
    gstreamer-video-1.0
)

# Add the plugins directory
if(BUILD_PLUGINS)
    add_subdirectory(gst/framemonitor)
endif()

# Add the applications directory
if(BUILD_APPS)
    add_subdirectory(gst-apps/lesson-02-plugins)
endif()

# Print build configuration
message(STATUS "Build configuration:")
message(STATUS "  Build plugins: ${BUILD_PLUGINS}")
message(STATUS "  Build apps: ${BUILD_APPS}")
message(STATUS "  GStreamer version: ${GST_VERSION}")