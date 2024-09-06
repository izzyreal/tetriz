#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

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
} State;

State state = { .tetromino_type = TETROMINO_NONE };

void clear_canvas()
{
    for (uint8_t y = 0; y < HEIGHT; y++)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            state.canvas[y][x] = ' ';
        }
    }
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
