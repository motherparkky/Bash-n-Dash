#ifndef BASH_N_DASH_ENGINE_H
#define BASH_N_DASH_ENGINE_H

#include <stdbool.h>

// ---------------------------------------------------------------------------
// Gameplay‑tuning constants (feel free to tweak)
// ---------------------------------------------------------------------------
#define FRAME_DT          (1.0f / 60.0f)   // Fixed timestep (seconds)
#define GRAVITY_PPS2      2000.0f          // Gravity  in px / s²
#define JUMP_VELOCITY     -700.0f          // Initial Y‑speed for a jump

#define OB_SPEED_START    220.0f           // Obstacle speed at t=0 (px/s)
#define OB_SPEED_STEP     20.0f            // Speed increase every OB_SPEED_INC_EVERY
#define OB_SPEED_INC_EVERY 15.0f           // Seconds between speed bumps

#define OB_SPAWN_GAP_MIN  0.8f             // Min seconds between spawns
#define OB_SPAWN_GAP_MAX  1.6f             // Max seconds between spawns

// ---------------------------------------------------------------------------
// Data structures
// ---------------------------------------------------------------------------
typedef struct {
    float x, y;    // Position (top‑left of sprite) in terminal cells
    float vy;      // Vertical velocity (px/s)
    int   w, h;    // Hitbox size  (width, height)
} Player;

extern volatile sig_atomic_t engine_is_game_over;
// ---------------------------------------------------------------------------
// Public engine API
// ---------------------------------------------------------------------------
/** Initialize engine state.  Must be called **after** ncurses `initscr()`. */
void engine_init(void);

/** Handle a jump keypress (called from input process via IPC). */
void engine_on_jump(void);

/**
 * Advance the simulation by one fixed timestep (FRAME_DT).
 *
 * @param dt         Time delta in seconds (normally FRAME_DT)
 */
void engine_update(float dt, int term_w);

/** Render the current frame to the given ncurses window (usually `stdscr`). */
void engine_render(void);

#endif // BASH_N_DASH_ENGINE_H

