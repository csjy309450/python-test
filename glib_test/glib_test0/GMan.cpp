//
// Created by yz on 18-3-17.
//

#include "GMan.h"

static void g_man_bye(void);
static void g_man_init(GMan *man);
static void g_man_class_init(GMan *man);

/**关键在于定义对象时将父对象实例定义为GBoy，父类设定为GBoyClass，在注册此对象时将其父对象类型设为BOY_TYPE。
 **/
GType g_man_get_type(void)
{
    static GType g_man_type = 0;
    if(!g_man_type) {
        static const GTypeInfo g_man_info = {
                sizeof(GManClass),
                NULL, NULL,
                (GClassInitFunc)g_man_class_init,
                NULL, NULL,
                sizeof(GMan),
                0,
                (GInstanceInitFunc)g_man_init
        };
        /**在注册此对象时将其父对象类型设为BOY_TYPE**/
        g_man_type = g_type_register_static(G_TYPE_BOY, "GMan", &g_man_info, G_TYPE_FLAG_VALUE_ABSTRACT);
    }
    return g_man_type;
}

static void g_man_init(GMan *man)
{
    man->job = "none";
    man->bye = g_man_bye;
}

static void g_man_class_init(GMan *man)
{
}

GMan*  g_man_new(void)
{
    GMan *man;
    man = (GMan*)g_object_new(G_TYPE_MAN, 0);
    return man;
}

gchar* g_man_get_job(GMan *man)
{
    return man->job;
}

void  g_man_set_job(GMan *man, gchar *job)
{
    man->job = job;
}

GMan*  g_man_new_with_name_age_and_job(gchar *name, gint age, gchar *job)
{
    GMan *man;
    man = g_man_new();
    g_boy_set_name((GBoy*)man, name);
    g_boy_set_age((GBoy*)man, age);
    g_man_set_job(man, job);
    return man;
}

static void g_man_bye(void)
{
    g_print("+++Goodbye everyone!\n");
}

/**在设定对象属性时如用到父对象的属性要强制转换下，如取得对象的name属性，就必须用BOY(obj)->name，  
 *因为Man本身没有name属性，而其父对象Boy有，所以用BOY宏将其强制为Boy类型的对象。  
**/
void g_man_info(GMan *man)
{
    /**定义对象时将父对象实例定义为Boy，父类设定为BoyClass**/
    g_print("+++the man name is %s\n", ((GBoy*)man)->name);
    g_print("+++the man age is %d\n", ((GBoy*)man)->age);
    g_print("+++the man job is %s\n", man->job);
}  