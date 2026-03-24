

#ifndef RENDER_H
#define RENDER_H

#include "types.h"

// the display renderer in window
void render_frame(GameState *gs, DWORD now);

// Display the end-of-game summary and wait for a keypress
void render_game_over(GameState *gs, DWORD now);

#endif /* RENDER_H */
