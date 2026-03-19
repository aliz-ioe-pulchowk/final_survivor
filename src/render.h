/* =============================================================================
 * render.h
 *
 * Frame composition and game-over screen output.
 *
 * Responsibilities:
 *   - Guard against mid-game window resize (re-enforce size + invalidate buf)
 *   - Compose a complete frame into the back buffer (status, enemies, wall,
 *     input prompt)
 *   - Flush only changed rows to the physical console (zero flicker)
 *   - Display the end-of-game summary and wait for a keypress
 * =============================================================================
 */

#ifndef RENDER_H
#define RENDER_H

#include "types.h"

/* =============================================================================
 * Per-frame render
 * =============================================================================
 */

/*
 * Compose and flush one full frame.  Call once per frame after all update
 * functions have run.
 *
 * Steps performed in order:
 *   1. Resize guard — query GetConsoleScreenBufferInfo(); if dimensions differ
 *      from CONSOLE_WIDTH x CONSOLE_HEIGHT call console_set_size(), cls, and
 *      memset front[] to 0 to force a full repaint next flush.
 *   2. buf_clear()  — blank back[] in RAM (no I/O).
 *   3. buf_write()  — paint four layers into back[]:
 *        a. status bar  row 0:  HP | kills | elapsed time | CPS | level
 *        b. enemy words at their current (col, row) positions
 *        c. castle wall at WALL_ROW
 *        d. input prompt at INPUT_ROW
 *   4. buf_flush()  — diff back[] vs front[], write only changed rows.
 */
void render_frame(GameState *gs, DWORD now);

/* =============================================================================
 * Game-over screen
 * =============================================================================
 */

/*
 * Clear the screen, print the end-of-game summary, and block until the player
 * presses any key.
 *
 * Displayed fields:
 *   - Enemies killed  (gs->kills)
 *   - Survival time   (now - gs->start_tick in seconds)
 *   - Difficulty level (gs->diff_level + 1)
 *   - Last recorded CPS (gs->last_cps)
 *
 * Restores cursor visibility before printing.
 */
void render_game_over(GameState *gs, DWORD now);

#endif /* RENDER_H */
