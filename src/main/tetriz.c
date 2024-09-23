#include "tetromino.h"
#include "constants.h"
#include "state.h"
#include "util.h"
#include "audio.h"
#include "graphics.h"

#include <stdint.h>
#include <ncurses.h>

void init_state(State *const state)
{
    init_array(&state->canvas, CANVAS_HEIGHT, CANVAS_WIDTH);
    init_array(&state->prev_canvas, CANVAS_HEIGHT, CANVAS_WIDTH);
    init_array(&state->playfield, PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH);

    state->user_has_requested_exit = false;
    state->tetromino_type = pick_random_tetromino_type();
    state->next_tetromino_type = pick_random_tetromino_type();
    state->tetromino_x = 3;
    state->tetromino_y = -1;
    state->tetromino_rotation = ROTATED_0_DEGREES;
    state->drop_interval = 700000;
    state->tetromino_drop_timer = get_current_time_microseconds();
}

TetrominoBounds get_current_tetromino_bounds(State *const state)
{
    TetrominoCellLayout tetromino;
    rotate(&TETROMINOS[state->tetromino_type], state->tetromino_rotation, &tetromino);
    
    TetrominoBounds bounds;

    for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
        {
            const char cell = tetromino[y][x];

            if (cell == ' ') continue;

            if (x < bounds.left) bounds.left = x;
            if (x > bounds.right) bounds.right = x;
            if (y < bounds.top) bounds.top = y;
            if (y > bounds.bottom) bounds.bottom = y;
        }
    }

    return bounds;
}

bool tetromino_is_within_playfield_bounds(State *const state)
{
    const TetrominoBounds b = get_current_tetromino_bounds(state);
    return state->tetromino_x + (b.left - 1) >= -1 &&
           state->tetromino_x + (b.right - 1) < 9;
}

bool tetromino_intersects_playfield(State *const state)
{
    TetrominoCellLayout tetromino;
    rotate(&TETROMINOS[state->tetromino_type], state->tetromino_rotation, &tetromino);

    for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
        {
            const int8_t playfield_y = state->tetromino_y + y;

            if (playfield_y < 0)
            {
                continue;
            }

            if (tetromino[y][x] != ' ' && state->playfield[playfield_y][state->tetromino_x + x] != ' ')
            {
                return true;
            }
        }
    }

    return false;
}

bool tetromino_should_assimilate(State *const state)
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

void assimilate_current_tetromino(State *const state)
{
    TetrominoCellLayout tetromino;
    rotate(&TETROMINOS[state->tetromino_type], state->tetromino_rotation, &tetromino);

    for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
        {
            const char cell = tetromino[y][x];

            if (cell != ' ')
            {
                state->playfield[state->tetromino_y + y][state->tetromino_x + x] = cell;
            }
        }
    }
}

void drop_tetromino(State *const state)
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
    state->tetromino_rotation = ROTATED_0_DEGREES;
    state->tetromino_type = state->next_tetromino_type;
    state->next_tetromino_type = pick_random_tetromino_type();
}

void handle_rotate(State *const state, const bool clockwise)
{
    clear_current_tetromino_canvas_area(state);

    const TetrominoRotation old_rotation = state->tetromino_rotation;

    const int8_t increment = clockwise ? 1 : -1;

    state->tetromino_rotation = (state->tetromino_rotation + increment) % TETROMINO_MAX_ROTATION_VARIANT_COUNT;

    if (!tetromino_is_within_playfield_bounds(state) || tetromino_intersects_playfield(state))
    {
        state->tetromino_rotation = old_rotation;
    }
}

void handle_left_right(State *const state, const bool left)
{
    clear_current_tetromino_canvas_area(state);
    const uint8_t increment = left ? -1 : 1;
    state->tetromino_x += increment;

    if (!tetromino_is_within_playfield_bounds(state) || tetromino_intersects_playfield(state))
    {
        state->tetromino_x -= increment;
    }
}

void process_kb(State *const state)
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

void drop_tetromino_if_enough_time_has_passed(State *const state, const uint32_t time_at_start_of_main_loop)
{
    if (time_at_start_of_main_loop - state->tetromino_drop_timer >= state->drop_interval)
    {
        drop_tetromino(state);
        state->tetromino_drop_timer = time_at_start_of_main_loop;
    }
}

int main()
{
    srand(time(NULL));

    /*
     * This seems to be necessary to avoid always getting the O tetromino first.
     * Let me know if you have suggestions to avoid this, and how to improve the
     * randomizer implementation in tetromino.h.
     */
    rand();

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
        
        TetrominoCellLayout tetromino;
        rotate(&TETROMINOS[state.tetromino_type], state.tetromino_rotation, &tetromino);

        draw_tetromino_to_canvas(&state, &tetromino);
       
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

