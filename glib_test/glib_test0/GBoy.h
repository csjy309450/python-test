//
// Created by yz on 18-3-17.
//

#ifndef GLIB_TEST_GBOY_H
#define GLIB_TEST_GBOY_H

#include <glib-object.h>

/**G_BOY_TYPE : 宏封装了boy_get_type函数，可以直接取得并替代Boy对象的ID标识；
 * G_BOY(obj) : 宏是G_TYPE_CHECK_INSTANCE_CAST宏的再一次封装，目的是将一个GObject对象强制转换为GBoy对象
**/
#define G_TYPE_BOY (g_boy_get_type())
#define G_BOY(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), G_TYPE_BOY, GBoy))

typedef struct _GBoy GBoy;
typedef struct _GBoyClass GBoyClass;

/**结构类型_GBoy是GBoy对象的实例，就是说我们每创建一个GBoy对象，也就同时创建了一个_GBoy结构。
 * _GBoy对象中的parent表示此对象的父类，GObject系统中所有对象的共同的根都是GObject类，所以这是必须的；
 * 其它的成员可以是公共的，这里包括表示年龄的age，表示名字的name和表示方法的函数指针cry，
 * 外部代码可以操作或引用它们。
**/
struct _GBoy
{
    GObject parent;
    gint age;
    gchar *name;
    void (*cry)(void);
};

/**结构类型_GBoyClass是GBoy对象的类结构，它是所有GBoy对象实例所共有的。
 * GBoyClass中的parent_class是GObjectClass，同GObject是所有对象的共有的根一样，GObejctClass是所有对象的类结构的根。
 * 在BoyClass中我们还定义了一个函数指针boy_born，也就是说这一函数指针也是所有Boy对象实例共有的，
 * 所有的Boy实例都可以调用它；同样，如果需要的话，你也可以在类结构中定义其它数据成员。
**/
struct _GBoyClass
{
    GObjectClass parent_class;
    void (*boy_born)(void);
};

/**其余的函数定义包括三种：一种是取得GBoy对象的类型ID的函数g_boy_get_type，这是必须有的；
 * 另一种是创建GBoy对象实例的函数g_boy_new和g_boy_new_with_*，这是非常清晰明了的创建对象的方式，当然你也可以用g_object_new函数来创建对象；
 * 第三种是设定或取得GBoy对象属性成员的值的函数g_boy_get_*和g_boy_set_*。
 * 正常情况下这三种函数都是一个对象所必需的，另外一个函数g_boy_info用来显示此对象的当前状态**/
GType g_boy_get_type(void);
GBoy *g_boy_new(void);
gint g_boy_get_age(GBoy *boy);
void g_boy_set_age(GBoy *boy, int age);
char * g_boy_get_name(GBoy *boy);
void g_boy_set_name(GBoy *boy, char *name);
GBoy *g_boy_new_with_name(gchar *name);
GBoy * g_boy_new_with_age(gint age);
GBoy * g_boy_new_with_name_and_age(gchar *name, gint age);
void  g_boy_info(GBoy *boy);


#endif //GLIB_TEST_GBOY_H
