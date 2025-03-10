#include <gst/gst.h>

void on_pad_added(GstElement* src, GstPad* new_pad, GstElement* sink);
static gboolean on_bus_message(GstBus* bus, GstMessage* msg, GMainLoop* loop);

int main(int argc, char* argv[]) {
    GMainLoop* loop;
    GstBus* bus;
    guint bus_watch_id;

    // Initialize GStreamer
    gst_init(&argc, &argv);

    // Create a main loop
    loop = g_main_loop_new(NULL, FALSE);

    // Create the elements
    GstElement* pipeline = gst_pipeline_new("video-player");
    GstElement* source = gst_element_factory_make("filesrc", "file-source");
    GstElement* decoder = gst_element_factory_make("decodebin", "decode-bin");
    GstElement* convert = gst_element_factory_make("videoconvert", "convert");  // Add videoconvert
    GstElement* monitor = gst_element_factory_make("framemonitor", "monitor");  // Our new plugin
    GstElement* videosink = gst_element_factory_make("autovideosink", "video-output");

    // Check that all elements were created successfully
    if (!pipeline || !source || !decoder || !convert || !monitor || !videosink) {
        g_printerr("One or more elements could not be created.\n");
        return -1;
    }

    // Set the source file location
    g_object_set(G_OBJECT(source), "location", "../resources/videos/big_buck_bunny_scene.mp4", NULL);

    // Add all elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline), source, decoder, convert, monitor, videosink, NULL);

    // Link the elements that can be linked right away
    if (!gst_element_link(source, decoder)) {
        g_printerr("Source and decoder could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    if (!gst_element_link_many(convert, monitor, videosink, NULL)) {
        g_printerr("Convert, monitor and sink could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // Connect the pad-added signal for the decoder
    g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), convert);

    // Get the bus
    bus = gst_element_get_bus(pipeline);
    bus_watch_id = gst_bus_add_watch(bus, (GstBusFunc)on_bus_message, loop);
    gst_object_unref(bus);

    // Set the pipeline to the playing state
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // Run the main loop
    g_main_loop_run(loop);

    // Clean up
    gst_element_set_state(pipeline, GST_STATE_NULL);
    g_source_remove(bus_watch_id);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}

// Callback function to dynamically link pads
void on_pad_added(GstElement* src, GstPad* new_pad, GstElement* sink) {
    GstPad* sink_pad = gst_element_get_static_pad(sink, "sink");

    // Check if the sink pad is already linked
    if (gst_pad_is_linked(sink_pad)) {
        g_object_unref(sink_pad);
        return;
    }

    // Attempt to link the newly created pad with the sink pad
    GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
    if (GST_PAD_LINK_FAILED(ret)) {
        g_printerr("Type is '%s' but link failed.\n",
            gst_structure_get_name(gst_caps_get_structure(gst_pad_get_current_caps(new_pad), 0)));
    }

    g_object_unref(sink_pad);
}

// Bus message handler
static gboolean on_bus_message(GstBus* bus, GstMessage* msg, GMainLoop* loop) {
    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR: {
            GError* err;
            gchar* debug_info;
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error received from element %s: %s\n", 
                      GST_OBJECT_NAME(msg->src), err->message);
            g_printerr("Debugging information: %s\n", 
                      debug_info ? debug_info : "none");
            g_clear_error(&err);
            g_free(debug_info);
            g_main_loop_quit(loop);
            break;
        }
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            g_main_loop_quit(loop);
            break;
        default:
            break;
    }
    return TRUE;
}