#ifndef TETRIZ_STATE_H
#define TETRIZ_STATE_H

#include "constants.h"
#include "tetromino.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    /* 2D array representing the total drawable area of the game. */
    char **canvas;
    /*
     * 2D array representing the canvas state from the previous main loop
     * iteration. This allows us to infer changes between iterations and update
     * only the modified parts of the screen, optimizing CPU usage.
     */
    char **prev_canvas;

    /*
     * 2D array representing the playfield area, in cell units, where the game
     * is played.
     * Each cell can be empty (represented by a space character) or non-empty
     * (containing the letter of the tetromino type). While a boolean
     * could model a single cell, this representation may aid in debugging.
     */
    char **playfield;

    /* Type of the currently falling tetromino in the game. */
    TetrominoType tetromino_type;
    /* Type of the next tetromino to be spawned in the game. */
    TetrominoType next_tetromino_type;
    /* Horizontal coordinate of the current tetromino in cell units. */
    int8_t tetromino_x_cells;
    /* Vertical coordinate of the current tetromino in cell units. */
    int8_t tetromino_y_cells;
    /* Rotation state of the current tetromino. */
    TetrominoRotation tetromino_rotation;

    /* Current level of the game, starting from 1 */
    uint8_t level;
    /*
     * Time interval (in microseconds) before the current tetromino moves down
     * one cell. Measured from an unspecified point in time (using
     * CLOCK_MONOTONIC).
     */
    uint32_t drop_interval_microseconds;
    /*
     * Timestamp (in microseconds) of the last tetromino drop.
     * Measured from an unspecified point in time (using CLOCK_MONOTONIC).
     */
    uint32_t last_drop_timestamp_microseconds;
    /* Tracks the player's current score, based on cleared lines. */
    int16_t score;

    /*
     * Accumulates cleared lines since the last level increase,
     * resetting after reaching 10 cleared lines.
     */
    uint8_t cleared_line_accumulator;

    /*
     * Indicates whether the user has requested to exit the game by pressing
     * 'q' on the keyboard.
     */
    bool user_has_requested_exit;
} State;

#endif // TETRIZ_STATE_H
