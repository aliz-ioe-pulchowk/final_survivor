/* =============================================================================
 * enemy.h
 *
 * Enemy lifecycle: spawning, per-frame physics, and difficulty scaling.
 *
 * Responsibilities:
 *   - Activate a free Enemy slot with a picked word and initial position
 *   - Advance all active enemies downward each frame (accumulator physics)
 *   - Detect wall collisions and apply HP damage
 *   - Increase speed and spawn rate every DIFF_INTERVAL_MS milliseconds
 * =============================================================================
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "types.h"

/* =============================================================================
 * Spawning
 * =============================================================================
 */

/*
 * Activate the first free slot in gs->enemies[] with a freshly picked word.
 *
 * The new enemy is placed at:
 *   y = PLAY_START_ROW
 *   x = random in [SPAWN_COL_MIN, SPAWN_COL_MAX]
 *   speed = gs->base_speed
 *   acc = 0
 *   active = 1
 *
 * Does nothing if all MAX_ENEMIES slots are currently occupied.
 * Calls words_pick() internally for the word selection.
 */
void enemy_spawn(GameState *gs);

/* =============================================================================
 * Physics update
 * =============================================================================
 */

/*
 * Advance every active enemy downward for one frame.
 *
 * Accumulator pattern per enemy:
 *   acc += speed * dt_sec
 *   while acc >= 1.0: y += 1, acc -= 1
 *
 * Wall collision:
 *   if (int)y >= WALL_ROW:
 *     gs->player_hp -= WALL_HIT_DAMAGE
 *     active = 0
 *
 * dt_sec is the clamped delta-time from the main loop (never exceeds
 * MAX_DT_MS / 1000.0f).
 */
void enemy_update(GameState *gs, float dt_sec);

/* =============================================================================
 * Difficulty scaling
 * =============================================================================
 */

/*
 * Check whether DIFF_INTERVAL_MS has elapsed since gs->last_diff_tick.
 * If so:
 *   gs->diff_level++
 *   gs->spawn_interval_ms *= SPAWN_SCALE  (floored at SPAWN_MIN_MS)
 *   gs->base_speed        += SPEED_PER_LEVEL
 *   gs->last_diff_tick     = now
 *
 * Existing on-screen enemies keep their current speed; only newly spawned
 * enemies inherit the updated base_speed.
 */
void enemy_update_difficulty(GameState *gs, DWORD now);

#endif /* ENEMY_H */
