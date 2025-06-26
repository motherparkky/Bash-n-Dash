#ifndef BND_OBSTACLE_H
#define BND_OBSTACLE_H
#include "common.h"
void ob_init(void);
void ob_spawn(float term_width);
void ob_update(float dt, float *speed);
bool ob_collide(float px, float py, int pw, int ph);
#endif // BND_OBSTACLE_H
