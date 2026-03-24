// contains : word loading and picking functions

#ifndef WORDS_H
#define WORDS_H

#include "types.h"

// loads words randomly from the specified file into gamestate pool.
int words_load(GameState *gs, const char *path);

// picks words randomly from the loaded words to spawn
const char *words_pick(GameState *gs);

#endif
