#ifndef TETRIZ_CONSTANTS_H
#define TETRIZ_CONSTANTS_H

#include <stdint.h>

const uint8_t CANVAS_WIDTH_CHARS = 80;
const uint8_t CANVAS_HEIGHT_CHARS = 40;

const uint8_t PLAYFIELD_Y_CHARS = 5;
const uint8_t PLAYFIELD_X_CHARS = 30;

const uint8_t PLAYFIELD_WIDTH_CHARS = 20;
const uint8_t PLAYFIELD_HEIGHT_CHARS = 20;

const uint8_t CELL_WIDTH_CHARS = 2;
const uint8_t CELL_HEIGHT_CHARS = 1;

const uint8_t PLAYFIELD_WIDTH_CELLS = 10;
const uint8_t PLAYFIELD_HEIGHT_CELLS = 20;

const uint8_t NEXT_TETROMINO_X_CHARS = 10;
const uint8_t NEXT_TETROMINO_Y_CHARS = 10;

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

#endif // TETRIZ_CONSTANTS_H
