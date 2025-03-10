#ifndef __GST_FRAME_MONITOR_H__
#define __GST_FRAME_MONITOR_H__

#include <gst/gst.h>
#include <gst/video/video.h>

G_BEGIN_DECLS

// Define the GType macro for our element
#define GST_TYPE_FRAME_MONITOR (gst_frame_monitor_get_type())
// Define the cast macros for our element
#define GST_FRAME_MONITOR(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_FRAME_MONITOR, GstFrameMonitor))
#define GST_FRAME_MONITOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_FRAME_MONITOR, GstFrameMonitorClass))
#define GST_IS_FRAME_MONITOR(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_FRAME_MONITOR))
#define GST_IS_FRAME_MONITOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_FRAME_MONITOR))

typedef struct _GstFrameMonitor GstFrameMonitor;
typedef struct _GstFrameMonitorClass GstFrameMonitorClass;

// Structure to hold the element's private data
struct _GstFrameMonitor {
    GstElement parent;  // Parent element structure

    // Pads
    GstPad *sinkpad;
    GstPad *srcpad;

    // Add private data here
    GstVideoInfo video_info;  // Store video information
    guint64 frame_count;      // Count processed frames
    GstClockTime last_pts;    // Last presentation timestamp
    gdouble fps;              // Current FPS
};

// Structure to hold the element's class data
struct _GstFrameMonitorClass {
    GstElementClass parent_class;  // Parent class structure
};

// Declare the get_type function
GType gst_frame_monitor_get_type(void);

// Declare chain and event functions
static GstFlowReturn gst_frame_monitor_chain(GstPad* pad, GstObject* parent, GstBuffer* buf);
static gboolean gst_frame_monitor_sink_event(GstPad* pad, GstObject* parent, GstEvent* event);

G_END_DECLS

#endif /* __GST_FRAME_MONITOR_H__ */