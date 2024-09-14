#include "constants.h"

typedef struct {
    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    char prev_canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    TetrominoType tetromino_type;
    TetrominoType next_tetromino_type;
    int8_t tetromino_x;
    int8_t tetromino_y;
    int8_t tetromino_rotation;
    uint32_t drop_interval;
    uint32_t tetromino_drop_timer;
    bool user_has_requested_exit;
    char playfield[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
} State;

