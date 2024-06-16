#include "mouse_pause.h"
#include "mouse_cfg.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static volatile bool mouse_pause_state;

static void
mouse_pause_end(int signum)
{
    mouse_pause_state = false;
}

void
mouse_pause_init(void)
{
    signal(SIGALRM, mouse_pause_end);
}

void
mouse_pause(const int x, const int y)
{
    if (mouse_pause_state) {
        return;
    }

    if ((x < PAUSE_X) && (y < PAUSE_Y)) {
        printf("Pause enable for 15s\r\n");
        mouse_pause_state = true;
        alarm(15);
        return;
    }

    mouse_pause_state = false;
}

bool
mouse_pause_state_get(void)
{
    return mouse_pause_state;
}
