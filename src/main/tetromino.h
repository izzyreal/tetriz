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

typedef char Tetromino[TETROMINO_SIZE][TETROMINO_SIZE];

static char TETROMINOS[7][TETROMINO_SIZE][TETROMINO_SIZE] =
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

typedef struct {
   int8_t left; 
   int8_t right; 
   int8_t top; 
   int8_t bottom; 
} TetrominoBounds;

void rotate(const Tetromino* t_unrotated, const TetrominoType tetromino_type, const uint8_t rotation, Tetromino* t_rotated)
{
    if (tetromino_type == TETROMINO_O)
    {
        for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
        {
            for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
            {
                (*t_rotated)[y][x] = (*t_unrotated)[y][x];
            }
        }
        return;
    }

    const uint8_t pivot = 1;

    for (uint8_t i = 0; i < TETROMINO_SIZE; i++)
    {
        for (uint8_t j = 0; j < TETROMINO_SIZE; j++)
        {
            const int8_t x = j - pivot;
            const int8_t y = i - pivot;
            uint8_t new_i, new_j;
            
            switch (rotation)
            {
                case ROTATED_0_DEGREES:
                    new_i = (pivot + y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot + x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case ROTATED_90_DEGREES:
                    new_i = (pivot - x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot + y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case ROTATED_180_DEGREES:
                    new_i = (pivot - y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot - x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case ROTATED_270_DEGREES:
                    new_i = (pivot + x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot - y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
            }

            (*t_rotated)[i][j] = (*t_unrotated)[new_i][new_j];
        }
    }
}

TetrominoType pick_random_tetromino_type()
{
    static uint32_t seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return (TetrominoType)(seed % TETROMINO_COUNT);
}

#endif // TETRIZ_TETROMINO_H
