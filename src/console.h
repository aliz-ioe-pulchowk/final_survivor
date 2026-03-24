#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"

// Hide the blinking cursor so it does not flicker over game text
void console_hide_cursor(HANDLE h);

// Restore the cursor to visible when the game ends
void console_show_cursor(HANDLE h);

// Set Console size
void console_set_size(HANDLE h);

// Clears back buffer
void buf_clear(ScreenBuf *sb);

// Write to back buffer
void buf_write(ScreenBuf *sb, int col, int row, const char *text);

// Compares back, front buffers -> Prints changed rows -> Updates buffers
void buf_flush(GameState *gs);

// Move cursor to col, row
void console_goto_xy(HANDLE h, int col, int row);

#endif
