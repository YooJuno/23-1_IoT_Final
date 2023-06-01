// g++ receiver.cpp -o receiver `pkg-config --cflags --libs gstreamer-1.0` 
#include <gst/gst.h>

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *depay, *decoder, *conv, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    /* Create the elements */
    source = gst_element_factory_make ("udpsrc", "source");
    GstCaps *caps = gst_caps_from_string("application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)H264");
    g_object_set(G_OBJECT(source), "caps", caps, NULL);
    gst_caps_unref(caps);
    depay = gst_element_factory_make ("rtph264depay", "depay");
    decoder = gst_element_factory_make ("avdec_h264", "decoder");
    conv = gst_element_factory_make ("videoconvert", "conv");
    sink = gst_element_factory_make ("autovideosink", "sink");

    /* Create the empty pipeline */
    pipeline = gst_pipeline_new ("test-pipeline");

    if (!pipeline || !source || !depay || !decoder || !conv || !sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    /* Set the source properties */
    g_object_set (source, "port", 5000, NULL);

    /* Build the pipeline */
    gst_bin_add_many (GST_BIN (pipeline), source, depay, decoder, conv, sink, NULL);
    if (gst_element_link_many (source, depay, decoder, conv, sink, NULL) != TRUE) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (pipeline);
        return -1;
    }

    /* Start playing */
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return -1;
    }

    /* Wait until error or EOS */
    bus = gst_element_get_bus (pipeline);
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    /* Free resources */
    if (msg != NULL)
        gst_message_unref (msg);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
    return 0;
}
