#ifndef TETRIZ_TETROMINO_H
#define TETRIZ_TETROMINO_H

#include <stdint.h>
#include <stdlib.h>

const uint8_t TETROMINO_COUNT = 7;

typedef enum { 
    TETROMINO_I = 0,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_L,
    TETROMINO_J,
    TETROMINO_S,
    TETROMINO_Z
} TetrominoType;

/* The below rotations are clockwise */
typedef enum {
    ROTATED_0_DEGREES = 0,
    ROTATED_90_DEGREES,
    ROTATED_180_DEGREES,
    ROTATED_270_DEGREES
} TetrominoRotation;

const uint8_t TETROMINO_SIZE = 4;

typedef char TetrominoCellLayout[TETROMINO_SIZE][TETROMINO_SIZE];

typedef struct {
    const TetrominoType t_type;
    TetrominoCellLayout *const cell_layout;
    const uint8_t rotation_variant_count;
} Tetromino;

static char TETROMINO_CELL_LAYOUTS[7][TETROMINO_SIZE][TETROMINO_SIZE] =
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

static Tetromino TETROMINOS[7] = {
    { TETROMINO_I, &TETROMINO_CELL_LAYOUTS[TETROMINO_I], 2 },
    { TETROMINO_O, &TETROMINO_CELL_LAYOUTS[TETROMINO_O], 1 },
    { TETROMINO_T, &TETROMINO_CELL_LAYOUTS[TETROMINO_T], 4 },
    { TETROMINO_L, &TETROMINO_CELL_LAYOUTS[TETROMINO_L], 4 },
    { TETROMINO_J, &TETROMINO_CELL_LAYOUTS[TETROMINO_J], 4 },
    { TETROMINO_S, &TETROMINO_CELL_LAYOUTS[TETROMINO_S], 2 },
    { TETROMINO_Z, &TETROMINO_CELL_LAYOUTS[TETROMINO_Z], 2 }
};

typedef struct {
   int8_t left; 
   int8_t right; 
   int8_t top; 
   int8_t bottom; 
} TetrominoBounds;

void rotate(const Tetromino* t_unrotated, const TetrominoRotation rotation, TetrominoCellLayout* t_rotated)
{
    const TetrominoRotation rotation_variant = rotation % t_unrotated->rotation_variant_count;

    const uint8_t pivot = 1;

    for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
        {
            uint8_t new_y, new_x;

            switch (rotation_variant)
            {
                case ROTATED_0_DEGREES:
                    new_y = y;
                    new_x = x;
                    break;
                case ROTATED_90_DEGREES:
                    new_y = (2 * pivot - x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_x = (y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case ROTATED_180_DEGREES:
                    new_y = (2 * pivot - y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_x = (2 * pivot - x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case ROTATED_270_DEGREES:
                    new_y = (x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_x = (2 * pivot - y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
            }

            const char new_cell = (*t_unrotated->cell_layout)[new_y][new_x];

            (*t_rotated)[y][x] = new_cell;
        }
    }
}

TetrominoType pick_random_tetromino_type()
{
    return (TetrominoType)(rand() % TETROMINO_COUNT);
}

#endif // TETRIZ_TETROMINO_H
