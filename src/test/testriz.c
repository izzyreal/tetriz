#include <acutest.h>

#include "test_util.h"
#include "test_data.h"

#include "tetromino.h"

void test_no_rotation()
{
    TetrominoCellLayout rotated_i;
    rotate(&TETROMINOS[TETROMINO_I], ROTATED_0_DEGREES, &rotated_i);
    check_tetrominos_equal(TETROMINOS[TETROMINO_I].cell_layout, &rotated_i);

    TetrominoCellLayout rotated_o;
    rotate(&TETROMINOS[TETROMINO_O], ROTATED_0_DEGREES, &rotated_o);
    check_tetrominos_equal(TETROMINOS[TETROMINO_O].cell_layout, &rotated_o);

    TetrominoCellLayout rotated_t;
    rotate(&TETROMINOS[TETROMINO_T], ROTATED_0_DEGREES, &rotated_t);
    check_tetrominos_equal(TETROMINOS[TETROMINO_T].cell_layout, &rotated_t);

    TetrominoCellLayout rotated_l;
    rotate(&TETROMINOS[TETROMINO_L], ROTATED_0_DEGREES, &rotated_l);
    check_tetrominos_equal(TETROMINOS[TETROMINO_L].cell_layout, &rotated_l);

    TetrominoCellLayout rotated_j;
    rotate(&TETROMINOS[TETROMINO_J], ROTATED_0_DEGREES, &rotated_j);
    check_tetrominos_equal(TETROMINOS[TETROMINO_J].cell_layout, &rotated_j);

    TetrominoCellLayout rotated_s;
    rotate(&TETROMINOS[TETROMINO_S], ROTATED_0_DEGREES, &rotated_s);
    check_tetrominos_equal(TETROMINOS[TETROMINO_S].cell_layout, &rotated_s);

    TetrominoCellLayout rotated_z;
    rotate(&TETROMINOS[TETROMINO_Z], ROTATED_0_DEGREES, &rotated_z);
    check_tetrominos_equal(TETROMINOS[TETROMINO_Z].cell_layout, &rotated_z);
}

void test_90_degrees_rotation()
{
    TetrominoCellLayout rotated_i;
    rotate(&TETROMINOS[TETROMINO_I], ROTATED_90_DEGREES, &rotated_i);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_I], &rotated_i);

    TetrominoCellLayout rotated_o;
    rotate(&TETROMINOS[TETROMINO_O], ROTATED_90_DEGREES, &rotated_o);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_O], &rotated_o);

    TetrominoCellLayout rotated_t;
    rotate(&TETROMINOS[TETROMINO_T], ROTATED_90_DEGREES, &rotated_t);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_T], &rotated_t);

    TetrominoCellLayout rotated_l;
    rotate(&TETROMINOS[TETROMINO_L], ROTATED_90_DEGREES, &rotated_l);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_L], &rotated_l);

    TetrominoCellLayout rotated_j;
    rotate(&TETROMINOS[TETROMINO_J], ROTATED_90_DEGREES, &rotated_j);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_J], &rotated_j);

    TetrominoCellLayout rotated_s;
    rotate(&TETROMINOS[TETROMINO_S], ROTATED_90_DEGREES, &rotated_s);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_S], &rotated_s);

    TetrominoCellLayout rotated_z;
    rotate(&TETROMINOS[TETROMINO_Z], ROTATED_90_DEGREES, &rotated_z);
    check_tetrominos_equal(&TETROMINOS_ROTATED_90[TETROMINO_Z], &rotated_z);
}

void test_180_degrees_rotation()
{
    TetrominoCellLayout rotated_i;
    rotate(&TETROMINOS[TETROMINO_I], ROTATED_180_DEGREES, &rotated_i);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_I], &rotated_i);

    TetrominoCellLayout rotated_o;
    rotate(&TETROMINOS[TETROMINO_O], ROTATED_180_DEGREES, &rotated_o);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_O], &rotated_o);

    TetrominoCellLayout rotated_t;
    rotate(&TETROMINOS[TETROMINO_T], ROTATED_180_DEGREES, &rotated_t);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_T], &rotated_t);

    TetrominoCellLayout rotated_l;
    rotate(&TETROMINOS[TETROMINO_L], ROTATED_180_DEGREES, &rotated_l);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_L], &rotated_l);

    TetrominoCellLayout rotated_j;
    rotate(&TETROMINOS[TETROMINO_J], ROTATED_180_DEGREES, &rotated_j);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_J], &rotated_j);

    TetrominoCellLayout rotated_s;
    rotate(&TETROMINOS[TETROMINO_S], ROTATED_180_DEGREES, &rotated_s);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_S], &rotated_s);

    TetrominoCellLayout rotated_z;
    rotate(&TETROMINOS[TETROMINO_Z], ROTATED_180_DEGREES, &rotated_z);
    check_tetrominos_equal(&TETROMINOS_ROTATED_180[TETROMINO_Z], &rotated_z);
}

void test_270_degrees_rotation()
{
    TetrominoCellLayout rotated_i;
    rotate(&TETROMINOS[TETROMINO_I], ROTATED_270_DEGREES, &rotated_i);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_I], &rotated_i);

    TetrominoCellLayout rotated_o;
    rotate(&TETROMINOS[TETROMINO_O], ROTATED_270_DEGREES, &rotated_o);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_O], &rotated_o);

    TetrominoCellLayout rotated_t;
    rotate(&TETROMINOS[TETROMINO_T], ROTATED_270_DEGREES, &rotated_t);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_T], &rotated_t);

    TetrominoCellLayout rotated_l;
    rotate(&TETROMINOS[TETROMINO_L], ROTATED_270_DEGREES, &rotated_l);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_L], &rotated_l);

    TetrominoCellLayout rotated_j;
    rotate(&TETROMINOS[TETROMINO_J], ROTATED_270_DEGREES, &rotated_j);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_J], &rotated_j);

    TetrominoCellLayout rotated_s;
    rotate(&TETROMINOS[TETROMINO_S], ROTATED_270_DEGREES, &rotated_s);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_S], &rotated_s);

    TetrominoCellLayout rotated_z;
    rotate(&TETROMINOS[TETROMINO_Z], ROTATED_270_DEGREES, &rotated_z);
    check_tetrominos_equal(&TETROMINOS_ROTATED_270[TETROMINO_Z], &rotated_z);
}

TEST_LIST = {
    { "no_rotation", test_no_rotation },
    { "90_degrees_rotation", test_90_degrees_rotation },
    { "180_degrees_rotation", test_180_degrees_rotation },
    { "270_degrees_rotation", test_270_degrees_rotation },
    { NULL, NULL }
};

