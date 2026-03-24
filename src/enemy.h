#ifndef ENEMY_H
#define ENEMY_H

#include "types.h"

// Spawn enemy
void enemy_spawn(GameState *gs);

// Update enemies positions and handle wall collision
void enemy_update(GameState *gs, float dt_sec);

// Update difficulty if enough time passed
void enemy_update_difficulty(GameState *gs, DWORD now);

#endif
