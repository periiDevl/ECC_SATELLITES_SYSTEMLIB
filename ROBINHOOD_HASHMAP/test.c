/* test_main.c
 *
 * Simple pure-C test for the Robin Hood map.
 *
 * Build (everything is C, nothing is C++):
 *   gcc -std=c11 robinhood_map.c test_main.c -o proh
 *   ./proh
 */

#include <stdio.h>
#include "robinhood_map.h"

int main(void)
{
    rh_map_t table;
    rh_map_init(&table);

    rh_value_t v1 = { .handler_id = 10, .period_ms = 1000, .priority = 0, .flags = 0x03 };
    rh_value_t v2 = { .handler_id = 20, .period_ms = 500,  .priority = 1, .flags = 0x01 };
    rh_map_insert(&table, 0x1001, v1);
    rh_map_insert(&table, 0x2001, v2);

    rh_value_t out;
    if (rh_map_lookup(&table, 0x1001, &out)) {
        printf("0x1001 -> handler_id=%u  (expected 10)\n", out.handler_id);
    } else {
        printf("0x1001 NOT FOUND (expected to be found)\n");
    }

    if (rh_map_lookup(&table, 0x2001, &out)) {
        printf("0x2001 -> handler_id=%u  (expected 20)\n", out.handler_id);
    } else {
        printf("0x2001 NOT FOUND (expected to be found)\n");
    }

    if (rh_map_lookup(&table, 0x9999, &out)) {
        printf("0x9999 -> handler_id=%u  (NOT expected, this is a bug)\n", out.handler_id);
    } else {
        printf("0x9999 not found  (expected, key was never inserted)\n");
    }

    rh_value_t v1_new = { .handler_id = 99, .period_ms = 1000, .priority = 0, .flags = 0x03 };
    rh_map_insert(&table, 0x1001, v1_new);
    rh_map_lookup(&table, 0x1001, &out);
    printf("0x1001 after update -> handler_id=%u  (expected 99)\n", out.handler_id);

    rh_map_remove(&table, 0x1001);
    if (rh_map_lookup(&table, 0x1001, &out)) {
        printf("0x1001 -> handler_id=%u  (NOT expected, should be removed)\n", out.handler_id);
    } else {
        printf("0x1001 not found  (expected, it was removed)\n");
    }

    rh_map_lookup(&table, 0x2001, &out);
    printf("0x2001 still -> handler_id=%u  (expected 20)\n", out.handler_id);

    return 0;
}
