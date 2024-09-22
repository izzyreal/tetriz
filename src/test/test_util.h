#ifndef TETRIZ_TEST_UTIL_H
#define TETRIZ_TEST_UTIL_H

#include "tetromino.h"

#define OUTPUT_BUFFER_SIZE ((TETROMINO_SIZE * 2 + 3) * (TETROMINO_SIZE + 2) + 1) 

void tetromino_to_string(const TetrominoCellLayout *t, char *output)
{
    uint8_t index = 0;

    output[index++] = '+';
    
    for (uint8_t x = 0; x < TETROMINO_SIZE * 2; ++x)
    {
        output[index++] = '-';
    }
    
    output[index++] = '+';
    output[index++] = '\n';

    for (uint8_t y = 0; y < TETROMINO_SIZE; ++y)
    {
        output[index++] = '|'; 
 
        for (uint8_t x = 0; x < TETROMINO_SIZE; ++x)
        {
            if ((*t)[y][x] == ' ')
            {
                output[index++] = ' ';
                output[index++] = ' '; 
            }
            else
            {
                output[index++] = '[';
                output[index++] = ']';
            }
        }

        output[index++] = '|';
        output[index++] = '\n';
    }

    output[index++] = '+';
    
    for (uint8_t x = 0; x < TETROMINO_SIZE * 2; ++x)
    {
        output[index++] = '-';
    }

    output[index++] = '+';
    output[index++] = '\n';

    output[index] = '\0';
}

void check_tetrominos_equal(const TetrominoCellLayout *expected, const TetrominoCellLayout *actual)
{
    bool equal = true;

    for (uint8_t y = 0; y < TETROMINO_SIZE; y++)
    {
        for (uint8_t x = 0; x < TETROMINO_SIZE; x++)
        {
            if ((*expected)[y][x] != (*actual)[y][x])
            {
                equal = false;
                break;
            }
        }
    }

    if (!TEST_CHECK(equal))
    {
        char expected_as_string[OUTPUT_BUFFER_SIZE];
        char actual_as_string[OUTPUT_BUFFER_SIZE];
        
        tetromino_to_string(expected, expected_as_string);
        tetromino_to_string(actual, actual_as_string);

        TEST_MSG("Expected:\n%s", expected_as_string);
        TEST_MSG("Actual:\n%s", actual_as_string);
    }
}

#endif // TETRIZ_TEST_UTIL_H
