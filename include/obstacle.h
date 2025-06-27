#ifndef BASH_N_DASH_OBSTACLE_H
#define BASH_N_DASH_OBSTACLE_H

#include <ncurses.h>

/* Obstacle constants */
#define OB_MAX            64
#define OB_WIDTH          3
#define OB_HEIGHT         2
#define OB_SPEED_START    220.0f      /* px/s */
#define OB_SPEED_INC_EVERY 15.0f      /* sec  */
#define OB_SPEED_INC_STEP  20.0f
#define OB_SPAWN_MIN       0.8f       /* sec  */
#define OB_SPAWN_MAX       1.6f       /* sec  */

typedef struct { float x, y; } Obstacle;

void ob_init(void);
void ob_spawn(float start_x, float ground_y);
void ob_update(float dt, float speed);
bool ob_collide(float px, float py, int pw, int ph);
void ob_draw(WINDOW *w);

#endif
