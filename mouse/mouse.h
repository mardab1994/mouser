#pragma once

int mouse_init(const int screen_width, const int screen_height);

int mouse_deinit(void);

void mouse_move(int x, int y);

void mouse_position_get(int *x, int *y);

void mouse_algo(void);