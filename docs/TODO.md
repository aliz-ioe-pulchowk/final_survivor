# Final Survivor — TODO

Implementation order flows from least dependent to most dependent.
Each phase must be complete and compiling before the next begins.

---

## Phase 1 — Foundation  `types.h`

Nothing else can be written without this. All structs and constants live here.

- [x] Define all `#define` constants (console dims, timing, balance, spawn)
- [x] Define `Enemy` struct — `word`, `x`, `y`, `acc`, `speed`, `active`
- [x] Define `ScreenBuf` struct — `back[H][W]` and `front[H][W]`
- [x] Define `GameState` struct — pool, enemies, screen, timers, input state, running flag
- [x] Add include guards

---

## Phase 2 — Console layer  `console.h` / `console.c`

Depends on: `types.h`

- [x] `console_goto_xy(HANDLE h, int col, int row)`
- [x] `console_hide_cursor(HANDLE h)`
- [x] `console_show_cursor(HANDLE h)`
- [x] `console_set_size(HANDLE h)` — shrink window to 1x1, resize buffer, expand window
- [x] `buf_clear(ScreenBuf *sb)` — fill every back[] row with spaces, null-terminate
- [x] `buf_write(ScreenBuf *sb, int col, int row, const char *text)` — clip at ROW_LEN
- [x] `buf_flush(GameState *gs)` — strcmp each row, write only changed rows, memcpy front

---

## Phase 3 — Word system  `words.h` / `words.c`

Depends on: `types.h`

- [ ] `words_load(GameState *gs, const char *path)` — malloc file into buffer, split on
      commas, strip whitespace, force lowercase, skip words over MAX_WORD_LEN, return count
- [ ] `words_pick(GameState *gs)` — rand() % pool_size, retry if word already active
      on screen, fallback to pool[0] if all words in use

---

## Phase 4 — Enemy system  `enemy.h` / `enemy.c`

Depends on: `types.h`, `words.h`

- [ ] `enemy_spawn(GameState *gs)` — scan for free slot, call words_pick, copy word,
      assign random x in [SPAWN_COL_MIN, SPAWN_COL_MAX], y = PLAY_START_ROW,
      speed = base_speed, acc = 0, active = 1
- [ ] `enemy_update(GameState *gs, float dt_sec)` — for each active enemy:
      acc += speed * dt_sec, commit integer y steps while acc >= 1.0,
      if y >= WALL_ROW: player_hp -= WALL_HIT_DAMAGE, active = 0
- [ ] `enemy_update_difficulty(GameState *gs, DWORD now)` — if elapsed since
      last_diff_tick >= DIFF_INTERVAL_MS: diff_level++, spawn_interval_ms *= SPAWN_SCALE
      (floor at SPAWN_MIN_MS), base_speed += SPEED_PER_LEVEL, update last_diff_tick

---

## Phase 5 — Input system  `input.h` / `input.c`

Depends on: `types.h`, `enemy.h`

- [ ] `input_try_submit(GameState *gs)` — strcmp input_buf against each active enemy word,
      on match: elapsed = (GetTickCount - first_key_tick) / 1000.0 floored at 0.05s,
      last_cps = input_len / elapsed, active = 0, kills++,
      always: input_len = 0, input_buf[0] = '\0', first_key_set = 0
- [ ] `input_handle(GameState *gs)` — while _kbhit(): _getch(ch),
      0x00/0xE0: consume second byte and continue,
      ESC: running = 0,
      Backspace: pop input_buf, if empty clear first_key_set,
      SPACE/Enter: call input_try_submit,
      printable [32..126]: if first char record first_key_tick + set first_key_set,
      append to input_buf

---

## Phase 6 — Render system  `render.h` / `render.c`

Depends on: `types.h`, `console.h`, `enemy.h`

- [ ] `render_frame(GameState *gs, DWORD now)`:
      - [ ] Resize guard — GetConsoleScreenBufferInfo, if size differs call
            console_set_size, system("cls"), memset front[] to 0
      - [ ] buf_clear
      - [ ] buf_write pass 1 — status bar row 0: HP, kills, elapsed time, CPS, level
      - [ ] buf_write pass 2 — each active enemy word at clamped (col, row)
      - [ ] buf_write pass 3 — castle wall of '=' chars at WALL_ROW
      - [ ] buf_write pass 4 — "> input_buf" at INPUT_ROW
      - [ ] buf_flush
- [ ] `render_game_over(GameState *gs, DWORD now)` — console_show_cursor, system("cls"),
      printf kills / elapsed time / diff_level / last_cps, block on _kbhit + _getch

---

## Phase 7 — Game coordinator  `game.h` / `game.c`

Depends on: all previous headers

- [ ] `game_init(GameState *gs)`:
      - [ ] memset gs to 0
      - [ ] words_load — print error and return 0 on failure
      - [ ] srand(time(NULL))
      - [ ] set player_hp, spawn_interval_ms, base_speed, running = 1
      - [ ] con_out = GetStdHandle(STD_OUTPUT_HANDLE)
      - [ ] console_set_size, console_hide_cursor, system("cls")
      - [ ] seed front[] rows with spaces
      - [ ] stamp start_tick, last_frame_tick, last_spawn_tick, last_diff_tick
- [ ] `game_run(GameState *gs)`:
      - [ ] frame limiter — GetTickCount, Sleep if dt < FRAME_MS, clamp dt to MAX_DT_MS
      - [ ] input_handle
      - [ ] enemy_update
      - [ ] spawner check — if elapsed >= spawn_interval_ms: enemy_spawn,
            update last_spawn_tick
      - [ ] enemy_update_difficulty
      - [ ] break if player_hp <= 0
      - [ ] render_frame

---

## Phase 8 — Entry point  `main.c`

Depends on: `game.h`, `render.h`

- [ ] Declare `GameState gs`
- [ ] Call `game_init(&gs)` — return 1 on failure
- [ ] Call `enemy_spawn(&gs)` — place the first word before the loop starts
- [ ] Call `game_run(&gs)`
- [ ] Call `render_game_over(&gs, GetTickCount())`
- [ ] Compile: `gcc main.c game.c render.c input.c enemy.c words.c console.c -o game.exe`