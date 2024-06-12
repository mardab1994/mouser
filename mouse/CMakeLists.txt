add_library(mouse
            mouse.h mouse.c
            mouse_pause.h mouse_pause.c)

target_link_libraries(mouse X11)
