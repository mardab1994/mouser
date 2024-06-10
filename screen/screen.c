#include "screen.h"

#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

screen_size_t
screen_size_get(void)
{
    struct fb_var_screeninfo vinfo;

    screen_size_t screen_size = {0, 0, -1};

    int fd = open("/dev/fb0", O_RDONLY);
    if (fd < 0) {
        printf("Failed to open frame buffer device\r\n");
        return screen_size;
    }

    // Get fixed screen information
    if (ioctl(fd, FBIOGET_FSCREENINFO, &vinfo) == -1) {
        printf("Error reading fixed information\r\n");
        return screen_size;
    }

    // Get the screen resolution
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        printf("Failed to get screen resolution\r\n");
        close(fd);
        return screen_size;
    }

    screen_size.width  = vinfo.xres;
    screen_size.height = vinfo.yres;
    screen_size.result = 0;

    printf("Screen resolution: %d x %d\r\n", vinfo.xres, vinfo.yres);

    close(fd);

    return screen_size;
}
