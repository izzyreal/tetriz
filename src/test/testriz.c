#include <acutest.h>

#include "test_util.h"
#include "test_data.h"

#include "tetrominos.h"

void test_no_rotation()
{
    Tetromino* unrotated_i = &TETROMINOS[TETROMINO_I]; 
    Tetromino* rotated_i = rotate(unrotated_i, ROTATED_0_DEGREES);
    check_tetrominos_equal(unrotated_i, rotated_i);

    Tetromino* unrotated_t = &TETROMINOS[TETROMINO_T];
    Tetromino* rotated_t = rotate(unrotated_t, ROTATED_0_DEGREES);
    check_tetrominos_equal(unrotated_t, rotated_t);

    Tetromino* unrotated_l = &TETROMINOS[TETROMINO_L];
    Tetromino* rotated_l = rotate(unrotated_l, ROTATED_0_DEGREES);
    check_tetrominos_equal(unrotated_l, rotated_l);

    Tetromino* unrotated_j = &TETROMINOS[TETROMINO_J];
    Tetromino* rotated_j = rotate(unrotated_j, ROTATED_0_DEGREES);
    check_tetrominos_equal(unrotated_j, rotated_j);

    Tetromino* unrotated_s = &TETROMINOS[TETROMINO_S];
    Tetromino* rotated_s = rotate(unrotated_s, ROTATED_0_DEGREES);
    check_tetrominos_equal(unrotated_s, rotated_s);

    Tetromino* unrotated_z = &TETROMINOS[TETROMINO_Z];
    Tetromino* rotated_z = rotate(unrotated_z, ROTATED_0_DEGREES);
    check_tetrominos_equal(unrotated_z, rotated_z);
}

void test_90_degrees_rotation()
{
    Tetromino* unrotated_i = &TETROMINOS[TETROMINO_I];
    Tetromino* rotated_i = rotate(unrotated_i, ROTATED_90_DEGREES);
    Tetromino* expected_i = &TETROMINOS_ROTATED_90[0];
    check_tetrominos_equal(expected_i, rotated_i);

    Tetromino* unrotated_t = &TETROMINOS[TETROMINO_T];
    Tetromino* rotated_t = rotate(unrotated_t, ROTATED_90_DEGREES);
    Tetromino* expected_t = &TETROMINOS_ROTATED_90[1];
    check_tetrominos_equal(expected_t, rotated_t);

    Tetromino* unrotated_l = &TETROMINOS[TETROMINO_L];
    Tetromino* rotated_l = rotate(unrotated_l, ROTATED_90_DEGREES);
    Tetromino* expected_l = &TETROMINOS_ROTATED_90[2];
    check_tetrominos_equal(expected_l, rotated_l);

    Tetromino* unrotated_j = &TETROMINOS[TETROMINO_J];
    Tetromino* rotated_j = rotate(unrotated_j, ROTATED_90_DEGREES);
    Tetromino* expected_j = &TETROMINOS_ROTATED_90[3];
    check_tetrominos_equal(expected_j, rotated_j);

    Tetromino* unrotated_s = &TETROMINOS[TETROMINO_S];
    Tetromino* rotated_s = rotate(unrotated_s, ROTATED_90_DEGREES);
    Tetromino* expected_s = &TETROMINOS_ROTATED_90[4];
    check_tetrominos_equal(expected_s, rotated_s);

    Tetromino* unrotated_z = &TETROMINOS[TETROMINO_Z];
    Tetromino* rotated_z = rotate(unrotated_z, ROTATED_90_DEGREES);
    Tetromino* expected_z = &TETROMINOS_ROTATED_90[5];
    check_tetrominos_equal(expected_z, rotated_z);
}

TEST_LIST = {
    { "no_rotation", test_no_rotation },
    { "90_degrees_rotation", test_90_degrees_rotation },
    { NULL, NULL }
};

