#ifndef TETRIZ_TYPES_H
#define TETRIZ_TYPES_H

#include "constants.h"

/*
 * 4x4 grid of cells. Each cell can be empty (represented by a space character)
 * or non-empty (containing the letter of the tetromino type). While a boolean 
 * could model a single cell, this representation may aid in debugging.
 */
typedef char TetrominoCellLayout[TETROMINO_SIZE_CELLS][TETROMINO_SIZE_CELLS];

#endif // TETRIZ_TYPES_H
