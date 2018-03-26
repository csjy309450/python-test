//
// Created by yz on 18-3-17.
//

#include "GBoy.h"

/**用LAST_SIGNAL来表示最后一个信号（不用实现的信号）是一种非常良好的编程风格**/
enum { BOY_BORN, LAST_SIGNAL };
static gint g_boy_signals[LAST_SIGNAL] = { 0 };
static void g_boy_cry (void);
static void g_boy_born(void);
static void g_boy_init(GBoy *boy);
static void g_boy_class_init(GBoyClass *boyclass);

/**构造并存储静态类对象，由宏G_TYPE_BOY封装，相当于c++中class Boy作用了
 * 与c++对比：
 * GBoy *boy = new Boy(); //c++
 * GBoy *boy = (GBoy *)g_object_new(G_TYPE_BOY, NULL); // gobject
 * */
GType g_boy_get_type(void)
{
    static GType boy_type = 0;
    if(!boy_type) {
        static const GTypeInfo boy_info = {
                sizeof(GBoyClass),
                NULL,NULL,
                (GClassInitFunc)g_boy_class_init,
                NULL,NULL,
                sizeof(GBoy),
                0,
                (GInstanceInitFunc)g_boy_init
        };
        /**g_type_register_static函数用来注册对象的类型，
         * 它的第一个参数是表示此对象的父类的对象类型，我们这里是G_TYPE_OBJECT，这个宏用来表示GObject的父类；
         * 第二个参数表示此对象的名称，这里为"Boy"；
         * 第三个参数是此对象的GTypeInfo结构型指针，这里赋值为&boyinfo；
         * 第四个参数是对象注册成功后返回此对象的整型ID标识。 **/
        boy_type = g_type_register_static(G_TYPE_OBJECT,"GBoy",&boy_info,G_TYPE_FLAG_VALUE_ABSTRACT);
    }
    return boy_type;
}

/**boy_init和boy_class_init，它们分别用来初始化实例结构和类结构。它们并不被在代码中明显调用，
 * 关键是将其用宏转换为地址指针，然后赋值到GTypeInfo结构中，然后由GType系统自行处理，
 * 同时将它们定义为静态的也是非常必要的。 **/
static void g_boy_init(GBoy *boy)
{
    boy->age = 0;
    boy->name = "none";
    boy->cry = g_boy_cry;
}

static void g_boy_class_init(GBoyClass *boyclass)
{
    boyclass->boy_born = g_boy_born;
    /**Boy对象定义了一个信号BOY_BORN，在对象创建时发出，表示Boy对象诞生。
    *同时还需要定义静态的整型指针数组来保存信号的标识
    **/
    g_boy_signals[BOY_BORN] = g_signal_new("boy_born",
                                         G_TYPE_BOY,
                                         G_SIGNAL_RUN_FIRST,
                                         G_STRUCT_OFFSET(GBoyClass, boy_born),
                                         NULL,NULL,
                                         g_cclosure_marshal_VOID__VOID,
                                         G_TYPE_NONE, 0, NULL);
}

GBoy *g_boy_new(void)
{
    GBoy *boy;
    boy = (GBoy *)g_object_new(G_TYPE_BOY, NULL);
    //g_signal_emit向指定义对象的实例发射信号
    g_signal_emit(boy, g_boy_signals[BOY_BORN], 0);
    return boy;
}
int g_boy_get_age(GBoy *boy)
{
    return boy->age;
}
void g_boy_set_age(GBoy *boy, int age)
{
    boy->age = age;
}
char *g_boy_get_name(GBoy *boy)
{
    return boy->name;
}
void g_boy_set_name(GBoy *boy, char *name)
{
    boy->name = name;
}
GBoy*  g_boy_new_with_name(gchar *name)
{
    GBoy *boy;
    boy = g_boy_new();
    g_boy_set_name(boy, name);
    return boy;
}
GBoy*  g_boy_new_with_age(gint age)
{
    GBoy* boy;
    boy = g_boy_new();
    g_boy_set_age(boy, age);
    return boy;
}
GBoy *g_boy_new_with_name_and_age(gchar *name, gint age)
{
    GBoy *boy;
    boy = g_boy_new();
    g_boy_set_name(boy,name);
    g_boy_set_age(boy,age);
    return boy;
}
static void g_boy_cry (void)
{
    g_print("---The Boy is crying......\n");
}
static void g_boy_born(void)
{
    g_print("---Message : A boy was born.\n");
}
void g_boy_info(GBoy *boy)
{
    g_print("---The Boy name is %s\n", boy->name);
    g_print("---The Boy age is %d\n", boy->age);
}