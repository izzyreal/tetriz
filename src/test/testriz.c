#include <acutest.h>

#include "test_util.h"
#include "test_data.h"

#include "tetrominos.h"

void test_no_rotation()
{
    Tetromino* unrotated_i = &TETROMINOS[TETROMINO_I]; 
    Tetromino rotated_i;
    rotate(unrotated_i, TETROMINO_I, ROTATED_0_DEGREES, &rotated_i);
    check_tetrominos_equal(unrotated_i, &rotated_i);

    Tetromino* unrotated_o = &TETROMINOS[TETROMINO_O]; 
    Tetromino rotated_o;
    rotate(unrotated_o, TETROMINO_O, ROTATED_0_DEGREES, &rotated_o);
    check_tetrominos_equal(unrotated_o, &rotated_o);

    Tetromino* unrotated_t = &TETROMINOS[TETROMINO_T];
    Tetromino rotated_t;
    rotate(unrotated_t, TETROMINO_T, ROTATED_0_DEGREES, &rotated_t);
    check_tetrominos_equal(unrotated_t, &rotated_t);

    Tetromino* unrotated_l = &TETROMINOS[TETROMINO_L];
    Tetromino rotated_l;
    rotate(unrotated_l, TETROMINO_L, ROTATED_0_DEGREES, &rotated_l);
    check_tetrominos_equal(unrotated_l, &rotated_l);

    Tetromino* unrotated_j = &TETROMINOS[TETROMINO_J];
    Tetromino rotated_j;
    rotate(unrotated_j, TETROMINO_J, ROTATED_0_DEGREES, &rotated_j);
    check_tetrominos_equal(unrotated_j, &rotated_j);

    Tetromino* unrotated_s = &TETROMINOS[TETROMINO_S];
    Tetromino rotated_s;
    rotate(unrotated_s, TETROMINO_S, ROTATED_0_DEGREES, &rotated_s);
    check_tetrominos_equal(unrotated_s, &rotated_s);

    Tetromino* unrotated_z = &TETROMINOS[TETROMINO_Z];
    Tetromino rotated_z;
    rotate(unrotated_z, TETROMINO_Z, ROTATED_0_DEGREES, &rotated_z);
    check_tetrominos_equal(unrotated_z, &rotated_z);
}

void test_90_degrees_rotation()
{
    Tetromino rotated_i;
    rotate(&TETROMINOS[TETROMINO_I], TETROMINO_I, ROTATED_90_DEGREES, &rotated_i);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_I], &rotated_i);

    Tetromino rotated_o;
    rotate(&TETROMINOS[TETROMINO_O], TETROMINO_O, ROTATED_90_DEGREES, &rotated_o);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_O], &rotated_o);

    Tetromino rotated_t;
    rotate(&TETROMINOS[TETROMINO_T], TETROMINO_T, ROTATED_90_DEGREES, &rotated_t);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_T], &rotated_t);

    Tetromino rotated_l;
    rotate(&TETROMINOS[TETROMINO_L], TETROMINO_L, ROTATED_90_DEGREES, &rotated_l);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_L], &rotated_l);

    Tetromino rotated_j;
    rotate(&TETROMINOS[TETROMINO_J], TETROMINO_J, ROTATED_90_DEGREES, &rotated_j);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_J], &rotated_j);

    Tetromino rotated_s;
    rotate(&TETROMINOS[TETROMINO_S], TETROMINO_S, ROTATED_90_DEGREES, &rotated_s);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_S], &rotated_s);

    Tetromino rotated_z;
    rotate(&TETROMINOS[TETROMINO_Z], TETROMINO_Z, ROTATED_90_DEGREES, &rotated_z);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_Z], &rotated_z);
}

void test_180_degrees_rotation()
{
    Tetromino rotated_i;
    rotate(&TETROMINOS[TETROMINO_I], TETROMINO_I, ROTATED_180_DEGREES, &rotated_i);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_I], &rotated_i);

    Tetromino rotated_o;
    rotate(&TETROMINOS[TETROMINO_O], TETROMINO_O, ROTATED_180_DEGREES, &rotated_o);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_O], &rotated_o);

    Tetromino rotated_t;
    rotate(&TETROMINOS[TETROMINO_T], TETROMINO_T, ROTATED_180_DEGREES, &rotated_t);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_T], &rotated_t);

    Tetromino rotated_l;
    rotate(&TETROMINOS[TETROMINO_L], TETROMINO_L, ROTATED_180_DEGREES, &rotated_l);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_L], &rotated_l);

    Tetromino rotated_j;
    rotate(&TETROMINOS[TETROMINO_J], TETROMINO_J, ROTATED_180_DEGREES, &rotated_j);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_J], &rotated_j);

    Tetromino rotated_s;
    rotate(&TETROMINOS[TETROMINO_S], TETROMINO_S, ROTATED_180_DEGREES, &rotated_s);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_S], &rotated_s);

    Tetromino rotated_z;
    rotate(&TETROMINOS[TETROMINO_Z], TETROMINO_Z, ROTATED_180_DEGREES, &rotated_z);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_Z], &rotated_z);
}

void test_270_degrees_rotation()
{
    Tetromino rotated_i;
    rotate(&TETROMINOS[TETROMINO_I], TETROMINO_I, ROTATED_270_DEGREES, &rotated_i);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_I], &rotated_i);

    Tetromino rotated_o;
    rotate(&TETROMINOS[TETROMINO_O], TETROMINO_O, ROTATED_270_DEGREES, &rotated_o);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_O], &rotated_o);

    Tetromino rotated_t;
    rotate(&TETROMINOS[TETROMINO_T], TETROMINO_T, ROTATED_270_DEGREES, &rotated_t);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_T], &rotated_t);

    Tetromino rotated_l;
    rotate(&TETROMINOS[TETROMINO_L], TETROMINO_L, ROTATED_270_DEGREES, &rotated_l);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_L], &rotated_l);

    Tetromino rotated_j;
    rotate(&TETROMINOS[TETROMINO_J], TETROMINO_J, ROTATED_270_DEGREES, &rotated_j);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_J], &rotated_j);

    Tetromino rotated_s;
    rotate(&TETROMINOS[TETROMINO_S], TETROMINO_S, ROTATED_270_DEGREES, &rotated_s);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_S], &rotated_s);

    Tetromino rotated_z;
    rotate(&TETROMINOS[TETROMINO_Z], TETROMINO_Z, ROTATED_270_DEGREES, &rotated_z);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_Z], &rotated_z);
}

TEST_LIST = {
    { "no_rotation", test_no_rotation },
    { "90_degrees_rotation", test_90_degrees_rotation },
    { "180_degrees_rotation", test_180_degrees_rotation },
    { "270_degrees_rotation", test_270_degrees_rotation },
    { NULL, NULL }
};

