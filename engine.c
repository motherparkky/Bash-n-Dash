#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include "engine.h"
#include "obstacle.h"
#include "util.h"

volatile sig_atomic_t engine_is_game_over = 0;

static Player  player;
static WINDOW *pad;

/* Obstacle & difficulty state */
static float speed_pxps;
static float spawn_timer, spawn_next;
static float time_accum;

void engine_init(void)
{
    int term_h, term_w; getmaxyx(stdscr, term_h, term_w);
    pad = newpad(term_h, term_w);

    player = (Player){ .x = 5, .y = term_h - 2, .vy = 0, .w = 3, .h = 2 };

    speed_pxps  = OB_SPEED_START;
    spawn_timer = 0.f;
    spawn_next  = 1.f;
    time_accum  = 0.f;

    ob_init();
}

void engine_on_jump(void)
{
    if (player.vy == 0) player.vy = JUMP_VELOCITY;
}

void engine_update(float dt, int term_w)
{
    int term_h; getmaxyx(stdscr, term_h, term_w);

    /* Player physics */
    player.vy += GRAVITY_PPS2 * dt;
    player.y  += player.vy * dt;
    if (player.y >= term_h - 2){ player.y = term_h - 2; player.vy = 0; }

    /* Obstacles */
    spawn_timer += dt;
    if (spawn_timer >= spawn_next){
        ob_spawn(term_w - 1, term_h - 1);
        spawn_timer = 0.f;
        spawn_next  = randf(OB_SPAWN_MIN, OB_SPAWN_MAX);
    }
    ob_update(dt, speed_pxps);

    /* Difficulty scaling */
    time_accum += dt;
    if (time_accum >= OB_SPEED_INC_EVERY){
        speed_pxps += OB_SPEED_INC_STEP;
        time_accum  = 0.f;
    }

    /* Collision */
    if (ob_collide(player.x, player.y - 1, player.w, player.h))
        engine_is_game_over = 1;
}

void engine_render(void)
{
    int term_h, term_w; getmaxyx(stdscr, term_h, term_w);
    werase(pad);

    ob_draw(pad);
    mvwaddstr(pad, (int)player.y - 1, (int)player.x, "(•ω•)");

    prefresh(pad, 0, 0, 0, 0, term_h - 1, term_w - 1);
}
