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

void clear_playfield_canvas_area(State *const state)
{
    for (uint8_t x = 0; x < PLAYFIELD_WIDTH_CHARS; x += CELL_WIDTH_CHARS)
    {
        for (uint8_t y = 0; y < PLAYFIELD_HEIGHT_CHARS; y += CELL_HEIGHT_CHARS)
        {
            draw_cell(state->canvas, x + PLAYFIELD_X_CHARS, y + PLAYFIELD_Y_CHARS, true);
        }
    }
}

void clear_current_tetromino_canvas_area(State *const state)
{
    for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
        {
            const uint8_t y_offset_in_playfield = state->tetromino_y + y;
            const uint8_t canvas_x = ((state->tetromino_x + x) * CELL_WIDTH_CHARS) + PLAYFIELD_X_CHARS;
            const uint8_t canvas_y = y_offset_in_playfield + PLAYFIELD_Y_CHARS;

            draw_cell(state->canvas, canvas_x, canvas_y, true);
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

            draw_cell(state->canvas, canvas_x, canvas_y, false);
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

void draw_border_to_canvas(State *const state)
{
    for (uint8_t y = 0; y < CANVAS_HEIGHT_CHARS; ++y)
    {
        for (uint8_t x = 0; x < CANVAS_WIDTH_CHARS; ++x)
        {
            if (y > 0 && y < CANVAS_HEIGHT_CHARS - 1 && x != 0 && x != CANVAS_WIDTH_CHARS - 1)
            {
                continue;
            }

            state->canvas[y][x] = '#';
        }
    }
}

void draw_playfield_border_to_canvas(State *const state)
{
    for (uint8_t y = PLAYFIELD_Y_CHARS; y < PLAYFIELD_Y_CHARS + PLAYFIELD_HEIGHT_CHARS + 1; ++y)
    {
        for (uint8_t x = PLAYFIELD_X_CHARS - 1; x < PLAYFIELD_X_CHARS + PLAYFIELD_WIDTH_CHARS + 1; ++x)
        {
            if (y > PLAYFIELD_Y_CHARS - 1 &&
                y < PLAYFIELD_Y_CHARS + PLAYFIELD_HEIGHT_CHARS &&
                x != PLAYFIELD_X_CHARS - 1 &&
                x != PLAYFIELD_X_CHARS + PLAYFIELD_WIDTH_CHARS)
            {
                continue;
            }

            state->canvas[y][x] = '!';
        }
    }
}

void draw_next_tetromino_to_canvas(State *const state)
{
    const Tetromino *t = &TETROMINOS[state->next_tetromino_type];

    for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
        {
            const char cell = (*t->cell_layout)[y][x];
            draw_cell(state->canvas, NEXT_TETROMINO_X_CHARS + (x * CELL_WIDTH_CHARS), NEXT_TETROMINO_Y_CHARS + y, cell == ' ');
        }
    }
}

void draw_tetromino_to_canvas(State *const state, TetrominoCellLayout *const tetromino)
{
    for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
        {
            const int8_t y_offset_in_playfield = state->tetromino_y + y;

            if (y_offset_in_playfield < 0)
            {
                continue;
            }

            const char cell = (*tetromino)[y][x];

            if (cell != ' ')
            {
                const uint8_t canvas_x = ((state->tetromino_x + x) * CELL_WIDTH_CHARS) + PLAYFIELD_X_CHARS;
                const uint8_t canvas_y = y_offset_in_playfield + PLAYFIELD_Y_CHARS;
                draw_cell(state->canvas, canvas_x, canvas_y, false);
            }
        }
    }
}

void clear_completed_lines(State *const state)
{
    bool lines_were_cleared = false;

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

