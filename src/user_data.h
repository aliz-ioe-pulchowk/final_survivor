#include <stdio.h>
#include "types.h"

#define SCOREBOARD_FILE "Scoreboard.txt"
typedef struct
{
    char  username[50];
    int   score;
    int   level;
    float timePlayed;
} UserData;

UserData getUserData(GameState *gs, char *username);

void saveUserData(UserData *user, char *filepath);

void displayAllUsers(char *filepath);

void clearScores(char *filepath);