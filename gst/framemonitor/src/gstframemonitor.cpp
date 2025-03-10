#include "gstframemonitor.h"

GST_DEBUG_CATEGORY_STATIC(frame_monitor_debug);
#define GST_CAT_DEFAULT frame_monitor_debug

// Define properties enum
enum {
    PROP_0,
    // Add properties here if needed
};

// Define pad templates
static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE(
    "sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS(GST_VIDEO_CAPS_MAKE("{ I420, NV12, RGB, BGR, RGBA, BGRA }"))
);

static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE(
    "src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS(GST_VIDEO_CAPS_MAKE("{ I420, NV12, RGB, BGR, RGBA, BGRA }"))
);

// Define the element's class initialization
#define gst_frame_monitor_parent_class parent_class
G_DEFINE_TYPE(GstFrameMonitor, gst_frame_monitor, GST_TYPE_ELEMENT);

static GstFlowReturn gst_frame_monitor_chain(GstPad* pad, GstObject* parent, GstBuffer* buf);
static gboolean gst_frame_monitor_sink_event(GstPad* pad, GstObject* parent, GstEvent* event);

// Initialize debug category
static void
gst_frame_monitor_class_init(GstFrameMonitorClass* klass)
{
    GstElementClass* element_class = GST_ELEMENT_CLASS(klass);

    // Initialize debug category
    GST_DEBUG_CATEGORY_INIT(frame_monitor_debug, "framemonitor", 0, "Frame monitoring element");

    // Set metadata for the element
    gst_element_class_set_static_metadata(element_class,
        "Frame Monitor",
        "Filter/Analyzer/Video",
        "Monitors video frames and displays information about them",
        "Rosemary Koikara <rosekoikara@gmail.com>");

    // Add pad templates
    gst_element_class_add_pad_template(element_class,
        gst_static_pad_template_get(&src_template));
    gst_element_class_add_pad_template(element_class,
        gst_static_pad_template_get(&sink_template));
}

// Initialize instance
static void
gst_frame_monitor_init(GstFrameMonitor* filter)
{
    // Create pads
    filter->sinkpad = gst_pad_new_from_static_template(&sink_template, "sink");
    filter->srcpad = gst_pad_new_from_static_template(&src_template, "src");

    // Set pad chain functions
    gst_pad_set_chain_function(filter->sinkpad, GST_DEBUG_FUNCPTR(gst_frame_monitor_chain));
    gst_pad_set_event_function(filter->sinkpad, GST_DEBUG_FUNCPTR(gst_frame_monitor_sink_event));

    // Add pads to element
    gst_element_add_pad(GST_ELEMENT(filter), filter->sinkpad);
    gst_element_add_pad(GST_ELEMENT(filter), filter->srcpad);

    // Initialize other members
    filter->frame_count = 0;
    filter->last_pts = GST_CLOCK_TIME_NONE;
    filter->fps = 0.0;
}

// Handle sink events
static gboolean
gst_frame_monitor_sink_event(GstPad* pad, GstObject* parent, GstEvent* event)
{
    GstFrameMonitor* filter = GST_FRAME_MONITOR(parent);
    gboolean ret = TRUE;

    switch (GST_EVENT_TYPE(event)) {
        case GST_EVENT_CAPS: {
            GstCaps* caps;
            gst_event_parse_caps(event, &caps);
            
            // Parse the caps into video info
            if (!gst_video_info_from_caps(&filter->video_info, caps)) {
                GST_ERROR_OBJECT(filter, "Failed to parse caps");
                ret = FALSE;
            } else {
                // Log the video format information
                GST_INFO_OBJECT(filter, "Video format: %s, Width: %d, Height: %d",
                    gst_video_format_to_string(GST_VIDEO_INFO_FORMAT(&filter->video_info)),
                    GST_VIDEO_INFO_WIDTH(&filter->video_info),
                    GST_VIDEO_INFO_HEIGHT(&filter->video_info));
            }
            break;
        }
        default:
            break;
    }

    // Forward event
    return gst_pad_event_default(pad, parent, event);
}

// Chain function
static GstFlowReturn
gst_frame_monitor_chain(GstPad* pad, GstObject* parent, GstBuffer* buf)
{
    GstFrameMonitor* filter = GST_FRAME_MONITOR(parent);
    GstClockTime pts = GST_BUFFER_PTS(buf);
    GstMapInfo map;

    // Map the buffer for reading
    if (!gst_buffer_map(buf, &map, GST_MAP_READ)) {
        GST_ERROR_OBJECT(filter, "Failed to map buffer");
        return GST_FLOW_ERROR;
    }

    // Calculate FPS
    if (GST_CLOCK_TIME_IS_VALID(filter->last_pts) && GST_CLOCK_TIME_IS_VALID(pts)) {
        GstClockTimeDiff diff = GST_CLOCK_DIFF(filter->last_pts, pts);
        if (diff > 0) {
            filter->fps = GST_SECOND / (gdouble)diff;
        }
    }
    filter->last_pts = pts;

    // Get buffer flags
    guint flags = GST_BUFFER_FLAGS(buf);
    gboolean is_keyframe = !(flags & GST_BUFFER_FLAG_DELTA_UNIT);
    gboolean is_header = !!(flags & GST_BUFFER_FLAG_HEADER);
    gboolean is_gap = !!(flags & GST_BUFFER_FLAG_GAP);
    gboolean is_droppable = !!(flags & GST_BUFFER_FLAG_DROPPABLE);

    // Log frame information
    g_print("Frame #%lu:\n"
            "  Size: %lu bytes\n"
            "  PTS: %" GST_TIME_FORMAT "\n"
            "  DTS: %" GST_TIME_FORMAT "\n"
            "  Duration: %" GST_TIME_FORMAT "\n"
            "  Calculated FPS: %.2f\n"
            "  Format: %s\n"
            "  Resolution: %dx%d\n"
            "  Flags: %s%s%s%s\n",
            filter->frame_count++,
            map.size,
            GST_TIME_ARGS(pts),
            GST_TIME_ARGS(GST_BUFFER_DTS(buf)),
            GST_TIME_ARGS(GST_BUFFER_DURATION(buf)),
            filter->fps,
            gst_video_format_to_string(GST_VIDEO_INFO_FORMAT(&filter->video_info)),
            GST_VIDEO_INFO_WIDTH(&filter->video_info),
            GST_VIDEO_INFO_HEIGHT(&filter->video_info),
            is_keyframe ? "KEYFRAME " : "",
            is_header ? "HEADER " : "",
            is_gap ? "GAP " : "",
            is_droppable ? "DROPPABLE" : "");

    // Unmap the buffer
    gst_buffer_unmap(buf, &map);

    // Push the buffer downstream
    return gst_pad_push(filter->srcpad, buf);
}