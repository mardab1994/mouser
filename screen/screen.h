#pragma once

typedef struct {
    int width;
    int height;
    int result;
} screen_size_t;

screen_size_t
screen_size_get(void);
