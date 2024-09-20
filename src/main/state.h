#ifndef TETRIZ_STATE_H
#define TETRIZ_STATE_H

#include "constants.h"
#include "tetrominos.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char** canvas;
    char** prev_canvas;
    TetrominoType tetromino_type;
    TetrominoType next_tetromino_type;
    int8_t tetromino_x;
    int8_t tetromino_y;
    TetrominoRotation tetromino_rotation;
    uint32_t drop_interval;
    uint32_t tetromino_drop_timer;
    bool user_has_requested_exit;
    char** playfield;
} State;

#endif // TETRIZ_STATE_H
