#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iconv.h>
#include <iostream>
#include <cstring>
#include <mysql/mysql.h>

using namespace std;

int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char server[] = "localhost";
    char user[] = "yz";
    char password[] = "123456";
    char database[] = "test";

    conn = mysql_init(NULL);

    //链接数据库
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    // 设置数据库的字符编码, 必须和数据库中实际的编码相符,否则获取到的数据乱码
    if (mysql_set_character_set(conn, "utf8") != 0)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return 1;
    }

    // 执行SQL语句
    if (mysql_query(conn, "select * from persons"))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    // 获取查询结果
    res = mysql_use_result(conn);

    MYSQL_FIELD * field;
    while((field= mysql_fetch_field(res))!=NULL){
        std::cout<<field->name<<" ";
        if(IS_NUM(field->type)){
            std::cout<<"num";
        }
        else{
            switch(field->type){
                case FIELD_TYPE_VAR_STRING:
                    std::cout<<"varchar";
                    break;
                case FIELD_TYPE_INTERVAL:
                    std::cout<<"int";
                    break;
                default:
                    break;
            }
        }
        std::cout<<std::endl;
    }


    printf("MySQL Tables in mysql database:\n");

    while ((row = mysql_fetch_row(res)) != NULL)
    {
        std::cout<<row[0]<<",\t"<<row[1]<<",\t"<<row[2]<<",\t"<<row[3]<<std::endl;
    }

    mysql_free_result(res);
    mysql_close(conn);

    printf("finish! \n");
    return 0;
}