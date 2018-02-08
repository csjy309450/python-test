#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <curses.h>

void ctl_term()
{
     FILE *f_tty_out=fopen("/dev/tty", "r");
    FILE *f_tty_in=fopen("/dev/tty", "w");
    while(1){
        char str_cmd[20]="",*pstr=str_cmd;
        fputs("cmd$ ", f_tty_in);
        fflush(f_tty_in);
        do{
            *(pstr)=fgetc(f_tty_out);
        }while(*(pstr++)!='\n');
        if(!strcmp(str_cmd,":quit\n"))
            break;
        system(str_cmd);
    }
    fputs("done.\n", f_tty_in);

    fclose(f_tty_in);
    fclose(f_tty_out);
}

int main()
{
    initscr();
    move(5,15);
    printw("%s","Hello world");
    refresh();
    sleep(2);
    beep();
    flash();

    getchar();
    endwin();
    return 0;
}

