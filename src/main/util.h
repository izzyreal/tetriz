#include <stdint.h>
#include <time.h>
#include <string.h>

uint32_t get_current_time_microseconds()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

void ding()
{
    system("tput bel");
}

void init_array(char*** a, uint8_t height, uint8_t width)
{
    *a = malloc(height * sizeof(char*));
    for (uint8_t i = 0; i < height; i++)
    {
        (*a)[i] = malloc(width * sizeof(char));
        memset((*a)[i], ' ', width);
    }
}

void free_array(char*** a, uint8_t height)
{
    for (uint8_t i = 0; i < height; i++)
    {
        free((*a)[i]);
    }
    free(*a);
}

void draw_cell(char** a, const int8_t x, const int8_t y, bool empty)
{
    if (empty)
    {
        a[y][x] = ' ';
        a[y][x+1] = ' ';
    }
    else
    {
        a[y][x] = '[';
        a[y][x+1] = ']';
    }
}

