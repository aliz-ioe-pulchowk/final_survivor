#include "input.h"
#include <conio.h>

//handles: input from stdout
void input_handle(GameState *gs)
{
    if (_kbhit())
    {
        char ch = _getch();
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) //take only letters.
        {
            gs->input_buf[gs->input_len++] = ch;
        }
        else if (ch == ' ') //spacebar pressed
        {
            input_handle(gs);
        } else if(ch == '\b') //backsapace '\b'
        {
            gs->input_buf[gs->input_len--]= '\0';
        }else if(ch == 27){ //27 = esc pressed
            gs->running = 0;
        }
        if(gs->input_len == 1)
            gs->first_key_tick = GetTickCount();
    }
}


// handles: user input and enemy word matching logic.
void input_try_submit(GameState *gs)
{
    float elapsed = ((GetTickCount() - gs->first_key_tick) / 1000.0);
    float calc_CPS = gs->input_len / elapsed;
    gs->last_cps = calc_CPS < 0.05 ? 0.05 : gs->input_len / elapsed < 0.05;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        Enemy *e = &gs->enemies[i];
        if (e->active == 0)
            continue;
        else if (strcmp(e->word, gs->input_buf) == 0)
        {
            e->active = 0;
            memset(e->word, 0, (size_t)MAX_WORD_LEN);
            gs->kills++;
            break;
        }
    }
    gs->input_len = 0;
    memset(&gs->input_buf, '\0', INPUT_BUF_LEN); // reset input_buf to \0.
    gs->first_key_set = 0;
}