#include "console.h"
#include "enemy.h"
#include "game.h"
#include "input.h"
#include "render.h"
#include "user_data.h"
#include "words.h"

#include <conio.h>
#include <stdio.h>

// safety net for -Wimplicit -function-declaration errors in some compilers

#define CLEAR_SCREEN system("cls");

// start's game
void startGame();

// routes user choice to the proper function
void router(int);

int main() {
    int choice = main_menu();
    router(choice);
    return 0;
}

void router(int choice) {
    switch (choice) {
        case 0:
            CLEAR_SCREEN
            printf("\n-- Terminating Program --\n");
            exit(0);
            break;
        case 1:
            CLEAR_SCREEN
            startGame();
            exit(0);
            break;
        case 2:
            CLEAR_SCREEN
            displayAllUsers(SCOREBOARD_FILE);
            break;
        case 3:
            CLEAR_SCREEN
            clearScores(SCOREBOARD_FILE);
            break;
        default:
            printf("...\n");
            break;
    }
    printf("Press Enter to continue...\n");
    getchar();
    getchar();

    CLEAR_SCREEN

    // return back to router
    router(main_menu());
}

void startGame() {
    char username[50];
    getchar();

    do {
        printf("Enter your Username: ");
        gets(username);

        if (strlen(username) == 0) {
            printf("\nEnter at least one character");
            getchar();
        }

        CLEAR_SCREEN
    } while (strlen(username) == 0);

    generate_wall(3);
    start_menu();
    GameState game;

    game_init(&game);
    enemy_spawn(&game);
    game_run(&game);

    render_game_over(&game, GetTickCount());
    UserData user = getUserData(&game, username);
    saveUserData(&user, SCOREBOARD_FILE);
    return;
}