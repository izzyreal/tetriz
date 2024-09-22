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

    for (uint8_t i = 0; i < TETROMINO_SIZE; i++)
    {
        for (uint8_t j = 0; j < TETROMINO_SIZE; j++)
        {
            uint8_t new_i, new_j;

            switch (rotation_variant)
            {
                case ROTATED_0_DEGREES:
                    new_i = i;
                    new_j = j;
                    break;
                case ROTATED_90_DEGREES:
                    new_i = (2 * pivot - j + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (i + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case ROTATED_180_DEGREES:
                    new_i = (2 * pivot - i + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (2 * pivot - j + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case ROTATED_270_DEGREES:
                    new_i = (j + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (2 * pivot - i + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
            }

            const char new_cell = (*t_unrotated->cell_layout)[new_i][new_j];

            (*t_rotated)[i][j] = new_cell;
        }
    }
}

TetrominoType pick_random_tetromino_type()
{
    return (TetrominoType)(rand() % TETROMINO_COUNT);
}

#endif // TETRIZ_TETROMINO_H
