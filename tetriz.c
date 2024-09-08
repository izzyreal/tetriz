#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <fcntl.h>
#include <time.h>

#include "tetrominos.h"

const uint8_t WIDTH = 80;
const uint8_t HEIGHT = 40;
const uint32_t DROP_INTERVAL = 1000000; // Drop every 1 second

typedef struct {
    char canvas[HEIGHT][WIDTH];
    TETROMINO_TYPE tetromino_type;
    uint8_t tetromino_x;
    uint8_t tetromino_y;
    uint8_t tetromino_rotation;
    bool user_requested_exit;
} State;

State state;
uint32_t tetromino_drop_timer = 0;

uint32_t get_current_time_microseconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

void clear_canvas()
{
    memset(state.canvas, ' ', sizeof(state.canvas));
}

void init_state()
{
    clear_canvas();
    state.tetromino_type = TETROMINO_T;
    state.tetromino_x = WIDTH / 2;
    state.tetromino_y = 1;
    state.tetromino_rotation = 0;
    tetromino_drop_timer = get_current_time_microseconds();
}

void draw_border()
{
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            if (y > 0 && y < (HEIGHT - 1) && x != 0 && x != WIDTH - 1)
            {
                continue;
            }

            state.canvas[y][x] = '#';
        }
    }
}

void draw_canvas_to_screen()
{
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            mvaddch(y, x, state.canvas[y][x]);
        }
    }
    refresh();
}

void clear_screen()
{
    clear();
}

void draw_tetromino()
{
    Tetromino tetromino = tetrominos[state.tetromino_type - 1];
    const bool* data = tetromino.data;
    uint8_t w, h;

    if (state.tetromino_rotation == 0 || state.tetromino_rotation == 2)
    {
        w = tetromino.width;
        h = tetromino.height;
    }
    else
    {
        w = tetromino.height;
        h = tetromino.width;
    }

    for (uint8_t y = 0; y < h; y++)
    {
        for (uint8_t x = 0; x < w; x++)
        {
            bool cell;
            switch (state.tetromino_rotation)
            {
                case 0:
                    cell = data[y * tetromino.width + x];
                    break;
                case 1:
                    cell = data[(tetromino.height - x - 1) * tetromino.width + y];
                    break;
                case 2:
                    cell = data[(tetromino.height - y - 1) * tetromino.width + (tetromino.width - x - 1)];
                    break;
                case 3:
                    cell = data[x * tetromino.width + (h - y - 1)];
                    break;
            }
            if (cell)
            {
                state.canvas[state.tetromino_y + y][state.tetromino_x + x] = 'O';
            }
        }
    }
}

void drop_tetromino()
{
    state.tetromino_y++;
}

void process_kb()
{
    int ch = getch();
    if (ch == 'x')
    {
        state.tetromino_rotation++;
        if (state.tetromino_rotation > 3) state.tetromino_rotation = 0;
    }
    else if (ch == 'q')
    {
        state.user_requested_exit = true;
    }
}

int main()
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    init_state();

    while (1)
    {
        uint32_t current_time = get_current_time_microseconds();

        process_kb();

        if (state.user_requested_exit)
        {
            break;
        }

        clear_screen();
        clear_canvas();
        draw_border();
        draw_tetromino();
        draw_canvas_to_screen();

        // Handle tetromino drop based on the drop timer
        if (current_time - tetromino_drop_timer >= DROP_INTERVAL) {
            drop_tetromino();
            tetromino_drop_timer = current_time;
        }

        usleep(50000); // Main loop runs at ~50ms intervals (20 FPS)
    }

    endwin();
    return 0;
}

