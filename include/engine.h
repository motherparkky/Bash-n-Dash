#ifndef BASH_N_DASH_ENGINE_H
#define BASH_N_DASH_ENGINE_H

#include <signal.h>
#include <stdbool.h>

/* Frame timing */
#define FPS            60
#define FRAME_USEC     16666           /* 1e6 / 60 */
#define FRAME_DT       (1.0f/60.0f)

/* Physics */
#define GRAVITY_PPS2   2000.0f         /* px/s^2 */
#define JUMP_VELOCITY  -700.0f         /* px/s   */

/* Player structure */
typedef struct {
    float x, y, vy;   /* position & vertical speed */
    int   w, h;       /* hitbox size */
} Player;

/* Public engine API */
void engine_init(void);
void engine_update(float dt, int term_w);
void engine_render(void);
void engine_on_jump(void);

/* Global game‑over flag (set by engine or main) */
extern volatile sig_atomic_t engine_is_game_over;

#endif /* BASH_N_DASH_ENGINE_H */
