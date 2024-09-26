#include "state.h"
#include "constants.h"

#include <ncurses.h>

void clear_line(State *const state, const uint8_t y)
{
    memset(state->playfield[y], ' ', PLAYFIELD_WIDTH_CELLS);
}

bool is_line_clear(const State *const state, const uint8_t y)
{
    for (uint8_t x = 0; x < PLAYFIELD_WIDTH_CELLS; ++x)
    {
        if (state->playfield[y][x] != ' ')
        {
            return false;
        }
    }

    return true;
}

void move_first_non_clear_line_to(State *const state, const uint8_t dest_index)
{
    for (int8_t src_index = dest_index - 1; src_index >= 0; --src_index)
    {
        if (!is_line_clear(state, src_index))
        {
            for (uint8_t x = 0; x < PLAYFIELD_WIDTH_CELLS; ++x)
            {
                state->playfield[dest_index][x] = state->playfield[src_index][x];
            }

            clear_line(state, src_index);
            break;
        }
    }
}

void consolidate_playfield(State *const state)
{
    for (int8_t y = PLAYFIELD_HEIGHT_CELLS - 1; y >= 0; --y)
    {
        if (is_line_clear(state, y))
        {
            move_first_non_clear_line_to(state, y);
        }
    }
}

void clear_playfield_in_canvas(State *const state)
{
    for (uint8_t x = 0; x < PLAYFIELD_WIDTH_CHARS; x += CELL_WIDTH_CHARS)
    {
        for (uint8_t y = 0; y < PLAYFIELD_HEIGHT_CHARS; y += CELL_HEIGHT_CHARS)
        {
            draw_empty_playfield_cell(state->canvas, x + PLAYFIELD_X_CHARS, y + PLAYFIELD_Y_CHARS);
        }
    }
}

void clear_current_tetromino_canvas_area(State *const state)
{
    for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
        {
            const int8_t x_offset_in_playfield = state->tetromino_x_cells + x;
            const int8_t y_offset_in_playfield = state->tetromino_y_cells + y;

            if (x_offset_in_playfield < 0 ||
                x_offset_in_playfield >= PLAYFIELD_WIDTH_CELLS ||
                y_offset_in_playfield < 0 ||
                y_offset_in_playfield >= PLAYFIELD_HEIGHT_CELLS)
            {
                continue;
            }

            const uint8_t canvas_x = (x_offset_in_playfield * CELL_WIDTH_CHARS) + PLAYFIELD_X_CHARS;
            const uint8_t canvas_y = y_offset_in_playfield + PLAYFIELD_Y_CHARS;

            draw_empty_playfield_cell(state->canvas, canvas_x, canvas_y);
        }
    }
}

void draw_canvas_to_screen(State *const state)
{
    for (uint8_t y = 0; y < CANVAS_HEIGHT_CHARS; ++y)
    {
        for (uint8_t x = 0; x < CANVAS_WIDTH_CHARS; ++x)
        {
            if (state->canvas[y][x] != state->prev_canvas[y][x])
            {
                mvaddch(y, x, state->canvas[y][x]);
            }
        }
    }
}

void draw_playfield_to_canvas(State *const state)
{
    for (uint8_t x = 0; x < PLAYFIELD_WIDTH_CELLS; ++x)
    {
        for (uint8_t y = 0; y < PLAYFIELD_HEIGHT_CELLS; ++y)
        {
            if (state->playfield[y][x] == ' ')
            {
                continue;
            }

            const uint8_t canvas_x = (x * CELL_WIDTH_CHARS) + PLAYFIELD_X_CHARS;
            const uint8_t canvas_y = (y * CELL_HEIGHT_CHARS) + PLAYFIELD_Y_CHARS;

            draw_non_empty_cell(state->canvas, canvas_x, canvas_y);
        }
    }
}

void copy_canvas_to_prev_canvas(State *const state)
{
    for (uint8_t x = 0; x < CANVAS_WIDTH_CHARS; ++x)
    {
        for (uint8_t y = 0; y < CANVAS_HEIGHT_CHARS; ++y)
        {
            state->prev_canvas[y][x] = state->canvas[y][x];
        }
    }
}

void draw_playfield_border_to_canvas(State *const state)
{
    char **const canvas = state->canvas;

    /*
     * The original 1985 Tetris border has a left, right and bottom edge.
     * The thickness of each edge is 2 characters. Below, we refer to the outer
     * columns/rows of these edges as border1, and the inner columns/rows as
     * border2.
     */
    const uint8_t left_border1_x = PLAYFIELD_X_CHARS - 2;
    const uint8_t left_border2_x = PLAYFIELD_X_CHARS - 1;
    const uint8_t right_border2_x = PLAYFIELD_X_CHARS + PLAYFIELD_WIDTH_CHARS;
    const uint8_t right_border1_x = PLAYFIELD_X_CHARS + PLAYFIELD_WIDTH_CHARS + 1;

    const uint8_t left_and_right_border_top = PLAYFIELD_Y_CHARS;
    const uint8_t left_and_right_border_bottom = PLAYFIELD_Y_CHARS + PLAYFIELD_HEIGHT_CHARS;

    for (uint8_t y = left_and_right_border_top;
         y <= left_and_right_border_bottom;
         y++)
    {
        canvas[y][left_border1_x] = '<';
        canvas[y][left_border2_x] = '!';
        canvas[y][right_border2_x] = '!';
        canvas[y][right_border1_x] = '>';
    }

    const uint8_t bottom_border2_y = PLAYFIELD_Y_CHARS + PLAYFIELD_HEIGHT_CHARS;
    const uint8_t bottom_border1_y = PLAYFIELD_Y_CHARS + PLAYFIELD_HEIGHT_CHARS + 1;
    const uint8_t bottom_border_left = PLAYFIELD_X_CHARS;
    const uint8_t bottom_border_right = PLAYFIELD_X_CHARS + PLAYFIELD_WIDTH_CHARS;
    
    for (uint8_t x = bottom_border_left; x < bottom_border_right; x++)
    {
        canvas[bottom_border2_y][x] = '=';
        canvas[bottom_border1_y][x] = (x % 2 == 0) ? '\\' : '/';
    }
}

void draw_next_tetromino_to_canvas(State *const state)
{
    const Tetromino *t = &TETROMINOS[state->next_tetromino_type];

    for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
        {
            const char cell_is_empty = (*t->cell_layout)[y][x] == ' ';
            const uint8_t canvas_x = NEXT_TETROMINO_X_CHARS + (x * CELL_WIDTH_CHARS);
            const uint8_t canvas_y = NEXT_TETROMINO_Y_CHARS + y;

            if (cell_is_empty)
            {
                draw_empty_cell(state->canvas, canvas_x, canvas_y);
            }
            else
            {
                draw_non_empty_cell(state->canvas, canvas_x, canvas_y);
            }
        }
    }
}

void draw_tetromino_to_canvas(State *const state, TetrominoCellLayout *const tetromino)
{
    for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
        {
            const int8_t y_offset_in_playfield = state->tetromino_y_cells + y;

            if (y_offset_in_playfield < 0)
            {
                continue;
            }

            const char cell_is_empty = (*tetromino)[y][x] == ' ';

            if (cell_is_empty)
            {
                continue;
            }

            const uint8_t canvas_x = ((state->tetromino_x_cells + x) *
                    CELL_WIDTH_CHARS) + PLAYFIELD_X_CHARS;
            const uint8_t canvas_y = y_offset_in_playfield + PLAYFIELD_Y_CHARS;

            draw_non_empty_cell(state->canvas, canvas_x, canvas_y);
        }
    }
}

