#include <ncurses.h>
#include <string.h>
#include "obstacle.h"

#define GROUND_Y(stdscr) (getmaxy(stdscr)-2)

typedef struct { float x, y; int w, h; } Ob;
#define OB_MAX 128
static Ob obs[OB_MAX];
static int head = 0, tail = 0;

static int count(void){ return (tail+OB_MAX-head)%OB_MAX; }

void ob_init(void){ head = tail = 0; }

void ob_spawn(float term_w){
    obs[tail] = (Ob){ .x = term_w-1, .y = LINES-3, .w = 3, .h = 2 };
    tail = (tail+1)%OB_MAX;
}

void ob_update(float dt, float *speed){
    for(int i=head;i!=tail;i=(i+1)%OB_MAX) obs[i].x -= *speed * dt;
    while(count() && obs[head].x+obs[head].w < 0) head=(head+1)%OB_MAX;
}

bool ob_collide(float px,float py,int pw,int ph){
    for(int i=head;i!=tail;i=(i+1)%OB_MAX){
        Ob* o=&obs[i];
        bool hit=!(px+pw<=o->x || o->x+o->w<=px || py+ph<=o->y || o->y+o->h<=py);
        if(hit) return true;
    }
    return false;
}

void ob_draw(WINDOW* w){
    for(int i=head;i!=tail;i=(i+1)%OB_MAX){
        Ob* o=&obs[i];
        for(int dy=0;dy<o->h;++dy)
            mvwaddnstr(w, (int)o->y-dy, (int)o->x, "###", o->w);
    }
}
