#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "dir_searcher.h"

#define MAX_PATH_NUM 255

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        printf("input a directory like \"./dir_search ./home\"");
        return 0;
    }
    char **top_dir=&(argv[1]);
    while((*top_dir!=NULL))
    {
        printf("***************begine**************\n");
        printf("searching <%s>\n", *(top_dir));
        printdir(*(top_dir++),0);
        printf("****************end***************\n");
    }
    printf("done.\n");

    return 0;
}
