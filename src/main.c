#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <mqueue.h>
#include <ncurses.h>
#include "engine.h"
#include "input.h"

static volatile sig_atomic_t g_tick = 0;
static void tick_handler(int s){ (void)s; g_tick = 1; }

/* Consume MQ and forward to engine */
static void consume_input(mqd_t mq)
{
    InputMsg m;
    while (mq_receive(mq, (char*)&m, sizeof m, NULL) != -1){
        if (m.key == ' ') engine_on_jump();
        if (m.key == 'q') engine_is_game_over = 1;
    }
}

int main(void)
{
    /* Open MQ */
    struct mq_attr attr = { .mq_maxmsg = 8, .mq_msgsize = sizeof(InputMsg) };
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0600, &attr);

    /* Fork child for keyboard */
    pid_t child = fork();
    if (child == 0){ input_child_loop(); return 0; }

    /* Parent: init curses */
    initscr(); cbreak(); noecho(); curs_set(0);

    /* Timer */
    struct itimerval iv = {
        .it_interval = { .tv_sec = 0, .tv_usec = FRAME_USEC },
        .it_value    = { .tv_sec = 0, .tv_usec = FRAME_USEC }
    };
    signal(SIGALRM, tick_handler);
    setitimer(ITIMER_REAL, &iv, NULL);

    engine_init();

    while (!engine_is_game_over){
        pause();                     /* wait for SIGALRM */
        g_tick = 0;
        consume_input(mq);
        engine_update(FRAME_DT, COLS);
        engine_render();
    }

    endwin();
    kill(child, SIGTERM);
    mq_close(mq); mq_unlink(MQ_NAME);
    return 0;
}
