#include "robinhood_map.h"
#include <string.h>

/* 32-bit integer hash (Murmur3 finalizer). Good avalanche behaviour,
 * appropriate for telemetry/command IDs. */
static uint32_t rh_hash(uint32_t key)
{
    key ^= key >> 16;
    key *= 0x7feb352dU;
    key ^= key >> 15;
    key *= 0x846ca68bU;
    key ^= key >> 16;
    return key;
}

void rh_map_init(rh_map_t *m)
{
    memset(m->meta, RH_EMPTY, sizeof(m->meta));
    m->count = 0;
}

bool rh_map_insert(rh_map_t *m, rh_key_t key, rh_value_t val)
{
    uint32_t mask = RH_CAPACITY - 1u;

    /* First pass: if the key already exists, just update its value.
     * This never grows the table, so it's always allowed. */
    uint32_t idx = rh_hash(key) & mask;
    uint8_t dist = 0;
    while (m->meta[idx] != RH_EMPTY && m->meta[idx] >= dist) {
        if (m->key[idx] == key) {
            m->val[idx] = val;
            return true;
        }
        idx = (idx + 1u) & mask;
        dist++;
    }

    /* Key is new. Refuse to insert past a safe load factor. */
    if (m->count >= RH_CAPACITY - 1u) {
        return false;
    }

    /* Second pass: actually insert, using Robin Hood displacement. */
    idx = rh_hash(key) & mask;
    dist = 0;
    rh_key_t   cur_key = key;
    rh_value_t cur_val = val;

    for (;;) {
        if (m->meta[idx] == RH_EMPTY) {
            m->key[idx]  = cur_key;
            m->val[idx]  = cur_val;
            m->meta[idx] = dist;
            m->count++;
            return true;
        }

        if (m->meta[idx] < dist) {
            /* Robin Hood steal: the entry sitting here is closer to its
             * own home slot than we are to ours, so it "keeps" the slot
             * and we take its place, carrying the displaced entry
             * onward to find a new home. This is what keeps probe
             * distances small and bounded instead of clustering. */
            rh_key_t   tmp_key  = m->key[idx];
            rh_value_t tmp_val  = m->val[idx];
            uint8_t    tmp_dist = m->meta[idx];

            m->key[idx]  = cur_key;
            m->val[idx]  = cur_val;
            m->meta[idx] = dist;

            cur_key = tmp_key;
            cur_val = tmp_val;
            dist    = tmp_dist;
        }

        idx = (idx + 1u) & mask;
        dist++;
    }
}

bool rh_map_lookup(const rh_map_t *m, rh_key_t key, rh_value_t *out)
{
    uint32_t mask = RH_CAPACITY - 1u;
    uint32_t idx = rh_hash(key) & mask;
    uint8_t dist = 0;

    for (;;) {
        if (m->meta[idx] == RH_EMPTY || m->meta[idx] < dist) {
            /* Robin Hood invariant: probe distances along a chain never
             * decrease, so once we see one smaller than ours, our key
             * cannot be further along -- safe to stop early. */
            return false;
        }
        if (m->key[idx] == key) {
            *out = m->val[idx];
            return true;
        }
        idx = (idx + 1u) & mask;
        dist++;
    }
}

bool rh_map_remove(rh_map_t *m, rh_key_t key)
{
    uint32_t mask = RH_CAPACITY - 1u;
    uint32_t idx = rh_hash(key) & mask;
    uint8_t dist = 0;

    /* Find the slot holding this key. */
    for (;;) {
        if (m->meta[idx] == RH_EMPTY || m->meta[idx] < dist) {
            return false; /* not present */
        }
        if (m->key[idx] == key) {
            break;
        }
        idx = (idx + 1u) & mask;
        dist++;
    }

    /* Backward-shift deletion: pull the following probe chain back one
     * slot instead of leaving a tombstone, so repeated delete/insert
     * cycles never degrade lookup speed. */
    uint32_t next = (idx + 1u) & mask;
    while (m->meta[next] != RH_EMPTY && m->meta[next] > 0) {
        m->key[idx]  = m->key[next];
        m->val[idx]  = m->val[next];
        m->meta[idx] = (uint8_t)(m->meta[next] - 1u);
        idx  = next;
        next = (next + 1u) & mask;
    }

    m->meta[idx] = RH_EMPTY;
    m->count--;
    return true;
}

uint32_t rh_map_max_probe(const rh_map_t *m)
{
    uint32_t worst = 0;
    for (uint32_t i = 0; i < RH_CAPACITY; i++) {
        if (m->meta[i] != RH_EMPTY && (uint32_t)m->meta[i] > worst) {
            worst = m->meta[i];
        }
    }
    return worst;
}
