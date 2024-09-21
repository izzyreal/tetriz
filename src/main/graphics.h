#include "state.h"
#include "constants.h"

#include <ncurses.h>

void clear_line(State *state, uint8_t y)
{
    memset(state->playfield[y], ' ', PLAYFIELD_WIDTH);
}

bool is_line_clear(State *state, uint8_t y)
{
    for (uint8_t x = 0; x < PLAYFIELD_WIDTH; ++x)
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
    for (int8_t src_index = dest_index - 1; src_index >= 0; --src_index)
    {
        if (!is_line_clear(state, src_index))
        {
            for (uint8_t x = 0; x < PLAYFIELD_WIDTH; ++x)
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
    for (int8_t y = PLAYFIELD_HEIGHT - 1; y >= 0; --y)
    {
        if (is_line_clear(state, y))
        {
            move_first_non_clear_line_to(state, y);
        }
    }
}

void clear_playfield_canvas_area(State *state)
{
    for (uint8_t x = 0; x < PLAYFIELD_WIDTH; ++x)
    {
        for (uint8_t y = 0; y < PLAYFIELD_HEIGHT; ++y)
        {
            draw_cell(state->canvas, (x*2) + PLAYFIELD_X, y + PLAYFIELD_Y, true);
        }
    }
}

void clear_current_tetromino_canvas_area(State *state)
{
    for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
        {
            const uint8_t y_offset_in_playfield = state->tetromino_y + y;
            const uint8_t canvas_x = ((state->tetromino_x + x) * 2) + PLAYFIELD_X;
            const uint8_t canvas_y = y_offset_in_playfield + PLAYFIELD_Y;

            draw_cell(state->canvas, canvas_x, canvas_y, true);
        }
    }
}

void draw_canvas_to_screen(State *state)
{
    for (uint8_t y = 0; y < CANVAS_HEIGHT; ++y)
    {
        for (uint8_t x = 0; x < CANVAS_WIDTH; ++x)
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
    for (uint8_t x = 0; x < PLAYFIELD_WIDTH; ++x)
    {
        for (uint8_t y = 0; y < PLAYFIELD_HEIGHT; ++y)
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
    for (uint8_t x = 0; x < CANVAS_WIDTH; ++x)
    {
        for (uint8_t y = 0; y < CANVAS_HEIGHT; ++y)
        {
            state->prev_canvas[y][x] = state->canvas[y][x];
        }
    }
}

void draw_border_to_canvas(State *state)
{
    for (uint8_t y = 0; y < CANVAS_HEIGHT; ++y)
    {
        for (uint8_t x = 0; x < CANVAS_WIDTH; ++x)
        {
            if (y > 0 && y < CANVAS_HEIGHT - 1 && x != 0 && x != CANVAS_WIDTH - 1)
            {
                continue;
            }

            state->canvas[y][x] = '#';
        }
    }
}

void draw_playfield_border_to_canvas(State *state)
{
    for (uint8_t y = PLAYFIELD_Y; y < PLAYFIELD_Y + PLAYFIELD_HEIGHT + 1; ++y)
    {
        for (uint8_t x = PLAYFIELD_X - 1; x < PLAYFIELD_X + (PLAYFIELD_WIDTH * 2) + 1; ++x)
        {
            if (y > PLAYFIELD_Y - 1 &&
                y < PLAYFIELD_Y + PLAYFIELD_HEIGHT &&
                x != PLAYFIELD_X - 1 &&
                x != PLAYFIELD_X + PLAYFIELD_WIDTH * 2)
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

    for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
        {
            const char cell = (*t->cell_layout)[y][x];
            draw_cell(state->canvas, 10 + (x * 2), 10 + y, cell == ' ');
        }
    }
}

void draw_tetromino_to_canvas(State *state, TetrominoCellLayout *tetromino)
{
    for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
        {
            const int8_t y_offset_in_playfield = state->tetromino_y + y;

            if (y_offset_in_playfield < 0)
            {
                continue;
            }

            const char cell = (*tetromino)[y][x];

            if (cell != ' ')
            {
                const uint8_t canvas_x = ((state->tetromino_x + x) * 2) + PLAYFIELD_X;
                const uint8_t canvas_y = y_offset_in_playfield + PLAYFIELD_Y;
                draw_cell(state->canvas, canvas_x, canvas_y, false);
            }
        }
    }
}

void clear_completed_lines(State *state)
{
    bool lines_were_cleared = false;

    for (uint8_t y = 0; y < PLAYFIELD_HEIGHT; ++y)
    {
        bool line_is_complete = true;

        for (uint8_t x = 0; x < PLAYFIELD_WIDTH; ++x)
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

