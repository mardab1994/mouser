#include <stdio.h>

#include "screen/screen.h"

int
main(void)
{
    screen_size_t screen_size = screen_size_get();
    if (screen_size.result) {
        return -1;
    }

    return 0;
}
