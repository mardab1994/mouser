#include "mouse.h"
#include "mouse_exit.h"
#include "mouse_pause.h"

#include <X11/Xlib.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define MARGIN (120)

static int mouse = -1;

static int mouse_screen_max_width;
static int mouse_screen_max_height;

static void
mouse_emit(int type, int code, int val)
{
    struct input_event ie;

    ie.type  = type;
    ie.code  = code;
    ie.value = val;

    ie.time.tv_sec  = 0;
    ie.time.tv_usec = 0;

    write(mouse, &ie, sizeof(ie));
}

static int64_t
millis(void)
{
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((int64_t)now.tv_sec) * 1000 + ((int64_t)now.tv_nsec) / 1000000;
}

static bool
mouse_slow_down(void)
{
    int64_t        current_time;
    static int64_t last_time = 0;

    current_time = millis();

    if ((current_time - last_time) < 5) {
        return true;
    }

    last_time = current_time;

    return false;
}

int
mouse_init(const int screen_width, const int screen_height)
{
    mouse_screen_max_width  = screen_width;
    mouse_screen_max_height = screen_height;

    struct uinput_setup usetup;

    mouse = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (mouse < 0) {
        return -1;
    }

    ioctl(mouse, UI_SET_EVBIT, EV_KEY);
    ioctl(mouse, UI_SET_KEYBIT, BTN_LEFT);

    ioctl(mouse, UI_SET_EVBIT, EV_REL);
    ioctl(mouse, UI_SET_RELBIT, REL_X);
    ioctl(mouse, UI_SET_RELBIT, REL_Y);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor  = 0x1234;

    usetup.id.product = 0x5678;
    strcpy(usetup.name, "myMouse");

    ioctl(mouse, UI_DEV_SETUP, &usetup);
    ioctl(mouse, UI_DEV_CREATE);
    sleep(1);

    mouse_pause_init();

    return 0;
}

int
mouse_deinit(void)
{
    if (mouse < 0) {
        return -1;
    }

    ioctl(mouse, UI_DEV_DESTROY);
    close(mouse);

    return 0;
}

void
mouse_move(int x, int y)
{
    mouse_emit(EV_REL, REL_X, x);
    mouse_emit(EV_REL, REL_Y, y);
    mouse_emit(EV_SYN, SYN_REPORT, 0);
}

void
mouse_position_get(int *x, int *y)
{
    Display     *display;
    Window       root_window, focus_window;
    int          revert;
    int          win_x, win_y;
    unsigned int mask;

    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return;
    }

    // Get the root window
    root_window = DefaultRootWindow(display);

    // Get the window with input focus
    XGetInputFocus(display, &focus_window, &revert);

    // Get the current pointer position
    XQueryPointer(display, root_window, &root_window, &root_window, x, y, &win_x, &win_y, &mask);

    XCloseDisplay(display);

    printf("Mouse coordinates: (%d, %d)\r\n", *x, *y);
}

void
mouse_algo(void)
{
    int current_pos_x, current_pos_y;
    mouse_position_get(&current_pos_x, &current_pos_y);

    int target_x = PAUSE_X;
    while (target_x <= PAUSE_X) {
        target_x = rand() % mouse_screen_max_width;
    }

    int target_y = PAUSE_Y;
    while (target_y <= PAUSE_Y) {
        target_y = rand() % mouse_screen_max_height;
    }

    printf("Target x = %d, y = %d\r\n", target_x, target_y);

    while (1) {
        mouse_position_get(&current_pos_x, &current_pos_y);

        mouse_pause(current_pos_x, current_pos_y);

        if (mouse_slow_down()) {
            continue;
        }

        if (mouse_pause_state_get()) {
            continue;
        }

        if ((abs(current_pos_x - target_x) < MARGIN) && (abs(current_pos_y - target_y) < MARGIN)) {
            printf("target achieved\r\n");
            return;
        }

        int x = 0;
        if (target_x - current_pos_x > 0) {
            x = 1;
        } else if (target_x - current_pos_x < 0) {
            x = -1;
        }

        int y = 0;
        if (target_y - current_pos_y > 0) {
            y = 1;
        } else if (target_y - current_pos_y < 0) {
            y = -1;
        }

        mouse_move(x, y);
    }
}
