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

// initialize the starting variables and constants
int game_init(GameState *gs);

// runs the game loop here
void game_run(GameState *gs);

#endif
