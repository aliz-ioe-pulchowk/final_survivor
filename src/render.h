#ifndef RENDER_H
#define RENDER_H

#include "types.h"

//renders the main menu of choices
int main_menu();

// game starting menu
void start_menu();

// helper to initialize wall
void generate_wall(int no_of_small_arrows);

// the display renderer in window
void render_frame(GameState *gs, DWORD now);

// Display the end-of-game summary and wait for a keypress
void render_game_over(GameState *gs, DWORD now);

#endif
