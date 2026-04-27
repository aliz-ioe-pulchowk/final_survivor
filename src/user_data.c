#include "user_data.h"
#include <stdio.h>
#include "types.h"

// Gets user data for a specific username (used for saving)
UserData getUserData(GameState *gs, char *username) {
    UserData user;
    strcpy(user.username, username);
    user.score      = gs->kills;
    user.level      = gs->diff_level + 1;
    user.timePlayed = (gs->last_frame_tick - gs->start_tick) / 1000.f;
    return user;
}

// Saves user data in the file
void saveUserData(UserData *user, char *filepath) {
    FILE *fp = fopen(filepath, "a");  // append mode

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "%s \t%d \t%d \t%.2f\n",
            user->username,
            user->score,
            user->level,
            user->timePlayed);

    fclose(fp);
}

// Displays score board
void displayAllUsers(char *filepath) {
    FILE *fp = fopen(filepath, "r");

    if (fp == NULL) {
        printf("No data found.\nPlay a game first.\n");
        return;
    }

    UserData user;

    printf("\n-- Stored User Records --\n");
    while (fscanf(fp, "%49s %d %d %f",
                  user.username,
                  &user.score,
                  &user.level,
                  &user.timePlayed) != EOF) {
        printf("Username: %s | Score: %d | Level: %d | Time: %.2f\n",
               user.username,
               user.score,
               user.level,
               user.timePlayed);
    }

    fclose(fp);
}

// Empties the scoreboard file
void clearScores(char *filepath) {
    FILE *fp = fopen(filepath, "w");
    if (fp == NULL) {
        printf("An error occurred.\nCould not clear scoreboard.");
    }

    printf("Successfully Cleared.\n");
    fclose(fp);
}