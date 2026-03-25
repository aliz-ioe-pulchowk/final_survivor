#include "console.h"
#include "enemy.h"
#include "game.h"
#include "input.h"
#include "render.h"
#include "words.h"


#include <stdio.h>
#include <conio.h>
//safety net for -Wimplicit -function-declaration errors in some compilers


void main_menu() {
    system("cls");
    printf(
        "\n<><><><><><><><><><><><><><><><><><><>"
        "\n         -- GAME  STARTING --         "
        "\n<><><><><><><><><><><><><><><><><><><>"
        "\nIt had been raining for days"
        "\nThe princess over the wall is wet"
        "\nShe cannot shoot the monsters coming"
        "\nInside her   castle. To save her"
        "\nYou force yourself inside"
        "\nThe blood keeps on dripping..drip..drip"
        "\nYou push the blockade forcefully"
        "\nThen you drown in water that came from"
        "\nPrincess.. (no more words are available)"
        "\n\n<><><><><><><><><><><><><><><><><><><>"
        "\n\nPress 'Enter' to continue...");

    _getch();
};

int main() {
    generate_wall(3);
    main_menu();

    GameState game;

    game_init(&game);
    enemy_spawn(&game);
    game_run(&game);

    render_game_over(&game, GetTickCount());
    return 0;
}