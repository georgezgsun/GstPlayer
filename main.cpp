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
      GstElement *pipeline1 = gst_parse_launch ("playbin uri=rtsp://10.0.9.112/h264 video-sink=nveglglessink", nullptr);
      GstElement *pipeline2 = gst_parse_launch ("playbin uri=rtsp://10.0.9.113:8554/0 video-sink=nveglglessink", nullptr);
      GstElement *pipeline3 = gst_parse_launch ("playbin uri=rtsp://10.0.9.116/1/h264major video-sink=nveglglessink", nullptr);
      GstElement *pipeline4 = gst_parse_launch ("playbin uri=rtsp://10.0.9.117:8554/0 video-sink=nveglglessink", nullptr);

      // prepare the ui
      gint w=640;
      gint h=360;
      QWidget window;
      window.setWindowTitle("Main Window");
      window.resize(1280,720);
      window.show();

      // define the window1
      QVideoWidget w1(&window);
      w1.setGeometry(0, 0,w+w, h+h);
      w1.setAttribute(Qt::WA_NativeWindow, true);
      w1.show();

      // define the window2
      QVideoWidget w2(&window);
      w2.setGeometry(w, 0, w / 2, h / 2);
      w2.setAttribute(Qt::WA_NativeWindow, true);
      w2.show();

      // define the window3
      QVideoWidget w3(&window);
      w3.setGeometry(0, h, w / 2, h / 2);
      w3.setAttribute(Qt::WA_NativeWindow, true);
      w3.show();

      // define the window2
      QVideoWidget w4(&window);
      w4.setGeometry(0, 0, w, h);
      w4.setAttribute(Qt::WA_NativeWindow, true);
      w4.show();

      WId xwinid1 = w1.winId();
      //this is the call to overlay the gstreamer's output to the Qt Widgets...
      gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (pipeline1), xwinid1);

      // run the pipeline1
      GstStateChangeReturn sret = gst_element_set_state (pipeline1, GST_STATE_PLAYING);
      if (sret == GST_STATE_CHANGE_FAILURE)
      {
        gst_element_set_state (pipeline1, GST_STATE_NULL);
        gst_object_unref (pipeline1);
        // Exit application
        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
      }

      WId xwinid2 = w2.winId();
      //this is the call to overlay the gstreamer's output to the Qt Widgets...
      gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (pipeline2), xwinid2);

      // run the pipeline2
      sret = gst_element_set_state (pipeline2, GST_STATE_PLAYING);
      if (sret == GST_STATE_CHANGE_FAILURE)
      {
        gst_element_set_state (pipeline2, GST_STATE_NULL);
        gst_object_unref (pipeline2);
        // Exit application
        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
      }

      WId xwinid3 = w3.winId();
      //this is the call to overlay the gstreamer's output to the Qt Widgets...
      gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (pipeline3), xwinid3);

      // run the pipeline3
      sret = gst_element_set_state (pipeline3, GST_STATE_PLAYING);
      if (sret == GST_STATE_CHANGE_FAILURE)
      {
        gst_element_set_state (pipeline3, GST_STATE_NULL);
        gst_object_unref (pipeline3);
        // Exit application
        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
      }

      WId xwinid4 = w4.winId();
      //this is the call to overlay the gstreamer's output to the Qt Widgets...
      gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (pipeline4), xwinid4);

      // run the pipeline4
      sret = gst_element_set_state (pipeline4, GST_STATE_PLAYING);
      if (sret == GST_STATE_CHANGE_FAILURE)
      {
        gst_element_set_state (pipeline4, GST_STATE_NULL);
        gst_object_unref (pipeline4);
        // Exit application
        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
      }

      int ret = app.exec();

      window.hide();
      gst_element_set_state (pipeline1, GST_STATE_NULL);
      gst_object_unref (pipeline1);

      return ret;
}
