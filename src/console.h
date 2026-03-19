/* =============================================================================
 * console.h
 *
 * Low-level Win32 console management and screen double-buffer operations.
 *
 * Responsibilities:
 *   - Cursor visibility
 *   - Forcing the console window and screen buffer to exact dimensions
 *   - Screen double-buffer: clear, write, flush (diff-only output)
 * =============================================================================
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"

/* =============================================================================
 * Cursor helpers
 * =============================================================================
 */

/* Hide the blinking cursor so it does not flicker over game text. */
void console_hide_cursor(HANDLE h);

/* Restore the cursor to visible when the game ends. */
void console_show_cursor(HANDLE h);

/* =============================================================================
 * Console sizing
 *
 * Forces both the screen buffer and the window rectangle to exactly
 * CONSOLE_WIDTH x CONSOLE_HEIGHT using the required three-step sequence:
 *   1. Shrink window to 1x1 to remove the size constraint.
 *   2. Resize the screen buffer to the target dimensions.
 *   3. Expand the window to fill the buffer.
 * =============================================================================
 */
void console_set_size(HANDLE h);

/* =============================================================================
 * Screen double-buffer
 *
 * All three functions operate on the ScreenBuf embedded in GameState.
 * No console I/O happens until buf_flush().
 * =============================================================================
 */

/*
 * Fill every row of back[] with spaces and null-terminate each row.
 * Called once at the start of every render pass.
 */
void buf_clear(ScreenBuf *sb);

/*
 * Write a C string into back[] at (col, row).
 * Text that would overflow past ROW_LEN is silently clipped.
 */
void buf_write(ScreenBuf *sb, int col, int row, const char *text);

/*
 * Diff back[] against front[], write only changed rows to the physical
 * console via goto_xy + fputs, then copy back[] -> front[].
 * Rows whose content is identical between frames produce zero console I/O.
 */
void buf_flush(GameState *gs);

/* =============================================================================
 * Internal helper (used by buf_flush and render)
 * =============================================================================
 */

/* Move the console cursor to (col, row), both 0-indexed. */
void console_goto_xy(HANDLE h, int col, int row);

#endif /* CONSOLE_H */
