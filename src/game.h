#ifndef GAME_H
#define GAME_H

#include "types.h"

// initialize the starting variables and constants
int game_init(GameState *gs);

// runs the game loop here
void game_run(GameState *gs);

#endif
