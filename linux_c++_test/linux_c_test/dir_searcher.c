#include <dirent.h>
#include <string.h>
#include "dir_searcher.h"

void printdir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp=opendir(dir))==NULL)
    {
        fprintf(stderr, "can't open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry=readdir(dp))!=NULL)
    {
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode))
        {
            if(strcmp(".", entry->d_name)==0||strcmp("..",entry->d_name)==0)
                continue;
            printf("%*s%s/\n",depth*2,"",entry->d_name);
            printdir(entry->d_name, depth+1);
        }
        else
            printf("%*s%s\n",depth*2,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);
}