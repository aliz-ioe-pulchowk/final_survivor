/* =============================================================================
 * game.h
 *
 * Top-level game initialisation and main loop coordination.
 *
 * Responsibilities:
 *   - Zero and populate the GameState struct
 *   - Call each system's init step in the correct order
 *   - Run the main loop: frame-limit, dispatch per-frame systems, check
 *     win/loss condition
 * =============================================================================
 */

#ifndef GAME_H
#define GAME_H

#include "types.h"

/* =============================================================================
 * Initialisation
 * =============================================================================
 */

/*
 * Prepare a freshly zeroed GameState for a new game.
 *
 * Steps performed in order:
 *   1. memset gs to 0.
 *   2. words_load() — parse words.txt into gs->pool[]; return 0 on failure.
 *   3. srand(time(NULL)).
 *   4. Populate scalar defaults: player_hp, spawn_interval_ms, base_speed,
 *      running = 1, con_out = GetStdHandle(STD_OUTPUT_HANDLE).
 *   5. console_set_size() + console_hide_cursor() + system("cls").
 *   6. Seed front[] with spaces so the first buf_flush paints everything.
 *   7. Stamp start_tick, last_frame_tick, last_spawn_tick, last_diff_tick.
 *
 * Returns 1 on success, 0 if word loading fails (caller should exit).
 */
int game_init(GameState *gs);

/* =============================================================================
 * Main loop
 * =============================================================================
 */

/*
 * Run the game until gs->running == 0 or gs->player_hp <= 0.
 *
 * Each iteration:
 *   1. Frame limiter — Sleep() if less than FRAME_MS has elapsed; clamp dt
 *      to MAX_DT_MS.
 *   2. input_handle()            — process keyboard events.
 *   3. enemy_update()            — advance physics.
 *   4. Spawner check             — if elapsed >= spawn_interval_ms call
 *                                  enemy_spawn() and update last_spawn_tick.
 *   5. enemy_update_difficulty() — scale difficulty every 10 s.
 *   6. HP check                  — break if player_hp <= 0.
 *   7. render_frame()            — compose and flush the frame.
 */
void game_run(GameState *gs);

#endif /* GAME_H */
