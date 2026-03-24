#include "input.h"
#include <conio.h>

// handles: input from stdout
void input_handle(GameState *gs) {
    if (_kbhit()) {
        char ch = _getch();
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            gs->input_buf[gs->input_len++] = ch;
        else if (ch == ' ' || ch == '\r')
            input_try_submit(gs);
        else if (ch == '\b') {  // backsapace '\b'
            if (gs->input_len > 0) {
                gs->input_buf[gs->input_len - 1] = '\0';
                gs->input_len--;
            }
        } else if (ch == 27)  // 27 = esc pressed
            gs->running = 0;

        if (gs->input_len == 1)
            gs->first_key_tick = GetTickCount();
    }
}

// handles: user input and enemy word matching logic.
void input_try_submit(GameState *gs) {
    float elapsed  = ((GetTickCount() - gs->first_key_tick) / 1000.0);
    float calc_CPS = gs->input_len / elapsed;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &gs->enemies[i];
        if (e->active == 0)
            continue;
        else if (strcmp(e->word, gs->input_buf) == 0) {
            e->active = 0;
            memset(e->word, 0, (size_t) MAX_WORD_LEN);
            gs->kills++;
            gs->last_cps = calc_CPS < 0.05 ? 0.05 : calc_CPS;
            break;
        }
    }
    gs->input_len = 0;
    memset(&gs->input_buf, '\0', INPUT_BUF_LEN);  // reset input_buf to \0.
    gs->first_key_set = 0;
}