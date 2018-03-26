#include <glib.h>
#include <exception>
#include "GBoy.h"
#include "GMan.h"

using namespace std;

int main(int argc, char *argv[])
{
    GBoy *tom, *peter;
    GMan *green, *brown;
    g_type_init();//注意，初始化类型系统，必需
    g_print("**********************\n");
    tom = g_boy_new_with_name("Tom");
    tom->cry();
    g_boy_info(tom);
    g_print("**********************\n");
    peter = g_boy_new_with_name_and_age("Peter", 10);
    peter->cry();
    g_boy_info(peter);
    g_print("**********************\n");
    g_print("######################\n");
    green = g_man_new();
    /**设定Man对象的name属性用到其父对象Boy的方法**/
    g_boy_set_name((GBoy*)green, "Green");
    g_boy_set_age((GBoy*)green, 28);
    g_man_set_job(green, "Doctor");
    g_man_info(green);
    green->bye();
    g_print("######################\n");
    brown = g_man_new_with_name_age_and_job("Brown", 30, "Teacher");
    g_man_info(brown);
    brown->bye();
    g_print("######################\n");
    GBoyClass * boy_class = (GBoyClass *)G_OBJECT_GET_CLASS(tom);
    boy_class->boy_born();


    return 0;
}