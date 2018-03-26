/**
 *main.c
 *Simple media player based on Gstreamer and GTK
 * Created by yz on 18-3-21.
*/

#ifndef PROJECT_GTK_PLAYER_H
#define PROJECT_GTK_PLAYER_H

#include <gst/gst.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <gst/video/videooverlay.h>
#include <string.h>

typedef enum {
    STATE_STOP,
    STATE_PLAY,
    STATE_PAUSE
} PlayerState;

void gui_status_update(PlayerState state);
void gui_update_time(const gchar *time, const gint64 position, const gint64 length);
// void gui_update_metadata(const gchar *title, const gchar *artist);
gboolean play_file();
GtkWidget *build_gui();
gboolean load_file(const gchar *uri);
void seek_to(gdouble percentage);



#endif //PROJECT_GTK_PLAYER_H
