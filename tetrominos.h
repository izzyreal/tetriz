#include <stdbool.h>

typedef enum { 
    TETROMINO_I,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_L,
    TETROMINO_J,
    TETROMINO_S,
    TETROMINO_Z
} TETROMINO_TYPE;

const uint8_t TETROMINO_SIZE = 4;

typedef const char Tetromino[TETROMINO_SIZE][TETROMINO_SIZE];

static const char TETROMINOS[7][TETROMINO_SIZE][TETROMINO_SIZE] =
{
    {
        {' ',' ',' ',' '},
        {'I','I','I','I'},
        {' ',' ',' ',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {' ','O','O',' '},
        {' ','O','O',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {'T','T','T',' '},
        {' ','T',' ',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {'L','L','L',' '},
        {'L',' ',' ',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {'J','J','J',' '},
        {' ',' ','J',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {' ','S','S',' '},
        {'S','S',' ',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {'Z','Z',' ',' '},
        {' ','Z','Z',' '},
        {' ',' ',' ',' '}
    }
};

