# Tetriz - Another Tetris clone in C, this time by Izmar

## Background

Long story short, I want to learn how to use VIM. Having watched the 2023 "Tetris" movie recently, and having seen the very first version of Tetris from 1984 on the E60, I settled on making a clone of this particular version of Tetris. This way I have a manageable vehicle for learning VIM.

## Development Setup

At some point I'll probably switch to Make or CMake, but for now I'll just share my compile command, which you should run from the root of the repository:

```sh
gcc src/main/tetriz.c -o ./build/tetriz -lncurses -Idep
```

Now you can start the game by running:

```sh
./build/tetriz
```
