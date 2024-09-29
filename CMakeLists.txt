cmake_minimum_required(VERSION 3.10)
project(Tetriz)

include_directories(deps)

add_executable(tetriz
    src/main/tetriz.c
    src/main/audio.h
    src/main/constants.h
    src/main/graphics.h
    src/main/melodies.h
    src/main/state.h
    src/main/tetromino.h
    src/main/types.h
    src/main/util.h
)

target_link_libraries(tetriz ncurses)

add_executable(testriz
    src/test/testriz.c
    src/test/test_data.h
    src/test/test_util.h
)

target_include_directories(testriz PRIVATE src/main)

target_link_libraries(testriz ncurses)

