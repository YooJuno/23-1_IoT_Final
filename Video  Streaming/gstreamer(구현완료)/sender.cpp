// g++ sender.cpp -o receiver `pkg-config --cflags --libs gstreamer-1.0` 
#include <gst/gst.h>

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *convert, *encoder, *pay, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    /* Create the elements */
    source = gst_element_factory_make ("autovideosrc", "source");
    convert = gst_element_factory_make ("videoconvert", "convert");
    encoder = gst_element_factory_make ("x264enc", "encoder");
    pay = gst_element_factory_make ("rtph264pay", "pay");
    sink = gst_element_factory_make ("udpsink", "sink");

    /* Create the empty pipeline */
    pipeline = gst_pipeline_new ("test-pipeline");

    if (!pipeline || !source || !convert || !encoder || !pay || !sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    /* Set the encoder properties */
    g_object_set (encoder, "tune", 4, NULL);  // 4 is for zerolatency
    g_object_set (encoder, "speed-preset", 1, NULL);  // 1 is for ultrafast

    /* Set the sink properties */
    g_object_set (sink, "host", "127.0.0.1", NULL);
    g_object_set (sink, "port", 5000, NULL);

    /* Build the pipeline */
    gst_bin_add_many (GST_BIN (pipeline), source, convert, encoder, pay, sink, NULL);
    if (gst_element_link_many (source, convert, encoder, pay, sink, NULL) != TRUE) {
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
