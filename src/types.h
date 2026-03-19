/* =============================================================================
 * types.h
 *
 * Shared constants and struct definitions.
 * Every other header and .c file includes this first.
 * No function declarations belong here.
 * =============================================================================
 */

#ifndef TYPES_H
#define TYPES_H

#include <windows.h>

/* =============================================================================
 * Tuneable constants
 * =============================================================================
 */

/* Word pool */
#define WORDS_FILE         "words.txt"
#define POOL_CAPACITY      200
#define MAX_WORD_LEN       24
#define INPUT_BUF_LEN      (MAX_WORD_LEN + 2)

/* Console layout */
#define CONSOLE_WIDTH      80
#define CONSOLE_HEIGHT     30
#define ROW_LEN            (CONSOLE_WIDTH - 1)  /* avoid auto line-wrap       */
#define WALL_ROW           (CONSOLE_HEIGHT - 2)
#define INPUT_ROW          (CONSOLE_HEIGHT - 1)
#define PLAY_START_ROW     1

/* Timing */
#define TARGET_FPS         30
#define FRAME_MS           (1000 / TARGET_FPS)
#define MAX_DT_MS          200                  /* dt cap against stalls      */

/* Game balance */
#define PLAYER_MAX_HP      100
#define WALL_HIT_DAMAGE    10
#define MAX_ENEMIES        50

/* Difficulty */
#define BASE_SPAWN_MS      3000
#define SPAWN_MIN_MS       500
#define BASE_SPEED         0.8f
#define DIFF_INTERVAL_MS   10000
#define SPEED_PER_LEVEL    0.35f
#define SPAWN_SCALE        0.80f

/* Spawn placement */
#define SPAWN_COL_MIN      1
#define SPAWN_COL_MAX      (CONSOLE_WIDTH - MAX_WORD_LEN - 2)

/* =============================================================================
 * Structs
 * =============================================================================
 */

/*
 * Enemy — one falling word on screen.
 * Physics uses the accumulator pattern: acc grows by speed*dt each frame and
 * commits integer row steps when it reaches 1.0.
 */
typedef struct {
    char  word[MAX_WORD_LEN]; /* word the player must type                   */
    float x;                  /* column — cast to int for rendering          */
    float y;                  /* row    — float for sub-row precision        */
    float acc;                /* fractional-row accumulator                  */
    float speed;              /* fall speed in rows per second               */
    int   active;             /* 1 = alive on screen, 0 = free slot          */
} Enemy;

/*
 * ScreenBuf — double-buffer for flicker-free rendering.
 * back  = frame assembled in RAM this tick (no console I/O yet).
 * front = last frame physically written to the console.
 * Only rows where back != front are written each flush.
 */
typedef struct {
    char back [CONSOLE_HEIGHT][CONSOLE_WIDTH];
    char front[CONSOLE_HEIGHT][CONSOLE_WIDTH];
} ScreenBuf;

/*
 * GameState — single source of truth for the entire game.
 * Passed by pointer to every system function.
 */
typedef struct {
    /* Word pool */
    char  pool[POOL_CAPACITY][MAX_WORD_LEN];
    int   pool_size;

    /* Enemies */
    Enemy enemies[MAX_ENEMIES];

    /* Rendering */
    ScreenBuf screen;
    HANDLE    con_out;

    /* Player */
    int   player_hp;
    int   kills;
    float last_cps;

    /* Timing */
    DWORD start_tick;
    DWORD last_frame_tick;
    DWORD last_spawn_tick;
    DWORD last_diff_tick;

    /* Difficulty */
    int   spawn_interval_ms;
    float base_speed;
    int   diff_level;

    /* Input state */
    char  input_buf[INPUT_BUF_LEN];
    int   input_len;
    DWORD first_key_tick;
    int   first_key_set;

    /* Loop control */
    int   running;
} GameState;

#endif /* TYPES_H */
