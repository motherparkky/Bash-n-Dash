#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <mqueue.h>
#include <ncurses.h>

#include "engine.h"
#include "input.h"
#define FRAME_USEC 16666  // 60 FPS

static volatile sig_atomic_t g_tick = 0;
static void tick_handler(int){ g_tick = 1; }

/* message struct sent by child */
typedef struct { char key; } BND_Msg;

static void consume_input(mqd_t mq){
    BND_Msg m;
    while(mq_receive(mq, (char*)&m, sizeof m, NULL) != -1){
        if(m.key == ' ') engine_on_jump();
        if(m.key == 'q') engine_is_game_over = 1;
    }
}

int main(void){
    srand(time(NULL) ^ getpid());
    /* open message queue before fork so child inherits descriptor */
    struct mq_attr attr = { .mq_flags=0, .mq_maxmsg=8, .mq_msgsize=sizeof(BND_Msg) };
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0600, &attr);

    pid_t pid = fork();
    if(pid == 0){
        /* child: keyboard input */
        input_child_loop();
        return 0;
    }

    /* parent: engine + renderer */
    initscr(); cbreak(); noecho(); curs_set(0);
    nodelay(stdscr, TRUE);

    signal(SIGALRM, tick_handler);
    struct itimerval iv = { .it_interval = {0, FRAME_USEC*1000}, .it_value = {0, FRAME_USEC*1000} };
    setitimer(ITIMER_REAL, &iv, NULL);

    engine_init();

    while(!engine_is_game_over){
        pause();           // wait for SIGALRM
        g_tick = 0;
        consume_input(mq);
        engine_update(1.0f/60.0f, COLS);
        engine_render();
    }

    endwin();
    kill(pid, SIGTERM);
    mq_close(mq);
    mq_unlink(MQ_NAME);
    return 0;
}
