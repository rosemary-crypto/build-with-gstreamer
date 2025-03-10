# Build With GStreamer

Welcome to the **Build With GStreamer** repository! This project is part of a series of tutorials on GStreamer development, designed to help you learn how to build multimedia applications using the GStreamer framework.

## Project Overview

This repository contains code examples, configurations, and resources used in the tutorial series.

### Directory Structure 

- **`gst-apps/`**: Contains the GStreamer applications demonstrated in the tutorials.
  - `lesson-01-basic-pipeline/`: Basic video playback pipeline
  - `lesson-02-plugins/`: Custom plugin usage examples
- **`resources/`**: Holds test videos, configuration files, and other resources needed to run the applications.
- **`gst/`**: Contains custom GStreamer plugins and elements.
  - `framemonitor/`: A simple frame monitoring plugin
- **`gst-libs/`** (future): Will contain custom GStreamer libraries as the lessons progress.

## Tutorial Series Structure

### Phase 1: Building the Foundation
- **Lesson 1**: Basic GStreamer Pipeline ✅
  - Understanding basic pipeline creation
  - Video playback implementation
  
- **Lesson 2**: Custom GStreamer Plugin Development 🚧
  - Creating a frame monitoring plugin
  - Understanding GStreamer element lifecycle
  - Buffer handling and frame analysis

### Phase 2: Advanced Features (Coming Soon)
- Custom metadata handling
- Real-time video processing
- GPU acceleration
- Multi-stream processing

## Getting Started

### Prerequisites
- GStreamer development libraries
- CMake (>= 3.10)
- C++ compiler with C++14 support

### Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/rosemary-crypto/build-with-gstreamer.git
   cd build-with-gstreamer
   ```

2. **Install GStreamer:**
   If GStreamer is not already installed, you can install it by following the steps in the [GStreamer Application Development Manual](https://gstreamer.freedesktop.org/documentation/installing/index.html?gi-language=c).

3. **Build the project:**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

4. **Set up the environment:**
   ```bash
   # Add the custom plugins to GStreamer's plugin path
   export GST_PLUGIN_PATH=$GST_PLUGIN_PATH:$PWD/gst/framemonitor

   # Enable debug output (optional)
   export GST_DEBUG=framemonitor:4
   ```

### Running Examples

Each lesson has its own example applications that demonstrate different aspects of GStreamer development.

1. **Check out the specific lesson:**
   ```bash
   git checkout lesson-02-plugins  # For lesson 2
   ```

2. **Run the examples:**
   ```bash
   # Lesson 1 - Basic Pipeline
   ./gst-apps/lesson-01-basic-pipeline/simple_player

   # Lesson 2 - Plugin Test
   ./gst-apps/lesson-02-plugins/framemonitor_test
   ```

## Project Structure

### Current Lessons
- **Lesson 1**: Basic video playback pipeline implementation
- **Lesson 2**: Custom frame monitoring plugin development

### Future Topics
- Custom GStreamer elements and plugins
- Video analytics integration
- Performance optimization
- Deployment strategies

## Branching and Tagging

- **Main Branch (`main`)**: Contains the most up-to-date and fully functional version of the project.
- **Lesson Branches**: Each lesson has its own branch, e.g., `lesson-01-basic-pipeline`.
- **Tags**: Significant milestones and completed lessons are tagged, e.g., `v1.0`, `lesson-01-complete`.

## Troubleshooting

### Common Issues
1. **Plugin not found:**
   - Ensure `GST_PLUGIN_PATH` is set correctly
   - Check if the plugin was built successfully

2. **Build errors:**
   - Verify GStreamer development packages are installed
   - Ensure CMake version is 3.10 or higher
   - Check C++ compiler supports C++14

### Debug Output
Enable debug logging for specific components:
```bash
export GST_DEBUG=framemonitor:4  # Debug output for framemonitor plugin
export GST_DEBUG=3               # General GStreamer debug output
```

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

## Contributions

Contributions are welcome! Feel free to:
- Submit issues for bugs or feature requests
- Fork the repository and submit pull requests
- Suggest improvements to documentation
- Share your experience and questions

## Contact

If you have any questions or need assistance:
- Open an issue on GitHub
- Connect on [LinkedIn](https://www.linkedin.com/in/rosemary-koikara/) ![LinkedIn Icon](https://cdn-icons-png.flaticon.com/16/174/174857.png)
- Check the [GStreamer documentation](https://gstreamer.freedesktop.org/documentation/) for additional resources