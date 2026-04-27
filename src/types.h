#ifndef TYPES_H
#define TYPES_H

#include <windows.h>

// CONSTANTS HERE

// Word pool
#define WORDS_FILE "words.txt"
#define POOL_CAPACITY 500
#define MAX_WORD_LEN 24
#define INPUT_BUF_LEN (MAX_WORD_LEN + 2)

// Console layout
#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 30
#define ROW_LEN (CONSOLE_WIDTH - 1)     // one less to avoid auto line-wrap
#define WALL_ROW (CONSOLE_HEIGHT - 3)   // castle wall position
#define INPUT_ROW (CONSOLE_HEIGHT - 1)  // input buffer position
#define PLAY_START_ROW 1                // words spawn position

// Timing
#define TARGET_FPS 30
#define FRAME_MS (1000 / TARGET_FPS)  // 1 frame time in ms
#define MAX_DT_MS 200                 // limit to frame delay

// Game balance
#define PLAYER_MAX_HP 100
#define WALL_HIT_DAMAGE 10  // dmg to wall per word crashing
#define MAX_ENEMIES 50      // at same time on screen

// Difficulty
#define BASE_SPAWN_MS 3000      // base gap between each word spawn
#define SPAWN_MIN_MS 500        // mininum gap between word spawn
#define BASE_SPEED 0.8f         // base speed of words (line per ticks)
#define DIFF_INTERVAL_MS 10000  // time between each increase in difficulty level
#define SPEED_PER_LEVEL 0.35f   // additive increase in enemy's speed per level
#define SPAWN_SCALE 0.85f       // multiplicative decrease in spawn gap (applies per tick, keep near 1)

// Spawn placement
#define SPAWN_COL_MIN 1
#define SPAWN_COL_MAX (CONSOLE_WIDTH - MAX_WORD_LEN - 2)

// Enemy is basically a word that we need to type to destroy
typedef struct {
    char  word[MAX_WORD_LEN];
    float x;
    float y;
    float acc;     // internal position (fractional part)
    float speed;   // characters per tick
    int   active;  // alive(1) or dead(0)
} Enemy;

// double-buffer rendering system to remove flickering
typedef struct {
    char back[CONSOLE_HEIGHT][CONSOLE_WIDTH];
    char front[CONSOLE_HEIGHT][CONSOLE_WIDTH];
} ScreenBuf;

// Contains all important info about game we may need
// pass this as pointer to functions if game's current state is required
typedef struct {
    // Word pool
    char pool[POOL_CAPACITY][MAX_WORD_LEN];
    int  pool_size;

    // Enemies
    Enemy enemies[MAX_ENEMIES];

    // Rendering
    ScreenBuf screen;   // contains front and back buffers
    HANDLE    con_out;  // Handle -> basically a pointer (ID) to something owned by OS(windows)

    // Player
    int   player_hp;
    int   kills;
    float last_cps;  // characters per second

    // Timing
    DWORD start_tick;
    DWORD last_frame_tick;
    DWORD last_spawn_tick;
    DWORD last_diff_tick;
    // DWORD -> 32 bit unsigned integer (Windows forces it to be 32 bit)
    // Window used this for most of its function so we expect this to be returned

    // Difficulty
    int   spawn_interval_ms;
    float base_speed;
    int   diff_level;

    // Input state
    char  input_buf[INPUT_BUF_LEN];  // typed buffer (can only type upto certain characters)
    int   input_len;
    DWORD first_key_tick;  // first key press time/tick
    int   first_key_set;   // if first key was pressed (input not empty) => 1 (true) else 0 (false)

    // Loop control
    int running;
} GameState;

#endif
