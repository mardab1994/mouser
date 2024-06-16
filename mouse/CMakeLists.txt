add_library(mouse
            mouse.h mouse.c
            mouse_pause.h mouse_pause.c
            mouse_exit.h mouse_exit.c)

target_link_libraries(mouse X11)
