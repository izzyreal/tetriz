#include "tetrominos.h"
#include "constants.h"
#include "state.h"
#include "util.h"
#include "audio.h"
#include "draw.h"

#include <stdint.h>
#include <ncurses.h>

void init_state(State *state)
{
    init_array(&state->canvas, CANVAS_HEIGHT, CANVAS_WIDTH);
    init_array(&state->prev_canvas, CANVAS_HEIGHT, CANVAS_WIDTH);
    init_array(&state->playfield, PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH);

    state->user_has_requested_exit = false;
    state->tetromino_type = pick_random_tetromino_type();
    state->next_tetromino_type = pick_random_tetromino_type();
    state->tetromino_x = 3;
    state->tetromino_y = -1;
    state->tetromino_rotation = 0;
    state->drop_interval = 700000;
    state->tetromino_drop_timer = get_current_time_microseconds();
}

void clear_line(State *state, uint8_t y)
{
    memset(state->playfield[y], ' ', PLAYFIELD_WIDTH);
}

bool is_line_clear(State *state, uint8_t y)
{
    for (uint8_t x=0;x<PLAYFIELD_WIDTH;x++)
    {
        if (state->playfield[y][x] != ' ')
        {
            return false;
        }
    }

    return true;
}

void move_first_non_clear_line_to(State *state, uint8_t dest_index)
{
    int8_t src_index = dest_index - 1;

    for (;src_index>=0;src_index--)
    {
        if (!is_line_clear(state, src_index))
        {
            for (uint8_t x=0;x<PLAYFIELD_WIDTH;x++)
            {
                state->playfield[dest_index][x] = state->playfield[src_index][x];
            }

            clear_line(state, src_index);
            break;
        }
    }
}

void consolidate_playfield(State *state)
{
    uint8_t y;

    for (y = PLAYFIELD_HEIGHT - 1; y >= 0; y--)
    {
        if (is_line_clear(state, y))
        {
            move_first_non_clear_line_to(state, y);
        }
    }
}

Tetromino* get_rotated_current_tetromino(State *state)
{
    Tetromino* tetromino = &TETROMINOS[state->tetromino_type];
    Tetromino* rotated_tetromino = rotate(tetromino, state->tetromino_rotation);
    return rotated_tetromino;
}

TetrominoBounds get_current_tetromino_bounds(State *state)
{
    Tetromino* tetromino = get_rotated_current_tetromino(state);
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

bool tetromino_is_within_playfield_bounds(State *state)
{
    TetrominoBounds b = get_current_tetromino_bounds(state);
    return state->tetromino_x + (b.left - 1) >= -1 &&
           state->tetromino_x + (b.right - 1) < 9;
}

bool tetromino_intersects_playfield(State *state)
{
    Tetromino* tetromino = get_rotated_current_tetromino(state);

    uint8_t x, y;
    int8_t playfield_y;

    for (x=0;x<TETROMINO_SIZE;x++)
    {
        for (y=0;y<TETROMINO_SIZE;y++)
        {
            playfield_y = state->tetromino_y + y;

            if (playfield_y < 0)
            {
                continue;
            }

            if ((*tetromino)[y][x] != ' ' && state->playfield[playfield_y][state->tetromino_x + x] != ' ')
            {
                return true;
            }
        }
    }

    return false;
}

bool tetromino_should_assimilate(State *state)
{
    TetrominoBounds bounds = get_current_tetromino_bounds(state);

    if (state->tetromino_y >= PLAYFIELD_HEIGHT - (bounds.bottom + 1))
    {
        return true;
    }

    state->tetromino_y++;

    const bool should_assimilate = tetromino_intersects_playfield(state); 

    state->tetromino_y--;

    return should_assimilate;
}

void assimilate_current_tetromino(State *state)
{
    Tetromino* tetromino = get_rotated_current_tetromino(state);

    uint8_t x,y;

    for (x=0;x<TETROMINO_SIZE;x++)
    {
        for (y=0;y<TETROMINO_SIZE;y++)
        {
            if ((*tetromino)[y][x] != ' ')
            {
                state->playfield[state->tetromino_y + y][state->tetromino_x + x] = (*tetromino)[y][x];
            }
        }
    }
}

void clear_playfield_canvas_area(State *state)
{
    int8_t x,y;

    for (x=0;x<PLAYFIELD_WIDTH;x++)
    {
        for (y=0;y<PLAYFIELD_HEIGHT;y++)
        {
            draw_cell(state->canvas, (x*2) + PLAYFIELD_X, y + PLAYFIELD_Y, true);
        }
    }
}

void clear_current_tetromino_canvas_area(State *state)
{
    int8_t i, j, y_offset_in_playfield, canvas_x, canvas_y;

    for (i = 0; i < TETROMINO_SIZE; i++)
    {
        for (j = 0; j < TETROMINO_SIZE; j++)
        {
            y_offset_in_playfield = state->tetromino_y + i;
            canvas_x = ((state->tetromino_x + j) * 2) + PLAYFIELD_X;
            canvas_y = y_offset_in_playfield + PLAYFIELD_Y;

            draw_cell(state->canvas, canvas_x, canvas_y, true);
        }
    }
}

void draw_canvas_to_screen(State *state)
{
    for (uint8_t y = 0; y < CANVAS_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < CANVAS_WIDTH; x++)
        {
            if (state->canvas[y][x] != state->prev_canvas[y][x])
            {
                mvaddch(y, x, state->canvas[y][x]);
            }
        }
    }
}

void draw_playfield_to_canvas(State *state)
{
    uint8_t x,y;

    for (x=0;x<PLAYFIELD_WIDTH;x++)
    {
        for (y=0;y<PLAYFIELD_HEIGHT;y++)
        {
            if (state->playfield[y][x] == ' ')
            {
                continue;
            }

            const uint8_t canvas_x = (x*2) + PLAYFIELD_X;
            const uint8_t canvas_y = y + PLAYFIELD_Y;

            draw_cell(state->canvas, canvas_x, canvas_y, false);
        }
    }
}

void copy_canvas_to_prev_canvas(State *state)
{
    for (uint8_t x=0;x<CANVAS_WIDTH;x++)
    {
        for (uint8_t y=0;y<CANVAS_HEIGHT;y++)
        {
            state->prev_canvas[y][x] = state->canvas[y][x];
        }
    }
}

void draw_border_to_canvas(State *state)
{
    for (uint8_t y = 0; y < CANVAS_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < CANVAS_WIDTH; x++)
        {
            if (y > 0 && y < (CANVAS_HEIGHT - 1) && x != 0 && x != CANVAS_WIDTH - 1)
            {
                continue;
            }

            state->canvas[y][x] = '#';
        }
    }
}

void draw_playfield_border_to_canvas(State *state)
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

            state->canvas[y][x] = '!';
        }
    }
}

void draw_next_tetromino_to_canvas(State *state)
{
    Tetromino* t = &TETROMINOS[state->next_tetromino_type];

    uint8_t x,y;

    for (x=0;x<TETROMINO_SIZE;x++)
    {
        for (y=0;y<TETROMINO_SIZE;y++)
        {
            const char cell = (*t)[y][x];
            draw_cell(state->canvas, 10+(x*2), 10+y, cell == ' ');
        }
    }
}

void draw_tetromino_to_canvas(State *state)
{
    Tetromino* rotated_tetromino = get_rotated_current_tetromino(state);

    uint8_t i, j;
    int8_t y_offset_in_playfield;

    for (i = 0; i < TETROMINO_SIZE; i++)
    {
        for (j = 0; j < TETROMINO_SIZE; j++)
        {
            y_offset_in_playfield = state->tetromino_y + i;

            const char cell = (*rotated_tetromino)[i][j];

            if (cell != ' ' && y_offset_in_playfield >= 0)
            {
                const uint8_t x = ((state->tetromino_x + j) * 2) + PLAYFIELD_X;
                const uint8_t y = y_offset_in_playfield + PLAYFIELD_Y;
                draw_cell(state->canvas, x, y, false);
            }
        }
    }

    free(rotated_tetromino);
}

void clear_completed_lines(State *state)
{
    uint8_t x,y;

    bool lines_were_cleared = false;

    for (y=0;y<PLAYFIELD_HEIGHT;y++)
    {
        bool line_is_complete = true;

        for (x=0;x<PLAYFIELD_WIDTH;x++)
        {
            if (state->playfield[y][x] == ' ')
            {
                line_is_complete = false;
                break;
            }
        }

        if (line_is_complete)
        {
            clear_line(state, y);
            lines_were_cleared = true;
            ding();
        }
    }

    if (lines_were_cleared)
    {
        clear_playfield_canvas_area(state);
        consolidate_playfield(state);
    }
}

void drop_tetromino(State *state)
{
    if (!tetromino_should_assimilate(state))
    {
        clear_current_tetromino_canvas_area(state);
        state->tetromino_y++;
        return;
    }

    assimilate_current_tetromino(state);

    clear_completed_lines(state);

    state->tetromino_y = -1;
    state->tetromino_x = 3;
    state->tetromino_rotation = 0;
    state->tetromino_type = state->next_tetromino_type;
    state->next_tetromino_type = pick_random_tetromino_type();
}

void handle_rotate(State *state, const bool clockwise)
{
    clear_current_tetromino_canvas_area(state);

    const uint8_t old_rotation = state->tetromino_rotation;

    if (clockwise)
    {
        state->tetromino_rotation++;
    }
    else
    {
        state->tetromino_rotation--;
    }

    uint8_t number_of_configurations;

    switch (state->tetromino_type)
    {
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

    if (clockwise && state->tetromino_rotation == number_of_configurations)
    {
        state->tetromino_rotation = 0;
    }
    else if (state->tetromino_rotation == -1)
    {
        state->tetromino_rotation = number_of_configurations - 1;
    }

    if (!tetromino_is_within_playfield_bounds(state) || tetromino_intersects_playfield(state))
    {
        state->tetromino_rotation = old_rotation;
    }
}

void handle_left_right(State *state, const bool left)
{
    clear_current_tetromino_canvas_area(state);
    const uint8_t increment = left ? -1 : 1;
    state->tetromino_x += increment;

    if (!tetromino_is_within_playfield_bounds(state) || tetromino_intersects_playfield(state))
    {
        state->tetromino_x -= increment;
    }
}

void process_kb(State *state)
{
    const int ch = getch();

    if (ch == 'x' || ch == 's')
    {
        handle_rotate(state, ch == 'x');
    }
    else if (ch == KEY_LEFT || ch == KEY_RIGHT)
    {
        handle_left_right(state, ch == KEY_LEFT);
    }
    else if (ch == KEY_DOWN)
    {
        drop_tetromino(state);
    }
    else if (ch == 'q')
    {
        state->user_has_requested_exit = true;
    }
}

void drop_tetromino_if_enough_time_has_passed(State *state, const uint32_t time_at_start_of_main_loop)
{
    if (time_at_start_of_main_loop - state->tetromino_drop_timer >= state->drop_interval)
    {
        drop_tetromino(state);
        state->tetromino_drop_timer = time_at_start_of_main_loop;
    }
}

int main()
{
    initscr();
    cbreak();
    noecho();
    timeout(0);
    keypad(stdscr, TRUE);
    curs_set(0);

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    ma_device device;

    init_audio(&config, &device);
    
    State state;

    init_state(&state);

    while (1)
    {
        const uint32_t current_time = get_current_time_microseconds();

        process_kb(&state);

        if (state.user_has_requested_exit)
        {
            break;
        }

        drop_tetromino_if_enough_time_has_passed(&state, current_time);

        draw_border_to_canvas(&state);
        draw_playfield_border_to_canvas(&state);
        draw_playfield_to_canvas(&state);
        draw_tetromino_to_canvas(&state);
        draw_next_tetromino_to_canvas(&state);

        draw_canvas_to_screen(&state);

        copy_canvas_to_prev_canvas(&state);

        napms(10);
    }

    free_array(&state.canvas, CANVAS_HEIGHT);
    free_array(&state.prev_canvas, CANVAS_HEIGHT);
    free_array(&state.playfield, PLAYFIELD_HEIGHT);

    endwin();
    return 0;
}

