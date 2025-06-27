#define _POSIX_C_SOURCE 199309L
#include <ncurses.h>
#include <mqueue.h>
#include <unistd.h>
#include <fcntl.h>
#include "input.h"

void input_child_loop(void)
{
    initscr(); cbreak(); noecho(); curs_set(0);
    nodelay(stdscr, TRUE);

    mqd_t mq = mq_open(MQ_NAME, O_WRONLY);
    InputMsg msg;

    while (1){
        int ch = getch();
        if (ch == ERR){ usleep(8000); continue; }
        if (ch == 'q'){ msg.key = 'q'; mq_send(mq, (char*)&msg, sizeof msg, 0); break; }
        if (ch == ' '){ msg.key = ' '; mq_send(mq, (char*)&msg, sizeof msg, 0); }
    }
    endwin();
    mq_close(mq);
}
