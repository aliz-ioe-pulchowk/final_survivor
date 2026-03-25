#include "game.h"
#include "conio.h"
#include "console.h"
#include "enemy.h"
#include "input.h"
#include "render.h"
#include "words.h"


#include <time.h>
#include <stdio.h>
#include <conio.h>


int game_init(GameState* gs) {
    srand((unsigned int) time(NULL));

    memset(gs, 0, sizeof(GameState));

    gs->pool_size = words_load(gs, WORDS_FILE);
    if (gs->pool_size < 2) {
        printf("\nCouldn't load words file.");
        return 0;
    } else {
        printf("\nLoaded words: %d", gs->pool_size);
        _getch();
    }

    gs->player_hp         = PLAYER_MAX_HP;
    gs->spawn_interval_ms = BASE_SPAWN_MS;
    gs->base_speed        = BASE_SPEED;
    gs->running           = 1;
    gs->con_out           = GetStdHandle(STD_OUTPUT_HANDLE);

    console_set_size(gs->con_out);  // resize buffer+window to match constants
    console_hide_cursor(gs->con_out);
    system("cls");

    // Seed front buffer so the first diff triggers a full initial paint
    for (int i = 0; i < CONSOLE_HEIGHT; ++i) {
        memset(gs->screen.front[i], ' ', ROW_LEN);
        gs->screen.front[i][ROW_LEN] = '\0';
    }

    DWORD now;
    now                 = GetTickCount();
    gs->start_tick      = now;
    gs->last_frame_tick = now;
    gs->last_spawn_tick = now;
    gs->last_diff_tick  = now;

    return 1;
}

void game_run(GameState* gs) {
    while (gs->running == 1 && gs->player_hp >= 0) {
        DWORD now     = GetTickCount();
        float elapsed = (float) now - gs->last_frame_tick;

        // compensate the frame
        float to_sleep_ms = FRAME_MS - elapsed;
        to_sleep_ms       = to_sleep_ms < 0 ? 0 : (to_sleep_ms > MAX_DT_MS ? MAX_DT_MS : to_sleep_ms);
        Sleep(to_sleep_ms);

        // handle input
        input_handle(gs);

        // update enemies position and handle collision with wall
        enemy_update(gs, to_sleep_ms / 1000);

        // handle spawn of words
        if ((now - gs->last_spawn_tick) >= gs->spawn_interval_ms) {
            enemy_spawn(gs);
            gs->last_spawn_tick = now;
        }

        // update the difficulty level
        enemy_update_difficulty(gs, now);

        // check if player died and handle it
        if (gs->player_hp <= 0) {
            gs->running = 0;
            break;
        }

        // flushing the buffers
        render_frame(gs, now);

        gs->last_frame_tick = now;
    }
}
