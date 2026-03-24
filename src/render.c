#include "console.h"
#include "render.h"
#include <stdio.h>

// helper function to dynamically generate custom sized wall.
static char *generate_wall(int no_of_small_arrows)
{
    static char wall[ROW_LEN];
    if (wall[ROW_LEN - 1] == '^')
        return wall;

    int a = no_of_small_arrows;
    int how_many = ROW_LEN / (2 * a + 2 * 1);
    int gap = 2 * a + 2;
    int fill = ROW_LEN % (2 * a + 2 * 1);
    int count = 0;

    for (int i = 0; i < fill / 2; i++)
        wall[count++] = '^';

    for (int i = 0; i < how_many; i++)
    {
        for (int j = 0; j < a; j++)
            wall[count++] = '^';

        wall[count++] = '|';
        wall[count++] = '|';

        for (int j = 0; j < a; j++)
            wall[count++] = '^';
    }
    for (int i = 0; i < fill / 2; i++)
        wall[count++] = '^';
}

// renders the actual content displayed on the window
void render_frame(GameState *gs, DWORD now)
{
    char temp[CONSOLE_WIDTH];
    float elapsed_time = (float)(now - gs->last_frame_tick) / 1000;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(gs->con_out, &csbi))
    {
        console_set_size(gs->con_out);
        system("cls");
        memset(gs->screen.front, 0, sizeof(gs->screen.front));
    }

    buf_clear(&gs->screen);

    snprintf(temp, sizeof(temp),
             "HP: %d | Kills: %d | Surived: %.2fs | CPS: %f | Level: %d",
             gs->player_hp, gs->kills, elapsed_time, gs->last_cps, gs->diff_level + 1);
    buf_write(&gs->screen, 0, 0, temp);

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        Enemy *e = &gs->enemies[i];
        buf_write(&gs->screen, e->x, e->y, e->word);
    }

    buf_write(&gs->screen, WALL_ROW, 0, generate_wall(3));
    buf_write(&gs->screen, INPUT_ROW, 0, ":> ");
    buf_flush(&gs);
}

// The game over console screen
void render_game_over(GameState *gs, DWORD now)
{
    float elapsed = (float)(now - gs->last_frame_tick) / 1000;
    console_show_cursor(gs->con_out);
    system("cls");
    printf(
        "\n<><><><><><><><><><><><><><><><><><><>"
        "\n           -- GAME OVER --            "
        "\n<><><><><><><><><><><><><><><><><><><>"

        "\n\n%-20s : %-4d"
        "\n%-20s : %-6.2f"
        "\n%-20s : %-4d"
        "\n%-20s : %-6.2f"

        "\n\n<><><><><><><><><><><><><><><><><><><>"
        "\n\nPress 'Enter' to continue...",

        // %s, %d orderwise placements.
        "Total Kills", gs->kills,
        "Survival Time", elapsed,
        "Level Reached", gs->diff_level + 1,
        "Last CPS", gs->last_cps);

    // flush all remaining buffer.
    fflush(stdout);
    while (!_kbhit())
        Sleep(100);

    // wait for key press to exit
    _getchar();
}