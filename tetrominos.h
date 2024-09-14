typedef enum { 
    TETROMINO_I = 1,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_L,
    TETROMINO_J,
    TETROMINO_S,
    TETROMINO_Z,
    TETROMINO_COUNT
} TetrominoType;

const uint8_t TETROMINO_SIZE = 4;

typedef char Tetromino[TETROMINO_SIZE][TETROMINO_SIZE];

static char TETROMINOS[7][TETROMINO_SIZE][TETROMINO_SIZE] =
{
    {
        ' ',' ',' ',' ',
        'I','I','I','I',
        ' ',' ',' ',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        ' ','O','O',' ',
        ' ','O','O',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        'T','T','T',' ',
        ' ','T',' ',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        'L','L','L',' ',
        'L',' ',' ',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        'J','J','J',' ',
        ' ',' ','J',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        ' ','S','S',' ',
        'S','S',' ',' ',
        ' ',' ',' ',' '
    },
    {
        ' ',' ',' ',' ',
        'Z','Z',' ',' ',
        ' ','Z','Z',' ',
        ' ',' ',' ',' '
    }
};

typedef struct {
   int8_t left; 
   int8_t right; 
   int8_t top; 
   int8_t bottom; 
} TetrominoBounds;

Tetromino* rotate(const Tetromino* tetromino, uint8_t rotation)
{
    Tetromino* rotated = malloc(sizeof(Tetromino));

    uint8_t i, j;
    const uint8_t pivot = 1;

    for (i = 0; i < TETROMINO_SIZE; i++)
    {
        for (j = 0; j < TETROMINO_SIZE; j++)
        {
            const int8_t x = j - pivot;
            const int8_t y = i - pivot;
            uint8_t new_i, new_j;
            
            switch (rotation)
            {
                case 0:
                    new_i = (pivot + y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot + x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case 1:
                    new_i = (pivot - x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot + y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case 2:
                    new_i = (pivot - y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot - x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
                case 3:
                    new_i = (pivot + x + TETROMINO_SIZE) % TETROMINO_SIZE;
                    new_j = (pivot - y + TETROMINO_SIZE) % TETROMINO_SIZE;
                    break;
            }

            (*rotated)[i][j] = (*tetromino)[new_i][new_j];
        }
    }
    return rotated;
}

