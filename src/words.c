#include "words.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PROBABILITY 0.40

// decide to accept the word or not
int accepted() {
    double random = (double) rand();
    if (random / RAND_MAX <= PROBABILITY) return 1;
    return 0;
}

int words_load(GameState *gs, const char *path) {
    gs->pool_size = POOL_CAPACITY;
    int   count   = 0;
    FILE *f;

    f = fopen(path, "r");
    if (!f)
        return 0;

    // case:  safety net to ensure first character is not a space
    int first_ch = fgetc(f);
    if (first_ch != EOF && first_ch != ' ')
        ungetc(first_ch, f);

    char ch;
    int  i = 0;
    char buffer[MAX_WORD_LEN];

    // read character by character
    while ((ch = fgetc(f)) != EOF) {
        if (ch == ',') {
            buffer[i] = '\0';
            i         = 0;

            // choose based on acceptance by random probability
            if (accepted()) {
                strcpy(gs->pool[count], buffer);
                count++;
            }

            if (count >= gs->pool_size) {
                fclose(f);
                return count;
            }

            int next = fgetc(f);
            if (next != ' ' && next != EOF)
                ungetc(next, f);
            continue;
        }

        if (i < MAX_WORD_LEN - 1)
            buffer[i++] = ch;
    }

    // If we hit EOF but have characters in the buffer, it's the last word
    if (i > 0) {
        buffer[i] = '\0';

        // Only add if it passes the probability gate, just like the others
        if (accepted() && count < POOL_CAPACITY) {
            strcpy(gs->pool[count], buffer);
            count++;
        }
    }

    fclose(f);
    gs->pool_size = count;

    return count;
}

const char *words_pick(GameState *gs) {
    if (gs->pool_size <= 0)
        return NULL;

    int taken = 0;
    for (int attempts = 0; attempts < 100; attempts++) {
        int choice = (rand() % gs->pool_size);

        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (gs->enemies[i].active &&
                strcmp(gs->pool[choice], gs->enemies[i].word) == 0) {
                taken = 1;
                break;
            }
        }

        if (!taken)
            return gs->pool[choice];
        else
            return words_pick(gs);
    }
    return NULL;
}