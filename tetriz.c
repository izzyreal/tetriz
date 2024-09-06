#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "tetrominos.h"

const uint8_t WIDTH = 80;
const uint8_t HEIGHT = 40;

typedef enum {
    TETROMINO_NONE,
    TETROMINO_STRAIGHT,
    TETROMINO_SQUARE,
    TETROMINO_T,
    TETROMINO_L,
    TETROMINO_SKEW
} TETROMINO_TYPE;

typedef struct {
    char canvas[HEIGHT][WIDTH];
    TETROMINO_TYPE tetromino_type;
    uint8_t tetromino_x;
    uint8_t tetromino_y;
} State;

State state;

void clear_canvas()
{
    memset(state.canvas, ' ', sizeof(state.canvas));
}

void init_state()
{
    clear_canvas();
    state.tetromino_type = TETROMINO_SKEW;
    state.tetromino_x = WIDTH / 2;
    state.tetromino_y = 1;
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
            printf("%c", state.canvas[y][x]);
        }
        printf("\n");
    }
}

void clear_screen()
{
    printf("\033[H\033[J");
}

void draw_tetromino()
{
    const bool* data;
    uint8_t w, h;

    switch (state.tetromino_type)
    {
        case TETROMINO_STRAIGHT:
            w = sizeof(T_DATA_STRAIGHT[0]) / sizeof(T_DATA_STRAIGHT[0][0]);
            data = &T_DATA_STRAIGHT[0][0];
            h = sizeof(T_DATA_STRAIGHT) / sizeof(T_DATA_STRAIGHT[0]);
            break;
        case TETROMINO_SQUARE:
            w = sizeof(T_DATA_SQUARE[0]) / sizeof(T_DATA_SQUARE[0][0]);
            data = &T_DATA_SQUARE[0][0];
            h = sizeof(T_DATA_SQUARE) / sizeof(T_DATA_SQUARE[0]);
            break;
        case TETROMINO_T:
            w = sizeof(T_DATA_T[0]) / sizeof(T_DATA_T[0][0]);
            data = &T_DATA_T[0][0];
            h = sizeof(T_DATA_T) / sizeof(T_DATA_T[0]);
            break;
        case TETROMINO_L:
            w = sizeof(T_DATA_L[0]) / sizeof(T_DATA_L[0][0]);
            data = &T_DATA_L[0][0];
            h = sizeof(T_DATA_L) / sizeof(T_DATA_L[0]);
            break;
        case TETROMINO_SKEW:
            w = sizeof(T_DATA_SKEW[0]) / sizeof(T_DATA_SKEW[0][0]);
            data = &T_DATA_SKEW[0][0];
            h = sizeof(T_DATA_SKEW) / sizeof(T_DATA_SKEW[0]);
            break;
        default:
            return;
    }

    for (uint8_t y = 0; y < h; y++)
    {
        for (uint8_t x = 0; x < w; x++)
        {
            if (data[y * w + x])
            {
                state.canvas[state.tetromino_y + y][state.tetromino_x + x] = 'O';
            }
        }
    }

    state.tetromino_y++;
}

int main()
{
    init_state();

    for (uint8_t i = 0; i < 30; i++)
    {
        clear_screen();
        clear_canvas();
        draw_border();
        draw_tetromino();
        draw_canvas_to_screen();
        usleep(500000);
    }
}
