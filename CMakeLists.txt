cmake_minimum_required(VERSION 3.2)

project(mouser
        VERSION 0.0.1
        LANGUAGES C)

add_executable(${CMAKE_PROJECT_NAME} main.c)

add_subdirectory(screen)
target_link_libraries(${CMAKE_PROJECT_NAME} screen)
