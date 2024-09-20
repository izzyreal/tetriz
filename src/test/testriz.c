#include <acutest.h>

#include "test_util.h"

#include "tetrominos.h"

void test_no_rotation()
{
    Tetromino* unrotated_i = &TETROMINOS[TETROMINO_I]; 
    Tetromino* rotated_i = rotate(unrotated_i, ROTATED_0_DEGREES);
    check_tetrominos_equal(unrotated_i, rotated_i);

    Tetromino* unrotated_o = &TETROMINOS[TETROMINO_O];
    Tetromino* rotated_o = rotate(unrotated_o, ROTATED_0_DEGREES);
    check_tetrominos_equal(unrotated_o, rotated_o);

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

TEST_LIST = {
   { "rotation", test_no_rotation },
   { NULL, NULL }
};

