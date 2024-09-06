#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "tetrominos.h"

const uint8_t WIDTH = 80;
const uint8_t HEIGHT = 40;

typedef struct {
    char canvas[HEIGHT][WIDTH];
    TETROMINO_TYPE tetromino_type;
    uint8_t tetromino_x;
    uint8_t tetromino_y;
    uint8_t tetromino_rotation;
} State;

State state;

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
    state.tetromino_rotation = 2;
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
    Tetromino tetromino = tetrominos[state.tetromino_type - 1];
    const bool* data = tetromino.data;
    uint8_t w, h;

    // Adjust width and height based on rotation
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
                case 0: // No rotation
                    cell = data[y * tetromino.width + x];
                    break;
                case 1: // 90 degrees
                    cell = data[(tetromino.height - x - 1) * tetromino.width + y];
                    break;
                case 2: // 180 degrees
                    cell = data[(tetromino.height - y - 1) * tetromino.width + (tetromino.width - x - 1)];
                    break;
                case 3: // 270 degrees
                    cell = data[x * tetromino.width + (tetromino.height - y - 1)];
                    break;
            }
            if (cell)
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
