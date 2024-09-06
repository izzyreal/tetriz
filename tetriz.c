#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>

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
    state.tetromino_rotation = 0;
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

    state.tetromino_y++;
}

void process_kb()
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    fd_set set;
    struct timeval timeout;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 500000; // 500ms timeout

    int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);

    if (rv > 0) {
        ch = getchar();
        if (ch == 'x') {
            state.tetromino_rotation++;
            if (state.tetromino_rotation > 3) state.tetromino_rotation = 0;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
}

struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void enable_raw_mode() {
    struct termios raw;

    tcgetattr(STDIN_FILENO, &orig_termios); // Save original state
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    atexit(disable_raw_mode);               // Restore on exit
}

int main()
{
    enable_raw_mode();
    init_state();

    for (uint8_t i = 0; i < 30; i++)
    {
        process_kb(); 
        clear_screen();
        clear_canvas();
        draw_border();
        draw_tetromino();
        draw_canvas_to_screen();
        usleep(500000);
    }
}
