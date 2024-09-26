#include "tetromino.h"
#include "state.h"
#include "util.h"
#include "audio.h"
#include "graphics.h"
#include "constants.h"
#include "types.h"

#include <stdint.h>
#include <ncurses.h>

uint32_t calculate_drop_interval(const uint8_t level)
{
    assert(level >= 1);
    uint32_t drop_interval = INITIAL_DROP_INTERVAL_MICROSECONDS *
                             pow(0.9, level - 1);
    return (drop_interval < MINIMUM_DROP_INTERVAL_MICROSECONDS) ?
           MINIMUM_DROP_INTERVAL_MICROSECONDS : drop_interval;
}

void increment_cleared_line_accumulator_and_maybe_level(State *const state)
{
    state->cleared_line_accumulator++;

    if (state->cleared_line_accumulator == 10)
    {
        state->cleared_line_accumulator = 0;
        state->level++;
        state->drop_interval_microseconds = calculate_drop_interval(state->level); 
    }
}

void clear_completed_lines(State *const state)
{
    uint8_t cleared_line_count = 0;

    for (uint8_t y = 0; y < PLAYFIELD_HEIGHT_CHARS; ++y)
    {
        bool line_is_complete = true;

        for (uint8_t x = 0; x < PLAYFIELD_WIDTH_CELLS; ++x)
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
            increment_cleared_line_accumulator_and_maybe_level(state);
            cleared_line_count++;

            if (cleared_line_count == 4)
            {
                /*
                 * It's impossible to clear more than 4 lines at a time,
                 * so when that has happened, we break, to avoid unnecessary
                 * iterations.
                 */
                break;
            }
       }
    }

    switch (cleared_line_count)
    {
        case 1:
            state->score += 1;
            break;
        case 2:
            state->score += 3;
            break;
        case 3:
            state->score += 5;
            break;
        case 4:
            state->score += 10;
            break;
        default:
            break;
    }

    if (cleared_line_count > 0)
    {
        clear_playfield_in_canvas(state);
        consolidate_playfield(state);
        ding();
        draw_score_to_canvas(state->canvas, state->score);
    }
}

void init_state(State *const state)
{
    init_array(&state->canvas, CANVAS_HEIGHT_CHARS, CANVAS_WIDTH_CHARS);
    init_array(&state->prev_canvas, CANVAS_HEIGHT_CHARS, CANVAS_WIDTH_CHARS);
    init_array(&state->playfield, PLAYFIELD_HEIGHT_CHARS, PLAYFIELD_WIDTH_CHARS);

    const TetrominoType current_type = pick_random_tetromino_type();

    state->user_has_requested_exit = false;
    state->tetromino_type = current_type;
    state->next_tetromino_type = pick_random_tetromino_type();
    state->tetromino_x_cells = TETROMINOS[current_type].spawn_x_pos_cells;
    state->tetromino_y_cells = -1;
    state->tetromino_rotation = ROTATED_0_DEGREES;
    state->cleared_line_accumulator = 0;
    state->level = 1;
    state->score = 0;
    state->drop_interval_microseconds = calculate_drop_interval(state->level);
    state->last_drop_timestamp_microseconds = get_current_time_microseconds();
}

bool tetromino_is_within_playfield_bounds(const State *const state)
{
    const TetrominoBounds b = get_tetromino_bounds(
        state->tetromino_type,
        state->tetromino_rotation);

    return state->tetromino_x_cells + (b.left - 1) >= -1 &&
           state->tetromino_x_cells + (b.right - 1) < 9;
}

bool tetromino_intersects_playfield(
        const TetrominoType tetromino_type,
        const TetrominoRotation rotation,
        const int8_t tetromino_x_cells,
        const int8_t tetromino_y_cells,
        char **playfield)
{
    TetrominoCellLayout tetromino;
    rotate(&TETROMINOS[tetromino_type], rotation, &tetromino);

    for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
        {
            const int8_t playfield_y_cells = tetromino_y_cells + y;

            if (playfield_y_cells < 0)
            {
                continue;
            }

            if (tetromino[y][x] != ' ')
            {
                const char playfield_cell = playfield[playfield_y_cells][tetromino_x_cells + x];

                if (playfield_cell != ' ')
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool tetromino_should_assimilate(const State *const state)
{
    const TetrominoBounds bounds = get_tetromino_bounds(
        state->tetromino_type,
        state->tetromino_rotation);

    if (state->tetromino_y_cells >= PLAYFIELD_HEIGHT_CELLS - (bounds.bottom + 1))
    {
        return true;
    }

    const bool should_assimilate = tetromino_intersects_playfield(
        state->tetromino_type,
        state->tetromino_rotation,
        state->tetromino_x_cells,
        state->tetromino_y_cells + 1,
        state->playfield);

    return should_assimilate;
}

void assimilate_current_tetromino(State *const state)
{
    TetrominoCellLayout tetromino;
    const Tetromino* unrotated_tetromino = &TETROMINOS[state->tetromino_type];
    rotate(unrotated_tetromino, state->tetromino_rotation, &tetromino);

    for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
        {
            const char cell = tetromino[y][x];

            if (cell != ' ')
            {
                state->playfield[state->tetromino_y_cells + y][state->tetromino_x_cells + x] = cell;
            }
        }
    }
}

void drop_tetromino(State *const state)
{
    if (!tetromino_should_assimilate(state))
    {
        clear_current_tetromino_canvas_area(state);
        state->tetromino_y_cells++;
        return;
    }

    assimilate_current_tetromino(state);

    clear_completed_lines(state);

    state->tetromino_type = state->next_tetromino_type;
    state->next_tetromino_type = pick_random_tetromino_type();
    state->tetromino_x_cells = TETROMINOS[state->tetromino_type].spawn_x_pos_cells;
    state->tetromino_y_cells = -1;
    state->tetromino_rotation = ROTATED_0_DEGREES;
 }

void handle_rotate(State *const state, const bool clockwise)
{
    clear_current_tetromino_canvas_area(state);

    const TetrominoRotation old_rotation = state->tetromino_rotation;

    const int8_t increment = clockwise ? 1 : -1;

    state->tetromino_rotation = (state->tetromino_rotation + increment) % TETROMINO_MAX_ROTATION_VARIANT_COUNT;

    const bool intersects_playfield = tetromino_intersects_playfield(
        state->tetromino_type,
        state->tetromino_rotation,
        state->tetromino_x_cells,
        state->tetromino_y_cells,
        state->playfield);

    if (!tetromino_is_within_playfield_bounds(state) || intersects_playfield)
    {
        state->tetromino_rotation = old_rotation;
    }
}

void handle_left_right(State *const state, const bool left)
{
    clear_current_tetromino_canvas_area(state);
    const uint8_t increment = left ? -1 : 1;
    state->tetromino_x_cells += increment;

    const bool intersects_playfield = tetromino_intersects_playfield(
        state->tetromino_type,
        state->tetromino_rotation,
        state->tetromino_x_cells,
        state->tetromino_y_cells,
        state->playfield);

    if (!tetromino_is_within_playfield_bounds(state) || intersects_playfield)
    {
        state->tetromino_x_cells -= increment;
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
    if (time_at_start_of_main_loop - state->last_drop_timestamp_microseconds >= state->drop_interval_microseconds)
    {
        drop_tetromino(state);
        state->last_drop_timestamp_microseconds = time_at_start_of_main_loop;
    }
}

void initialize_color()
{
    start_color();
    init_color(COLOR_GREEN, 3, 800, 1);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
}

void reset_color()
{
    attroff(COLOR_PAIR(1));
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

    initialize_color();

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    ma_device device;

    init_audio(&config, &device);
    
    State state;

    init_state(&state);

    clear_playfield_in_canvas(&state);

    draw_playfield_border_to_canvas(&state);
   
    draw_score_to_canvas(state.canvas, state.score);

    while (1)
    {
        const uint32_t current_time = get_current_time_microseconds();

        process_kb(&state);

        if (state.user_has_requested_exit)
        {
            break;
        }

        drop_tetromino_if_enough_time_has_passed(&state, current_time);

        draw_playfield_to_canvas(&state);
        
        TetrominoCellLayout tetromino;
        rotate(&TETROMINOS[state.tetromino_type], state.tetromino_rotation, &tetromino);

        draw_tetromino_to_canvas(&state, &tetromino);
       
        draw_next_tetromino_to_canvas(&state);

        draw_canvas_to_screen(&state);

        copy_canvas_to_prev_canvas(&state);

        napms(10);
    }

    reset_color();

    endwin();

    free_array(&state.canvas, CANVAS_HEIGHT_CHARS);
    free_array(&state.prev_canvas, CANVAS_HEIGHT_CHARS);
    free_array(&state.playfield, PLAYFIELD_HEIGHT_CHARS);

    return 0;
}

