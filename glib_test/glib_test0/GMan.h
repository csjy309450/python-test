//
// Created by yz on 18-3-17.
//

#ifndef GLIB_TEST_GMan_H
#define GLIB_TEST_GMan_H

/**********Man继承自GBoy对象，Man对象在GBoy对象的基础上又增加了一个属性job和一个方法bye***********/
#include "GBoy.h"

#define G_TYPE_MAN (g_man_get_type())
#define G_MAN(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), G_TYPE_MAN, GMan))

typedef struct _GMan GMan;
typedef struct _GManClass GManClass;

struct _GMan
{
    GBoy parent;
    gchar *job;
    void (*bye)(void);
};
struct _GManClass
{
    GBoyClass parent_class;
};

GType g_man_get_type();
GMan *g_man_new(void);
gchar *g_man_get_job(GMan *man);
void g_man_set_job(GMan *man, gchar *job);
GMan *g_man_new_with_name_age_and_job(gchar *name, gint age, gchar *job);
void g_man_info(GMan *man);

#endif //GLIB_TEST_GMan_H
