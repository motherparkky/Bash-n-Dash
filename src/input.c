#include <ncurses.h>
#include <mqueue.h>
#include <unistd.h>
#include "input.h"

typedef struct { char key; } BND_Msg;

void input_child_loop(void){
    mqd_t mq = mq_open(MQ_NAME, O_WRONLY);
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); curs_set(0);

    while(1){
        int ch = getch();
        if(ch == 'q' || ch == ' '){
            BND_Msg m = { .key = (char)ch };
            mq_send(mq, (char*)&m, sizeof m, 0);
            if(ch == 'q') break;
        }
        usleep(8000); // 8 ms poll
    }
    endwin();
}
