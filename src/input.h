/* =============================================================================
 * input.h
 *
 * Non-blocking keyboard input and word submission logic.
 *
 * Responsibilities:
 *   - Drain the key buffer each frame without blocking the game loop
 *   - Maintain gs->input_buf and the CPS timing state
 *   - On SPACE/Enter: match input_buf against active enemies, destroy on hit,
 *     record CPS, then clear the buffer
 *   - Handle ESC (quit), Backspace (pop + timer reset), extended keys (discard)
 * =============================================================================
 */

#ifndef INPUT_H
#define INPUT_H

#include "types.h"

/* =============================================================================
 * Per-frame input poll
 * =============================================================================
 */

/*
 * Call once per frame from the main loop.
 *
 * Drains all pending keystrokes via _kbhit() / _getch() without blocking.
 *
 * Key handling:
 *   0x00 / 0xE0  extended prefix  consume second byte, ignore
 *   ESC (0x1B)                    set gs->running = 0
 *   Backspace (0x08)              pop last char from input_buf;
 *                                 clear first_key_set if buffer becomes empty
 *   SPACE / Enter                 call input_try_submit()
 *   Printable ASCII [32..126]     append to input_buf;
 *                                 record first_key_tick on first char
 */
void input_handle(GameState *gs);

/* =============================================================================
 * Word submission
 *
 * Called internally by input_handle() on SPACE or Enter.
 * Exposed in the header so it can be unit-tested independently.
 * =============================================================================
 */

/*
 * Attempt to match gs->input_buf against every active enemy word.
 *
 * On match:
 *   - Compute CPS = input_len / elapsed_seconds (floor elapsed at 0.05 s)
 *   - Store CPS in gs->last_cps
 *   - Set enemy active = 0, increment gs->kills
 *
 * Whether or not a match is found:
 *   - Clear input_buf (input_len = 0, input_buf[0] = '\0')
 *   - Reset first_key_set = 0
 */
void input_try_submit(GameState *gs);

#endif /* INPUT_H */
