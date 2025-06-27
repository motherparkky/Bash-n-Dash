#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "obstacle.h"
#include "util.h"

static Obstacle obs[OB_MAX];
static int head = 0, tail = 0;

static inline int ob_count(void){ return (tail + OB_MAX - head) % OB_MAX; }

void ob_init(void){ head = tail = 0; memset(obs, 0, sizeof obs); }

void ob_spawn(float start_x, float ground_y)
{
    if (((tail + 1) % OB_MAX) == head) return; /* full; drop */
    obs[tail] = (Obstacle){ .x = start_x, .y = ground_y - 1 }; /* top-left */
    tail = (tail + 1) % OB_MAX;
}

void ob_update(float dt, float speed)
{
    for(int i = head; i != tail; i = (i + 1) % OB_MAX)
        obs[i].x -= speed * dt;

    while (ob_count() && obs[head].x + OB_WIDTH < 0)
        head = (head + 1) % OB_MAX;
}

bool ob_collide(float px, float py, int pw, int ph)
{
    for(int i = head; i != tail; i = (i + 1) % OB_MAX){
        Obstacle *o = &obs[i];
        bool hit = !(px+pw <= o->x || o->x+OB_WIDTH <= px ||
                py+ph <= o->y || o->y+OB_HEIGHT <= py);
        if (hit) return true;
    }
    return false;
}

void ob_draw(WINDOW *w)
{
    for(int i = head; i != tail; i = (i + 1) % OB_MAX){
        Obstacle *o = &obs[i];
        for(int dy = 0; dy < OB_HEIGHT; ++dy)
            mvwaddnstr(w, (int)o->y - dy, (int)o->x, "###", OB_WIDTH);
    }
}
