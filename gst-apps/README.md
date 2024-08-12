# GStreamer Applications (`gst-apps/`)

This directory contains GStreamer applications developed as part of the tutorial series. Each application demonstrates specific GStreamer concepts and techniques, ranging from basic pipeline setups to more advanced custom elements.

## Directory Structure

- **`basic-pipeline/`**: Contains the example applications for simple GStreamer pipelines, such as reading from a file and displaying video output.
  - **`simple_player.cpp`**: A basic video player that reads from a file, decodes the video, and displays it using `autovideosink`.

- **(Additional folders will be added as the series progresses)**

## Running the Examples

To run the examples in this directory, follow these steps:

1. **Navigate to the desired example directory:**

   ```bash
   cd gst-apps/basic-pipeline
   ```

2. **Compile the application:**

   For C++ applications, you can use `g++` to compile:

   ```bash
   g++ simple_player.cpp -o simple_player `pkg-config --cflags --libs gstreamer-1.0`
   ```

3. **Run the compiled application:**

   ```bash
   ./simple_player
   ```

## Resources and Configurations

Some examples may require specific video files or configuration files. Make sure these are placed in the appropriate directory within the `resources/` folder.

## License

All applications in this directory are licensed under the MIT License - see the [LICENSE](../LICENSE) file for more details.