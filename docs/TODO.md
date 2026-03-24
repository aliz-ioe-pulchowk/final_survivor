# Final Survivor — TODO

Implementation order flows from least dependent to most dependent.
Each phase must be complete and compiling before the next begins.

---

## Phase 1 — Foundation `types.h`

Nothing else can be written without this. All structs and constants live here.

- [x] Define all `#define` constants (console dims, timing, balance, spawn)
- [x] Define `Enemy` struct — `word`, `x`, `y`, `acc`, `speed`, `active`
- [x] Define `ScreenBuf` struct — `back[H][W]` and `front[H][W]`
- [x] Define `GameState` struct — pool, enemies, screen, timers, input state, running flag
- [x] Add include guards

---

## Phase 2 — Console layer `console.h` / `console.c`

Depends on: `types.h`

- [x] `console_goto_xy(HANDLE h, int col, int row)`
- [x] `console_hide_cursor(HANDLE h)`
- [x] `console_show_cursor(HANDLE h)`
- [x] `console_set_size(HANDLE h)` — shrink window to 1x1, resize buffer, expand window
- [x] `buf_clear(ScreenBuf *sb)` — fill every back[] row with spaces, null-terminate
- [x] `buf_write(ScreenBuf *sb, int col, int row, const char *text)` — clip at ROW_LEN
- [x] `buf_flush(GameState *gs)` — strcmp each row, write only changed rows, memcpy front

---

## Phase 3 — Word system `words.h` / `words.c`

Depends on: `types.h`

- [x] `words_load(GameState *gs, const char *path)` — malloc file into buffer, split on
      commas, strip whitespace, force lowercase, skip words over MAX_WORD_LEN, return count
- [x] `words_pick(GameState *gs)` — rand() % pool_size, retry if word already active
      on screen, fallback to pool[0] if all words in use

---

## Phase 4 — Enemy system `enemy.h` / `enemy.c`

Depends on: `types.h`, `words.h`

- [x] `enemy_spawn(GameState *gs)` — scan for free slot, call words_pick, copy word,
      assign random x in [SPAWN_COL_MIN, SPAWN_COL_MAX], y = PLAY_START_ROW,
      speed = base_speed, acc = 0, active = 1
- [x] `enemy_update(GameState *gs, float dt_sec)` — for each active enemy:
      acc += speed \* dt_sec, commit integer y steps while acc >= 1.0,
      if y >= WALL_ROW: player_hp -= WALL_HIT_DAMAGE, active = 0
- [x] `enemy_update_difficulty(GameState *gs, DWORD now)` — if elapsed since
      last_diff_tick >= DIFF_INTERVAL_MS: diff_level++, spawn_interval_ms \*= SPAWN_SCALE
      (floor at SPAWN_MIN_MS), base_speed += SPEED_PER_LEVEL, update last_diff_tick

---

## Phase 5 — Input system `input.h` / `input.c`

Depends on: `types.h`, `enemy.h`

- [x] `input_try_submit(GameState *gs)` — strcmp input_buf against each active enemy word,
      on match: elapsed = (GetTickCount - first_key_tick) / 1000.0 floored at 0.05s,
      last_cps = input_len / elapsed, active = 0, kills++,
      always: input_len = 0, input_buf[0] = '\0', first_key_set = 0
- [x] `input_handle(GameState *gs)` — while \_kbhit(): \_getch(ch),
      0x00/0xE0: consume second byte and continue,
      ESC: running = 0,
      Backspace: pop input_buf, if empty clear first_key_set,
      SPACE/Enter: call input_try_submit,
      printable [32..126]: if first char record first_key_tick + set first_key_set,
      append to input_buf

---

## Phase 6 — Render system `render.h` / `render.c`

Depends on: `types.h`, `console.h`, `enemy.h`

- [x] `render_frame(GameState *gs, DWORD now)`: 
      - [x] Resize guard — GetConsoleScreenBufferInfo, if size differs call
            console_set_size, system("cls"), memset front[] to 0 
      - [x] buf_clear - [ ] buf_write pass 1 — status bar row 0: HP, kills, elapsed time, CPS, level
      - [x] buf_write pass 2 — each active enemy word at clamped (col, row) 
      - [x] buf_write pass 3 — castle wall of '=' chars at WALL_ROW
      - [x] buf_write pass 4 — "> input_buf" at INPUT_ROW - [ ] buf_flush
- [x] `render_game_over(GameState *gs, DWORD now)` 
      - console_show_cursor, system("cls")
      - printf kills / elapsed time / diff_level / last_cps
      - block on \_kbhit + \_getch

---

## Phase 7 — Game coordinator `game.h` / `game.c`

Depends on: all previous headers

- [x] `game_init(GameState *gs)`: 
- [x] memset gs to 0 
- [x] words_load — print error and return 0 on failure 
- [x] srand(time(NULL)) 
- [x] set player_hp, spawn_interval_ms, base_speed, running = 1 
- [x] con_out = GetStdHandle(STD_OUTPUT_HANDLE) 
- [x] console_set_size, console_hide_cursor, system("cls") 
- [x] seed front[] rows with spaces 
- [x] stamp start_tick, last_frame_tick, last_spawn_tick, last_diff_tick
- [x] `game_run(GameState *gs)`: 
- [x] frame limiter — GetTickCount, Sleep if dt < FRAME_MS, clamp dt to MAX_DT_MS 
- [x] input_handle 
- [x] enemy_update 
- [x] spawner check — if elapsed >= spawn_interval_ms: enemy_spawn, update last_spawn_tick 
- [x] enemy_update_difficulty - [x] break if player_hp <= 0 
- [x] render_frame

---

## Phase 8 — Entry point `main.c`

Depends on: `game.h`, `render.h`

- [x] Declare `GameState gs`
- [x] Call `game_init(&gs)` — return 1 on failure
- [x] Call `enemy_spawn(&gs)` — place the first word before the loop starts
- [x] Call `game_run(&gs)`
- [x] Call `render_game_over(&gs, GetTickCount())`
- [x] Compile: `gcc main.c game.c render.c input.c enemy.c words.c console.c -o game.exe`
