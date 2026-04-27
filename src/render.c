#include <stdio.h>
#include <conio.h>
#include "render.h"
#include "console.h"

int main_menu()
{
    int choice;
    int is_invalid = 1;
    while (is_invalid)
    {
        printf(
            "\n<><><><><><><><><><><><><><><><><><><><><>"
            "\n         -- FINALS SURVIVOR --         "
            "\n<><><><><><><><><><><><><><><><><><><><><>"
            "\n\n  (1) New Game "
            "\n  (2) View Scoreboard"
            "\n  (3) Clear ScoreBoard"
            "\n  (0) Exit "
            "\n\n<><><><><><><><><><><><><><><><><><><><>"
            "\nYour response: ");
        scanf(" %d", &choice);
        
        is_invalid = choice > 3 || choice < 0;
        if (is_invalid)
        {
            system("cls");
            printf("Invalid choice: [0 - 4] available.\n");
            continue;
        }
    }
    return choice;
}

void start_menu() {
    system("cls");
    printf(
        "\n<><><><><><><><><><><><><><><><><><><><><>"
        "\n         -- GAME  STARTING --         "
        "\n<><><><><><><><><><><><><><><><><><><><><>"
        "\n\nYou look up at the sky in wonder,"
        "\nonly to see words falling towards you,"
        "\nthe sole warrior defending the castle."
        "\n\nSurvive the chaos as the final Survivor."
        "\n\n<><><><><><><><><><><><><><><><><><><><>"
        "\n\nPress 'Enter' to continue...");

    _getch();
};

// helper function to dynamically generate custom sized wall.
char wall[ROW_LEN];
void generate_wall(int no_of_small_arrows) {
    int a        = no_of_small_arrows;
    int how_many = ROW_LEN / (2 * a + 2 * 1);
    int gap      = 2 * a + 2;
    int fill     = ROW_LEN % (2 * a + 2 * 1);
    int count    = 0;

    const char x = '-';
    for (int i = 0; i < fill / 2; i++)
        wall[count++] = x;

    for (int i = 0; i < how_many; i++) {
        for (int j = 0; j < a; j++)
            wall[count++] = x;

        wall[count++] = '|';
        wall[count++] = '|';

        for (int j = 0; j < a; j++)
            wall[count++] = x;
    }
    for (int i = 0; i < fill / 2; i++)
        wall[count++] = x;
}

// renders the actual content displayed on the windows
void render_frame(GameState *gs, DWORD now) {
    char  temp[CONSOLE_WIDTH];
    float elapsed_time = (float) (gs->last_frame_tick - gs->start_tick) / 1000;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(gs->con_out, &csbi)) {
        console_set_size(gs->con_out);
        memset(gs->screen.front, 0, sizeof(gs->screen.front));
    }

    buf_clear(&gs->screen);

    snprintf(temp, sizeof(temp),
             "HP: %d | Kills: %d | Surived: %.2fs | CPS: %.2f | Level: %d",
             gs->player_hp, gs->kills, elapsed_time, gs->last_cps, gs->diff_level + 1);
    buf_write(&gs->screen, 0, 0, temp);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &gs->enemies[i];
        buf_write(&gs->screen, e->x, e->y, e->word);
    }

    buf_write(&gs->screen, 0, WALL_ROW, wall);
    buf_write(&gs->screen, 0, INPUT_ROW, "=> ");
    buf_write(&gs->screen, 3, INPUT_ROW, gs->input_buf);
    buf_flush(gs);
}

// The game over console screen
void render_game_over(GameState *gs, DWORD now) {
    float elapsed = (float) (gs->last_frame_tick - gs->start_tick) / 1000;
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

        "\n\n<><><><><><><><><><><><><><><><><><><>\n",

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
    _getch();
}
