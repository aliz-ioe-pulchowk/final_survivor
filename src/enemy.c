#include "enemy.h"
#include "words.h"

void enemy_spawn(GameState* gs) {
    const char* picked = words_pick(gs);  // picks a random but absent word

    Enemy e = {
        .word   = picked,
        .x      = rand() / RAND_MAX * (SPAWN_COL_MAX - SPAWN_COL_MIN) + SPAWN_COL_MIN,
        .y      = PLAY_START_ROW,
        .speed  = gs->base_speed,
        .acc    = 0,
        .active = 1};

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (gs->enemies[i].active != 0)
            continue;
        gs->enemies[i] = e;
        break;
    }
}

void enemy_update(GameState* gs, float dt_sec) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy* e = &gs->enemies[i];
        if (e->active == 0) continue;

        e->acc += e->speed * dt_sec;

        while (e->acc >= 1.0) {
            e->y++;    // move 1 line down
            e->acc--;  // remove capacity to move 1 line down
        }

        // collided with wall
        if ((int) e->y >= WALL_ROW) {
            gs->player_hp -= WALL_HIT_DAMAGE;
            e->active = 0;  // dead
        }
    }
}

void enemy_update_difficulty(GameState* gs, DWORD now) {
    float elapsed = now - (float) gs->last_diff_tick;  // time elapsed since last difficulty change
    if (elapsed < DIFF_INTERVAL_MS)
        return;

    // update difficulty
    gs->diff_level++;
    gs->spawn_interval_ms *= SPAWN_SCALE;
    gs->base_speed += SPEED_PER_LEVEL;
    gs->last_diff_tick = now;

    // clamp the interval
    if (gs->spawn_interval_ms < SPAWN_MIN_MS)
        gs->spawn_interval_ms = SPAWN_MIN_MS;
    else
        gs->spawn_interval_ms;
}
