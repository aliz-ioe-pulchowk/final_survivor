#include "console.h"
#include <stdio.h>
#include <string.h>

void console_hide_cursor(HANDLE h) {
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(h, &ci);
}

void console_show_cursor(HANDLE h) {
    CONSOLE_CURSOR_INFO ci = {10, TRUE};
    SetConsoleCursorInfo(h, &ci);
}

void console_set_size(HANDLE h) {
    // set window viewport to smallest size
    SMALL_RECT rect = {0, 0, 0, 0};        // left, top, right, bottom
    SetConsoleWindowInfo(h, TRUE, &rect);  // TRUE implies absolute coords

    // set buffer to required size
    COORD size = {CONSOLE_WIDTH, CONSOLE_HEIGHT};
    SetConsoleScreenBufferSize(h, size);

    // set window to buffer size now
    SMALL_RECT rect_full = {0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1};
    SetConsoleWindowInfo(h, TRUE, &rect_full);
}

void buf_clear(ScreenBuf* sb) {
    for (int i = 0; i < CONSOLE_HEIGHT; ++i) {
        memset(sb->back[i], ' ', ROW_LEN);
        sb->back[i][ROW_LEN] = '\0';
    }
}

void buf_write(ScreenBuf* sb, int col, int row, const char* text) {
    if (row < 0 || row >= CONSOLE_HEIGHT || col < 0 || col >= ROW_LEN) return;
    for (int i = 0; col + i < ROW_LEN && text[i] != '\0'; i++)
        sb->back[row][col + i] = text[i];
}

void buf_flush(GameState* gs) {
    for (int i = 0; i < CONSOLE_HEIGHT; ++i) {
        if (strcmp(gs->screen.back[i], gs->screen.front[i]) == 0) continue;
        goto_xy(gs->con_out, 0, i);
        fputs(gs->screen.back[i], stdout);
    }
    memcpy(gs->screen.front, gs->screen.back, sizeof(gs->screen.back));
    fflush(stdout);
}

void console_goto_xy(HANDLE h, int col, int row) {
    COORD pos = {col, row};
    SetConsoleCursorPosition(h, pos);
}
