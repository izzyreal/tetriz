#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

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
    state.tetromino_type = TETROMINO_NONE;
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

uint8_t foo_x = 1;

void draw_foo()
{
    state.canvas[1][foo_x] = 'O';
    foo_x++;
}

int main()
{
    for (uint8_t i = 0; i < 10; i++)
    {
        clear_screen();
        clear_canvas();
        draw_border();
        draw_foo();
        draw_canvas_to_screen();
        usleep(50000);
    }
}
