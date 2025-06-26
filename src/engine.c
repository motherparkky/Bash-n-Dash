#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include "engine.h"
#include "obstacle.h"

volatile sig_atomic_t engine_is_game_over = 0;

static Player player;

static WINDOW *buf;
static float speed_pxps;
static float spawn_timer, spawn_next;
static float time_accum;

static float randf(float a, float b){ return a + (rand()/(float)RAND_MAX)*(b-a); }

void engine_init(void){
    int term_h, term_w; getmaxyx(stdscr, term_h, term_w);
    buf = newpad(term_h, term_w);
    player = (Player){ .x = 5, .y = term_h-2, .vy = 0, .w = 3, .h = 2 };
    speed_pxps   = 220.0f;
    spawn_timer  = 0.f;
    spawn_next   = 1.0f;
    time_accum   = 0.f;
    ob_init();
}

void engine_on_jump(void){
    int term_h, term_w; getmaxyx(stdscr, term_h, term_w);
    if(player.vy == 0) player.vy = -700.f;
}

void engine_update(float dt, int term_w){
    /* physics */
    int term_h; getmaxyx(stdscr, term_h, term_w);
    const float gravity = 2000.f;
    player.vy += gravity * dt;
    player.y  += player.vy * dt;
    if(player.y >= term_h-2){ player.y = term_h-2; player.vy = 0; }

    /* spawn / move obstacles */
    spawn_timer += dt;
    if(spawn_timer >= spawn_next){
        ob_spawn(term_w);
        spawn_timer = 0.f;
        spawn_next  = randf(0.8f, 1.6f);
    }
    ob_update(dt, &speed_pxps);

    /* scale difficulty */
    time_accum += dt;
    if(time_accum >= 15.f){
        speed_pxps += 20.f;
        time_accum  = 0.f;
    }

    /* collision */
    if(ob_collide(player.x, player.y-1, player.w, player.h))
        engine_is_game_over = 1;
}

void engine_render(void){
    int term_h, term_w; getmaxyx(stdscr, term_h, term_w);
    werase(buf);

    /* draw obstacles */
    extern void ob_draw(WINDOW*); // forward; implemented in obstacle.c
    ob_draw(buf);

    /* draw player */
    mvwaddstr(buf, (int)player.y-1, (int)player.x, "(•ω•)");

    pnoutrefresh(buf, 0,0, 0,0, term_h-1, term_w-1);
    doupdate();
}
