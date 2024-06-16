#include <stdio.h>
#include <stdlib.h>

#include "mouse/mouse.h"
#include "screen/screen.h"

int
main(void)
{
    screen_size_t screen_size = screen_size_get();
    if (screen_size.result) {
        return -1;
    }

    if (mouse_init(screen_size.width, screen_size.height)) {
        return -2;
    }

    atexit(mouse_deinit);

    while (1) {
        mouse_algo();
    }

    return 0;
}
