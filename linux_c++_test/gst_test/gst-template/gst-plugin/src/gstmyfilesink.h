/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstfilesink.h:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_MY_FILE_SINK_H__
#define __GST_MY_FILE_SINK_H__

#include <stdio.h>

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

G_BEGIN_DECLS

#define GST_TYPE_MY_FILE_SINK \
  (gst_my_file_sink_get_type())
#define GST_MY_FILE_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MY_FILE_SINK,GstMyFileSink))
#define GST_MY_FILE_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MY_FILE_SINK,GstMyFileSinkClass))
#define GST_IS_MY_FILE_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MY_FILE_SINK))
#define GST_IS_MY_FILE_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MY_FILE_SINK))

typedef struct _GstMyFileSink GstMyFileSink;
typedef struct _GstMyFileSinkClass GstMyFileSinkClass;

/**
 * GstMyFileSink:
 *
 * Opaque #GstMyFileSink structure.
 */
struct _GstMyFileSink {
  GstBaseSink parent;

  /*< private >*/
  gchar *filename;
  gchar *uri;
  FILE *file;

  gboolean seekable;
  guint64 current_pos;

  gint    buffer_mode;
  guint   buffer_size;
  gchar  *buffer;
  
  gboolean append;
};

struct _GstMyFileSinkClass {
  GstBaseSinkClass parent_class;
};

G_GNUC_INTERNAL GType gst_my_file_sink_get_type (void);

G_END_DECLS

#endif /* __GST_MY_FILE_SINK_H__ */
