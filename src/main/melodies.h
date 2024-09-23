typedef struct {
    float note; // MIDI note number
    float pos; // Position in ticks
    float duration; // Duration in ticks
} Note;

const Note lead_melody[] = {{64, 0, 96}, {59, 96, 48}, {60, 144, 48}, {62, 192, 96}, {60, 288, 48}, {59, 336, 48}, {57, 384, 96}, {57, 480, 48}, {60, 528, 48}, {64, 576, 96}, {62, 672, 48}, {60, 720, 48}, {59, 768, 96}, {59, 864, 48}, {60, 912, 48}, {62, 960, 96}, {64, 1056, 96}, {60, 1152, 96}, {57, 1248, 96}, {57, 1344, 96}, {62, 1536, 144}, {65, 1680, 48}, {69, 1728, 96}, {67, 1824, 48}, {65, 1872, 48}, {64, 1920, 144}, {60, 2064, 48}, {64, 2112, 96}, {62, 2208, 48}, {60, 2256, 48}, {59, 2304, 96}, {59, 2400, 48}, {60, 2448, 48}, {62, 2496, 96}, {64, 2592, 96}, {60, 2688, 96}, {57, 2784, 96}, {57, 2880, 96}, {64, 3072, 96}, {59, 3168, 48}, {60, 3216, 48}, {62, 3264, 96}, {60, 3360, 48}, {59, 3408, 48}, {57, 3456, 96}, {57, 3552, 48}, {60, 3600, 48}, {64, 3648, 96}, {62, 3744, 48}, {60, 3792, 48}, {59, 3840, 96}, {59, 3936, 48}, {60, 3984, 48}, {62, 4032, 96}, {64, 4128, 96}, {60, 4224, 96}, {57, 4320, 96}, {57, 4416, 96}, {62, 4608, 144}, {65, 4752, 48}, {69, 4800, 96}, {67, 4896, 48}, {65, 4944, 48}, {64, 4992, 144}, {60, 5136, 48}, {64, 5184, 96}, {62, 5280, 48}, {60, 5328, 48}, {59, 5376, 96}, {59, 5472, 48}, {60, 5520, 48}, {62, 5568, 96}, {64, 5664, 96}, {60, 5760, 96}, {57, 5856, 96}, {57, 5952, 96}, {64, 6144, 192}, {60, 6336, 192}, {62, 6528, 192}, {59, 6720, 192}, {60, 6912, 192}, {57, 7104, 192}, {56, 7296, 192}, {59, 7488, 144}, {64, 7680, 192}, {60, 7872, 192}, {62, 8064, 192}, {59, 8256, 192}, {60, 8448, 96}, {64, 8544, 96}, {69, 8640, 96}, {72, 8736, 96}, {71, 8832, 336}};

const Note bass_melody[] = { {40, 0, 48}, {52, 48, 48}, {40, 96, 48}, {52, 144, 48}, {40, 192, 48}, {52, 240, 48}, {40, 288, 48}, {52, 336, 48}, {45, 384, 48}, {57, 432, 48}, {45, 480, 48}, {57, 528, 48}, {45, 576, 48}, {57, 624, 48}, {45, 672, 48}, {57, 720, 48}, {44, 768, 48}, {56, 816, 48}, {44, 864, 48}, {56, 912, 48}, {40, 960, 48}, {52, 1008, 48}, {40, 1056, 48}, {52, 1104, 48}, {45, 1152, 48}, {57, 1200, 48}, {45, 1248, 48}, {57, 1296, 48}, {45, 1344, 48}, {45, 1392, 48}, {47, 1440, 48}, {48, 1488, 48}, {38, 1536, 48}, {50, 1584, 48}, {38, 1632, 48}, {50, 1680, 48}, {38, 1728, 48}, {50, 1776, 48}, {38, 1824, 48}, {50, 1872, 48}, {36, 1920, 48}, {48, 1968, 48}, {36, 2016, 48}, {48, 2064, 48}, {36, 2112, 48}, {48, 2160, 48}, {36, 2208, 48}, {48, 2256, 48}, {28, 2304, 48}, {40, 2352, 48}, {28, 2400, 48}, {40, 2448, 48}, {40, 2496, 48}, {52, 2544, 48}, {40, 2592, 48}, {52, 2640, 48}, {33, 2688, 48}, {45, 2736, 48}, {33, 2784, 48}, {45, 2832, 48}, {33, 2880, 48}, {45, 2928, 48}, {45, 2976, 48}, {45, 3024, 48}, {40, 3072, 12}, {52, 3120, 48}, {40, 3168, 48}, {52, 3216, 48}, {40, 3264, 48}, {52, 3312, 48}, {40, 3360, 48}, {52, 3408, 48}, {45, 3456, 48}, {57, 3504, 48}, {45, 3552, 48}, {57, 3600, 48}, {45, 3648, 48}, {57, 3696, 48}, {45, 3744, 48}, {57, 3792, 48}, {44, 3840, 48}, {56, 3888, 48}, {44, 3936, 48}, {56, 3984, 48}, {40, 4032, 48}, {52, 4080, 48}, {40, 4128, 48}, {52, 4176, 48}, {45, 4224, 48}, {57, 4272, 48}, {45, 4320, 48}, {57, 4368, 48}, {45, 4416, 48}, {45, 4464, 48}, {47, 4512, 48}, {48, 4560, 48}, {38, 4608, 48}, {50, 4656, 48}, {38, 4704, 48}, {50, 4752, 48}, {38, 4800, 48}, {50, 4848, 48}, {38, 4896, 48}, {50, 4944, 48}, {36, 4992, 48}, {48, 5040, 48}, {36, 5088, 48}, {48, 5136, 48}, {36, 5184, 48}, {48, 5232, 48}, {36, 5280, 48}, {48, 5328, 48}, {28, 5376, 48}, {40, 5424, 48}, {28, 5472, 48}, {40, 5520, 48}, {40, 5568, 48}, {52, 5616, 48}, {40, 5664, 48}, {52, 5712, 48}, {33, 5760, 48}, {45, 5808, 48}, {33, 5856, 48}, {45, 5904, 48}, {33, 5952, 48}, {45, 6000, 48}, {45, 6048, 48}, {45, 6096, 48}, {33, 6144, 48}, {45, 6192, 48}, {33, 6240, 48}, {45, 6288, 48}, {33, 6336, 48}, {45, 6384, 48}, {33, 6432, 48}, {45, 6480, 48}, {28, 6528, 48}, {40, 6576, 48}, {28, 6624, 48}, {40, 6672, 48}, {28, 6720, 48}, {40, 6768, 48}, {28, 6816, 48}, {40, 6864, 48}, {33, 6912, 48}, {45, 6960, 48}, {33, 7008, 48}, {45, 7056, 48}, {33, 7104, 48}, {45, 7152, 48}, {33, 7200, 48}, {45, 7248, 48}, {28, 7296, 48}, {40, 7344, 48}, {28, 7392, 48}, {40, 7440, 48}, {28, 7488, 48}, {40, 7536, 48}, {40, 7584, 48}, {40, 7632, 48}, {33, 7680, 48}, {45, 7728, 48}, {33, 7776, 48}, {45, 7824, 48}, {33, 7872, 48}, {45, 7920, 48}, {33, 7968, 48}, {45, 8016, 48}, {28, 8064, 47}, {40, 8112, 48}, {28, 8160, 47}, {40, 8208, 48}, {28, 8256, 47}, {40, 8304, 48}, {28, 8352, 47}, {40, 8400, 48}, {33, 8448, 48}, {45, 8496, 48}, {33, 8544, 48}, {45, 8592, 48}, {33, 8640, 48}, {45, 8688, 48}, {33, 8736, 48}, {45, 8784, 48}, {32, 8832, 48}, {44, 8880, 48}, {32, 8928, 48}, {44, 8976, 48}, {44, 9072, 48}, {44, 9120, 48}, {44, 9168, 48}};

const int lead_melody_length = sizeof(lead_melody) / sizeof(lead_melody[0]);
const int bass_melody_length = sizeof(bass_melody) / sizeof(bass_melody[0]);
