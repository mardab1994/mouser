#include "mouse_exit.h"
#include "mouse_cfg.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ALFA = 0,
    BRAVO,
    CHARLIE,
    DELTA,
    LAST_CONRNER
} corner_type_t;

typedef struct {
    bool achievet;
    int  x;
    int  y;
} corner_t;

static corner_t corners[LAST_CONRNER] = {0};

static void
mouse_exit_clear(void)
{
    for (int i = 0; i < LAST_CONRNER; i++) {
        corners[i].achievet = false;
    }
}

void
mouse_exit_init(const int screen_width, const int screen_height)
{
    corners[ALFA]    = (corner_t){.x = MARGIN, .y = screen_height - MARGIN};
    corners[BRAVO]   = (corner_t){.x = MARGIN, .y = MARGIN};
    corners[CHARLIE] = (corner_t){.x = screen_width - screen_width, .y = MARGIN};
    corners[DELTA]   = (corner_t){.x = screen_width - MARGIN, .y = screen_height - MARGIN};
}

void
mouse_exit(const int x, const int y)
{
    if (x <= corners[ALFA].x && y >= corners[ALFA].y) {
        if (!corners[ALFA].achievet) {
            corners[ALFA].achievet = true;
            printf("ALFA\r\n");
            return;
        }

        if (corners[BRAVO].achievet || corners[CHARLIE].achievet || corners[DELTA].achievet) {
            mouse_exit_clear();
            return;
        }
        return;
    }

    if (x <= corners[BRAVO].x && y <= corners[BRAVO].y) {
        if (!corners[ALFA].achievet) {
            mouse_exit_clear();
            return;
        }

        if (!corners[BRAVO].achievet) {
            corners[BRAVO].achievet = true;
            printf("BRAVO\r\n");
            return;
        }
        if (corners[CHARLIE].achievet || corners[DELTA].achievet) {
            mouse_exit_clear();
            printf("BRAVO discard\r\n");
            return;
        }
        return;
    }

    if (x >= corners[CHARLIE].x && y <= corners[CHARLIE].y && !corners[CHARLIE].achievet) {
        if (!corners[ALFA].achievet || !corners[BRAVO].achievet) {
            mouse_exit_clear();
            return;
        }

        if (!corners[CHARLIE].achievet) {
            corners[CHARLIE].achievet = true;
            printf("CHARLIE\r\n");
            return;
        }

        if (corners[DELTA].achievet) {
            mouse_exit_clear();
            printf("CHARLIE discard\r\n");
            return;
        }
        return;
    }

    if (x >= corners[DELTA].x && y >= corners[DELTA].y && !corners[DELTA].achievet) {
        if (!corners[ALFA].achievet || !corners[BRAVO].achievet || !corners[CHARLIE].achievet) {
            mouse_exit_clear();
            return;
        }

        if (!corners[DELTA].achievet) {
            corners[DELTA].achievet = true;
            printf("DELTA\r\n");
        }
    }

    /// verify gesture and exit if need
    for (int i = 0; i < LAST_CONRNER; i++) {
        if (!corners[i].achievet) {
            return;
        }
    }

    exit(0);
}
