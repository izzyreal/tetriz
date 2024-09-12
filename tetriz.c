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
const uint32_t DROP_INTERVAL = 300000;

/* The below constants are in chars, and it should be noted that each block within a tetromino is 2 characters wide when drawn */
const uint8_t PLAYFIELD_Y = 5;
const uint8_t PLAYFIELD_X = 10;
const uint8_t PLAYFIELD_WIDTH = 20;
const uint8_t PLAYFIELD_HEIGHT = 20;

typedef struct {
    char canvas[HEIGHT][WIDTH];
    TETROMINO_TYPE tetromino_type;
    uint8_t tetromino_x;
    uint8_t tetromino_y;
    uint8_t tetromino_rotation;
    bool user_has_requested_exit;
    char playfield[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
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
    state.tetromino_type = TETROMINO_J;
    state.tetromino_x = 0;
    state.tetromino_y = 0;
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

void draw_playfield_border()
{
    for (uint8_t y = (PLAYFIELD_Y - 1); y < (PLAYFIELD_Y + PLAYFIELD_HEIGHT + 1); y++)
    {
        for (uint8_t x = (PLAYFIELD_X - 1); x < (PLAYFIELD_X + PLAYFIELD_WIDTH + 1); x++)
        {
            if (y > (PLAYFIELD_Y - 1) &&
                    y < (PLAYFIELD_Y + PLAYFIELD_HEIGHT) &&
                    x != (PLAYFIELD_X - 1) &&
                    x != PLAYFIELD_X + PLAYFIELD_WIDTH)
            {
                continue;
            }
            state.canvas[y][x] = '!';
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
    Tetromino* tetromino = &TETROMINOS[state.tetromino_type - 1];

    for (uint8_t y = 0; y < TETROMINO_SIZE; y++)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE; x++)
        {
            const char cell = (*tetromino)[y][x];
            if (cell != ' ')
            {
                state.canvas[state.tetromino_y + y + PLAYFIELD_Y][((state.tetromino_x + x)*2) + PLAYFIELD_X] = '[';
                state.canvas[state.tetromino_y + y + PLAYFIELD_Y][((state.tetromino_x + x)*2) + PLAYFIELD_X + 1] = ']';
            }
        }
    }
}

void drop_tetromino()
{
    state.tetromino_y++;

    if (state.tetromino_y >= PLAYFIELD_HEIGHT - (TETROMINO_SIZE - 1))
    {
        state.tetromino_y = 0;
        state.tetromino_x = 0;
    }
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
        state.user_has_requested_exit = true;
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

        if (state.user_has_requested_exit)
        {
            break;
        }

        clear_screen();
        clear_canvas();
        draw_border();
        draw_playfield_border();
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

