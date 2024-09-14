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

const uint8_t PLAYFIELD_Y = 5;
const uint8_t PLAYFIELD_X = 30;
const uint8_t PLAYFIELD_WIDTH = 10;
const uint8_t PLAYFIELD_HEIGHT = 20;

typedef struct {
    char canvas[HEIGHT][WIDTH];
    TetrominoType tetromino_type;
    TetrominoType next_tetromino_type;
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

void clear_playfield()
{
    memset(state.playfield, ' ', sizeof(state.playfield));
}

TetrominoType pick_random_tetromino_type()
{
    static uint32_t seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return (TetrominoType)((seed % (TETROMINO_COUNT - 1)) + 1);
}

void init_state()
{
    clear_canvas();
    clear_playfield();
    state.tetromino_type = pick_random_tetromino_type();
    state.next_tetromino_type = pick_random_tetromino_type();
    state.tetromino_x = 3;
    state.tetromino_y = -1;
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
    for (uint8_t y = PLAYFIELD_Y; y < (PLAYFIELD_Y + PLAYFIELD_HEIGHT + 1); y++)
    {
        for (uint8_t x = (PLAYFIELD_X - 1); x < (PLAYFIELD_X + (PLAYFIELD_WIDTH * 2) + 1); x++)
        {
            if (y > (PLAYFIELD_Y - 1) &&
                    y < (PLAYFIELD_Y + PLAYFIELD_HEIGHT) &&
                    x != (PLAYFIELD_X - 1) &&
                    x != PLAYFIELD_X + (PLAYFIELD_WIDTH*2))
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

void draw_next_tetromino()
{
    Tetromino* t = &TETROMINOS[state.next_tetromino_type - 1];
    uint8_t x,y;
    for (x=0;x<TETROMINO_SIZE;x++)
        for (y=0;y<TETROMINO_SIZE;y++)
        {
            char cell = (*t)[y][x];
            if (cell == ' ') continue;
            state.canvas[10+y][10+(x*2)] = '[';
            state.canvas[10+y][10+(x*2)+1] = ']';
        }
}

void draw_tetromino()
{
    Tetromino* rotated_tetromino = get_rotated_current_tetromino();

    int8_t i, j, x, y, y_offset_in_playfield;

    for (i = 0; i < TETROMINO_SIZE; i++)
    {
        for (j = 0; j < TETROMINO_SIZE; j++)
        {
            x = j;
            y = i;

            y_offset_in_playfield = state.tetromino_y + i;

            const char cell = (*rotated_tetromino)[y][x];

            if (cell != ' ' && y_offset_in_playfield >= 0)
            {
                state.canvas[y_offset_in_playfield + PLAYFIELD_Y][((state.tetromino_x + j) * 2) + PLAYFIELD_X] = '[';
                state.canvas[y_offset_in_playfield + PLAYFIELD_Y][((state.tetromino_x + j) * 2) + PLAYFIELD_X + 1] = ']';
            }
        }
    }

    free(rotated_tetromino);
}

TetrominoBounds get_current_tetromino_bounds()
{
    Tetromino* tetromino = get_rotated_current_tetromino();
    TetrominoBounds bounds;

    for (uint8_t x = 0; x < TETROMINO_SIZE; x++)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE; y++)
        {
            const char cell = (*tetromino)[y][x];

            if (cell == ' ') continue;

            if (x < bounds.left) bounds.left = x;
            if (x > bounds.right) bounds.right = x;
            if (y < bounds.top) bounds.top = y;
            if (y > bounds.bottom) bounds.bottom = y;
        }
    }

    free(tetromino);

    return bounds;
}

bool tetromino_intersects_playfield()
{
    Tetromino* tetromino = get_rotated_current_tetromino();

    int8_t x, y, playfield_y;

    for (x=0;x<TETROMINO_SIZE;x++)
    {
        for (y=0;y<TETROMINO_SIZE;y++)
        {
            playfield_y = state.tetromino_y + y;

            if (playfield_y < 0) continue;

            if ((*tetromino)[y][x] != ' ' && state.playfield[playfield_y][state.tetromino_x + x] != ' ')
            {
                return true;
            }
        }
    }

    return false;
}

bool tetromino_should_assimilate()
{
    TetrominoBounds bounds = get_current_tetromino_bounds();

    if (state.tetromino_y >= PLAYFIELD_HEIGHT - (bounds.bottom + 1)) return true;

    state.tetromino_y++;

    bool should_assimilate = tetromino_intersects_playfield(); 

    state.tetromino_y--;

    return should_assimilate;
}

void assimilate_current_tetromino()
{
    Tetromino* tetromino = get_rotated_current_tetromino();

    int8_t x,y;

    for (x=0;x<TETROMINO_SIZE;x++)
    {
        for (y=0;y<TETROMINO_SIZE;y++)
        {
            if ((*tetromino)[y][x] != ' ')
            {
                state.playfield[state.tetromino_y + y][state.tetromino_x + x] = (*tetromino)[y][x];
            }
        }
    }
}

void clear_line(uint8_t y)
{
    memset(state.playfield[y], ' ', PLAYFIELD_WIDTH);
}

bool is_line_clear(uint8_t y)
{
    for (uint8_t x=0;x<PLAYFIELD_WIDTH;x++)
    {
        if (state.playfield[y][x] != ' ') return false;
    }
    return true;
}

void move_first_non_clear_line_to(uint8_t dest_index)
{
    int8_t src_index = dest_index - 1;
    
    for (;src_index>=0;src_index--)
    {
        if (!is_line_clear(src_index))
        {
            for (uint8_t x=0;x<PLAYFIELD_WIDTH;x++)
            {
                state.playfield[dest_index][x] = state.playfield[src_index][x];
            }

            clear_line(src_index);
            break;
        }
    }
}

void consolidate_playfield()
{
    int8_t y;
    
    for (y = PLAYFIELD_HEIGHT - 1; y >= 0; y--)
    {
        if (is_line_clear(y)) move_first_non_clear_line_to(y);
    }
}

void clear_completed_lines()
{
    uint8_t x,y;

    bool lines_were_cleared = false;

    for (y=0;y<PLAYFIELD_HEIGHT;y++)
    {
        bool line_is_complete = true;

        for (x=0;x<PLAYFIELD_WIDTH;x++)
        {
            if (state.playfield[y][x] == ' ')
            {
                line_is_complete = false;
                break;
            }
        }

        if (line_is_complete)
        {
            clear_line(y);
            lines_were_cleared = true;
            ding();
        }
    }

    if (lines_were_cleared) consolidate_playfield();
}

void drop_tetromino()
{
    if (!tetromino_should_assimilate())
    {
        state.tetromino_y++;
        return;
    }

    assimilate_current_tetromino();

    clear_completed_lines();

    state.tetromino_y = -1;
    state.tetromino_x = 3;
    state.tetromino_rotation = 0;
    state.tetromino_type = state.next_tetromino_type;;
    state.next_tetromino_type = pick_random_tetromino_type();
}

bool tetromino_is_within_playfield_bounds()
{
    TetrominoBounds b = get_current_tetromino_bounds();
    return state.tetromino_x + (b.left - 1) >= -1 &&
        state.tetromino_x + (b.right - 1) < 9;
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

    if (!tetromino_is_within_playfield_bounds() || tetromino_intersects_playfield())
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
        if (!tetromino_is_within_playfield_bounds() || tetromino_intersects_playfield()) state.tetromino_x++;
    }
    else if (ch == KEY_RIGHT)
    {
        state.tetromino_x++;
        if (!tetromino_is_within_playfield_bounds() || tetromino_intersects_playfield()) state.tetromino_x--;
    }
    /*else if (ch == KEY_UP)
      {
      TetrominoType old_type = state.tetromino_type;
      state.tetromino_type++;
      if (state.tetromino_type == TETROMINO_COUNT) state.tetromino_type = TETROMINO_I;
      if (!tetromino_is_within_playfield_bounds()) state.tetromino_type = old_type;
      }*/
      else if (ch == KEY_DOWN)
      {
          drop_tetromino();
      }
      else if (ch == 'q')
      {
          state.user_has_requested_exit = true;
      }
}

void draw_playfield()
{
    int8_t x,y;
    for (x=0;x<PLAYFIELD_WIDTH;x++)
    {
        for (y=0;y<PLAYFIELD_HEIGHT;y++)
        {
            if (state.playfield[y][x] == ' ') continue;
            state.canvas[y + PLAYFIELD_Y][(x*2) + PLAYFIELD_X] = '[';
            state.canvas[y + PLAYFIELD_Y][(x*2) + PLAYFIELD_X + 1] = ']';
        }
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

        clear();
        clear_canvas();

        draw_border();
        draw_playfield_border();
        draw_playfield();
        draw_tetromino();
        draw_next_tetromino();

        draw_canvas_to_screen();

        // Handle tetromino drop based on the drop timer
        if (current_time - tetromino_drop_timer >= DROP_INTERVAL) {
            drop_tetromino();
            tetromino_drop_timer = current_time;
        }

        napms(10);
    }

    endwin();
    return 0;
}

