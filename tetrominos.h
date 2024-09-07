#include <stdbool.h>

typedef enum { 
    TETROMINO_NONE,
    TETROMINO_STRAIGHT,
    TETROMINO_SQUARE,
    TETROMINO_T,
    TETROMINO_L,
    TETROMINO_SKEW
} TETROMINO_TYPE;

const bool T_DATA_STRAIGHT[1][4] = { { true, true, true, true } };
const bool T_DATA_SQUARE[2][2] = { { true, true}, { true, true } };
const bool T_DATA_T[2][3] = { { true, true, true}, { false, true, false } };
const bool T_DATA_L[3][2] = { { true, false }, { true, false }, { true, true } };
const bool T_DATA_SKEW[2][3] = { { false, true, true }, { true, true, false } };

typedef struct {
    const bool* data;
    uint8_t width;
    uint8_t height;
} Tetromino;

Tetromino tetrominos[] = {
    { &T_DATA_STRAIGHT[0][0], sizeof(T_DATA_STRAIGHT[0]) / sizeof(T_DATA_STRAIGHT[0][0]), sizeof(T_DATA_STRAIGHT) / sizeof(T_DATA_STRAIGHT[0]) },
    { &T_DATA_SQUARE[0][0], sizeof(T_DATA_SQUARE[0]) / sizeof(T_DATA_SQUARE[0][0]), sizeof(T_DATA_SQUARE) / sizeof(T_DATA_SQUARE[0]) },
    { &T_DATA_T[0][0], sizeof(T_DATA_T[0]) / sizeof(T_DATA_T[0][0]), sizeof(T_DATA_T) / sizeof(T_DATA_T[0]) },
    { &T_DATA_L[0][0], sizeof(T_DATA_L[0]) / sizeof(T_DATA_L[0][0]), sizeof(T_DATA_L) / sizeof(T_DATA_L[0]) },
    { &T_DATA_SKEW[0][0], sizeof(T_DATA_SKEW[0]) / sizeof(T_DATA_SKEW[0][0]), sizeof(T_DATA_SKEW) / sizeof(T_DATA_SKEW[0]) }
};

