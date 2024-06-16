#pragma once

#include <stdbool.h>

void mouse_pause_init(void);

void mouse_pause(const int x, const int y);

bool mouse_pause_state_get(void);