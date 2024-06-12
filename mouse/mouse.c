#include "mouse.h"
#include "mouse_exit.h"
#include "mouse_pause.h"

#include <fcntl.h>
#include <linux/uinput.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>


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