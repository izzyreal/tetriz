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
const uint32_t DROP_INTERVAL = 500000;

/* The below constants are in chars, and it should be noted that each block within a tetromino is 2 characters wide when drawn */
const uint8_t PLAYFIELD_Y = 5;
const uint8_t PLAYFIELD_X = 10;
const uint8_t PLAYFIELD_WIDTH = 20;
const uint8_t PLAYFIELD_HEIGHT = 20;

typedef struct {
    char canvas[HEIGHT][WIDTH];
    TETROMINO_TYPE tetromino_type;
    int8_t tetromino_x;
    int8_t tetromino_y;
    int8_t tetromino_rotation;
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

void ding()
{
    system("tput bel");
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

void drop_tetromino()
{
    state.tetromino_y++;

    if (state.tetromino_y >= PLAYFIELD_HEIGHT - (TETROMINO_SIZE - 1))
    {
        state.tetromino_y = 0;
        state.tetromino_x = 5;
    }
}

Tetromino* rotate(const Tetromino* tetromino, uint8_t rotation)
{
    Tetromino* rotated = malloc(sizeof(Tetromino));

    uint8_t i, j;
    const uint8_t pivot = 1;

    for (i = 0; i < TETROMINO_SIZE; i++) {
        for (j = 0; j < TETROMINO_SIZE; j++) {
            int x = j - pivot;
            int y = i - pivot;
            uint8_t new_i, new_j;
            switch (rotation) {
                case 0:
                    new_i = (pivot + y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot + x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case 1:
                    new_i = (pivot - x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot + y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case 2:
                    new_i = (pivot - y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot - x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case 3:
                    new_i = (pivot + x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot - y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
            }
            (*rotated)[i][j] = (*tetromino)[new_i][new_j];
        }
    }
    return rotated;
}

Tetromino* get_rotated_current_tetromino()
{
    Tetromino* tetromino = &TETROMINOS[state.tetromino_type - 1];
    Tetromino* rotated_tetromino = rotate(tetromino, state.tetromino_rotation);
    return rotated_tetromino;
}

void draw_tetromino()
{
    Tetromino* rotated_tetromino = get_rotated_current_tetromino();

    uint8_t i, j, x, y;

    for (i = 0; i < TETROMINO_SIZE; i++) {
        for (j = 0; j < TETROMINO_SIZE; j++) {
            x = j;
            y = i;
            const char cell = (*rotated_tetromino)[y][x];
            if (cell != ' ') {
                state.canvas[state.tetromino_y + i + PLAYFIELD_Y][((state.tetromino_x + j) * 2) + PLAYFIELD_X] = '[';
                state.canvas[state.tetromino_y + i + PLAYFIELD_Y][((state.tetromino_x + j) * 2) + PLAYFIELD_X + 1] = ']';
            }
        }
    }

    free(rotated_tetromino);
}

bool tetromino_is_within_playfield_bounds()
{
    Tetromino* new_tetromino = get_rotated_current_tetromino();
    int8_t left_bound = 127, right_bound = 0;
    for (uint8_t x = 0; x < TETROMINO_SIZE; x++)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE; y++)
        {
            const char cell = (*new_tetromino)[y][x];

            if (cell == ' ') continue;
            
            if (x < left_bound)
            {
                left_bound = x;
            }

            if (x > right_bound)
            {
                right_bound = x;
            }
        }
    }
    free(new_tetromino);
    return state.tetromino_x + (left_bound - 1) >= -1 &&
           state.tetromino_x + (right_bound - 1) < 9;
}

void handle_rotate(bool clockwise)
{
    int8_t old_rotation = state.tetromino_rotation;

    if (clockwise) state.tetromino_rotation++; else state.tetromino_rotation--;

    uint8_t number_of_configurations;

    switch (state.tetromino_type) {
        case TETROMINO_I:
        case TETROMINO_S:
        case TETROMINO_Z:
            number_of_configurations = 2;
            break;
        case TETROMINO_L:
        case TETROMINO_J:
        case TETROMINO_T:
            number_of_configurations = 4;
            break;
        case TETROMINO_O:
            number_of_configurations = 1;
            break;
        default:
            break;
    }

    if (clockwise && state.tetromino_rotation == number_of_configurations) state.tetromino_rotation = 0;
    else if (state.tetromino_rotation == -1) state.tetromino_rotation = number_of_configurations - 1;
    
    if (!tetromino_is_within_playfield_bounds())
    {
        state.tetromino_rotation = old_rotation;
    }
}

void process_kb()
{
    int ch = getch();
    if (ch == 'x' || ch == 's')
    {
        handle_rotate(ch == 'x');
    }
    else if (ch == KEY_LEFT)
    {
        state.tetromino_x--;
        if (!tetromino_is_within_playfield_bounds()) state.tetromino_x++;
    }
    else if (ch == KEY_RIGHT)
    {
        state.tetromino_x++;
        if (!tetromino_is_within_playfield_bounds()) state.tetromino_x--;
    }
    else if (ch == KEY_UP)
    {
        TETROMINO_TYPE old_type = state.tetromino_type;
        state.tetromino_type++;
        if (state.tetromino_type == TETROMINO_COUNT) state.tetromino_type = TETROMINO_I;
        if (!tetromino_is_within_playfield_bounds()) state.tetromino_type = old_type;
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
    timeout(0);
    keypad(stdscr, TRUE);

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
            //drop_tetromino();
            tetromino_drop_timer = current_time;
        }

        napms(10);
    }

    endwin();
    return 0;
}

