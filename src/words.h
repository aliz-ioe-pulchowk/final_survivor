/* =============================================================================
 * words.h
 *
 * Word pool management: loading from file and selecting words for spawning.
 *
 * Responsibilities:
 *   - Parse a comma-separated words.txt into gs->pool[]
 *   - Provide a word picker that avoids duplicates already on screen
 * =============================================================================
 */

#ifndef WORDS_H
#define WORDS_H

#include "types.h"

/* =============================================================================
 * File loading
 * =============================================================================
 */

/*
 * Read the comma-separated word file at `path` into gs->pool[].
 *
 * Rules applied during parsing:
 *   - All whitespace (space, tab, \r, \n) around tokens is stripped.
 *   - Words are forced to lowercase for consistent match comparisons.
 *   - Words longer than MAX_WORD_LEN-1 chars are silently skipped.
 *   - Loading stops once POOL_CAPACITY words have been collected.
 *
 * Returns the number of words loaded, or 0 on failure.
 * Sets gs->pool_size to the returned count.
 */
int words_load(GameState *gs, const char *path);

/* =============================================================================
 * Word selection
 * =============================================================================
 */

/*
 * Pick a random word from gs->pool[] that is not currently active on screen.
 *
 * Strategy: rand() % pool_size, retry up to pool_size times if the chosen
 * word already appears in enemies[].  With a large pool this almost always
 * succeeds on the first draw.
 *
 * Falls back to an arbitrary pool entry if every word is currently on screen
 * (only possible when pool_size <= MAX_ENEMIES).
 *
 * Returns a pointer to the chosen string inside gs->pool[].
 */
const char *words_pick(GameState *gs);

#endif /* WORDS_H */
