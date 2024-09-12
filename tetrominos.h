#include <stdbool.h>

typedef enum { 
    TETROMINO_I = 1,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_L,
    TETROMINO_J,
    TETROMINO_S,
    TETROMINO_Z,
    TETROMINO_COUNT
} TETROMINO_TYPE;

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

