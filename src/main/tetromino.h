#ifndef TETRIZ_TETROMINO_H
#define TETRIZ_TETROMINO_H

#include <stdint.h>
#include <stdlib.h>

/* Total number of different tetromino types. */
const uint8_t TETROMINO_TYPE_COUNT = 7;

/* Fixed width and height for tetrominos to simplify memory management. */
const uint8_t TETROMINO_SIZE_CELLS = 4;

/* Maximum number of rotation variants for any tetromino type (0, 90, 180, and 270 degrees). */
const uint8_t TETROMINO_MAX_ROTATION_VARIANT_COUNT = 4;

/*
 * Horizontal and vertical coordinate of a tetromino's rotational pivot within
 * its cell layout (0-based index).
 */
const uint8_t TETROMINO_PIVOT_CELLS = 1;

typedef enum { 
    TETROMINO_I,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_L,
    TETROMINO_J,
    TETROMINO_S,
    TETROMINO_Z
} TetrominoType;

/*
 * All possible rotation variants of a tetromino.
 *
 * Note that:
 * 1. The "o" tetromino does not rotate, so it's always rotated to 0 degrees.
 * 2. The "i", "s" and "z" can only be rotated to 0 or 90 degrees.
 * 3. The "t", "l" and "j" can be rotated to 0, 90, 180 or 270 degrees.
 */
typedef enum {
    ROTATED_0_DEGREES,
    ROTATED_90_DEGREES,
    ROTATED_180_DEGREES,
    ROTATED_270_DEGREES
} TetrominoRotation;

/*
 * 4x4 grid of cells. Each cell can be empty (represented by a space character)
 * or non-empty (containing the letter of the tetromino type). While a boolean 
 * could model a single cell, this representation may aid in debugging.
 */
typedef char TetrominoCellLayout[TETROMINO_SIZE_CELLS][TETROMINO_SIZE_CELLS];

/*
 * Our star of the show! A tetromino is a single piece that falls from the top
 * of the Matrix (the space where the game is played, referred to as "playfield"
 * in this codebase.
 *
 * A tetromino is a geometric shape in the same family of shapes as a domino.
 * This concept predates the original Tetris game, and is independent
 * of it.
 *
 * A Tetromino instance is immutable once created.
 */
typedef struct {
    const TetrominoType t_type;
    const TetrominoCellLayout *const cell_layout;
    /*
     * The number of unique rotation variants, which is constant for a given
     * tetromino type.
     */
    const uint8_t rotation_variant_count;
} Tetromino;

/* Cell layout for each tetromino type at 0 degrees rotation. */
const TetrominoCellLayout TETROMINO_CELL_LAYOUTS[TETROMINO_TYPE_COUNT] =
{
    {
        ' ',' ',' ',' ',
        'I','I','I','I',
        ' ',' ',' ',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        ' ','O','O',' ',
        ' ','O','O',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        'T','T','T',' ',
        ' ','T',' ',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        'L','L','L',' ',
        'L',' ',' ',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        'J','J','J',' ',
        ' ',' ','J',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        ' ','S','S',' ',
        'S','S',' ',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        'Z','Z',' ',' ',
        ' ','Z','Z',' ',
        ' ',' ',' ',' '
    }
};

/*
 * Array of Tetromino instances for each tetromino type, providing 
 * convenient access to their properties and layouts.
 */
const Tetromino TETROMINOS[TETROMINO_TYPE_COUNT] = {
    { TETROMINO_I, &TETROMINO_CELL_LAYOUTS[TETROMINO_I], 2 },
    { TETROMINO_O, &TETROMINO_CELL_LAYOUTS[TETROMINO_O], 1 },
    { TETROMINO_T, &TETROMINO_CELL_LAYOUTS[TETROMINO_T], 4 },
    { TETROMINO_L, &TETROMINO_CELL_LAYOUTS[TETROMINO_L], 4 },
    { TETROMINO_J, &TETROMINO_CELL_LAYOUTS[TETROMINO_J], 4 },
    { TETROMINO_S, &TETROMINO_CELL_LAYOUTS[TETROMINO_S], 2 },
    { TETROMINO_Z, &TETROMINO_CELL_LAYOUTS[TETROMINO_Z], 2 }
};

/*
 * Encapsulates the bounds of a tetromino's cell layout at a given rotation.
 *
 * Used in gameplay logic, such as determining whether a tetromino
 * can move, or needs to be assimilated with the playfield.
 */
typedef struct {
   int8_t left; 
   int8_t right; 
   int8_t top; 
   int8_t bottom; 
} TetrominoBounds;

/*
 * Rotates the given tetromino's cell layout to the specified rotation.
 *
 * The input tetromino is always provided in its unrotated state.
 *
 * Parameters:
 * - input_tetromino: Pointer to the original Tetromino that is to be rotated.
 * - desired_rotation: The rotation to apply (0, 90, 180, or 270 degrees).
 * - out_rotated_layout: Pointer to a TetrominoCellLayout where the rotated
 *   layout will be stored. This layout is filled in by the function.
 */
void rotate(const Tetromino *input_tetromino, const TetrominoRotation desired_rotation, TetrominoCellLayout *out_rotated_layout)
{
    const TetrominoRotation rotation_variant = desired_rotation % input_tetromino->rotation_variant_count;

    for (uint8_t y = 0; y < TETROMINO_SIZE_CELLS; ++y)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE_CELLS; ++x)
        {
            uint8_t new_y, new_x;

            switch (rotation_variant)
            {
                case ROTATED_0_DEGREES:
                    new_y = y;
                    new_x = x;
                    break;
                case ROTATED_90_DEGREES:
                    new_y = (2 * TETROMINO_PIVOT_CELLS - x + TETROMINO_SIZE_CELLS) % TETROMINO_SIZE_CELLS;
                    new_x = (y + TETROMINO_SIZE_CELLS) % TETROMINO_SIZE_CELLS;
                    break;
                case ROTATED_180_DEGREES:
                    new_y = (2 * TETROMINO_PIVOT_CELLS - y + TETROMINO_SIZE_CELLS) % TETROMINO_SIZE_CELLS;
                    new_x = (2 * TETROMINO_PIVOT_CELLS - x + TETROMINO_SIZE_CELLS) % TETROMINO_SIZE_CELLS;
                    break;
                case ROTATED_270_DEGREES:
                    new_y = (x + TETROMINO_SIZE_CELLS) % TETROMINO_SIZE_CELLS;
                    new_x = (2 * TETROMINO_PIVOT_CELLS - y + TETROMINO_SIZE_CELLS) % TETROMINO_SIZE_CELLS;
                    break;
            }

            const char new_cell = (*input_tetromino->cell_layout)[new_y][new_x];

            (*out_rotated_layout)[y][x] = new_cell;
        }
    }
}

/*
 * Implements an unbiased randomizer for selecting a tetromino type,
 * based on the "Truly Pseudo Random" method described at
 * https://simon.lc/the-history-of-tetris-randomizers. This randomizer
 * is used in the original version of Tetris, which this codebase aims to clone.
 *
 * I'm not confident that this implementation accurately replicates the
 * original. Suggestions for improvements are welcome.
 */
TetrominoType pick_random_tetromino_type()
{
    const double random_value = (double)rand() / RAND_MAX;
    const int result = (int)(random_value * TETROMINO_TYPE_COUNT);
    return (TetrominoType)(result);
}

#endif // TETRIZ_TETROMINO_H
