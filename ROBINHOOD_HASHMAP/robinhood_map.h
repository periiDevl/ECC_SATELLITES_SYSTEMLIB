/*
 * robinhood_map.h
 *
 * Static, open-addressed, Robin-Hood-hashed flat hash map.
 * Zero dynamic allocation -- all storage is a fixed-size array baked
 * into the struct below.
 *
 * TO ADAPT TO YOUR OWN KEY/VALUE TYPES:
 *   Edit RH_CAPACITY and rh_value_t below. The key type is fixed at
 *   uint32_t (typical for telemetry/command IDs) -- if you need a
 *   different key type, change rh_key_t and the hash function to match.
 */

#ifndef ROBINHOOD_MAP_H
#define ROBINHOOD_MAP_H

#include <stdint.h>
#include <stdbool.h>

/* ---- Edit these two to fit your table -------------------------------- */

#define RH_CAPACITY 64   /* must be a power of two, and less than 255 */

typedef uint32_t rh_key_t;

typedef struct {
    uint32_t handler_id;
    uint16_t period_ms;
    uint8_t  priority;
    uint8_t  flags;
} rh_value_t;

/* ------------------------------------------------------------------------
 * Nothing below this line needs to change.
 * ------------------------------------------------------------------------
 */

#define RH_EMPTY 0xFFu  /* sentinel meaning "this slot is unused" */

typedef struct {
    rh_key_t   key[RH_CAPACITY];
    rh_value_t val[RH_CAPACITY];
    uint8_t    meta[RH_CAPACITY]; /* RH_EMPTY, or probe distance 0..254 */
    uint32_t   count;
} rh_map_t;

/* Initialize (or reset) a map. Call this once before using it. */
void rh_map_init(rh_map_t *m);

/* Insert a key/value pair, or update the value if the key already
 * exists. Returns false only if the table is already at its safe
 * load-factor ceiling and the key is new. */
bool rh_map_insert(rh_map_t *m, rh_key_t key, rh_value_t val);

/* Look up a key. If found, copies the value into *out and returns true.
 * If not found, returns false and *out is left untouched. */
bool rh_map_lookup(const rh_map_t *m, rh_key_t key, rh_value_t *out);

/* Remove a key. Returns true if the key was present and removed. */
bool rh_map_remove(rh_map_t *m, rh_key_t key);

/* Worst-case probe distance currently in the table. Check this against
 * your real-time budget once the table is filled with its real,
 * final key set -- this is your empirical worst-case lookup cost. */
uint32_t rh_map_max_probe(const rh_map_t *m);

#endif /* ROBINHOOD_MAP_H */
