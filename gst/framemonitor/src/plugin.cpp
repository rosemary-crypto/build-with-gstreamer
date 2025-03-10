#include <gst/gst.h>
#include "gstframemonitor.h"

// Define package information
#define PACKAGE "framemonitor"
#define VERSION "1.0"
#define LICENSE "MIT"
#define DESCRIPTION "Frame monitoring element"
#define BINARY_PACKAGE "GStreamer Frame Monitor"
#define PACKAGE_NAME "GStreamer Frame Monitor"
#define PACKAGE_STRING "GStreamer Frame Monitor 1.0"
#define ORIGIN "https://github.com/rosemary-crypto/build-with-gstreamer"

static gboolean
plugin_init(GstPlugin* plugin)
{
    return gst_element_register(
        plugin,                // plugin: The plugin within which we're registering
        "framemonitor",        // name: The name of the element
        GST_RANK_NONE,        // rank: The rank of the element (none, marginal, secondary, primary)
        GST_TYPE_FRAME_MONITOR // type: The GType of the element
    );
}

// Plugin definition
GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,    // major version
    GST_VERSION_MINOR,    // minor version
    framemonitor,         // name of the plugin
    DESCRIPTION,          // description
    plugin_init,          // initialization routine
    VERSION,             // version
    LICENSE,             // license
    PACKAGE_NAME,        // package
    ORIGIN              // origin
)