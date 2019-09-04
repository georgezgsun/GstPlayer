#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

#include <QApplication>
#include <QWidget>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QTimer>

// Code to play rtsp stream on qt widgets
int main(int argc, char *argv[])
{
      gst_init (nullptr, nullptr);
      QApplication app(argc, argv);
      app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit ()));

      // prepare the pipeline
      GstElement *pipeline1 = gst_parse_launch ("playbin uri=rtsp://10.0.9.113:8554/0 video-sink=xvimagesink", nullptr);
      GstElement *pipeline2 = gst_parse_launch ("playbin uri=rtsp://10.0.9.117:8554/0 video-sink=xvimagesink", nullptr);
      GstElement *pipeline3 = gst_parse_launch ("playbin uri=rtsp://10.0.9.119/1/h264major video-sink=xvimagesink", nullptr);
      GstElement *pipeline4 = gst_parse_launch ("playbin uri=rtsp://10.0.9.1150 video-sink=xvimagesink", nullptr);

      // prepare the ui
      gint w=640;
      gint h=360;
      QWidget window;
      window.setWindowTitle("Main Window");
      window.resize(1280,720);

      QVideoWidget w1(&window);
      w1.setGeometry(0, 0,w, h);
      w1.show();

      // prepare the ui
      QVideoWidget w2(&window);
      w2.setGeometry(640, 0, w, h);
      w2.show();

      WId xwinid1 = w1.winId();
      //this is the call to overlay the gstreamer's output to the Qt Widgets...
      gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (pipeline1), xwinid1);


      // run the pipeline
      GstStateChangeReturn sret = gst_element_set_state (pipeline1, GST_STATE_PLAYING);
      if (sret == GST_STATE_CHANGE_FAILURE)
      {
        gst_element_set_state (pipeline1, GST_STATE_NULL);
        gst_object_unref (pipeline1);
        // Exit application
        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
      }

      int ret = app.exec();

      window.hide();
      gst_element_set_state (pipeline1, GST_STATE_NULL);
      gst_object_unref (pipeline1);

      return ret;
}
