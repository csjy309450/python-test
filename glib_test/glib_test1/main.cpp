#include <glib.h>
#include <stdio.h>
#include <unistd.h>

gchar * name = "key\0";

void ProcessFunc_cb(gpointer data,
                    gpointer user_data)
{
    gint * p_int = (gint*)data;
    gchar * str = (gchar *)user_data;
    GThread *self = g_thread_self();

    g_print("%s: %d\n", str, *p_int);
    g_free(data);
}

int main(int argc, char* argv[])
{
    GError *err;
    gint * t_data = g_new(gint,1);
    *t_data = 2;
    GThreadPool * pool = g_thread_pool_new(ProcessFunc_cb,name,4,FALSE,NULL);
    if(!pool)
        return 1;
    for(gint i=0;i<100;i++){
        gint * t_data = g_new(gint,1);
        *t_data = i;
        g_thread_pool_push(pool,(gpointer)t_data,NULL);
    }
    getchar();
    return 0;
}